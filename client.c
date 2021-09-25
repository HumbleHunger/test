#include<fcntl.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<sys/time.h>
#include<sys/epoll.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include<mysql/mysql.h>
#include<sys/sem.h>
#include"std.h"
#include<semaphore.h>
typedef struct message{
	char msg[1024];  //服务器发送消息
	struct message* next;
}MSG, *MSGL;

MSGL head = NULL; //消息头结点
MSGL historyHead = NULL; //历史消息头结点
MSGL offlineMsg = NULL; //离线消息头结点
MSGL offlineFile = NULL; //离线文件头结点
//定义信号量
sem_t sem_producer;
sem_t sem_consumer;

typedef struct _friend{
	int status; //状态
	char friname[30];
	struct _friend *next;
}friend, *friendL;

friendL frimsg = NULL;

typedef struct _group{
	int status; //身份
	char name[30];  //记录
	struct _group * next;
}Group, *groupL;

groupL grpmsg = NULL;

void my_err(char *str, int line)
{
	fprintf(stderr, "line:%d\n", line);
	perror(str);
	exit(-1);
}

/*
函数功能：创建用户
参数：服务器描述符
*/
int user_init(int* cli_fd){
    int fd = *cli_fd;
    
    char use[200] = {"init:"};
	char username[30] = {0};
	char passwd[30] = {0};
	char ans[30] = {0};
	char flag[20] = {0}; //接受服务器回应
	char buf[50] = {0};
	   
	printf("----------------------\n");
	//fflush(stdout); fflush(stdin);
    memset(buf, 0, sizeof(buf));
	//strcpy(buf, "\t注册\n");
    printf("\t注册\n");
	
	printf("请输入要注册的用户名：\n"); 
	
	fgets(username, sizeof(username), stdin);
	//read(STDIN_FILENO, username, sizeof(username));
    username[strlen(username)-1] = '\0';
    sprintf(use+strlen(use), " %s", username);

	printf("请输入密码，长度小于20: \n");
	fgets(passwd, sizeof(passwd), stdin);
    //read(STDIN_FILENO, passwd, sizeof(passwd));
    passwd[strlen(passwd)-1] = '\0';
    sprintf(use+strlen(use), " %s", passwd);

	printf("密保问题: 你的号码\n");
    fgets(ans, sizeof(ans), stdin);
	//read(STDIN_FILENO, ans, sizeof(ans));
    ans[strlen(ans)-1] = '\0';
    sprintf(use+strlen(use), " %s\n", ans);
    if(send(fd, use, strlen(use), 0) < 0){
        my_err("send", __LINE__);
    }
    
	int len, count = 0;
    len = recv(fd, flag, sizeof(flag), 0);
	if(len == -1)
		my_err("recv", __LINE__);
	if(strncmp(flag, "success", 7) == 0)
	{
		printf("注册成功\n");
		printf("----------------------\n");
		return 1;
	}
	else
		return 0;
}


/********************************
 *函数功能：用户登录
  函数参数：服务器套接字 
 */
int user_login(int* sockfd, char* username){
	int fd = *sockfd;
	//char username[30] = {0};
	char password[30] = {0};
	char msg[256] = {"login:"};
	
	printf("请输入用户名：\n");
	fgets(username, sizeof(username), stdin);
	
	username[strlen(username) - 1] = '\0';
	sprintf(msg+strlen(msg), " %s", username);
	
	printf("请输入密码：\n");

	fgets(password, sizeof(password), stdin);

	password[strlen(password)-1] = '\0';
	sprintf(msg+strlen(msg), " %s\n", password);
	//printf("%s\n", msg);
	if(send(fd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}

	memset(msg, 0, sizeof(msg));
	if(recv(fd, msg, sizeof(msg), 0) < 0){
		my_err("recv", __LINE__);
	}
	if(!strncmp(msg, "success", 7)){
		printf("login success\n");
		return 1;
	}
	
	return 0;
}

/*
函数功能：找密码
函数参数：服务器描述符
*/
int user_find(int *sockfd){
	char username[30] = {0};
	char ans[30] = {0};
	char msg[256] = {"find:"};
	char buf[50] = {0};
	strcpy(buf, "请输入用户名：\n"); //getchar();
	write(STDOUT_FILENO, buf, strlen(buf));
	//read(STDIN_FILENO, username, sizeof(username));//MYFLUSH;
	fgets(username, sizeof(username), stdin);
	username[strlen(username) - 1] = '\0';
	sprintf(msg+strlen(msg), " %s", username);
	strcpy(buf, "请输入密保："); 
	write(STDOUT_FILENO, buf, strlen(buf));
	// read(STDIN_FILENO, ans, sizeof(ans));
	fgets(ans, sizeof(ans), stdin);
	ans[strlen(ans) - 1] = '\0';
	sprintf(msg+strlen(msg), " %s\n", ans);

	//printf("%s\n", msg);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}

	memset(msg, 0, sizeof(msg));
	if(recv(*sockfd, msg, sizeof(msg), 0) < 0){
		my_err("recv", __LINE__);
	}
	if(!strncmp(msg, "success", 7)){
		char pwd[30] = {0};
		sscanf(msg, "%[^ ] %[^\r\n]", buf, pwd);
		printf("find password success, your password is %s\n", pwd);
		getchar();
		system("clear");
		return 1;
	}
	system("clear");
	return 0;
}

/*
函数功能：好友添加
参数：描述符，报头，好友名
*/
char friendname[30] = {0};  //记录要添加好友名
static char name[30] = {0};  //记录登录用户
void friend_add(int *sockfd, char* msg, char* friendname){
	char ans[30] = {0};	
	char buf[50] = {0};
	strcpy(buf, "请输入要添加好友名：");
	printf("%s\n",buf);//getchar();
	//write(STDOUT_FILENO, buf, strlen(buf)); 
	read(STDIN_FILENO, friendname, sizeof(friendname));
	friendname[strlen(friendname) - 1] = '\0';
	sprintf(msg+strlen(msg), " %s\n", friendname);
	//printf("%s\n", msg);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
	memset(msg, 0, sizeof(msg));	
	return;
}


/*----------------------------------------------------------------
函数功能：查询好友列表
参数：描述符，报头
*/
void friend_look(int * sockfd){	
	//usleep(drand48() * 100000);
	//sleep(rand()%3);
	//加锁
	sem_wait(&sem_consumer); //相当于--
	
	printf("========================================\n");
	printf("|\t好友姓名\t好友状态\t|\n");
	printf("----------------------------------------\n");
	friendL p = frimsg;
	
	while(p->next != NULL){
		friendL q = p->next;
		printf("\t%-8s\t\t%-8d\t\n", q->friname, q->status);
		//usleep(1000000);
		p->next = q->next;
		free(q);
	}
	printf("========================================\n");
	sem_post(&sem_producer);
	printf("按【enter】返回\n"); getchar();
}


static int Flag = 0;  //标识符：判断是否在私聊界面
//私聊 参数：msg：用户名
void friend_chat(int * sockfd, char* msg){
	char username[30] = {0};
	strncpy(username, msg, strlen(msg)); 
	//sprintf(msg, "%s %s %s", "friend:", "chat",
	//加锁
	sem_wait(&sem_consumer); //相当于--

	
	//查找私聊对象是否为好友及在线
	friendL p = frimsg; int count = 0;
	while(p->next != NULL){
		friendL q = p->next;
		if(q->status){
			printf("|%10s|", q->friname);
			count++;
			if(!(count % 2)){
				printf("\n");
			}
		}
		
		p->next = q->next;
	}
	if(count % 2){
		printf("\n");
	}
	sem_post(&sem_producer);

	if(!count){ //非好友
		printf("您的好友均未上线!按【enter】退出\n"); getchar(); //getchar();
		return;
	}

	char friName[30] = {0};//getchar();
	printf("请选择私聊对象：\n");
	fgets(friName, sizeof(friName), stdin);
	friName[strlen(friName) - 1] = '\0';
	
	
	system("clear");
	printf("################################\n");
	printf("========QQ chat interface=======\n");
	printf("************quit 退出************\n");
	while(1){
		
		Flag = 1;
		char buf[512] = {0}; //char msg[2048] = {0};
		
		read(STDIN_FILENO, buf, sizeof(buf));
		buf[strlen(buf)-1] = '\0';
		if(!strncmp(buf, "quit", 4) && !strncmp(buf, "quit", strlen(buf))){
			Flag = 0;
			fflush(stdin); fflush(stdout);
			break;
		}
		char msg1[2048] = {0};
		memset(msg1, 0, sizeof(msg1));
		sprintf(msg1, "%s %s %s %s %s\n", "friend:", "chat", username, friName, buf);
		if(send(*sockfd, msg1, strlen(msg1), 0) < 0){
			my_err("send", __LINE__);
		}
		
	}
}

//函数功能：屏蔽好友消息
//参数：描述符， msg：报头
void no_friend_chat(int *sockfd, char* msg){
	//friend_look(sockfd, msg);
	printf("请选择好友姓名：\n");// getchar();
	char friName[30] = {0};
	fgets(friName, sizeof(friName), stdin);
	friName[strlen(friName)-1] = '\0';
	sprintf(msg +strlen(msg), " %s\n", friName);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

//函数功能：删除好友
//参数：报头
void friend_delete(int *sockfd, char* msg){
	char friName[30] = {0};
	printf("请输入要删除好友姓名：\n");// getchar();
	fgets(friName, sizeof(friName), stdin);
	sprintf(msg + strlen(msg), "%s\n", friName);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

/*
函数功能：查询历史消息
参数：报头
*/
void friend_history(int* sockfd, char* msg){
	char friName[30] = {0};
	printf("请输入好友姓名：\n"); //getchar();
	fgets(friName, sizeof(friName), stdin);
	friName[strlen(friName) - 1] = '\0';
	sprintf(msg + strlen(msg), "%s\n", friName);
	
	system("clear"); //getchar();
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
	
	printf("=========History Record=============\n");
	printf("*********friend: %6s*********\n", friName);
	printf("---------按【enter】返回------------\n");
	
   	sem_wait(&sem_consumer);
	MSGL p = historyHead;
	MSGL q = historyHead->next;
	while(p->next != NULL){
		p->next = q->next;
		write(STDOUT_FILENO, q->msg, strlen(q->msg));
		free(q);
		q = p->next;
	}
	sem_post(&sem_producer);
	MYFLUSH;
	// getchar();
}
//处理消息
void do_service(int * sockfd){
	MSGL p = head;
	MSGL q = head->next;
	char msg[4096] = {0};
	while(1){
		if(p->next == NULL){
			printf("暂无消息,按【enter】退出\n");MYFLUSH;//getchar();
			break;
		}
		if(p->next != NULL){
			p->next = q->next;
			strncpy(msg, q->msg, strlen(q->msg));
			free(q);
			q = p->next;
		}
		char *ten = "$groupagaintalk";

		if(!strncmp(msg, "addfriend", 9)){  //添加
			char buf[256] = {0};char log[10] = {0};
			sscanf(msg, "%[^ ] %[^ ] %[^\r\n]", log, buf, friendname);
			//printf("%s\n", msg);
			if(!strncmp(buf, "fail", 4)){				
			//	system("clear");
				printf("该用户不存在，按【enter】返回\n"); MYFLUSH;//getchar();
				break;
			}else{
			//write(STDOUT_FILENO, buf, strlen(buf)); //getchar();		
				char c[10] = {0};  
				do{
					printf("%s\n", buf); //getchar();
					fflush(stdout); fflush(stdin);
					memset(c, 0, sizeof(c));
					scanf("%s", c); MYFLUSH;
					
					c[strlen(c)] = '\0';
					//printf("%s\n", c);//getchar();			
				}while(strncmp(c, "yes", 3) && strncmp(c, "no", 2));
					memset(msg, 0, sizeof(msg));
					sprintf(msg, "%s %s %s %s %s\n", "friend:", "addrec", c, name, friendname);
				//printf("%s\n", msg);
				if(send(*sockfd, msg, strlen(msg), 0) < 0){
					my_err("send", __LINE__);
				}
				// break;
			}
		}else if(!strncmp(msg, "~friendchat", 11)){
			char buf[1024] = {0}; char log[20] = {0}; char friName[30] = {0};
			sscanf(msg, "%[^ ] %[^ ] %[^\r\n]", log, friName, buf);
			printf("您的好友%s向您发了条消息：%s\n", friName, buf);
			while(p->next != NULL){
				// printf("p != NULL\n");
				if(!strncmp(q->msg, "~friendchat", 11)){
					p->next = q->next;
					strncpy(msg, q->msg, strlen(q->msg));
					free(q);
					sscanf(msg, "%[^ ] %[^ ] %[^\r\n]", log, friName, buf);
					printf("您的好友%s向您发了条消息：%s\n", friName, buf);
					q = p->next;			
				}else{
					break;
				}
			}
			printf("是否进入私聊（yes/no）：\n");
			char sel[10];
			do{
				memset(sel, 0, sizeof(sel));
				read(STDIN_FILENO, sel, sizeof(sel));
				sel[strlen(sel)-1] = '\0';
				// printf("%s\n", sel);
			}while(strncmp(sel, "yes", 3) && strncmp(sel, "no", 2));
			if(!strncmp(sel, "yes", 3)){
				
				char msg1[256] = {0};
				memset(msg1, 0, sizeof(msg1));
				sprintf(msg1, "%s %s %s$", "friend:", "search", name);
				if(send(*sockfd, msg1, strlen(msg1), 0) < 0){
					my_err("send", __LINE__);
				}
				// printf("%s\n", msg1);
				friend_chat(sockfd, name);
				break;
			}
			
		}else if(!strncmp(msg, ten, strlen(ten))){
			char buf[1024] = {0}; char log[24] = {0}; 
			char friName[30] = {0}; char groupname[30] = {0};
			sscanf(msg, "%[^ ] %[^ ] %[^ ] %[^+_+]", log, friName, groupname, buf);
			printf("%s在%s群中发了条消息：%s\n", friName, groupname, buf);
				
		}
	
	}
}

/*
函数功能：好友管理
参数：描述符，用户名
*/
void user_friend_mag(int* sockfd, char* username){
	char msg[1024] = {0};				
	while(1){
		memset(msg, 0, sizeof(msg));
	
		system("clear");
		printf("-----------------------------------------------------------\n");
		printf("           welcome to qq Friend MAG Interface              \n");
		printf("请输入   1(私聊)   \t\t2(好友添加)\n         3(好友删除)\t\t4(处理消息)\n         5(查看好友列表)\t6(屏蔽好友消息)\n         7(查看聊天记录)\t0(退出)\n");
		char sel;
		// int ret = 0;//getchar();
		read(STDIN_FILENO, &sel, 1); MYFLUSH;
				
		
		if(sel == '1'){
			system("clear");
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s$", "friend:", "search", username);
			if(send(*sockfd, msg, strlen(msg), 0) < 0){
				my_err("send", __LINE__);
			}
			
			memset(msg, 0, sizeof(msg));
			//sprintf(msg, "%s %s %s", "friend:", "chat", username);			
			sprintf(msg, "%s", username);
			friend_chat(sockfd, msg);
		}else if(sel == '2'){
			system("clear");
			sprintf(msg, "%s %s %s", "friend:", "add", username);			
			friend_add(sockfd, msg, friendname);	
		}else if(sel == '3'){ //好友删除
			system("clear");
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s\n", "friend:", "delete", username);
			friend_delete(sockfd, msg);
		}else if(sel == '0'){
			break;
		}else if(sel == '4'){
			system("clear");
			do_service(sockfd);
		}else if(sel == '5'){ //查看好友列表
			system("clear");			
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s$", "friend:", "search", username);
			if(send(*sockfd, msg, strlen(msg), 0) < 0){
				my_err("send", __LINE__);
			}
			//sleep(1);
			friend_look(sockfd);
		}else if(sel == '6'){ //屏蔽好友消息
			system("clear");
			memset(msg, 0, sizeof(msg));
			// sprintf(msg, "%s %s %s\n", "friend:", "search", username);
			// if(send(*sockfd, msg, strlen(msg), 0) < 0){
			// 	my_err("send", __LINE__);
			// }
			sprintf(msg, "%s %s %s\n", "friend:", "mask", username);
			no_friend_chat(sockfd, msg);
		}else if(sel == '7'){ //查看聊天记录
			system("clear");
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s ", "friend:", "history", username);
			friend_history(sockfd, msg);
		}
	}
}

/*
函数功能：群创建
参数：描述符，报头
*/
void group_create(int* sockfd, char* msg){
	char groupname[30] = {0};
	printf("请输入要创建的群名称：\n");//getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname)-1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}

}

/*
函数功能：加退群
参数：描述符，报头
*/
void group_add(int* sockfd,char* msg){
	char groupname[30] = {0};
	printf("请输入要加入的群名称\n"); // getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

/*
函数功能：退群
参数：描述符，包头
*/
void group_out(int* sockfd, char* msg){
	char groupname[30] = {0};
	printf("请输入要退出的群名称\n"); //getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

/*
函数功能：查看群成员及加入的群
参数：描述符，用户名
*/
void group_information(int* sockfd, char* usrname){
	
	printf("========================================\n");
	printf("|\t\t已加群组\t\t|\n");
	printf("----------------------------------------\n");
	sem_wait(&sem_consumer); //相当于--	
	groupL p = grpmsg->next;
	int count = 0;
	if(!p){
		count = 1;
	}
	while(p != NULL){
		printf("\t\t%-16s\t\t\n", p->name);
		//usleep(1000000);
		p = p->next;
	}
	printf("========================================\n");
	if(count){
		sem_post(&sem_producer);
		printf("按【enter】返回\n"); MYFLUSH; //getchar(); getchar();
		return;
	}	
	// 
	
	printf("请输入您要查看的组名：\n"); //getchar();
	char msg[1024] = {0}; 
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "%s %s %s\n", "group:", "memmsg", usrname);
	char groupname[30] = {0};
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	p = grpmsg;  count = 0;
	while(p->next != NULL){
		groupL q = p->next;
		if((!strncmp(groupname, q->name, strlen(groupname))) && 
			strlen(groupname) == strlen(q->name)){
			sprintf(msg + strlen(msg), "%s\n", groupname);			
			count++;
		}
		//usleep(1000000);
		p->next = q->next;
		free(q);
	}
	sem_post(&sem_producer);
	if(!count){
		printf("您未在该群中\n");
		printf("按【enter】返回\n"); MYFLUSH;//getchar();
		return;
	}
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
				my_err("send", __LINE__);
	}
	printf("========================================\n");
	printf("******0(群主)1(管理员)2(普通成员)*******\n");
	printf("     %-16s群内成员           \n", groupname);
	printf("----------------------------------------\n");
	
	sem_wait(&sem_consumer); //相当于--	
	p = grpmsg;
	groupL q = grpmsg->next;
	while(p->next != NULL){
		p->next = q->next ;
		printf("     %-16s%-7d           \n", q->name, q->status);
		//usleep(1000000);
		free(q);
		q = p->next;
	}

	printf("========================================\n");
	sem_post(&sem_producer);
	printf("按【enter】返回\n"); MYFLUSH;//getchar();	
}

/*
函数功能：解散群
参数：描述符，报头
*/
void group_destroy(int* sockfd, char* msg){
	char groupname[30] = {0};
	printf("请输入要解散的群名：\n"); //getchar();
	fgets(groupname, sizeof(groupname), stdin); 
	groupname[strlen(groupname) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

/*
函数功能：设置群管理员
参数：
*/
void group_set_mager(int* sockfd, char* msg){
	char groupname[30] = {0}; char manager[30] = {0};
	printf("请输入要设置管理员的群名称：\n"); //getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	printf("请输入要设置为管理员的用户名称：\n"); //getchar();
	fgets(manager, sizeof(manager), stdin);
	manager[strlen(manager) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s %s\n", groupname, manager);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

/*
函数功能：踢人
参数：
*/
void group_delete_per(int* sockfd, char* msg){
	char member[30] = {0}; char groupname[30] = {0};
	printf("请输入踢人的群名：\n");// getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	printf("请输入要踢出群的成员名：\n"); 
	fgets(member, sizeof(member), stdin);
	member[strlen(member) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s %s\n", groupname, member);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}

}

/*
函数功能：群聊
参数：
*/
void group_chat(int* sockfd, char* msg){
	char groupname[30] = {0};char msgcp[1024] = {0};
	
	printf("请输入进入聊天的群名称\n");// getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
	sem_wait(&sem_consumer); //等待发来群在线成员名
	groupL p = grpmsg->next;
	// int count = 0;
	if(!p){
		// count = 1;
		sem_post(&sem_producer);
		printf("该群无在线成员或您未加入该群\n");
		printf("按【enter】返回\n");MYFLUSH;// getchar(); 
		return;
	}
	Flag = 1;
	printf("==========================================\n");
	printf("          <%s> 群聊界面\n", groupname);
	printf("*****在线成员如下均可接受消息***********\n");
	printf("--------------quit(退出)-------------------\n");
	p = grpmsg;
	while(p->next != NULL){
		groupL q = p->next;
		printf("          %s\n", q->name);
		p->next = q->next;
		free(q);
		q = p->next;
	}
	sem_post(&sem_producer);
	printf("------------------------------------------\n");
	char buf[900] = {0};
	do{
		memset(buf, 0, sizeof(buf));
		read(STDIN_FILENO, buf, sizeof(buf));
		buf[strlen(buf) - 1] = '\0';
		if(!strncmp(buf, "quit", 4) && (strlen(buf) == 4)){
			break;
		}
		memset(msgcp, 0, sizeof(msgcp));
		sprintf(msgcp, "%s %s %s %s %s\n", "group:", "againtalk", name, groupname, buf);
		if(send(*sockfd, msgcp, strlen(msgcp), 0) < 0){
			my_err("send", __LINE__);
		}
		
	}while(strncmp(buf, "quit", 4) || !(strlen(buf) == 4));
	Flag = 0;
}

/*
函数功能：查看群聊记录
参数
*/
void group_history(int* sockfd, char* msg){
	char groupname[30] = {0};
	printf("请输入查看聊天记录的群名：\n"); //getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
	sem_wait(&sem_consumer);
	MSGL p = historyHead;
	if(!(p->next)){
		sem_post(&sem_producer);
		printf("%s群不存在或无历史记录，按【enter】退出\n", groupname);
		MYFLUSH; //getchar();
		return;
	}
	printf("==================================\n");
	printf("       %s 群聊记录\n", groupname);
	printf("----------------------------------\n");
	MSGL q = historyHead->next;
	while(p->next != NULL){
		p->next = q->next ;
		write(STDOUT_FILENO, q->msg, strlen(q->msg));
		// printf("%s\n", q->msg);
		free(q);
		q = p->next;
	}
	sem_post(&sem_producer);
	printf("按【enter】返回\n"); MYFLUSH; //getchar();
}

/*
函数功能：群管理
参数：描述符，用户名
*/
void user_group_mag(int* sockfd, char* username){
	char msg[1024] = {0};				
	while(1){
		memset(msg, 0, sizeof(msg));
	
		system("clear");
		printf("-----------------------------------------------------------\n");
		printf("           welcome to qq Group MAG Interface              \n");
		printf("请输入   1(群的创建)\t\t2(群聊)\n         3(群的解散)\t\t4(加群)\n         5(查看群组信息)\t6(退群)\n         7(查看聊天记录)\t8(设置群管理员)\n         9(踢人)\t\ta(处理消息)\n\t 0(退出)\n");
		char sel;
		read(STDIN_FILENO, &sel, 1); MYFLUSH;
		system("clear");		
		if(sel == '0'){
			break;
		}else if(sel == '2'){
			
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "talk", username);
			group_chat(sockfd, msg);
		}else if(sel == '1'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "create", username);
			group_create(sockfd, msg);
		}else if(sel == '3'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "destroy", username);
			group_destroy(sockfd, msg);
		}else if(sel == '4'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "add", username);
			group_add(sockfd, msg);
		}else if(sel == '5'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s\n", "group:", "message", username);
			if(send(*sockfd, msg, strlen(msg), 0) < 0){
				my_err("send", __LINE__);
			}
			group_information(sockfd, username);
		}else if(sel == '6'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "out", username);
			group_out(sockfd, msg);
		}else if(sel == '7'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "record", username);
			group_history(sockfd, msg);
		}else if(sel == '8'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "mager", username);
			group_set_mager(sockfd, msg);
		}else if(sel == '9'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "group:", "kick", username);
			group_delete_per(sockfd, msg);
		}else if(sel == 'a'){
			do_service(sockfd);
		}
	}
}

/*
函数功能：解析文件名
函数参数：文件路径，路劲长度，文件名
*/
static void encode_filename(char* path, int size, char* filename1){
	
	char *filename = NULL;
	while(strchr(path, '/') != (char*)0){
		for(int i = 0; *path != '\0' && i < size; i++){
			if(*path == '/'){
				*path++;
				filename = path;
				
				size = strlen(path);
				break;
			}
			*path++;
		}
	}
	filename[strlen(filename)] = '\0';

	strncpy(filename1, filename, strlen(filename));
}
/*
函数功能：发送用户文件
*/
void user_send_file(char* msg, int *sockfd){
	//加锁
	sem_wait(&sem_consumer); //相当于--
	//查找私聊对象是否为好友及在线
	friendL p = frimsg; int count = 0;
	while(p->next != NULL){
		friendL q = p->next;
		if(q->status){
			printf("|%10s|", q->friname);
			count++;
			if(!(count % 2)){
				printf("\n");
			}
		}
		p->next = q->next;
	}
	if(count % 2){
		printf("\n");
	}
	sem_post(&sem_producer);

	if(!count){ //非好友
		printf("您的好友均未上线!按【enter】退出\n"); MYFLUSH;
		return;
	}
	
	char friname[30] = {0}; char mail[1016] = {0};
	char msgcp[128] = {0}; strncpy(msgcp, msg, strlen(msg));
	printf("请输入发送文件的好友名：\n");
	fgets(friname, sizeof(friname), stdin);
	friname[strlen(friname) - 1] = '\0';
	char path[50] = {0};static char filename[40] = {0}; //char* filename = (char*)calloc(40, sizeof(char));
	printf("请输入文件绝对路径：\n");
	fgets(path, sizeof(path), stdin);
	path[strlen(path) - 1] = '\0';
	// printf("%s\n", path);
	int fd = open(path, O_RDONLY);
	if(fd < 0){
		perror("open error"); getchar();
		return;//exit(1);
	}
	encode_filename(path, strlen(path), filename);
	// printf("%s %d\n", filename, __LINE__); getchar();
	ssize_t size = 0; int sizesum = 0; count = 0;
	while((size = read(fd, mail, sizeof(mail))) > 0){
		sizesum += size;
		count++;
		// size1 += size;
	} 
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "%s %s %s %d\n",msgcp, friname, filename, sizesum + 8);
	if(send(*sockfd, msg, strlen(msg), 0) != strlen(msg)){
		fprintf(stderr,"write error: %s\n",strerror(errno));
		exit(1);
	}
	lseek(fd, 0, SEEK_SET);//sleep(1);
	char *mailsum = (char *)calloc(1024, sizeof(char));
	memset(mail, 0, sizeof(mail));
	sprintf(mailsum, "fileone ");int i = 0;
	while(read(fd, mailsum + 8, 1016) > 0){
		// my_err("read", __LINE__);
		usleep(drand48() * 1000000);
		i++;
		if(i == count){
			if(write(*sockfd, mailsum, sizesum % 1016 + 8) < 0){
				my_err("write", __LINE__);
			}
			// printf("%s\n", mailsum);			
			break;
		}
		if(write(*sockfd, mailsum, strlen(mailsum)) < 0){
			my_err("write", __LINE__);
		}
		// printf("%s\n", mailsum);
		memset(mailsum, 0, sizeof(mailsum));
		sprintf(mailsum, "fileone ");
	}
	
	// getchar();
}
/*
函数功能：发送文件
函数参数：描述符，用户名
*/
void file_send(int* sockfd, char* username){
	while(1){
		system("clear");
		printf("---------------------------------------------------------\n");
		printf("           welcome to qq File MAG Interface              \n");
		printf("请输入\t1(发送用户文件)\t0(退出)\n");
		char sel; char msg[1024] = {0};
		if(read(STDIN_FILENO, &sel, 1) <= 0){
				my_err("read", __LINE__);
		} MYFLUSH;
		system("clear");
		if(sel == '1'){
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s$", "friend:", "search", username);
			if(send(*sockfd, msg, strlen(msg), 0) < 0){
				my_err("send", __LINE__);
			}
			
			memset(msg, 0, sizeof(msg));
			sprintf(msg, "%s %s %s", "file:", "usersend", username);
			user_send_file(msg, sockfd);
		}else if(sel == '0'){
			break;
		}
	}	
			
}

/*
函数功能：接受文件
参数：接收次数,发送者姓名
*/
void file_receive(char* filename, char* mail){
	char path[60] = {0};
	sprintf(path, "./chatfile/%s", filename);

	int fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if(fd < 0){
		perror("open error");
	}
	if(write(fd, mail, strlen(mail)) < 0){
		perror("Write error");
	}
}

/*
函数功能：好友离线消息
函数参数：
*/
void user_offline_mag(int* sockfd, char* msg){
	char friname[30] = {0};
	printf("请输入离线好友姓名：\n"); //getchar();
	fgets(friname, sizeof(friname), stdin);
	friname[strlen(friname) - 1] = '\0';
	printf("请输入要发送消息：\n");
	char msg1[512] = {0};
	fgets(msg1, sizeof(msg1), stdin);
	msg1[strlen(msg1) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s %s\n", friname, msg1);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
}

/*
函数功能：群离线消息发送
函数参数：
*/
void group_offline_mag(int* sockfd, char* msg){
	char groupname[30] = {0}; char mail[512] = {0};
	printf("请输入发送离线消息的群名：\n");// getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';	
	printf("请输入发送的消息：\n");
	fgets(mail, sizeof(mail), stdin);
	mail[strlen(mail) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s %s\n", groupname, mail);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}

}

/*
函数功能：接受离线消息
*/
void recv_offline_msg(){
	MSGL p = offlineMsg;
	MSGL q = offlineMsg->next;
	printf("==========================================\n");
	while(p->next != NULL){
		p->next = q->next;
		write(STDIN_FILENO, q->msg, strlen(q->msg));
		free(q);
		q = p->next;
	}
	printf("按【enter】退出\n"); MYFLUSH;//getchar(); getchar();
}

/*
函数功能：查看群留言
*/
void search_offline_msg(int *sockfd, char *msg){
	char groupname[30] = {0}; 
	printf("请输入查看的群名：\n"); //getchar();
	fgets(groupname, sizeof(groupname), stdin);
	groupname[strlen(groupname) - 1] = '\0';
	sprintf(msg + strlen(msg), " %s\n", groupname);
	if(send(*sockfd, msg, strlen(msg), 0) < 0){
		my_err("send", __LINE__);
	}
	sem_wait(&sem_consumer);
	system("clear");
	printf("==========================================\n");
	printf("        群留言界面\n");
	printf("------------------------------------------\n");
	MSGL p = offlineMsg;
	MSGL q = offlineMsg->next;
	while(p->next != NULL){
		p->next = q->next ;
		write(STDOUT_FILENO, q->msg, strlen(q->msg));
		free(q);
		q = p->next;
	}
	sem_post(&sem_producer);
	printf("按【enter】退出\n");MYFLUSH;// getchar(); //getchar();
}


/*
函数功能：发送离线文件
*/
void user_offline_file(int* sockfd, char* msg){
	char friname[30] = {0}; char mail[1024] = {0};
	char msgcp[256] = {0}; strncpy(msgcp, msg, strlen(msg));
	printf("请输入离线好友姓名：\n");
	fgets(friname, sizeof(friname), stdin);
	friname[strlen(friname) - 1] = '\0';
	// char username[30] = {0}; strncpy(username, name, strlen(name));
	char path[50] = {0};static char filename[40] = {0}; //char* filename = (char*)calloc(40, sizeof(char));
	printf("请输入文件绝对路径：\n");
	fgets(path, sizeof(path), stdin);
	path[strlen(path) - 1] = '\0';
	// printf("%s\n", path);
	int fd = open(path, O_RDONLY);
	if(fd < 0){
		perror("open error"); getchar();
		return;//exit(1);
	}
	encode_filename(path, strlen(path), filename);
	// printf("%s %d\n", filename, __LINE__); getchar();
	ssize_t size = 0; int count = 0;// srand(time(NULL));
	static char msg1[2000] = {0};
	while((size = read(fd, mail, sizeof(mail))) > 0){
		memset(msg1, 0, sizeof(msg1)); usleep(drand48() * 1000000);// printf("%s\n", mail); getchar();
		sprintf(msg1, "%s %s %s\n %s`", msgcp, friname, filename, mail);
		printf("%s %s\n", friname, filename);
		printf("%s\n", mail); getchar();
		if(write(*sockfd, msg1, strlen(msg1)) < 0){
			my_err("write", __LINE__);
		}
		memset(mail, 0, sizeof(mail));
	}
	

}

/*
函数功能：接受离线文件
参数：
*/
void recv_offline_file(){	
	char filename[30] = {0}; char sender[30] = {0};
	char timebuf[20] = {0}; char mail[900] = {0}; char mail1[900] = {0};
	sem_wait(&sem_consumer);
	// char *sender1 = NULL; //char* mail1 = NULL;
	// char *filename1 = NULL; char* timebuf1 = NULL;	
	MSGL q = offlineFile;
	if(!q->next){
		printf("按【enter】返回\n"); MYFLUSH;
		sem_post(&sem_producer);
		return;
	}
	MSGL p = offlineFile->next;
	printf("您的文件将被存放在./chatfile目录中\n");
	int fd = 0;
	while(q->next != NULL){
		q->next = p->next;
		sscanf(p->msg, "%[^ ] %[^ ] %[^《] %[^齉]", sender, filename, timebuf, mail1);
		// mail+=3;
		strncpy(mail, mail1+3, strlen(mail1) - 3); 
		char path[60] = {0};
		sprintf(path, "./chatfile/%s", filename);
		if(!fd){ //第一次进入
			if(!access(path, F_OK)){ //文件存在
				srand(time(NULL));
				sprintf(path+ strlen(path), "%d", rand() % 10);
			}
		}
		
		fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
		if(fd < 0){
			perror("open error");
		}else{
			int size = 0;
			if((size = write(fd, mail, strlen(mail))) != strlen(mail)){
				perror("Write error");
			}
		}
		free(p);
		p = q->next;
	}
	sem_post(&sem_producer);
	if(fd < 0){
		printf("按【enter】返回\n"); MYFLUSH;
		return;
	}
	printf("文件由%s在%s发送已成功接收,按【enter】返回\n", sender, timebuf); MYFLUSH;
}

/*
函数功能：离线传输
函数参数：描述符，用户名
*/
void trans_leave(int* sockfd, char* username){
	while(1){
		system("clear");
		printf("---------------------------------------------------------\n");
		printf("     welcome to qq Offline MSG MAG Interface              \n");
		printf("请输入\t1(发送好友消息)\t2(发送群消息)\n        3(发送好友文件)\t0(退出)\n      \t5(接收离线消息)\t6(查看群留言)\n      \t7(接收好友文件)\n");
		char sel; char msg[1024] = {0};
		if(read(STDIN_FILENO, &sel, 1) <= 0){
				my_err("read", __LINE__);
		} MYFLUSH;
		system("clear");
		if(sel == '1'){
			sprintf(msg, "%s %s %s", "trans:", "Fmsg", username);
			user_offline_mag(sockfd, msg);
		}else if(sel == '2'){
			sprintf(msg, "%s %s %s", "trans:", "Grpmsg", username);
			group_offline_mag(sockfd, msg);
		}else if(sel == '3'){
			sprintf(msg, "%s %s %s", "trans:", "Ffile", username);
			user_offline_file(sockfd, msg);
		}else if(sel == '5'){
			recv_offline_msg();
		}else if (sel == '6'){
			char *msg = (char*)calloc(1024, sizeof(char));
			sprintf(msg, "%s %s %s", "trans:", "Searchmsg", username);
			search_offline_msg(sockfd, msg);
		}else if(sel == '7'){
			sprintf(msg, "%s %s %s\n", "trans:", "RCfile", username);			
			if(send(*sockfd, msg, strlen(msg), 0) < 0){
				my_err("send", __LINE__);
			}
			recv_offline_file();			
		}else if(sel == '0'){
			break;
		}
	}
}

/*
事物：接受消息
*/
int logSig = 0;
void* receive(void* arg){
	//printf("%s\n", __FUNCTION__);
	int sockfd = *(int*)arg;
	char tmp[2000] = {0}; //
	//create(&head);
	head = (MSGL)malloc(sizeof(MSG));
	assert(head != NULL); head->next = NULL;	
	historyHead = (MSGL)malloc(sizeof(MSG));
	assert(historyHead != NULL); historyHead->next = NULL;
	frimsg = (friendL)malloc(sizeof(friend));
	assert(frimsg != NULL); frimsg->next = NULL;
	grpmsg = (groupL)malloc(sizeof(Group));
	assert(grpmsg != NULL); grpmsg->next = NULL;
	offlineMsg = (MSGL)malloc(sizeof(MSG));
	assert(offlineMsg != NULL); offlineMsg->next = NULL;
	offlineFile = (MSGL)malloc(sizeof(MSG)); offlineFile->next = NULL;
	assert(offlineFile != NULL);
	while(1){			
		if(recv(sockfd, tmp, sizeof(tmp), 0) < 0){
			//printf("errno: %d\n", errno);
			my_err("recv", __LINE__);
		}
		// printf("%s\n", tmp);
		// printf("%s %d\n", tmp, __LINE__);
		char *sec = "$friendhistory"; char *sen = "#groupname:";		
		char *thr = "#groupmember:"; char *five = "&grouptalk:";		
		char *ten = "$groupagaintalk"; char *ele = "$offfrimsg";
		char *twel = "&offsearch";//printf("%s %d\n", tmp, __LINE__);
		char *ano = "#offFile";
		if(!strncmp(tmp, "~friendsearch", 13)){
			friendL q = frimsg;
			friendL p = (friendL)malloc(sizeof(friend));
			assert(p != NULL);
			char friMsg[512] = {0}; char log[20] = {0};
			 int status = 0; 
			sscanf(tmp, "%[^ ] %[^~]", log, friMsg);
			sprintf(friMsg + strlen(friMsg), "~");
			//printf("%s\n",friMsg);
			while(strlen(friMsg)){
				friendL p = (friendL)malloc(sizeof(friend)); char friMsg1[512] = {0};
				char stu[10] = {0}; char friname[30] = {0};
				sscanf(friMsg, "%[^ ] %[^\r\n] %[^~]", friname, stu, friMsg1);
				strncpy(p->friname, friname, strlen(friname));
				status = atoi(stu);
				p->status = status;
				memset(friname, 0, sizeof(friname));
				status = 0;
				memset(friMsg, 0, sizeof(friMsg));
				memcpy(friMsg, friMsg1, strlen(friMsg1));
				sprintf(friMsg + strlen(friMsg), "~");
				//printf("%s\n", friMsg1);
				while(q->next != NULL){
					q = q->next;
				}
				q->next = p;
				p->next = NULL;
				if(strlen(friMsg) == 1){
					break;
				}
				memset(friMsg1, 0, sizeof(friMsg1));
			}	
			
				//解锁
			sem_post(&sem_consumer); 
			sem_wait(&sem_producer);
			
		}else if(!strncmp(tmp, "~friendchat", 11)){
			char buf[1024] = {0}; char log[20] = {0}; char friName[30] = {0};
			sscanf(tmp, "%[^ ] %[^ ] %[^\r\n]", log, friName, buf); 
			//printf("%s\n", buf);
			if(!Flag){
				MSGL p = (MSGL)malloc(sizeof(MSG));
				assert(p != NULL);

				sprintf(p->msg, "%s", tmp);
				//printf("%s\n", p->msg);
				MSGL q = head;
				assert(head != NULL);
				while(q->next != NULL){			
					q = q->next;
				}
				q->next = p;
				p->next = NULL;

				printf("您有条消息待处理\n");

			}else{
				printf("%25s <%s\n", buf, friName);
			}
		}else if(!strncmp(tmp, "$friendhistory", strlen(sec))){			
			char log[20] = {0};char buf[1024] = {0};
			char sender[30] = {0}; int count = 0;
			int row = 0; char sRow[5] = {0};
			if(!count){
				sscanf(tmp, "%[^ ] %[^\r\n]", log, sRow);
				row = atoi(sRow);
				// printf("%d\n", row);
				count++;
			}
			
				for(int i = 0; i < row; i++){
					memset(tmp, 0, sizeof(tmp));
					if(recv(sockfd, tmp, sizeof(tmp), 0) < 0){
						//printf("errno: %d\n", errno);
						my_err("recv", __LINE__);
					}
					// printf("recv: %s\n", tmp);
					MSGL p = (MSGL)malloc(sizeof(MSG));
					assert(p != NULL);
					MSGL q = historyHead;
					strncpy(p->msg, tmp, strlen(tmp));
					
					while(q->next != NULL){
						q = q->next;
					}
					q->next = p;
					p->next = NULL;
				
			}
			sem_post(&sem_consumer);
			sem_wait(&sem_producer);
		}else if(!strncmp(tmp, "#groupmember:", strlen(thr))){
			char log[20] = {0}; char grpcp[1024] = {0};
			sscanf(tmp, "%[^:] %[^#]", log, grpcp);
			memcpy(grpcp, grpcp + 1, strlen(grpcp) - 1);
			while(strlen(grpcp)){
				groupL p = (groupL)malloc(sizeof(Group)); char grpcp1[1024] = {0};
				char grpmem[30] = {0};	char sta[5] = {0};
				sscanf(grpcp, "%[^ ] %[^\r\n] %[^#]", grpmem, sta, grpcp1);				
				strncpy(p->name, grpmem, strlen(grpmem));
				p->status = atoi(sta);
				memset(grpmem, 0, sizeof(grpmem));
				groupL q = grpmsg;
				memset(grpcp, 0, sizeof(grpcp));
				memcpy(grpcp, grpcp1, strlen(grpcp1));
				sprintf(grpcp + strlen(grpcp), "#");
				
				//printf("%s\n", friMsg1);
				while(q->next != NULL){
					q = q->next;
				}
				q->next = p;
				p->next = NULL;
				if(!strlen(grpcp1)){
					break;
				}
				memset(grpcp1, 0, sizeof(grpcp1));
			}
			
			//解锁
			sem_post(&sem_consumer); 
			sem_wait(&sem_producer);
		}else if(!strncmp(tmp, "#groupname:", strlen(sen))){
			char log[20] = {0}; char grpcp[1024] = {0};
			sscanf(tmp, "%[^:] %[^@]", log, grpcp);
			memcpy(grpcp, grpcp + 1, strlen(grpcp) - 1);
			char *four = ".nogroup\r\n";
			if(!strncmp(grpcp, ".nogroup\r\n", strlen(four))){
				
				memset(grpcp, 0, sizeof(grpcp));
			}
			//printf("%s\n", grpcp);
			while(strlen(grpcp)){
				groupL p = (groupL)malloc(sizeof(Group)); char grpcp1[1024] = {0};
				char grpname[30] = {0};	
				sscanf(grpcp, "%[^\r\n] %[^@]", grpname, grpcp1);				
				//printf("%s %s %d\n", grpname, grpcp1, __LINE__);
				strncpy(p->name, grpname, strlen(grpname));
				memset(grpname, 0, sizeof(grpname));
				groupL q = grpmsg;
				memset(grpcp, 0, sizeof(grpcp));
				memcpy(grpcp, grpcp1, strlen(grpcp1));
				sprintf(grpcp + strlen(grpcp), "@");
				
				//printf("%s\n", friMsg1);
				while(q->next != NULL){
					q = q->next;
				}
				q->next = p;
				p->next = NULL;
				if(!strlen(grpcp1)){
					break;
				}
				memset(grpcp1, 0, sizeof(grpcp1));
			}
			//解锁
			sem_post(&sem_consumer); 
			sem_wait(&sem_producer);
		}else if(!strncmp(tmp, five, strlen(five))){
			char log[20] = {0}; char grpcp[1024] = {0};
			sscanf(tmp, "%[^:] %[^>]", log, grpcp);
			memcpy(grpcp, grpcp + 1, strlen(grpcp) - 1);
			char* six = ".nomember\r\n";
			//printf("%s\n", grpcp);
			if(!strncmp(six, grpcp, strlen(six))){
				//printf("%s\n", grpcp);
			
				memset(grpcp, 0, sizeof(grpcp));
			}
			while(strlen(grpcp)){
				groupL p = (groupL)malloc(sizeof(Group)); char grpcp1[1024] = {0};
				char grpname[30] = {0};	
				sscanf(grpcp, "%[^\r\n] %[^>]", grpname, grpcp1);				
				//printf("%s %s %d\n", grpname, grpcp1, __LINE__);
				strncpy(p->name, grpname, strlen(grpname));
				memset(grpname, 0, sizeof(grpname));
				groupL q = grpmsg;
				memset(grpcp, 0, sizeof(grpcp));
				memcpy(grpcp, grpcp1, strlen(grpcp1));
				sprintf(grpcp + strlen(grpcp), ">");
				
				//printf("%s\n", friMsg1);
				while(q->next != NULL){
					q = q->next;
				}
				q->next = p;
				p->next = NULL;
				if(!strlen(grpcp1)){
					break;
				}
				memset(grpcp1, 0, sizeof(grpcp1));
			}
			//解锁
			sem_post(&sem_consumer); 
			sem_wait(&sem_producer);
		}else if(!strncmp(tmp, ten, strlen(ten))){
			char mail[1024] = {0}; char log[24] = {0}; 
			char friName[30] = {0}; char groupname[30] = {0};
			sscanf(tmp, "%[^ ] %[^ ] %[^ ] %[^+_+]", log, friName, groupname, mail);
			if(!Flag){
				MSGL p = (MSGL)malloc(sizeof(MSG));
				assert(p != NULL);

				sprintf(p->msg, "%s", tmp);
				//printf("%s\n", p->msg);
				MSGL q = head;
				assert(head != NULL);
				while(q->next != NULL){			
					q = q->next;
				}
				q->next = p;
				p->next = NULL;

				printf("您有条消息待处理\n");

			}else{
				printf("%25s <%s\n", mail, friName);
			}	
		}else if(!strncmp(tmp, "*grouprecord", 12)){
			char log[20] = {0}; char num[5] = {0};
			sscanf(tmp, "%[^ ] %[^\r\n]", log, num);
			
			int count = atoi(num);
			
			for(int i = 0; i < count; i++){
				memset(tmp, 0, sizeof(tmp));
				if(recv(sockfd, tmp, sizeof(tmp), 0) < 0){
					my_err("recv", __LINE__);
				}
				// printf("%s\n", tmp);
				MSGL p = historyHead;
				MSGL q = (MSGL)malloc(sizeof(MSG));
				assert(q != NULL);
				strncpy(q->msg, tmp, strlen(tmp));
				while(p->next != NULL){
					p = p->next;
				}
				p->next = q;
				q->next = NULL;
			}
			//解锁
			sem_post(&sem_consumer); 
			sem_wait(&sem_producer);	
		}else if(!strncmp(tmp, ele, strlen(ele))){
			char log[20] = {0}; char grpcp[1024] = {0}; char num[5] = {0};
			sscanf(tmp, "%[^ ] %[^\r\n]", log, num);
			int count = atoi(num);
			for(int i = 0; i < count; i++){
				memset(tmp, 0, sizeof(tmp));
				if(recv(sockfd, tmp, sizeof(tmp), 0) < 0){
					my_err("recv", __LINE__);
				}
				// printf("%s\n", tmp);
				MSGL p = offlineMsg;
				MSGL q = (MSGL)malloc(sizeof(MSG));
				assert(q != NULL);
				strncpy(q->msg, tmp, strlen(tmp));
				while(p->next != NULL){
					p = p->next;
				}
				p->next = q;
				q->next = NULL;
			}
			while(!logSig){
				usleep(drand48() * 1000);
			}
			if(offlineMsg->next != NULL){
				printf("您有离线消息待接收\n");
			}
			
		}else if(!strncmp(tmp, twel, strlen(twel))){
			char log[20] = {0}; char num[5] = {0};
			sscanf(tmp, "%[^ ] %[^\r\n]", log, num);
			int count = atoi(num);
			for(int i = 0; i < count; i++){
				MSGL p = (MSGL)malloc(sizeof(MSG));
				assert(p != NULL); memset(tmp, 0, sizeof(tmp));
				if(recv(sockfd, tmp, sizeof(tmp), 0) < 0){
					my_err("recv", __LINE__);
				}
				strncpy(p->msg, tmp, strlen(tmp));
				MSGL q = offlineMsg;
				while(q->next != NULL){
					q = q->next;
				}
				q->next = p;
				p->next = NULL;
			}
			sem_post(&sem_consumer);
			sem_wait(&sem_producer);	
		}else if(!strncmp(tmp, ano, strlen(ano))){
			char num[5] = {0}; char log[10] = {0}; //char mail[1024] = {0};
			sscanf(tmp, "%[^ ] %[^\r\n]", log, num);
			int count = atoi(num);
			printf("%d\n", count);
			for(int i = 0; i < count; i++){
				memset(tmp, 0, sizeof(tmp));
				if(read(sockfd, tmp, sizeof(tmp)) < 0){
					my_err("read error", __LINE__);
				}
				MSGL p = offlineFile;
				MSGL q = (MSGL)malloc(sizeof(MSG));
				assert(q != NULL);
				strncpy(q->msg, tmp, strlen(tmp));
				while(p->next != NULL){
					p = p->next;
				}
				p->next = q;
				q->next = NULL;
			}
			sem_post(&sem_consumer);
			sem_wait(&sem_producer);

			// printf("%s\n", tmp);
			// char *tmp1 = tmp;char *mail1 = NULL;
			// char *ptr = NULL;
			// // sscanf(tmp, "%[^ ], %[^ \r\n$]", log, mail);
			
			// ptr = strtok_r(tmp1, " ", &mail1);
			// // ptr[1] = strtok_r(mail1, " ", &)
				
			// char mail[20] = {0};
			// if(strlen(mail1) < 20){
			// 	strncpy(mail, mail1, strlen(mail1));
			// }	
			// printf("%s\n", mail1);		
			// // sprintf(mail + strlen(mail), "\r\n$");
			// // printf("%s %s\n", log, mail);
			// if(!strncmp(mail1, "fail", 4)){
			// 	printf("您无离线文件\n");
			// }else{
			// 	MSGL p = offlineFile;
			// 	MSGL q = (MSGL)malloc(sizeof(MSG));
			// 	assert(q != NULL);
			// 	strncpy(q->msg, mail1, strlen(mail1));
			// 	while(p->next != NULL){
			// 		p = p->next;
			// 	}
			// 	p->next = q;
			// 	q->next = NULL;

			// }
		}else if(!strncmp(tmp, "fileone", 7)){
			char log[15] = {0};  char num[5] = {0}; char mail[1024] = {0};
			static char filename[30] = {0}; static char friname[30] = {0};
			sscanf(tmp, "%[^ ] %[^`] %[^ ] %[^\r\n]", log, mail, friname, filename);
			memset(tmp, 0, sizeof(tmp));
			
			// printf("%s\n", mail);
			file_receive(filename, mail);
			memset(mail, 0, sizeof(mail));
			printf("您的文件%s由%s发送将被存放在./chatfile目录中\n", filename, friname + 1);
		}else{
			MSGL p = (MSGL)malloc(sizeof(MSG));
			assert(p != NULL);

			sprintf(p->msg, "%s", tmp);
			//printf("%s\n", p->msg);
			MSGL q = head;
			assert(head != NULL);
			while(q->next != NULL){			
				q = q->next;
			}
			q->next = p;
			p->next = NULL;

			if(strlen(tmp) > 0){
				sleep(rand() % 3);
				printf("您有条消息待处理!\n");
			}				
		}
	}
}

void *mang(void* arg){
	int sockfd = *(int*)arg;
	char sel;
	
	while(1){
		system("clear");
		printf("------------------------------------------\n");
		printf("           welcome to qq MAG Interface              \n");
		printf("请输入\t1(好友管理)\t2(群管理)\n      \t3(发送文件)\t4(离线传输)\n      \t5(处理消息)\t0(退出)\n");
		
		logSig = 1; //登录标志
		if(read(STDIN_FILENO, &sel, 1) <= 0){
			my_err("read", __LINE__);
		} MYFLUSH;
			
		system("clear");
		if(sel == '1'){
			//char msg[256] = {"friend:"};
			user_friend_mag(&sockfd, name);			
		}else if(sel == '2'){
		//char msg[256] = {"group:"};
			user_group_mag(&sockfd, name);
		}else if(sel == '3'){
			// char msg[256] = {"file:"};
			file_send(&sockfd, name);
		}else if(sel == '4'){
			// char msg[256] = {"trans:"};
			trans_leave(&sockfd, name);
		}else if(sel == '0'){
			break;
		}else if(sel == '5'){
			do_service(&sockfd);
		}
	}
	return (void*)0;
}

int main(int argc, char **argv){
    
    if(argc < 2){
        printf("Usage: %s ip port\n", argv[0]);
        exit(1);
    }
    int port = htons(8888);
    if(argc == 3) port = htons(atoi(argv[2]));
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 2){
        my_err("socket", __LINE__);
    }       
    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));   
    memset(&client_addr, 0, sizeof(client_addr));
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr);  
    server_addr.sin_port = port;
    server_addr.sin_family = AF_INET;

    if(connect(sockfd, (struct sockaddr*)&server_addr,
     sizeof(struct sockaddr_in)) < 0){
         my_err("connect", __LINE__);
     }
    char sel;
	int ret = 0;//是否成功登录
	//char name[30] = {0};  //记录登录用户
	
	while(1)
	{
		printf("------------------------------------------\n");
		printf("           welcome to qq chat             \n");
		printf("请输入1(注册)，2(登录), 3(找回密码)\n");
		//scanf("%c", &select); MYFLUSH;
		read(STDIN_FILENO, &sel, 1); MYFLUSH;
		

		system("clear");
		if(sel == '1')
		{
			while(!user_init(&sockfd))
				printf("注册失败，请重新注册\n");
			while(!(ret = user_login(&sockfd, name)))	
			 	printf("登录失败请重新登录\n");
			break;
		}else if(sel == '2'){
			while(!(ret = user_login(&sockfd, name)))	
			 	printf("登录失败请重新登录\n");
			break;
		}else if(sel == '3'){
			int i = 2;
			for(i = 2; i>-1; i--){				
				if(!user_find(&sockfd)){
					if(i == 0) break;
					printf("找回密码失败，你还有%d次机会\n", i);
					continue;
				}
				break;
			}
		}
		
    }
	if(ret){		
		
		pthread_t thread1, thread2;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

			//初始化信号量
		sem_init(&sem_producer, 0, 0);
		sem_init(&sem_consumer, 0, 0);
		

		pthread_create(&thread1, &attr, receive, (void*)&sockfd);
		pthread_create(&thread2, NULL, mang, (void*)&sockfd);
		pthread_attr_destroy(&attr);
		//pthread_join(thread1, NULL);
		pthread_join(thread2, NULL);
		
		//释放信号量资源
		sem_destroy(&sem_producer);
		sem_destroy(&sem_consumer);
		if(head != NULL){
			free(head);
		}
		if(historyHead != NULL){
			free(historyHead);
		}
		if(frimsg != NULL){
			free(frimsg);
		}
		if(grpmsg != NULL){
			free(grpmsg);
		}
		if(offlineMsg != NULL){
			free(offlineMsg);
		}
	}
    return 0;
}

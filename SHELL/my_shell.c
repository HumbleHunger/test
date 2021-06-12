/*************************************************************************
	> File Name: my_shell.c
	> Author: 
	> Mail: 
	> Created Time: 2020年04月09日 星期四 21时33分47秒
 ************************************************************************/

#include<stdio.h>
#include"my_shell.h"
int main(char argc,char **argv)
{
    create(&q);
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);
    sigprocmask(SIG_BLOCK,&mask,NULL);
    char *buf=NULL;
    int argcount=0;
    char arglist[100][256];
    while(1){
        print_shell();
        buf=(char *)malloc(256);
        if(buf==NULL){
            printf("malloc failed");
            exit(-1);
        }
        memset(buf,0,256);
        get_input(buf);
        //printf("%s\n",buf);//调试
        if(strcmp(buf,"exit\n")==0 || strcmp(buf,"logout\n")==0){
            break;
        }
        argcount=0;
        memset(arglist,0,sizeof(arglist));
        for(int i=0;i<100;i++){
            arglist[i][0]=0;
        }
        explain_input(buf,&argcount,arglist);
        do_cmd(argcount,arglist);
    }
    del(&q);
}
void print_shell()
{
    printf("\e[1m\e[32mliuyuji@my_shell:\e[0m");
    char *buf=NULL;
    printf("\e[1m\e[34m%s\e[0m",getcwd(buf,0));
    free(buf);
    struct passwd *usrname;
	usrname = getpwuid(getuid());
    if(strcmp(usrname->pw_name,"root")==0)
        printf("# ");
	else
        printf("$ ");
}
void get_input(char *buf)
{
    char ch=getchar();
    int len=0;
    while(len<256 && ch!='\n'){
        buf[len++]=ch;
        ch=getchar();
    }
    if(len==256){
        printf("cmd too long\n");
        exit(-1);
    }
    buf[len++]='\n';
    buf[len]=0;
    insert(&q,buf);
}
void explain_input(char *buf,int *argcount,char arglist[100][256])
{
    int i=0;
    int j=0;
    int k=0;
    while(buf[i]!='\n'){
        if(buf[i]==' '){
            i++;
            continue;
        }
        while(buf[i]!=' ' && buf[i]!='\n'){
            arglist[j][k]=buf[i];
            i++;
            k++;
        }
        j++;
        *argcount=*argcount+1;
        k=0;
        //printf("%s\n",arglist[j-1]);
    }
}
void do_cmd(int argcount,char arglist[100][256])
{
    if(strcmp(arglist[0],"cd")==0){
        if(argcount==2){
            if(chdir(arglist[1])<0){
                printf("cd failed\n");
            }
            return;
        }
        else{
            printf("worry command\n");
            return;
        }
    }
    if(strcmp(arglist[0],"history")==0){
        history(&q);
        return;
    }
    char * arg[argcount+1];
    for(int i=0;i<argcount;i++){
        arg[i]=(char *)arglist[i];
    }
    arg[argcount]=NULL;
    int background=0;
    for(int i=0;i<argcount;i++){
        if(strncmp(arg[i],"&",1)==0){
            if(i==argcount-1){
                background=1;
                arg[argcount-1]=NULL;
                break;
            }
            else{
                printf("worry command\n");
                return;
            }
        }
    }
    int flag=0;
    int how=0;
    for(int i=0;arg[i]!=NULL;i++){
        if(strcmp(arg[i],">")==0){
            flag++;
            how=out_re;
            if(arg[i+1]==NULL || i==0){
                flag++;
            }
        }
        if(strcmp(arg[i],"<")==0){
            flag++;
            how=in_re;
            if(i==0 || arg[i+1]==NULL){
                flag++;
            }
        }
        if(strcmp(arg[i],">>")==0){
            flag++;
            how=out_rere;
            if(arg[i+1]==NULL || i==0){
                flag++;
            }
        }
        if(strcmp(arg[i],"<<")==0){
            flag++;
            how=in_rere;
            if(arg[i+1]==NULL || i==0){
                flag++;
            }
        }
        if(strcmp(arg[i],"|")==0){
            flag++;
            how=have_pipe;
            if(arg[i+1]==NULL){
                flag++;
            }
            if(i==0){
                flag++;
            }
        }
    }
    if(flag>1){
        printf("worry command\n");
        return;
    }
    char *file;
    if(how==out_re){
        for(int i=0;;i++){
            if(strcmp(arg[i],">")==0){
                file=arg[i+1];
                arg[i]=NULL;
                break;
            }
        }
    }
    if(how==in_re){
        for(int i=0;;i++){
            if(strcmp(arg[i],"<")==0){
                file=arg[i+1];
                arg[i]=NULL;
                break;
            }
        }
    }
    if(how==out_rere){
        for(int i=0;;i++){
            if(strcmp(arg[i],">>")==0){
                file=arg[i+1];
                arg[i]=NULL;
                break;
            }
        }
    }
    if(how==in_rere){
        for(int i=0;;i++){
            if(strcmp(arg[i],"<<")==0){
                file=arg[i+1];
                arg[i]=NULL;
                break;
            }
        }
    }
    char * argnext[argcount+1];
    if(how==have_pipe){
        for(int i=0;;i++){
            if(strcmp(arg[i],"|")){
                arg[i]=NULL;
                int j;
                for(j=i+1;arg[j]!=NULL;j++){
                    argnext[j-i-1]=arg[j];
                    i++;
                }
                argnext[j-i-1]=arg[j];
                break;
            }
        }
    }
    int pid;
    if( (pid=fork()) < 0){
        printf("fork worry\n");
        return;
    }
    int fd,fd2;
    switch(how){
        case 0:
        if(pid==0){
            if(!(find_command(arg[0]))){
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 1:
        if(pid==0){
            if(!(find_command(arg[0]))){
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            fd=open(file,O_RDWR|O_CREAT|O_TRUNC,S_IRWXU);
            dup2(fd,1);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 2:
        if(pid==0){
            if(!(find_command(arg[0]))){
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            fd=open(file,O_RDONLY,0644);
            dup2(fd,0);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 3:
        if(pid==0){
            int pid2;
            int status2;
            int fd2;
            if((pid2=fork())<0){
                printf("fork2 error\n");
                return;
            }
            if(pid2==0){
                if(!(find_command(arg[0]))){
                    printf("%s :command not found\n",arg[0]);
                    exit(0);
                }
                fd2=open("/tmp/donotknowfile",O_CREAT|O_WRONLY|O_TRUNC,0644);
                dup2(fd2,1);
                execvp(arg[0],arg);
                exit(0);
            }
            if(waitpid(pid2,&status2,0)==-1){
                printf("wait for child process error\n");
            }
            if(!find_command(argnext[0])){
                printf("%s : command not found\n",argnext[0]);
                exit(0);
            }
            fd2=open("/tmp/donotknofile",O_RDONLY);
            dup2(fd2,0);
            execvp(argnext[0],argnext);
            if(remove("/tmp/donotknofile")){
                printf("remove error\n");
            }
            exit(0);
        }
        break;
        case 4:
        if(pid==0){
            if(!(find_command(arg[0]))){
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            fd=open(file,O_RDWR|O_CREAT|O_APPEND,S_IRWXU);
            dup2(fd,1);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        case 5:
        if(pid==0){
            if(!(find_command(arg[0]))){
                printf("%s : command not found\n",arg[0]);
                exit(0);
            }
            fd=open(file,O_RDONLY);
            dup2(fd,0);
            execvp(arg[0],arg);
            exit(0);
        }
        break;
        default:
        break;
    }
    if(background==1){
        printf("[process id %d]\n",pid);
        return;
    }
    int status;
    if(waitpid(pid,&status,0)==-1){
        printf("wait for child process error\n");
    }
}
int find_command(char *command)
{
	DIR* dp;
	struct dirent* dirp;
	char* path[]={"./","/bin","/usr/bin",NULL};
	if(strncmp(command,"./",2)==0)
	{
		command=command+2;
	}

	int i=0;
	while(path[i]!=NULL)
	{
		if( (dp=opendir(path[i]))==NULL)
		{
			printf("can not open /bin \n");
		}
		while((dirp=readdir(dp))!=NULL)
		{
			if(strcmp(dirp->d_name,command)==0)
			{
				closedir(dp);
				return 1;
			}
		}
		closedir(dp);
		i++;
	}
	return 0;
}
void history(Queue *p)
{
    Node *operate=p->head;
    for(int i=1;i<=p->len;i++){
        printf("%d.%s",i,operate->date);
        operate=operate->next;
    }
}
int full(Queue *p)
{
    if(p->len==p->num){
        return 1;
    }
    else{
        return 0;
    }
}
void create(Queue *p)
{
    p->num=100;
    p->head=NULL;
    p->tail=NULL;
    p->len=0;
}
int insert(Queue *p,char *buf)
{
    if(full(p)==1){
        rm(p);
    }
    Node *operate,*record;
    operate=(Node *)malloc(sizeof(Node));
    if(p->head==NULL){
        p->head=operate;
    }
    else{
        record=p->tail;
        record->next=operate;
    }
    operate->date=buf;
    p->tail=operate;
    p->len++;

}
void rm(Queue *p)
{
    Node *operate=p->head;
    Node *record=operate->next;
    free(operate);
    p->head=record;
    p->len--;
}
void del(Queue *p)
{
    Node *operate,*record;
    operate=p->head;
    for(int i=0;i<p->len;i++){
        record=operate->next;
        free(operate->date);
        free(operate);
        operate=record;
    }
}

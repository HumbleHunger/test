/*************************************************************************
	> File Name: my_ls.c
	> Author: 
	> Mail: 
	> Created Time: 2020年02月29日 星期六 19时49分53秒
 ************************************************************************/
#include <stdio.h>
#include "my_ls.h"
int main(int argc,char *argv[])
{
    int k=0;
    int num=0;
    char param[32];
    int flag_p=PARAM_NONE;
    //记录参数
    for(int i=1;i<argc;i++){
        if(argv[i][0]=='-'){
            for(int j=1;j<strlen(argv[i]);j++){
                param[k]=argv[i][j];
                k++;
            }
        num++;//记录选项参数个数，方便确认是否指定目录
        }
    }
    //将参数转化为数字标记
    for(int i=0;i<k;i++){
        if(param[i]=='a'){
            flag_p|=PARAM_A;
            continue;
        }else if(param[i]=='l'){
            flag_p|=PARAM_L;
            continue;
        }else if(param[i]=='R'){
            flag_p|=PARAM_R;
            continue;
        }else if(param[i]=='r'){
            flag_p|=PARAM_r;
            continue;
        }else{
            printf("参数错误");
        }
    }
    param[k]=0;
    char path[PATH_MAX+1];//储存目录名
    //若未指定目录则执行
    if(num==argc-1){
        strcpy(path,"./");
        path[2]=0;
        display_dir(flag_p,path);
        if((flag_p & PARAM_R)!=0 && (flag_p & PARAM_L)==0){
            printf("\n");
        }
        return 0;
    }
    //若参数中含有目录项
    int i=1;
    struct stat buf;    
    do{
        if(argv[i][0]=='-'){
            i++;
            continue;
        }else{
            strcpy(path,argv[i]);
        }
        if(stat(path,&buf)==-1)
            my_err("stat",__LINE__);
        if(S_ISDIR(buf.st_mode)){
            if(path[strlen(argv[i])-1]!='/'){
                path[strlen(argv[i])]='/';
                path[strlen(argv[i])+1]='\0';
            }else
                path[strlen(argv[i])]='\0';
            display_dir(flag_p,path);
            i++;
         }
        else{
            display(flag_p,path,0);
            i++;
        }
    }while(i<argc);
    if((flag_p & PARAM_R)!=0 && (flag_p & PARAM_L)==0){
        printf("\n");
    }
    return 0;
}
void display_dir(int flag_p,char *path)
{
    //打开目录
    DIR *dir;
    if((dir=opendir(path))==NULL){
        my_err("opendir",__LINE__);
        return;
    }
    //获取该目录下文件总数和最长文件名的长度
    struct dirent *ptr;
    int count=0;
    while((ptr=readdir(dir))!=NULL){
        if(g_maxlen<strlen(ptr->d_name)){
            g_maxlen=strlen(ptr->d_name);
        }
        count++;
    }
    if(count>256){
        my_err("too many files under this directory",__LINE__);
    }
    closedir(dir);
    //获取该目录下所有文件名并与其路径名储存
    char **f_names;
    f_names=(char **)malloc(sizeof(char *)*20000);
    for(int i=0;i<20000;i++){
        f_names[i]=(char *)malloc(PATH_MAX+1);
    }
    int path_len=strlen(path);
    if((dir=opendir(path))==NULL){
        my_err("opendir",__LINE__);
    } 
    for(int i=0;i<count;i++){
        ptr=readdir(dir);
        strncpy(f_names[i],path,path_len);
        f_names[i][path_len]=0;
        strcat(f_names[i],ptr->d_name);
        f_names[i][path_len+strlen(ptr->d_name)]=0;
    }
    closedir(dir);
    //使用冒泡法对文件名排序
    char temp[PATH_MAX+1];
    for(int i=0;i<count-1;i++){
        for(int j=0;j<count-1-i;j++){
            if(strcmp(f_names[j],f_names[j+1])>0){
                strcpy(temp,f_names[j]);
                temp[strlen(f_names[j])]=0;
                strcpy(f_names[j],f_names[j+1]);
                f_names[j][strlen(f_names[j+1])]=0;
                strcpy(f_names[j+1],temp);
                f_names[j+1][strlen(temp)]=0;
            }
        }
    }
    //遍历并调用函数打印文件
    char **dirs;
    if((flag_p & PARAM_R)!=0){
        dirs=(char **)malloc(sizeof(char *)*20000);
        for(int i=0;i<20000;i++){
            dirs[i]=(char *)malloc(PATH_MAX+1);
        }
    }
    struct stat buf;
    int j=0;
    //若有-R选项先打印路径名
    if((flag_p & PARAM_R)!=0){
        printf("%s:\n",path);
    }
    //将行剩余空间设置为最大值
    g_leave_len=MAXROWLEN;
    if((flag_p & PARAM_r)==0){
        for(int i=0;i<count;i++){
            //如果参数含有-R选项
            if((flag_p & PARAM_R)!=0){
                //获取文件名
                int k=0;
                char name[NAME_MAX+1];
                for(int a=0;a<strlen(f_names[i]);a++){
                    if(f_names[i][a]=='/'){
                        k=0;
                        continue;
                    }
                    name[k]=f_names[i][a];
                    k++;
                }
                name[k]=0;
                //记录目录文件
                memset(&buf,0,sizeof(buf));
                if(lstat(f_names[i],&buf)==-1){
                    my_err("lstat",__LINE__);
                }
                if(S_ISDIR(buf.st_mode)){
                    if(name[0]!='.'){
                        strcpy(dirs[j],f_names[i]);
                        if(dirs[j][strlen(dirs[j])-1]!='/'){
                            dirs[j][strlen(dirs[j])]='/';
                            dirs[j][strlen(dirs[j])+1]=0;
                        }
                        j++;
                    }
                }
            }
            memset(&buf,0,sizeof(buf));
            if(lstat(f_names[i],&buf)==-1){
                my_err("lstat",__LINE__);
            }
            int color=display_color(buf);
            display(flag_p,f_names[i],color);
        }
    }
    if((flag_p & PARAM_r)!=0){
        for(int i=count-1;i>=0;i--){
            //如果参数含有-R选项
            if((flag_p & PARAM_R)!=0){
                //获取文件名
                int k=0;
                char name[NAME_MAX+1];
                for(int a=0;a<strlen(f_names[i]);a++){
                    if(f_names[i][a]=='/'){
                        k=0;
                        continue;
                    }
                    name[k]=f_names[i][a];
                    k++;
                }
                name[k]=0;
                //记录目录文件
                memset(&buf,0,sizeof(buf));
                if(lstat(f_names[i],&buf)==-1){
                    my_err("lstat",__LINE__);
                }
                if(S_ISDIR(buf.st_mode)){
                    if(name[0]!='.'){
                        strcpy(dirs[j],f_names[i]);
                        if(dirs[j][strlen(dirs[j])-1]!='/'){
                            dirs[j][strlen(dirs[j])]='/';
                            dirs[j][strlen(dirs[j])+1]=0;
                        }
                        j++;
                    }
                }
            }
            memset(&buf,0,sizeof(buf));
            if(lstat(f_names[i],&buf)==-1){
                my_err("lstat",__LINE__);
            }
            int color=display_color(buf);
            display(flag_p,f_names[i],color);
        }
    }
    //若参数中有-R选项
    if((flag_p & PARAM_R)!=0){
            display_Subdir(flag_p,j,dirs);
    }
    if((flag_p & PARAM_L)==0 && (flag_p & PARAM_R)==0){
        printf("\n");
    }
    free(f_names);
    free(dirs);
}
void display(int flag,char *pathname,int color)
{
    //从路径名中解析出文件名
    int j=0;
    char name[NAME_MAX+1];
    for(int i=0;i<strlen(pathname);i++){
        if(pathname[i]=='/'){
            j=0;
            continue;
        }
        name[j++]=pathname[i];
    }
    name[j]=0;
    //获取文件信息并储存
    struct stat buf;
    if(lstat(pathname,&buf)==-1){
        my_err("lstat",__LINE__);
    }
    //根据命令参数调用不同函数打印文件信息
    flag=flag & (~PARAM_r);
    switch(flag){
        case PARAM_NONE :
            if(name[0]!='.'){
                print_fname(name,color);
            }
            break;
        case PARAM_A :
            print_fname(name,color);
            break;
        case PARAM_L :
            if(name[0]!='.'){
                print_finfo(buf,name);
                printf("  ");
                print_color(name,color);
                printf("\n");
            }
            break;
        case PARAM_R :
            if(name[0]!='.'){
                print_fname(name,color);
            }
            break;
        case PARAM_A+PARAM_L :
            print_finfo(buf,name);
            printf("  ");
            print_color(name,color);
            printf("\n");
            break;
        case PARAM_A+PARAM_R :
            print_fname(name,color);
            break;
        case PARAM_L+PARAM_R :
            if(name[0]!='.'){
                print_finfo(buf,name);
                printf("  ");
                print_color(name,color);
                printf("\n");
            }
            break;
        case PARAM_A+PARAM_L+PARAM_R :
            print_finfo(buf,name);
            printf("  ");
            print_color(name,color);
            printf("\n");
            break;
    }
}
void print_fname(char *name,int color)
{
    //判断本行是否有充足空间打印
    if(g_leave_len<g_maxlen){
        printf("\n");
        g_leave_len=MAXROWLEN;
    }
    //对齐输出
    int len=g_maxlen-strlen(name);
    print_color(name,color);
    for(int i=0;i<len;i++){
        printf(" ");
    }
    printf("  ");
    //实时记录行剩余空间
    g_leave_len-=(g_maxlen+2);
}
void print_finfo(struct stat buf,char *name)
{
    //打印文件类型和文件访问权限
    if(S_ISLNK(buf.st_mode)){
        printf("l");
    }else if(S_ISREG(buf.st_mode)){
        printf("-");
    }else if(S_ISDIR(buf.st_mode)){
        printf("d");
    }else if(S_ISCHR(buf.st_mode)){
        printf("c");
    }else if(S_ISBLK(buf.st_mode)){
        printf("b");
    }else if(S_ISFIFO(buf.st_mode)){
        printf("f");
    }else if(S_ISSOCK(buf.st_mode)){
        printf("s");
    }
    if(buf.st_mode&S_IRUSR){
        printf("r");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IWUSR){
        printf("w");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IXUSR){
        printf("x");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IRGRP){
        printf("r");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IWGRP){
        printf("w");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IXGRP){
        printf("x");
    }else{
        printf("-");
    }
        if(buf.st_mode&S_IROTH){
        printf("r");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IWOTH){
        printf("w");
    }else{
        printf("-");
    }
    if(buf.st_mode&S_IXOTH){
        printf("x");
    }else{
        printf("-");
    }
    printf("  ");
    //打印硬链接数
    printf("%4ld ",buf.st_nlink);
    //打印文件所有者以及所在组
    struct passwd *psd=psd=getpwuid(buf.st_uid);//id转为名字
    struct group *grp=getgrgid(buf.st_gid);
    printf("%-8s",psd->pw_name);
    printf("%-8s",grp->gr_name);
    //打印文件大小
    printf("%6ld",buf.st_size);
    //打印文件创建时间
    char buf_time[32];
    strcpy(buf_time,ctime(&buf.st_mtime));
    buf_time[strlen(buf_time)-1]=0;
    printf("   %s",buf_time);
}
void display_Subdir(int flag_p,int j,char **dirs)
{
    for(int i=0;i<j;i++){
        if((flag_p & PARAM_L)==0){
            printf("\n");
        }
        display_dir(flag_p,dirs[i]);
    }
}
int display_color(struct stat buf)
{
    int color=0;
    if(S_ISDIR(buf.st_mode)) {
        color = BLUE;
    }
    if((buf.st_mode & S_IXUSR) && color != BLUE) {
        color = GREEN;
    }
    return color;
}
void print_color(char *name,int color)
{
    if(color == GREEN) {
        printf("\033[1m\033[32m%-s\033[0m",name);
    } else if(color == BLUE){
        printf("\033[1m\033[34m%-s\033[0m",name);
    } else if(color == NORMAL){
        printf("%-s",name);
    }
}
void my_err(const char *str,int line)
{
    fprintf(stderr,"line:%d",line);
    perror(str);
}

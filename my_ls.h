/*************************************************************************
	> File Name: my_ls.h
	> Author: 
	> Mail: 
	> Created Time: 2020年03月04日 星期三 20时41分37秒
 ************************************************************************/

#ifndef _MY_LS_H
#define _MY_LS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <linux/limits.h>
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
//采用二进制思想解析参数
#define PARAM_NONE 0
#define PARAM_A 1
#define PARAM_L 2
#define PARAM_R 4
#define PARAM_r 8
#define MAXROWLEN 200
#define NORMAL 0
#define GREEN  1 
#define BLUE   2
#define S_BLUE 3
#define YELLOW 4
int g_leave_len =MAXROWLEN;
int g_maxlen;
void display_dir(int flag_p,char *path);
void display(int flag,char *pathname,int color);
void print_fname(char *name,int color);
void print_finfo(struct stat buf,char *name);
void my_err(const char *str,int line);
void display_Subdir(int flag_p,int j,char **dirs);
int display_color(struct stat buf);
void print_color(char *name,int color);

#endif

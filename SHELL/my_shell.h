/*************************************************************************
	> File Name: my_shell.h
	> Author: 
	> Mail: 
	> Created Time: 2020年04月17日 星期五 20时10分25秒
 ************************************************************************/

#ifndef _MY_SHELL_H
#define _MY_SHELL_H

#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<dirent.h>
#include<pwd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef struct node{
    char *date;
    struct node *next;
}Node;
typedef struct queue{
    Node *head;
    Node *tail;
    int num;
    int len;
}Queue;
Queue q;

#define normal 0
#define out_re 1
#define in_re 2
#define have_pipe 3
#define out_rere 4
#define in_rere 5

void print_shell();
void get_input(char *);
void explain_input(char *,int *,char arglist[100][256]);
void do_cmd(int,char arglist[100][256]);
int find_command(char *);
void history(Queue *p);
int full(Queue *p);
void create(Queue *p);
int insert(Queue *p,char *buf);
void rm(Queue *p);
void del(Queue *p);

#endif

#ifndef __STD_H__
#define __STD_H__
#include <stdio.h>
#include<mysql/mysql.h>

extern void my_err(char *str, int line);
#define MYFLUSH ({int ch; while((ch = getchar())!= EOF && ch != '\n' && ch != '\r');})

#endif
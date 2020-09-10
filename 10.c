/*************************************************************************
	> File Name: 10.c
	> Author: 
	> Mail: 
	> Created Time: 2019年12月15日 星期日 15时56分05秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
void main()
{
    int i,a;
    int year,t,num,no,t1;
    scanf("%d",&a);
    struct {
        int n;
        char m[10];
        int y;
    }H[a];
    for(i=0;i<a;i++){
        scanf("%d. %s %d",&H[i].n,H[i].m,&H[i].y);
    }
    printf("%d\n",a);
    for(i=0;i<a;i++){
        t=0;
        if(strcmp(H[i].m,"pop")==0){
            t=H[i].n+1;
        }
        if(strcmp(H[i].m,"no")==0){
            t=20+H[i].n+1;
        }
        if(strcmp(H[i].m,"zip")==0){
            t=40+H[i].n+1;
        }
        if(strcmp(H[i].m,"zotz")==0){
            t=60+H[i].n+1;
        }
        if(strcmp(H[i].m,"tzec")==0){
            t=80+H[i].n+1;
        }
        if(strcmp(H[i].m,"xul")==0){
            t=100+H[i].n+1;
        }
        if(strcmp(H[i].m,"yoxkin")==0){
            t=120+H[i].n+1;
        }
        if(strcmp(H[i].m,"mol")==0){
            t=140+H[i].n+1;
        }
        if(strcmp(H[i].m,"chen")==0){
            t=160+H[i].n+1;
        }
        if(strcmp(H[i].m,"yax")==0){
            t=180+H[i].n+1;
        }
        if(strcmp(H[i].m,"zac")==0){
            t=200+H[i].n+1;
        }
        if(strcmp(H[i].m,"ceh")==0){
            t=220+H[i].n+1;
        }
        if(strcmp(H[i].m,"mac")==0){
            t=240+H[i].n+1;
        }
        if(strcmp(H[i].m,"kankin")==0){
            t=260+H[i].n+1;
        }
        if(strcmp(H[i].m,"muan")==0){
            t=280+H[i].n+1;
        }
        if(strcmp(H[i].m,"pax")==0){
            t=300+H[i].n+1;
        }
        if(strcmp(H[i].m,"koyab")==0){
            t=320+H[i].n+1;
        }
        if(strcmp(H[i].m,"cumhu")==0){
            t=340+H[i].n+1;
        }
        if(strcmp(H[i].m,"uayet")==0){
            t=360+H[i].n+1;
        }
        t1=t+H[i].y*365;
        char nod[10];
        num=t1%260%13;
        if(num==0){num=13;}
        no=t1%260%20;
        if(no==1){strcpy(nod,"imix");}
        if(no==2){strcpy(nod,"ik");}
        if(no==3){strcpy(nod,"akbal");}
        if(no==4){strcpy(nod,"kan");}
        if(no==5){strcpy(nod,"chicchan");}
        if(no==6){strcpy(nod,"cimi");}
        if(no==7){strcpy(nod,"manik");}
        if(no==8){strcpy(nod,"lamat");}
        if(no==9){strcpy(nod,"muluk");}
        if(no==10){strcpy(nod,"ok");}
        if(no==11){strcpy(nod,"chuen");}
        if(no==12){strcpy(nod,"eb");}
        if(no==13){strcpy(nod,"ben");}
        if(no==14){strcpy(nod,"ix");}
        if(no==15){strcpy(nod,"mem");}
        if(no==16){strcpy(nod,"cib");}
        if(no==17){strcpy(nod,"caban");}
        if(no==18){strcpy(nod,"eznab");}
        if(no==19){strcpy(nod,"canac");}
        if(no==0){strcpy(nod,"ahau");}
        year=(int)t1/260;
        printf("%d %s %d\n",num,nod,year);
    }
}

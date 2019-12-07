#include <stdio.h>
void main()
{
	int i,n;
	typedef struct{
			int y;
			int m;
			int d;
		}date;
	date a[2];
	for(i=0;i<2;i++){
		scanf("%d/%d/%d",&(a[i].y),&(a[i].m),&(a[i].d));
	}
	for(i=0;i<2;i++){
		if(a[i].y%4 != 0){
		switch(a[i].m){
			case 1:n=0;break;
                        case 2:n=31;break;
                        case 3:n=59;break;
                        case 4:n=90;break;
                        case 5:n=120;break;
                        case 6:n=151;break;
                        case 7:n=181;break;
                        case 8:n=212;break;
                        case 9:n=243;break;
                        case 10:n=273;break;
                        case 11:n=304;break;
                        case 12:n=334;break;
		}
		printf("%d\n",n+a[i].d);
		}
		else{
		switch(a[i].m){
                        case 1:n=0;break;
                        case 2:n=31;break;
                        case 3:n=60;break;
                        case 4:n=91;break;
                        case 5:n=121;break;
                        case 6:n=152;break;
                        case 7:n=182;break;
                        case 8:n=213;break;
                        case 9:n=244;break;
                        case 10:n=274;break;
                        case 11:n=305;break;
                        case 12:n=335;break;
		}
		printf("%d\n",n+a[i].d);
		}
	}
}

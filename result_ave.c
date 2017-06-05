#include<stdio.h>
#include<stdlib.h>

#define FOR_MAX 100

int main(){

    FILE *fp;
    char s[256];
    int num;
    double ave;

    if((fp = fopen("result.txt","r"))==NULL){
	printf("file open error\n");
	exit(1);
    }


    num=0;
    while(fgets(s,256,fp)!=NULL){

	num += atoi(s);
    
    }
    fclose(fp);

    ave = num/(double)FOR_MAX;
    printf("ave = %.3lf\n",ave);

    return 0;
}

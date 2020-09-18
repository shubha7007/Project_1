#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{

char SERVERIP[30];
char ZONE[12];
char FORMAT[4];

int temp;

if( argc == 1 || argc > 4)
{
 if(argc == 1){
	/* config code */
	return 0;
 }
 else{
	printf("To many arguments ...\nPlease enter  SERVERIP : ZONE : FORMAT\n");
	return 0;
 }
}

printf("You have enter  SERVERIP:%s  ZONE: %s  FORMAT: %s\n",argv[1],argv[2],argv[3]);

strcpy(SERVERIP,argv[1]);
strcpy(ZONE,argv[2]);
strcpy(FORMAT,argv[3]);

temp = atoi(FORMAT);

printf("%d \n",temp);

if(temp == 1){
	/******** format 1 *********/
}
else if(temp == 2){
	/******** format 2 *********/
}
else if(temp == 3){
	/******** format 3 *********/
}
else if(temp == 4){
	/******** format 4 *********/
}

return 0;

}

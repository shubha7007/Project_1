#ifndef _HEADER
#define _HEADER
#include "get_time.h"
#endif

#ifdef __WIN32
HANDLE hTimer = NULL;
#endif

int main(int argc, char *argv[])
{

	int chase = 0;
	config_tmp data;
	struct tm * ptm;

	/*used to register alarm signal with handle */
#ifdef linux
	signal(SIGALRM,timeout);
	alarm(5);
#endif

#ifdef __WIN32
	int t;
	DWORD tid;
	hTimer = CreateEvent(NULL, FALSE, FALSE, NULL);
	CreateThread(NULL, 0,Timer, NULL, 0, &tid);
	SetEvent(hTimer);

	WSADATA wsaData;	

	if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0)
	{
		fprintf(stderr,"WSAStartup failed.\n");
		exit(1);
	}
#endif

	// clearing data variable 

	memset(&data, 0, sizeof(config_tmp));

	// handling command line argument
	get_config(&data);

	if( argc == 1 || (argc > 7))
	{
		if(argc == 1){
			chase = 1;
		}
		else{
			printf("Wrong no of arguments ...\nPlease enter: ./get_time -s SERVERIP -z ZONE -f FORMAT\n");
			return 0;
		}
	}

	// this will execute which user give parameter in command line
	if(chase==0)
	{

		// filling structure with parameter given by command line 

		for(int i=0; i < argc; i++)
		{
			if(strcmp(argv[i],"-s")==0){
				if(++i > (argc-1)){
					printf("enter server ip\n");
					exit(0);
				}
				strcpy(data.host_name,argv[i]);
			}
			else if(strcmp(argv[i],"-z")==0){
				if(++i > (argc-1)){
					printf("enter time zone\n");
					exit(0);
				}
				strcpy(data.zone,"TZ=");
				strcat(data.zone,argv[i]);
			}
			else if(strcmp(argv[i],"-f")==0){
				if(++i > (argc-1)){
					printf("enter format\n");
					exit(0);
				}
				data.format_1 = atoi(argv[i]);

			}
		}
	}


	/***********Used to validate server_ip and format******************/

	struct sockaddr_in sa;
	struct hostent * server;
	server= (struct hostent *) gethostbyname(data.host_name);	
	if ( server == NULL ){
		printf("please enter valid server ip\n");
		return 0;
	}

	if((data.format_1 != 1) && (data.format_1 != 2) && (data.format_1 != 3) && (data.format_1 != 4))
	{
		printf("please enter valid display format\n");
		return 0;
	}

	
	/***********Used to get ntp time****************/
	time_t txTm = get_ntptime(&data);


	/***********Used to get UTC time */
	txTm = ( time_t ) ( txTm - NTP_TIMESTAMP_DELTA );

	//Setting TV variable
	putenv(data.zone);

	//Setting time zone
	tzset();

	//Getting local time 
	ptm = localtime(&txTm);

	//Display data and time
	display_time(&data , ptm);

#ifdef __WIN32
	CloseHandle(hTimer);
#endif

	return 0;

}

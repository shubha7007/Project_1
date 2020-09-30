#include "get_time.h"


int main(int argc, char *argv[])
{

	int chase = 0;
	config_tmp data;
	struct tm * ptm;

	// handling command line argument

	if( argc == 1 || (argc != 7))
	{
		if(argc == 1){
			chase = 1;
		}
		else{
			printf("Wrong no of arguments ...\nPlease enter: ./get_time -s SERVERIP -z ZONE -f FORMAT\n");
			return 0;
		}
	}


	if(chase==0)
	{

		// filling structure with parameter given by command line 

		for(int i=0; i < argc; i++)
		{
			if(strcmp(argv[i],"-s")==0){
				strcpy(data.host_name,argv[++i]);
			}
			else if(strcmp(argv[i],"-z")==0){
				strcpy(data.zone,"TZ=");
				strcat(data.zone,argv[++i]);
			}
			else if(strcmp(argv[i],"-f")==0){
				data.format_1 = atoi(argv[++i]);
			}
		}
	}
	else
	{

		// filling structure with parameter given by config file 

		get_config(&data);
	}

	time_t txTm = get_ntptime(&data);

	// Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
	// Subtract 70 years worth of seconds from the seconds since 1900.
	// This leaves the seconds since the UNIX epoch of 1970.
	// (1900)------------------(1970)**************************************(Time Packet Left the Server)

	txTm = ( time_t ) ( txTm - NTP_TIMESTAMP_DELTA );

	//Setting TV variable
	putenv(data.zone);

	//Setting time zone
	tzset();

	//Getting local time 
	ptm = localtime(&txTm);

	//Display data and time
	display_time(&data , ptm);

	return 0;

}

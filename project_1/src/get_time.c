#include "get_time.h"


int main(int argc, char *argv[])
{

	int chase = 0;
	config_tmp data;
	struct tm * ptm;

	if( argc == 1 || argc > 4 || argc < 4)
	{
		if(argc == 1){
			chase = 1;
		}
		else{
			printf("To many arguments ...\nPlease enter  SERVERIP : ZONE : FORMAT\n");
			return 0;
		}
	}

	if(chase==0)
	{
		strcpy(data.host_name,argv[1]);
		strcpy(data.zone,"TZ=");
		strcat(data.zone,argv[2]);
		data.format_1 = atoi(argv[3]);
	}
	else
	{
		get_config(&data);
	}

	time_t txTm = get_ntptime(&data);

	txTm = ( time_t ) ( txTm - NTP_TIMESTAMP_DELTA );
	putenv(data.zone);
	tzset();
	ptm = localtime(&txTm);

	display_time(&data , ptm);

	return 0;

}

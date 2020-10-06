#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include<signal.h>

#ifdef linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#ifdef _WIN32
#include <winsock2.h>
#include<windows.h>
#include<stdint.h>

#endif

#define NTP_TIMESTAMP_DELTA 2208988800ull

/******   Below structure is used to communicate with NTP server   ******/
typedef struct
{
	uint8_t li_vn_mode;
	uint8_t stratum;
	uint8_t poll;
	uint8_t precision;
	uint32_t rootDelay;
	uint32_t rootDispersion;
	uint32_t refId;
	uint32_t refTm_s;
	uint32_t refTm_f;
	uint32_t origTm_s;
	uint32_t origTm_f;
	uint32_t rxTm_s;
	uint32_t rxTm_f;
	uint32_t txTm_s;
	uint32_t txTm_f;
} ntp_packet;

/***** Below is structure which is used to store user parameter *****/
typedef struct 
{

	char host_name[100];
	char zone[100];
	char format[5];
	int format_1;
} config_tmp;

/****** Below is declaration of function used in program ******/
void get_config(config_tmp * );

time_t get_ntptime(config_tmp * );

void error( char* );

time_t get_ntptime(config_tmp * );

void display_time( config_tmp *, struct tm * );


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define NTP_TIMESTAMP_DELTA 2208988800ull

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
    
void error( char* msg )
{
    perror( msg ); // Print the error message to stderr.

    exit( 0 ); // Quit the process.
}


int main(int argc, char *argv[])
{

int chase = 0;

 if( argc == 1 || argc > 4)
 {
  if(argc == 1){
 	//config code 
	chase = 1;
  }
  else{
	printf("To many arguments ...\nPlease enter  SERVERIP : ZONE : FORMAT\n");
 	return 0;
  }
 }



char host_name[100] = {0};
char  TZ1[50] = "TZ=";
char  TZ2[50];
char FORMAT[10];
int temp;

FILE * f1;
char buf3[10000];
char * pos = NULL;

struct tm * ptm;
char buf[256] = {0};

//-----------------------------------------------
if(chase==0)
{
strcpy(host_name,argv[1]);
strcpy(TZ2,argv[2]);
strcat(TZ1,TZ2);
strcpy(FORMAT,argv[3]);
}
else
{
f1 = fopen("config.h","r");

if(!f1)
{
printf("Error in opening config file\n");
return 0;
}

while(fgets(buf3,sizeof(buf3),f1)!=NULL)
{
	if(strstr(buf3,"NTP_IP"))
	{
	 pos = strstr(buf3,"=");
	 if(pos)
	 {
	  sscanf(pos+1,"%s",host_name);
	 }
	}
	else if(strstr(buf3,"ZONE"))
	{
	 pos = strstr(buf3,"=");
	 if(pos)
	 {
	  sscanf(pos+1,"%s",TZ2);
	  strcat(TZ1,TZ2);
	 }
	}
	else if(strstr(buf3,"FORMAT"))
	{
	 pos = strstr(buf3,"=");
	 if(pos)
	 {
	  sscanf(pos+1,"%s",FORMAT);
	 }
	}
}
}
//----------------------------------------------

temp = atoi(FORMAT);
//printf("You have enter  SERVERIP:%s  ZONE: %s  FORMAT: %s\n",host_name,TZ1,FORMAT);

//-----------------------------------------------

//////////////// getting time for ntp server ////////////////////

int sockfd, n;
int portno = 123;

ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

memset( &packet, 0, sizeof( ntp_packet ) );

 *( ( char * ) &packet + 0 ) = 0x1b;
struct sockaddr_in serv_addr;
struct hostent *server;

sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

  if ( sockfd < 0 )
    error( "ERROR opening socket" );

  server= (struct hostent *) gethostbyname(host_name);

  if ( server == NULL )
    error( "ERROR, no such host" );

  bzero( ( char* ) &serv_addr, sizeof( serv_addr ) );

  serv_addr.sin_family = AF_INET;

serv_addr.sin_addr = *((struct in_addr *)server->h_addr);
bzero(&(serv_addr.sin_zero),8);

  serv_addr.sin_port = htons( portno );

  if ( connect( sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr) ) < 0 )
    error( "ERROR connecting" );

n = write( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );

  if ( n < 0 )
    error( "ERROR writing to socket" );

n = read( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );

  if ( n < 0 )
    error( "ERROR reading from socket" );

  packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.
  packet.txTm_f = ntohl( packet.txTm_f ); // Time-stamp fraction of a second.

  time_t txTm = ( time_t ) ( packet.txTm_s - NTP_TIMESTAMP_DELTA );
//-------------------------------

putenv(TZ1);
tzset();
sleep(1);
ptm = localtime(&txTm);

//------------------------------

/////////////////   display time according to option   ////////////////////


if(temp == 1){
  strftime(buf, 256, "%G-%m-%d", ptm);
  printf("%s",buf);
  printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
else if(temp == 2){
  strftime(buf, 256, "%H-%m-%d", ptm);
  printf("%s",buf);
  printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
else if(temp == 3){
  strftime(buf, 256, "%G-%h-%d <%p>", ptm);
  printf("%s",buf);
  printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
else if(temp == 4){
  printf ("%2d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
  strftime(buf, 256, "  %d %h %G\n", ptm);
  printf("%s",buf);
}

return 0;

}

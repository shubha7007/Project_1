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

 if( argc == 1 || argc > 4)
 {
  if(argc == 1){
 	/* config code */
  }
  else{
	printf("To many arguments ...\nPlease enter  SERVERIP : ZONE : FORMAT\n");
 	return 0;
  }
 }

char SERVERIP[30];
char ZONE[12];
char FORMAT[4];
int temp;

struct tm * ptm;

char buf[256] = {0};

char  buf1[50] = "TZ=";
char  buf2[50];


//////////////// getting time for ntp server ////////////////////

int sockfd, n;
int portno = 123;
char* host_name = argv[1];

//char* host_name = "us.pool.ntp.org";

ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

memset( &packet, 0, sizeof( ntp_packet ) );

 *( ( char * ) &packet + 0 ) = 0x1b;
struct sockaddr_in serv_addr;
struct hostent *server;

sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );

  if ( sockfd < 0 )
    error( "ERROR opening socket" );

//  server = gethostbyname( host_name );
//server= (struct hostent *) gethostbyname((char *)"66.151.147.38");

  server= (struct hostent *) gethostbyname(host_name);

  if ( server == NULL )
    error( "ERROR, no such host" );

  bzero( ( char* ) &serv_addr, sizeof( serv_addr ) );

  serv_addr.sin_family = AF_INET;

  //bcopy( ( char* )server->h_addr, ( char* ) &serv_addr.sin_addr.s_addr, server->h_length );

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
  //ptm = gmtime(&txTm);
//  setenv( "TZ", "IST", 1 );

  //setenv( "TZ", argv[2], 1 );
//  tzset();

//  ptm = localtime(&txTm);
//-------------------------------


//putenv("TZ=Africa/Addis_Ababa");
strcpy(buf2,argv[2]);
strcat(buf1,buf2);
putenv(buf1);
tzset();
sleep(2);
ptm = localtime(&txTm);


//////////////////////// getting format option ////////////////////////////

printf("You have enter  SERVERIP:%s  ZONE: %s  FORMAT: %s\n",argv[1],argv[2],argv[3]);

strcpy(SERVERIP,argv[1]);
strcpy(ZONE,argv[2]);
strcpy(FORMAT,argv[3]);

temp = atoi(FORMAT);

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

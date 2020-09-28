#include "get_time.h"

void get_config(config_tmp * tmp)
{

char * pos = NULL;
char buf1[100]={0};
char buf2[100]={0};
FILE * f1;

f1 = fopen("config.h","r");

if(!f1)
{
printf("Error in opening config file\n");
}

while(fgets(buf1,sizeof(buf1),f1)!=NULL)
{
        if(strstr(buf1,"NTP_IP"))
        {
         pos = strstr(buf1,"=");
         if(pos)
         {
          sscanf(pos+1,"%s",tmp->host_name);
         }
        }
        else if(strstr(buf1,"ZONE"))
        {
         pos = strstr(buf1,"=");
         if(pos)
         {
          strcpy(tmp->zone,"TZ=");
          sscanf(pos+1,"%s",buf2);
          strcat(tmp->zone,buf2);
         }
        }
        else if(strstr(buf1,"FORMAT"))
        {
         pos = strstr(buf1,"=");
         if(pos)
         {
          sscanf(pos+1,"%s",tmp->format);
         }
        tmp->format_1 = atoi(tmp->format);
        }
}

}

void error( char* msg )
{
    perror( msg ); // Print the error message to stderr.

    exit( 0 ); // Quit the process.
}



time_t get_ntptime(config_tmp * tmp)
{

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

  server= (struct hostent *) gethostbyname(tmp->host_name);

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

  return packet.txTm_s;
}

void display_time( config_tmp *tmp, struct tm * ptm )
{

char buf[100]={0};

if(tmp->format_1 == 1){
  strftime(buf, 256, "%G-%m-%d", ptm);
  printf("%s",buf);
  printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
else if(tmp->format_1 == 2){
  strftime(buf, 256, "%H-%m-%d", ptm);
  printf("%s",buf);
  printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
else if(tmp->format_1 == 3){
  strftime(buf, 256, "%G-%h-%d <%p>", ptm);
  printf("%s",buf);
  printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
}
else if(tmp->format_1 == 4){
  printf ("%2d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
  strftime(buf, 256, "  %d %h %G\n", ptm);
  printf("%s",buf);
}
}

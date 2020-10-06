#include "get_time.h"

#define comment 0

void get_config(config_tmp * tmp)
{

	char * pos = NULL;
	char buf1[100]={0};
	char buf2[100]={0};
	char buf3[100]={0};
	FILE * f1, *fp;
	int status;

	//getting config file

	fp = popen("find . -name config.h","r");
	if(fp==NULL)
		exit(0);

	fgets(buf3,100, fp);
	buf3[strcspn(buf3,"\r\n")]=0;

	status = pclose(fp);
	if(status == -1)
	{
		printf("Error in closing pipe fp\n");
	}

	//opening config file

	f1 = fopen(buf3,"r");

	if(!f1){
		exit(0);
	}

	// filling user parameter into config_tmp structure 

	while(fgets(buf1,sizeof(buf1),f1)!=NULL)
	{
		if(strstr(buf1,"NTP_IP")){
			pos = strstr(buf1,"=");
			if(pos){
				sscanf(pos+1,"%s",tmp->host_name);
			}
		}
		else if(strstr(buf1,"ZONE")){
			pos = strstr(buf1,"=");
			if(pos){
				strcpy(tmp->zone,"TZ=");
				sscanf(pos+1,"%s",buf2);
				strcat(tmp->zone,buf2);
			}
		}
		else if(strstr(buf1,"FORMAT")){
			pos = strstr(buf1,"=");
			if(pos){
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

	int n;
	int portno = 123;

#ifdef linux
	int sockfd;
#endif

#ifdef _WIN32
	SOCKET sockfd;
	WSADATA wsaData;	

	if(WSAStartup(MAKEWORD(1,1),&wsaData)!=0)
	{
		fprintf(stderr,"WSAStartup failed.\n");
		exit(1);
	}

#endif

	// Create and zero out the packet. All 48 bytes worth.

	ntp_packet packet;
	memset( &packet, 0, sizeof( ntp_packet ) );

	// Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.

	*( ( char * ) &packet + 0 ) = 0x1b;




//-----------------------------------------------------------------------------------------------------------------------------------------

#ifdef comment
	// Create a UDP socket, convert the host-name to an IP address, set the port number,
	// connect to the server, send the packet, and then read in the return packet.

	struct sockaddr_in serv_addr;
	struct hostent *server;

	sockfd = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // Create a UDP socket.

	if ( sockfd < 0 )
		error( "ERROR opening socket" );

	server= (struct hostent *) gethostbyname(tmp->host_name);

	if ( server == NULL )
		error( "ERROR, no such host" );

	// Zero out the server address structure.


	memset( (char *) &serv_addr, 0, sizeof( serv_addr ) );

	serv_addr.sin_family = AF_INET;

	serv_addr.sin_addr = *((struct in_addr *)server->h_addr);

	memset( &(serv_addr.sin_zero), 0, sizeof( char[8] ) );

	serv_addr.sin_port = htons( portno );

	// Call up the server using its IP address and port number.


	if ( connect( sockfd, ( struct sockaddr * ) &serv_addr, sizeof( serv_addr) ) < 0 )
		error( "ERROR connecting" );
#endif
//-------------------------------------------------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------------


	// Send it the NTP packet it wants. If n == -1, it failed.

#ifdef linux
	n = write( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );
#endif

#ifdef _WIN32
	n = send( sockfd, ( char* ) &packet, sizeof( ntp_packet ),0 );
#endif
	if ( n < 0 )
		error( "ERROR writing to socket" );

#ifdef linux
	n = read( sockfd, ( char* ) &packet, sizeof( ntp_packet ) );
#endif

#ifdef _WIN32
	n = recv( sockfd, ( char* ) &packet, sizeof( ntp_packet ),0 );
#endif

	if ( n < 0 )
		error( "ERROR reading from socket" );

	// ntohl() converts the bit/byte order from the network's to host's "endianness".

	packet.txTm_s = ntohl( packet.txTm_s ); // Time-stamp seconds.

	return packet.txTm_s;
}

void display_time( config_tmp *tmp, struct tm * ptm )
{

	char buf[100]={0};

	// Display date and time in format given by user

	switch(tmp->format_1)
	{
		case 1:
#ifdef linux
			strftime(buf, 256, "%G-%m-%d", ptm);
#endif

#ifdef _WIN32
			strftime(buf, 100, "%Y-%m-%d", ptm);
#endif

			printf("%s",buf);
			printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			break;
		case 2:
#ifdef linux
			strftime(buf, 256, "%H-%m-%d", ptm);
#endif

#ifdef _WIN32
			strftime(buf, 256, "%y-%m-%d", ptm);
#endif

			printf("%s",buf);
			printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			break;
		case 3:
#ifdef linux
			strftime(buf, 256, "%G-%h-%d <%p>", ptm);
#endif

#ifdef _WIN32
			strftime(buf, 256, "%Y-%B-%d <%p>", ptm);
#endif

			printf("%s",buf);
			printf ("  %2d:%02d:%02d\n", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
			break;
		case 4:
			printf ("%2d:%02d:%02d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
#ifdef linux
			strftime(buf, 256, "  %d %h %G\n", ptm);
			printf("%s",buf);
#endif

#ifdef _WIN32
			strftime(buf, 256, "  %d %B %Y\n", ptm);
			printf("%s",buf);
#endif
			break;

		default:
			printf("wrong format");
	}
}

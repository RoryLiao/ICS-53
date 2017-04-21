/*
	JT
	TC
	Liao, Rory
*/

#include "csapp.h"

#define PROXY_LOG "proxy.log"

typedef struct {
    int connfd; /* Connection file descriptor */
    struct sockaddr_in clientaddr; /* Client IP address */
} connection;

FILE *log_file;

void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size) {
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /* 
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s", time_str, a, b, c, d, uri);
}

//Function to process HTTP request from a connected socket created
//from listening socket
void request(int connfd, struct sockaddr_in *sockaddr)
{
	int flag = 0;
	int content_t, clientfd;	//content_t = type of content being requested
	rio_t rio;
	int response_len;  
	char *host,
		buffer[MAXLINE],
		cgiargs[MAXLINE],
		method[MAXLINE],
		uri[MAXLINE],
		version[MAXLINE],
		hostName[MAXLINE],
		fileName[MAXLINE],
		log_entry[MAXLINE];      


	//Set up rio to read request line from connected socket
	Rio_readinitb(&rio, connfd);

	//Read line
	Rio_readlineb(&rio, buffer, MAXLINE);


	//Split the request line into the apporopriate headers
	sscanf(buffer, "%s %s %s", method, uri, version);


	//Read host request header
	Rio_readlineb(&rio, buffer, MAXLINE);
	sscanf(buffer,"%s %s", fileName, hostName);


	int n = sprintf(buffer, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", uri, hostName);

	clientfd = Open_clientfd(hostName, "80");
	Rio_readinitb(&rio, clientfd);
	Rio_writen(clientfd, buffer, strlen(buffer));

	int x = Rio_readlineb(&rio, buffer, MAXLINE);
	while (x != 0)
	{
		Fputs(buffer, stdout);
		x = Rio_readlineb(&rio, buffer, MAXLINE);
		response_len += x;

		if(strstr(buffer, "<html") != NULL || strstr(buffer, "<HTML") != NULL)
		{
			flag = 1;
		}
		if(flag)
			Rio_writen(connfd, buffer, strlen(buffer));
		if(strstr(buffer, "</html") != NULL || strstr(buffer, "</HTML") != NULL)
			flag = 0;
	}

	format_log_entry(log_entry, sockaddr, uri, response_len);  
    fprintf(log_file, "%s %d\n", log_entry, response_len);
    fflush(log_file);

	Close(clientfd);
	printf("Connection closed.\n");
}

int main(int argc, char* argv[])
{
	int listenfd;
	SA clientAddr;
	socklen_t clientLen;

	//Check for existence of port number in command line
	if(argc < 2)
	{
		printf("ERROR: No port number listed\n");
		exit(1);
	}
	else if(argc != 2)
	{
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
	}

	log_file = Fopen(PROXY_LOG, "a");

	//Create listening socket using port number from the command line
	listenfd = Open_listenfd(argv[1]);	
	
	while(1)
	{
		//Accept queued connection request from listening socket
		clientLen = sizeof(clientAddr);
		int connectedfd = Accept(listenfd, &clientAddr, &clientLen);

		struct sockaddr_in *sin = (struct sockaddr_in *) &clientAddr;
		//Read and process HTTP request
		request(connectedfd, sin);
		//Close the connected socket after finished processing HTTP request
		Close(connectedfd);

	}

}	
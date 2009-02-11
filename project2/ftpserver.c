/**
 *  ftpserver.c - FTP server to transfer files using socket programming
 *
 *  @author Dana Merrick
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
	char buf[BUFSIZ];
	unsigned int server_sockfd, client_sockfd, client_len;
	struct sockaddr_in client_address, server_address;
	int len, i;
  char cmd[5]; // this string will hold the command issued


	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		exit(1);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(6996);

	if (bind(server_sockfd, (struct sockaddr *) &server_address, 
	    sizeof(server_address)) < 0) {
		perror("bind error");
		close(server_sockfd);
		exit(2);
	}
	
  if (listen(server_sockfd, 5) < 0) {
		perror("listen error");
		exit(3);
	}
	
  client_len = sizeof(client_address);
	
  if ((client_sockfd = accept(server_sockfd, 
	    (struct sockaddr *) &client_address, &client_len)) < 0) {
		perror("accept error");
		close(server_sockfd);
		exit(4);
	}

	while ((len=read(client_sockfd, buf, BUFSIZ)) > 0) {

		for (i = 0; i < 4; ++i) {
      // save the first for chars as a the command
      // I used this instead of strncmp() so we could separate the arguements
			cmd[i] = toupper(buf[i]);
    }
    cmd[4] = '\0';

		for (i = 4; i < len; ++i) {
      // save the rest as the command arguements
			buf[i] = toupper(buf[i]);
    }

    // write the buffer back to the client
		write(client_sockfd, buf, len);

    printf("DEBUG: buf=%s",buf);
    //printf("DEBUG: cmd=%s<<<\n",cmd);

    if (strcmp(cmd,"PUT ") == 0) { 
      puts("Command was PUT!"); 
    }

    if (strcmp(cmd,"GET ") == 0) { 
      puts("Command was GET!"); 
    }

    // exit if period is sent
		if (buf[0] == '.')
			break;
	}
  // close the socket
	close(client_sockfd);

  // exit cleanly
  return 0;
}

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
#include <sys/stat.h>
#include <fcntl.h>


int main(void)
{
	char buf[BUFSIZ], file_buf[BUFSIZ];
	unsigned int server_sockfd, client_sockfd, client_len;
	struct sockaddr_in client_address, server_address;
	int len, i, file_len;
  char cmd[4]; // this string will hold the command issued
  char file_arg[BUFSIZ]; // the arguement passed from the client
  int local, remote; // the local and remote files, respectively


  // open socket or die trying
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		exit(1);
	}

  // set up socket
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(6996);

  // bind socket to server address
	if (bind(server_sockfd, (struct sockaddr *) &server_address, 
	    sizeof(server_address)) < 0) {
		perror("bind error");
		close(server_sockfd);
		exit(2);
	}
	
  // listen for connections
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

		for (i = 0; i < 3; ++i) {
      // save the first for chars as a the command
      // I used this instead of strncmp() so we could separate the arguements
			cmd[i] = toupper(buf[i]);
    }
    cmd[3] = '\0';

		for (i = 4; i < len; ++i) {
      // save the rest as the command arguements
      file_arg[i-4]=buf[i];
			//buf[i] = toupper(buf[i]);
    }

    // write the buffer back to the client
		//write(client_sockfd, buf, len);

    printf("DEBUG: file_arg=%s",file_arg);
    //printf("DEBUG: cmd=%s<<<\n",cmd);

    if (strcmp(cmd,"PUT") == 0) { 

      puts("Command was PUT!"); 
      local = open(file_arg, O_WRONLY | O_CREAT | O_TRUNC, 00644);

       if (!local) {
         fprintf(stderr, "ERROR: local file could not be opened.", local);
       }

	    while ((file_len=read(client_sockfd, file_buf, BUFSIZ)) > 0) {
        file_buf[5]='\0';
		    write(local, file_buf, file_len);
      }
      break;
      //remote = open(local, O_RDONLY);

    } else if (strcmp(cmd,"GET") == 0) { 
      
      puts("Command was GET!"); 
    
    } else if (strcmp(cmd,"BYE") == 0) { 
		
      puts("Command was BYE!"); 
      break;
    
    } else {
    
      printf("unrecognised command: %s",cmd);

    }

	}


  // close the socket and files
	close(client_sockfd);
  close(local);
  //close(remote);

  // exit cleanly
  return 0;
}

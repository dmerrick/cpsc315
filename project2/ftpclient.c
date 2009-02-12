/**
 *  ftpclient.c - Client for FTP server problem.
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

int receiveFile(char file, unsigned int socket_fd);
int sendFile(char file, unsigned int socket_fd);

int main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	int server_sockfd, len;
	struct sockaddr_in server_address;
	struct hostent *host;		/* the host (server) */
  int i;
  char cmd[4];
	char file_arg[BUFSIZ];

  // check arguements are sane
	if (argc != 2) {
		fprintf(stderr, "usage: %s server\n", argv[0]);
		exit(1);
	}

  // get host by name
	host = gethostbyname(argv[1]);
	if (host == (struct hostent *) NULL) {
    // or error
		perror("gethostbyname ");
		exit(2);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	memcpy(&server_address.sin_addr, host->h_addr, host->h_length);
	server_address.sin_port = htons(6996);

	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		exit(3);
	}

	if (connect(server_sockfd, (struct sockaddr *) &server_address, 
   	    sizeof(server_address)) < 0) {
		perror("connect error");
		exit(4);
	}

	do {
		write(STDOUT_FILENO, "> ", 3);
		if ((len=read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
			write(server_sockfd, buf, len);
			if ((len=read(server_sockfd, buf, BUFSIZ)) > 0) {
				write(STDOUT_FILENO, buf, len);

     	  for (i = 0; i < 3; ++i) {
          // save the first for chars as a the command
          // I used this instead of strncmp() so we could separate the arguements
			    cmd[i] = toupper(buf[i]);
        }
        cmd[3] = '\0';

		    for (i = 4; i < len; ++i) {
          // save the rest as the command arguement
          file_arg[i-4]=buf[i];
			    //buf[i] = toupper(buf[i]);
        }

        // write the buffer back to the client
		    //write(client_sockfd, buf, len);

        printf("DEBUG: file_arg=%s",file_arg);
        printf("DEBUG: cmd=%s\n",cmd);
        
        if (strcmp(cmd,"PUT") == 0) { 

          puts("Command was PUT!"); 
          if (sendFile(file_arg, server_sockfd))
            break;
    
        } else if (strcmp(cmd,"GET") == 0) { 
          
          puts("Command was GET!"); 
          if (receiveFile(file_arg,server_sockfd))
            break;
    
        } else if (strcmp(cmd,"BYE") == 0) { 
		
          puts("Command was BYE!"); 
          exit(0);
    
        } else {
    
          printf("unrecognised command: %s",cmd);

        }

		  }
    }
	} while (strcmp(cmd,"BYE") != 0);

  // close the socket
	close(server_sockfd);
  // and exit cleanly
	exit(0);
}


int sendFile(char file, unsigned int socket_fd) {
  char file_buf[BUFSIZ];
  int file_len;
  int local; // the local file

  puts("Command was GET!"); 

  local = open(file, O_RDONLY);

  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
  }

	while ((file_len=read(socket_fd, file_buf, BUFSIZ)) > 0) {
    // write to the socket
    if (write(socket_fd,file_buf,file_len) < file_len) {
      fprintf(stderr, "ERROR: error writing to socket");
      return 2;
    }
  }

  close(local);

  return 0;
}

int receiveFile(char file, unsigned int socket_fd) {
  char file_buf[BUFSIZ];
  int file_len;
  int local; // the local file

  puts("Command was PUT!"); 
  
  local = open(file, O_WRONLY | O_CREAT | O_TRUNC, 00644);

  
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
  }

  /*
	if ((file_len=read(socket_fd, file_buf, BUFSIZ)) > 0) {
    printf("%s\n",file_buf);
	  write(local, file_buf, file_len);
  }
  */

  close(local);

  return 0;
}


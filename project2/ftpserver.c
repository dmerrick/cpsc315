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

int receiveFile(char file, unsigned int socket_fd);
int sendFile(char file, unsigned int socket_fd);

int main(void)
{
	char buf[BUFSIZ];
	unsigned int server_sockfd, client_sockfd, client_len;
	struct sockaddr_in client_address, server_address;
	int len, i;
  char cmd[4]; // this string will hold the command issued
  char file_arg[BUFSIZ]; // the arguement passed from the client


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
      // save the rest as the command arguement
      file_arg[i-4]=buf[i];
			//buf[i] = toupper(buf[i]);
    }

    // write the buffer back to the client
		//write(client_sockfd, buf, len);

    printf("DEBUG: file_arg=%s",file_arg);
    //printf("DEBUG: cmd=%s<<<\n",cmd);

    if (strcmp(cmd,"PUT") == 0) { 

      if( receiveFile(file_arg, client_sockfd) )
        break;

    } else if (strcmp(cmd,"GET") == 0) { 
      
      if(sendFile(file_arg, client_sockfd))
        break;
    
    } else if (strcmp(cmd,"BYE") == 0) { 
		
      puts("Command was BYE!"); 
      break;
    
    } else {
    
      printf("unrecognised command: %s",cmd);

    }

	}

  // close the socket and files
	close(client_sockfd);

  // exit cleanly
  return 0;
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

  close(local);

  return 0;
}

int receiveFile(char file, unsigned int socket_fd) {
  char file_buf[BUFSIZ];
  int file_len;
  int local; // the local file
  char file_name,dir_name;

  puts("Command was PUT!"); 

  // create new directory
  mode_t process_mask = umask(0);
  int result_code = mkdir(dir_name, S_IRWXU | S_IRWXG | S_IRWXO)
  umask(process_mask);
  
  local = open(file, O_WRONLY | O_CREAT | O_TRUNC, 00644);

  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
  }

	if ((file_len=read(socket_fd, file_buf, BUFSIZ)) > 0) {
	  write(STDOUT_FILENO, file_buf, file_len);
	  write(local, file_buf, file_len);
  }

  close(local);

  return 0;
}


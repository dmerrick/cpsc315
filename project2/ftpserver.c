/**
 *  ftpserver.c - FTP server to transfer files using socket programming
 *
 *  @author Dana Merrick
 */

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>

// size of "put" and "get" plus nul
#define CMDSIZE 4

// function signatures
int receiveFile(char file, unsigned int socket_fd);
int sendFile(char file, unsigned int socket_fd);

/**
 * Main method.
 */
int main(void)
{
	char buf[BUFSIZ];
	unsigned int server_sockfd, client_sockfd, client_len;
	struct sockaddr_in client_address, server_address;
	int len, i;
  // my variables
  char cmd[CMDSIZE]; // this string will hold the command issued
  char file_arg[BUFSIZ]; // the arguement passed from the client

  // setting up socket...

  // open socket or die trying
	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("generate error");
		exit(1);
	}

  // socket details
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

  // socket set up, let's start working with it

	while ((len=read(client_sockfd, buf, BUFSIZ)) > 0) {

    // split the buffer into two parts
		for (i = 0; i < CMDSIZE-1; ++i) {
      // save the first for chars as a the command
      // I used this instead of strncmp() so we could separate the arguements
			cmd[i] = toupper(buf[i]);
    }
    // close off cmd with nul char
    cmd[CMDSIZE-1] = '\0';

		for (i = CMDSIZE; i < len; ++i) {
      // save the rest as the command arguement
      file_arg[i-CMDSIZE]=buf[i];
    }

    // send the command back to the client
		write(client_sockfd, cmd, CMDSIZE);

    // debugging messages
    //printf("DEBUG: file_arg=%s",file_arg);
    //printf("DEBUG: cmd=%s\n",cmd);

    // start parsing the command
    if (strcmp(cmd,"PUT") == 0) { 

      // try and receive the file
      if( receiveFile(file_arg, client_sockfd) )
        break;

    } else if (strcmp(cmd,"GET") == 0) { 
      
      // try and send the file
      if(sendFile(file_arg, client_sockfd))
        break;
    
    } else if (strcmp(cmd,"BYE") == 0) { 
		
      puts("Command was BYE!"); 
      break;
    
    } else {
    
      printf("unrecognised command: %s",cmd);
      // return to the loop

    }

	}

  // close the socket and files
	close(client_sockfd);

  // exit cleanly
  return 0;
}

/**
 * Sends a file to the client via a given socket.
 *
 * @param filename string
 * @param client socket file descriptor
 * @return status
 */
int sendFile(char file, unsigned int socket_fd) {
  char file_buf[BUFSIZ]; // buffer for data to send
  int file_len; // filesize
  int local; // the local file

  // debug message
  puts("Command was GET!"); 

  // open file for reading
  local = open(file, O_RDONLY);

  // verify file was opened correctly
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
  }

  // close the file
  close(local);

  // exit cleanly
  return 0;
}

/**
 * Sends a file over a given socket.
 *
 * @param filename string
 * @param client socket file descriptor
 * @return status
 */
int receiveFile(char file, unsigned int socket_fd) {
  char file_buf[BUFSIZ]; // buffer for data to send
  int file_len; // filesize
  int local; // the local file descriptor
  char file_name; // local filename
  // should be "from_client_<filename>"

  // debug message
  puts("Command was PUT!"); 

  // open file or create it
  local = open(file, O_WRONLY | O_CREAT | O_TRUNC, 00644);

  // very we opened it correctly
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
  }

  // read the file data from the client
	if ((file_len=read(socket_fd, file_buf, BUFSIZ)) > 0) {
    // write it to the terminal...
	  write(STDOUT_FILENO, file_buf, file_len);
    // ...and to the local file
	  write(local, file_buf, file_len);
  }

  // close the file
  close(local);

  // exit cleanly
  return 0;
}


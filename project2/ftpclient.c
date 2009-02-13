/**
 *  ftpclient.c - Client for FTP server problem.
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
int main(int argc, char *argv[])
{
	char buf[BUFSIZ];
	int server_sockfd, len;
	struct sockaddr_in server_address;
	struct hostent *host; // the host
  int i; // for parsing the command
  char cmd[4]; // the command issued
	char file_arg[BUFSIZ]; // the argument passed from the client

  // check arguements are sane
	if (argc != 2) {
		fprintf(stderr, "usage: %s server\n", argv[0]);
		exit(1);
	}

  // setting up the socket...

  // get host by name
	host = gethostbyname(argv[1]);
	if (host == (struct hostent *) NULL) {
    // or error
		perror("gethostbyname ");
		exit(2);
	}

  // socket details
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

  // socket is set up

  // start connection loop
	do {
    // draw prompt
		write(STDOUT_FILENO, "> ", 3);
    // read stdin
		if ((len=read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
      // send the user input to to socket
			write(server_sockfd, buf, len);
      // read from the socket
			if ((len=read(server_sockfd, buf, BUFSIZ)) > 0) {
        // send the data to stdout
				write(STDOUT_FILENO, buf, len);

        // extract the command from the buffer
     	  for (i = 0; i < CMDSIZE-1; ++i) {
          // save the first for chars as a the command
          // I used this instead of strncmp() so we could separate the arguements
			    cmd[i] = toupper(buf[i]); // caps for easy parsing
        }
        // close off cmd with nul char
        cmd[CMDSIZE-1] = '\0';

		    for (i = CMDSIZE; i < len; ++i) {
          // save the rest as the filename
          file_arg[i-CMDSIZE]=buf[i];
        }

		    //write(server_sockfd, buf, len);

        // debugging messages
        //printf("DEBUG: file_arg=%s",file_arg);
        //printf("DEBUG: cmd=%s\n",cmd);
        
        // start parsing the command
        if (strcmp(cmd,"PUT") == 0) { 

          //puts("Command was PUT!"); 
          // try and send the file
          if (sendFile(file_arg, server_sockfd))
            break;
    
        } else if (strcmp(cmd,"GET") == 0) { 
          
          //puts("Command was GET!"); 
          // try and receive the file
          if (receiveFile(file_arg,server_sockfd))
            break;
    
        } else if (strcmp(cmd,"BYE") == 0) { 
		
          puts("Command was BYE!"); 
          exit(0);
    
        } else {
    
          printf("unrecognised command: %s",cmd);
          // return to the loop

        }

		  }
    }
  // FIXME: should this be while(true)?
	} while (strcmp(cmd,"BYE") != 0);

  // close the socket
	close(server_sockfd);
  // and exit cleanly
	exit(0);
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

  // send the data
	while ((file_len=read(socket_fd, file_buf, BUFSIZ)) > 0) {
    // write to the socket
    if (write(socket_fd,file_buf,file_len) < file_len) {
      fprintf(stderr, "ERROR: error writing to socket");
      return 2;
    }
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

  // debug message
  puts("Command was PUT!"); 
  
  // open the file or create it
  local = open(file, O_WRONLY | O_CREAT | O_TRUNC, 00644);

  // verify we opened it correctly 
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

  /*
	if ((file_len=read(socket_fd, file_buf, BUFSIZ)) > 0) {
    printf("%s\n",file_buf);
	  write(local, file_buf, file_len);
  }
  */

  // close the file
  close(local);

  // exit cleanly
  return 0;
}


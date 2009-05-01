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
// for forEVER loop
#define EVER ;;

// function signatures
int receiveFile(char *file);
int sendFile(char *file);

int server_sockfd, client_sockfd;
socklen_t client_len;
struct sockaddr_in client_address, server_address;


/**
 * Main method.
 */
int main(void)
{

  // setting up socket...
  if (initialize() > 0) {
    perror("initialize error");
    exit(1);
  }

	
  for(EVER) {
	char buf[BUFSIZ];
	int len;
  
  if ((client_sockfd = accept(server_sockfd, 
	    (struct sockaddr *) &client_address, &client_len)) < 0) {
		perror("accept error");
		close(server_sockfd);
		exit(2);
	}

  // socket set up, let's start working with it

	if ((len=recv(client_sockfd, buf, BUFSIZ, 0)) > 0) {

  if(!strcmp("BYE", buf) || !strcmp("EOF", buf) ) {
    return 0;
  }

  if ( interpret(buf, len) > 0) {
    perror("error interpreting data");
  }

	}

  // close the socket and files
	close(client_sockfd);
}

  // exit cleanly
  return 0;
}

/**
 * Sends a file to the client via a given socket.
 *
 * @param filename string
 * @return status
 */
int receiveFile(char *file) {
  char file_buf[BUFSIZ]; // buffer for data to send
  int file_len; // filesize
  int local; // the local file

  // debug message
  puts("Command was GET!"); 

  // open file for reading
  local = open(file, O_RDONLY, 0);

  // verify file was opened correctly
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
    send(client_sockfd, "ERROR", 6, 0);
    return 1;
  }

  // send ready message
  send(client_sockfd, "READY", 6, 0);
  // wait for ready message from client
  recv(client_sockfd, file_buf, 6, 0);

  if(!strcmp("ERROR", file_buf)) {
    perror("client returned error");
    return 2;
  }

  // send the data
  while ((file_len=read(local, file_buf, BUFSIZ)) > 0) {
    // write to the socket
    if (send(client_sockfd,file_buf,file_len,0) < 0) {
      perror("ERROR: error writing to socket");
      break;
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
int sendFile(char *file) {
  char file_buf[BUFSIZ]; // buffer for data to send
  int file_len; // filesize
  int local; // the local file descriptor
  int i; // for EOF scan
  char filename[BUFSIZ]="from_client_";

  strcat(filename,file);

  // debug message
  puts("Command was PUT!"); 

  recv(client_sockfd, file_buf, BUFSIZ, 0);

  if (!strcmp("ERROR", file_buf)) {
    perror("client returned error");
    return 1;
  }

  // open file or create it
  local = open(filename, O_WRONLY | O_CREAT, 00644);

  // very we opened it correctly
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
    send(client_sockfd, "ERROR", 6, 0);
    return 2;
  }

  send(client_sockfd, "READY", 6, 0);


  // read the file data from the client
   while((file_len = recv(client_sockfd, file_buf, BUFSIZ, 0)) > 0) {
    // write it to the terminal...
	  //write(STDOUT_FILENO, file_buf, file_len);
    // ...and to the local file
      write(local, file_buf, file_len);

    // scan for EOF
    for(i=0; i<BUFSIZ; i++) {
      if( file_buf[i]==EOF)
        break;
    }

  // close the file
  close(local);

  // exit cleanly
  return 0;
}
}

int initialize(void) {
 // open socket or die trying
  if ((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("generate error");
    return 1;
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
    return 2;
  }

  // listen for connections
  if (listen(server_sockfd, 5) < 0) {
    perror("listen error");
    return 3;
  }

  client_len = sizeof(client_address);

  return 0;
}

int interpret(char *buf, int len) {
  char cmd[CMDSIZE]; // this string will hold the command issued
  char file_arg[BUFSIZ]; // the arguement passed from the client
  int i; // for loops

    // split the buffer into two parts
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

   file_arg[len-CMDSIZE] = '\0';

    // send the command back to the client
		//write(client_sockfd, cmd, CMDSIZE);

    // debugging messages
    //printf("DEBUG: file_arg=%s",file_arg);
    //printf("DEBUG: cmd=%s\n",cmd);

    // start parsing the command
    if (strcmp(cmd,"PUT") == 0) { 

      // try and receive the file
      return receiveFile(file_arg);

    } else if (strcmp(cmd,"GET") == 0) { 
      
      // try and send the file
      return sendFile(file_arg);
    
    } else {
    
      printf("unrecognised command: %s",cmd);
      return 1;

    }
}

  
  

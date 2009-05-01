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
#define EVER ;;

// function signatures
int receiveFile(char *file);
int sendFile(char *file);

// global variables
int server_sockfd;
struct sockaddr_in server_address;
struct hostent *host; // the host

/**
 * Main method.
 */
int main(int argc, char *argv[])
{
	//char file_arg[BUFSIZ]; // the argument passed from the client

  // check arguements are sane
	if (argc != 2) {
		fprintf(stderr, "usage: %s server\n", argv[0]);
		exit(1);
	}

  // setting up the socket...
  if (initialize(argv[1]) > 0) {
    perror("initialize error");
    exit(2);
  }

  for(EVER) {
  int len;
	char buf[BUFSIZ];
  char cmd[CMDSIZE]; // the command issued


	if ((server_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("generate error");
		exit(3);
	}

	if (connect(server_sockfd, (struct sockaddr *) &server_address, 
   	    sizeof(server_address)) < 0) {
		perror("connect error");
		exit(4);
	}

  // socket is set up

    // draw prompt
		write(STDOUT_FILENO, "> ", 3);
    // read stdin
		if ((len=read(STDIN_FILENO, buf, BUFSIZ)) > 0) {
      buf[len-1] = '\0';
      // send the user input to to socket
			send(server_sockfd, buf, len, 0);

        // extract the command from the buffer
     	  for (i = 0; i < CMDSIZE-1; ++i) {
          // save the first for chars as a the command
          // I used this instead of strncmp() so we could separate the arguements
			    cmd[i] = toupper(buf[i]); // caps for easy parsing
        }
        // close off cmd with nul char
        cmd[CMDSIZE-1] = '\0';


  if(!strcmp("BYE", buf)  || !strcmp("EOF", buf)) {
    //write(STDOUT_FILENO, "BYE\n", 5);
    return 0;
  }

  if ( interpret(buf, len) > 0) {
    perror("error interpreting data");
  }


}
  // close the socket
	close(server_sockfd);
}
  // and exit cleanly
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
  puts("Command was PUT!"); 


  // open file for reading
  local = open(filename, O_RDONLY);

  // verify file was opened correctly
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
    send(server_sockfd, "ERROR", 6, 0);
    return 1;
  }

  // send ready message
  send(server_sockfd, "READY", 6, 0);
  // wait for ready message from client
  recv(server_sockfd, file_buf, 6, 0);

  if(!strcmp("ERROR", file_buf)) {
    perror("server returned error");
    return 2;
  }

  // send the data
  while ((file_len=read(local, file_buf, BUFSIZ)) > 0) {
    // write to the socket
    if (send(server_sockfd,file_buf,file_len,0) < 0) {
      fprintf(stderr, "ERROR: error writing to socket");
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
 * @return status
 */
int sendFile(char *file) {
  char file_buf[BUFSIZ]; // buffer for data to send
  int file_len; // filesize
  int local; // the local file descriptor
  int i; // for EOF scan
  char filename[BUFSIZ] = "from_server_";

  strcat(filename,file);

  // debug message
  puts("Command was GET!"); 

  recv(server_sockfd, file_buf, BUFSIZ, 0);

  if (!strcmp("ERROR", file_buf)) {
    perror("server returned error");
    return 1;
  }

  // open file or create it
  local = open(filename, O_WRONLY | O_CREAT, 00644);

  // very we opened it correctly
  if (!local) {
    fprintf(stderr, "ERROR: local file could not be opened: %s\n", file);
    send(server_sockfd, "ERROR", 6, 0);
    return 2;
  }

  send(server_sockfd, "READY", 6, 0);


  // read the file data from the client
   while((file_len = recv(server_sockfd, file_buf, BUFSIZ, 0)) > 0) {
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

int initialize(char *hostname) {
  // get host by name
  host = gethostbyname(hostname);
  if (host == (struct hostent *) NULL) {
    // or error
    perror("gethostbyname ");
    return 1;
  }

  // socket details
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  memcpy(&server_address.sin_addr, host->h_addr, host->h_length);
  server_address.sin_port = htons(6996);

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


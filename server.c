#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define PORT 8080

int main() {
    int server_fd, commSocket, nbytes, file;
    char buffer[4096] = {0};
    char message[64] = "Server recieves the message";
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    
    // open file for writing client information
    if((file = open("result.txt", O_WRONLY| O_APPEND | O_CREAT), 0644) < 0) {
    	perror("perror");
        exit(EXIT_FAILURE);
    }

    //open server socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Could not create socket\n");
        exit(EXIT_FAILURE);
    }
    
    //set socket option
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    	perror("setsocketopt");
    	exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP address
    address.sin_port = htons(PORT); //set port to 8080
    
    //bind socket with ip
    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address)) <0) {
    	perror("bind failed\n");
    	exit(EXIT_FAILURE);
    }

    

    //socket listening
    if(listen(server_fd, 3) < 0) {
    	perror("listen failed\n");
    	exit(EXIT_FAILURE);
    } else {
    	printf("Server listening ...\n");
    }


    //bind commSocket and server socket 
    if((commSocket = accept(server_fd, (struct sockaddr *) & address, (socklen_t*) &addrlen)) < 0 ) {
    	perror("accept error\n");
    	exit(EXIT_FAILURE);
    } 

    nbytes = recv( commSocket, buffer, sizeof(buffer), 0);
    write(file, buffer, nbytes);

    if(send(commSocket, message, strlen(message), 0) > 0){
    	printf("\nRecieve message sent\n");
    } else {
    	printf("\nRecieve messge failed to send");
    }
    memset(buffer, 0, sizeof(buffer));

    
    close(file);
    close(commSocket);
    close(server_fd);
    return 0;
}

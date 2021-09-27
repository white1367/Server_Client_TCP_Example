#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define PORT 8080

int main(){
    //for socket
    int socket_fd;
    char buffer[4096] = {0};
    struct sockaddr_in address;
    //for piping
    int status;
    int link[2];
    int index;
    char *args[32];


    if((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0) {
	perror("perror");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1"); //IP address
    address.sin_port = htons(PORT); //set port to 8080

    if(connect(socket_fd, (struct sockaddr*) & address, sizeof(address)) < 0) {
        perror("perror");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, sizeof(buffer));
    index = 0;
    args[index++] = "/sbin/ip";
    args[index++] = "a";
    args[index++] = NULL;

    if(pipe(link)==-1) {
	exit(1);
    }
    if(!fork()){
	//child process
	dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        execve(args[0], args, NULL);
        close(link[1]);
    } else {
        //parent process
        int nbytes = read(link[0], buffer, sizeof(buffer));
        send(socket_fd, buffer, strlen(buffer), 0);
    }

    close(socket_fd);
}

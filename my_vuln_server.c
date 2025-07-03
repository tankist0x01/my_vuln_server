#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>    
#include <sys/socket.h>   
#include <netinet/in.h>   

#define BUF_SZ 512
#define PORT 4556
#define SM_BUF 64

int vuln(char *buf) {
    char small[SM_BUF];
    strcpy(small, buf);

    return 1;
}

int main() {

    // struct
    int sockfd, client_fd;
    struct sockaddr_in addr, client_addr;
    ssize_t n;
    int opt = 1;

    // socket()
    sockfd = socket(AF_INET, SOCK_STREAM, 0);


    // close port
    // int getsockopt(int sockfd, int level, int optname,
    //                  void *optval, socklen_t *optlen);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(sockfd);
        exit(1);
    }

    // bind()

    
    // memset()
    // void *memset(void *s, int c, size_t n);
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(4556);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);


    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(1);
    };

    // listen()

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(1);
    };
    printf("Listening on %d port\n", PORT);


    // accept()

    socklen_t client_len = sizeof(client_addr);

    if ((client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len)) < 0) {
        perror("accept");
        close(sockfd);
        exit(1);
    };

    printf("Client connected: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));



    // recv()
    // ssize_t recv(int sockfd, void *buf, size_t len, int flags);

    char buf[BUF_SZ];


    while ((n = recv(client_fd, buf, sizeof(buf)-1, 0)) > 0) {
        buf[n] = '\0';
        printf("%s\n", buf);

        vuln(buf);
        
        // send()
        send(client_fd, buf, strlen(buf), 0);

    };

    if (n < 0) {
        perror("recv");
    }

    close(sockfd);
    close(client_fd);
    return 0;
}

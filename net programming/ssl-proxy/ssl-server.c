#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <signal.h>

#include <getopt.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <openssl/md5.h>
#include <openssl/evp.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/tcp.h>

int sock,_sock;
SSL *ssl;
SSL_CTX *ctx;
BIO *bio;
int result;
int count;
char buffer[1000];
char hahahaha[100] = "HTTP/1.1 200 Connection Established\r\n\r\n";
struct sockaddr_in saddr;

void sigCatcher(int n) {
    printf("a child process dies\n");
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

void ssl_init()
{
	SSL_library_init();
	SSL_load_error_strings();
	ctx=SSL_CTX_new(TLSv1_2_server_method());
	
	if((SSL_CTX_use_certificate_file(ctx,"public.crt",SSL_FILETYPE_PEM))==1)
		printf("SSL_CTX_use_certificate_file is OK()\n");
	else 
		printf("SSL_CTX_use_certificate_file failed\n");
	
	if((SSL_CTX_use_RSAPrivateKey_file(ctx,"private.pem",SSL_FILETYPE_PEM))==1)
		printf("SSL_use_RSAPrivateKey_file is OK()\n");
	else 
		printf("SSL_use_RSAPrivateKey_file failed\n");
	
}

void error_out(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}
void create_socket()
{
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
		error_out("socket()");
	else 
		printf("socket() is OK\n");

	int optval = 1;
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
        perror("setsockopt error");
        exit(1);
    }
	
	saddr.sin_port=htons(5555);
	saddr.sin_family=AF_INET;
	memset(&(saddr.sin_zero),0,8);
	
	saddr.sin_addr.s_addr=inet_addr("0.0.0.0");
	//inet_aton("127.0.0.1",&(saddr.sin_addr.s_addr));
	
	if((bind(sock,(struct sockaddr*)(&saddr),sizeof(saddr)))<0)
		error_out("bind():");
	else 
		printf("bind() is OK\n");
}
	
void finalize()
{
	BIO_free(bio);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	close(sock);
}

unsigned int maxfd(unsigned int a, unsigned int b) {
    unsigned int fd = a;
    if(b > fd) fd = b;
    return fd + 1;
}

void handle_client(int clnt_sock, SSL *ssl) {
    char *buf = (char*)malloc(sizeof(char) * 1000);
    char *host = (char*)malloc(sizeof(char) * 100);
    char *port = (char *)malloc(sizeof(char) * 100);
    printf("sizeof buf == %d\n", sizeof(buf));
    int count, i, j, start, t;
    int remote_sock;
    struct addrinfo hints, *res;
    int ttt = 30;
/*
    while(ttt--) {
	count = SSL_read(ssl, buf, 1000);
        printf("buf == %s\n", buf);
        printf("count == %d\n", count);
	
    }
*/
    count = SSL_read(ssl, buf , 1000);
    printf("buf == %s\n", buf);
    printf("count == %d\n", count);
    start = 0;
    for(i = 0; i < count ; ++i) {
        if(start != 0) break;
        if(buf[i] == ' ') {
            while(buf[i++] == ' ') continue;
	    i--;
            t = 0;
            for(; i < count ; ++i) {
                if(buf[i] == ':') break;
                host[t ++] = buf[i];
            }
            host[t ++] = 0;
            t = 0;
            i ++;
            for(;i < count; ++i) {
                if(buf[i] == ' ') break;
                port[t ++] = buf[i];
            }
	    port[t ++] = 0;
            start = 1;
            break;
        }
    }

    printf("host == %s\n", host);
    printf("port == %s\n", port);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int err = getaddrinfo(host, port, &hints, &res);
    if (err) {
        perror("getaddrinfo error");
        return;
    }

    remote_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(remote_sock, res->ai_addr, res->ai_addrlen);

        printf("new socket created: local = %d | remote = %d\n", clnt_sock, remote_sock);
    
    SSL_write(ssl, hahahaha, strlen(hahahaha));
    printf("send ok");
    fd_set io;
    unsigned char buff1[40960];
    unsigned char buff2[40960];

    for( ; ; ) {
        FD_ZERO(&io);
        FD_SET(clnt_sock, &io);
        FD_SET(remote_sock, &io);
        if( select(maxfd(clnt_sock, remote_sock), &io, NULL, NULL, NULL) < 0){
            puts("select error");
            break;
        }
        if(FD_ISSET(clnt_sock, &io)) {
            int count =  SSL_read(ssl, buff1, sizeof(buff1));
            if(count < 0) {
                perror("error count");
                close(clnt_sock);
                close(remote_sock);
                return ;
            }
            if(count == 0) {
                printf("count 0");
                close(clnt_sock);
                close(remote_sock);
                return ;
            }
            printf("log : read %d byte from client\n", count);

            send(remote_sock, buff1, count, 0);
        }

        if(FD_ISSET(remote_sock, &io)) {
            int count = recv(remote_sock, buff2, sizeof(buff2), 0);
            if(count < 0) {
                perror("error count");
                close(clnt_sock);
                close(remote_sock);
                return ;
            }
            if(count == 0) {
                puts("count 0");
                close(clnt_sock);
                close(remote_sock);
                return ;
            }
            printf("log : read %d byte from remote\n", count);

            SSL_write(ssl, buff2, count);
        }
    }

}



	
    int main(int argc, char **argv)
{
	signal(SIGCHLD, sigCatcher);

	ssl_init();
	
	ssl=SSL_new(ctx);
	
	create_socket();
	
	if((listen(sock,2))<0)
		error_out("listen():");
	else 
		printf("listen() is OK\n");

	while(1) {
                    if((_sock=accept(sock,0,0))<0)
                        error_out("accept():");
                    else 
                        printf("accept() is OK\n");

                    // clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &clnt_adr_size);
                    if(fork() == 0) {
	                   close(sock);
                            if((bio=BIO_new_socket(_sock,BIO_NOCLOSE))==NULL)
                                    printf("BIO_new_socket() failed\n");
                                else 
                                    printf("BIO_new_socket() is OK\n");
                                
                                SSL_set_bio(ssl,bio,bio);
                                
                                if((result=SSL_accept(ssl))==1)
                                    printf("SSL_accept() is OK\n");
                                else 
                                    printf("SSL_accept result:%d\n",result);


	         		handle_client(_sock ,ssl);
	            	exit(0);
	        }
	  	close(_sock);
	 }

	
	// count = 0;
	// while(1) {
	// 	SSL_read(ssl, buffer, sizeof(buffer));
	// 	printf("%s\n", buffer);
	// 	SSL_write(ssl, buf, sizeof(buf));
	// }
	finalize();
	return 0;
}


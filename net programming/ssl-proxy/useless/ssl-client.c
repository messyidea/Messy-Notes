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

int sock,res;
struct sockaddr_in saddr;
SSL_CTX *ctx;
	SSL *ssl;
	BIO *bio;
	
	
void error_out(char *msg)
{
	perror(msg);
	exit(1);
}
	
void connect_socket()
{
	if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
		error_out("socket()");
	else 
		printf("socket() is OK\n");
	
	saddr.sin_port=htons(443);
	saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
	saddr.sin_family=AF_INET;
		
	memset(&(saddr.sin_zero),0,8);
	
	if((connect(sock,(struct sockaddr*)&saddr,sizeof(struct sockaddr)))<0)
		error_out("connect():");
	else 
		printf("connect() is OK\n");

}			

void ssl_init()
{
	SSL_library_init();
	SSL_load_error_strings();
	ctx=SSL_CTX_new(TLSv1_2_client_method());
}


	
void finalize()
{
	BIO_free(bio);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	close(sock);
}
	
int main()
{
	ssl_init();
	connect_socket();
	
	if((bio=BIO_new_socket(sock,BIO_NOCLOSE))==NULL)
		printf("BIO_new() failed\n");
	else 
		printf("BIO_new_socket() is OK\n");
	
	if((ssl=SSL_new(ctx))==NULL)
		printf("SSL_new() failed\n");
	else 
		printf("SSL_new is OK\n");
	
	SSL_set_bio(ssl,bio,bio);
	
	res=SSL_connect(ssl);
	printf("SSL_connect result:%d\n",res);
	
	finalize();
	
	return 0;
}

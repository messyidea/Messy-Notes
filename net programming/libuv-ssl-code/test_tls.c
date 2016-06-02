
//%LICENSE////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2015 Devchandra M. Leishangthem (dlmeetei at gmail dot com)
//
// Distributed under the MIT License (See accompanying file LICENSE)
//
//////////////////////////////////////////////////////////////////////////
//
//%///////////////////////////////////////////////////////////////////////////

#include "uv_tls.h"
#include "stdio.h"
#include <assert.h>
#include "test_tls.h"
#include <netdb.h>
#include <string.h>


// typedef struct {
//     uv_tls_t handle;
//     uv_buf_t buf;
//     char *host;
//     char *port;
//     int status;
//     uv_tcp_t backend_handle;
//     uv_buf_t backend_buf;

//     uv_write_t wreq;
// } client_conn;

struct client_conn {
    uv_tls_t handle;
    uv_tcp_t backend_handle;
    uv_buf_t buf;
    char *host;
    char *port;
    int status;
    
    //uv_buf_t backend_buf;

    uv_write_t wreq;
};


static void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void on_write(uv_tls_t *tls, int status) {
    printf("\n\n write ok \n");
    //uv_tls_close((uv_handle_t*)&tls->skt, (uv_close_cb)free);
}

void after_tls_write(uv_tls_t *tls, int status) {
    printf("after tls write");
    struct client_conn *client = (struct client_conn *)tls;
    uv_read_start((uv_stream_t*)&(client->backend_handle), on_alloc, on_remote_read);
    uv_tls_read((uv_stream_t*)&client->handle, NULL, uv_pure_rd_cb);
    return ;
}

void uv_pure_rd_cb(uv_stream_t *strm, ssize_t nrd, const uv_buf_t *bfr) {

    struct client_conn *client = (struct client_conn *)strm;
    if (nrd == 0 || nrd < 0) {
        puts("free and stop");
        uv_read_stop((uv_stream_t*)&(client->backend_handle));
        uv_tls_close((uv_handle_t*)&client->handle, (uv_close_cb)free);
        free(client);
        return ;
    }
    uv_write(&(client->wreq), (uv_stream_t*)&client->backend_handle, &client->buf, 1, after_remote_send);
}

void on_remote_read(uv_stream_t* strm, ssize_t nrd, const uv_buf_t* buf) {
    printf("\non remote read\n");
    printf("remote read data == %d\n", nrd);
    // printf("get %s\n", buf->base);
    struct client_conn* client = strm->data;
    printf("dizhi %d\n", (int)strm);
    printf("client dizhi 2333 = %d\n", (int)client);
    //struct client_conn* client = (struct client_conn*)(((void*)strm) - sizeof(uv_tls_t));
    if (nrd == 0 || nrd < 0) {
        puts("free and stop");
        uv_read_stop((uv_stream_t*)&(client->backend_handle.data));
        uv_tls_close((uv_handle_t*)&client->handle, (uv_close_cb)free);
        free(client);
        return ;
    }
    assert( client != NULL);
    uv_buf_t wbuf = uv_buf_init(buf->base, buf->len);
    wbuf.len = buf->len;
    printf("\n client status == %d\n", client->status);
    uv_tls_write((uv_tls_t*)&(client->handle), (uv_buf_t*)&wbuf, after_tls_write);
}




void after_remote_send(uv_write_t* req, int status) {
    printf("after remote send");
    int r;
    struct client_conn *client = req->data;
    // struct client_conn *client = (struct client_conn*)(req )
    assert(client != NULL);
    //(client->backend_handle).data = client;
    printf("dizhi2 == %d\n", (int)((client->backend_handle.data)));
    printf("dizhi2 tttttttttt == %d\n", (int)(client));
    r = uv_read_start((uv_stream_t*)&(client->backend_handle), on_alloc, on_remote_read);
    assert(r == 0);
    //uv_tls_read((uv_stream_t*)&client->handle, NULL, uv_pure_rd_cb);
}

void on_backend_connect(uv_connect_t* req, int status) {
    printf("connect statsus =  %d\n", status);
    printf("xixix33333");
    struct client_conn *client = req->data;
    printf("xixix22222");
    assert(client != NULL);
    printf("xixix");
    client->status = 1;
    // client->backend_handle.data = client;
    printf("start write");
    uv_buf_t wbuf = uv_buf_init(client->buf.base, client->buf.len);
    client->wreq.data = wbuf.base;

    uv_write_t* wreq = (uv_write_t*) malloc(sizeof *wreq);
    wreq->data = client;

    uv_write(wreq, (uv_stream_t*)&(client->backend_handle), &wbuf, 1, after_remote_send);


}

void do_connect(uv_getaddrinfo_t* req, int status, struct addrinfo* res) {
    int r, err;
    printf("do connect");
    uv_connect_t *creq = (uv_connect_t *)malloc(sizeof(uv_connect_t));
    struct client_conn *client = req->data;

    r = uv_tcp_init(req->loop, &(client->backend_handle));
    assert(r == 0);

    creq->data = client;
    assert( client != NULL);
    printf("xixixixxxxxxxxxxxxxxxxxxxxxx");
    (client->backend_handle).data = client;
    printf("dizhi33 == %d\n", (int)(&(client->backend_handle.data)));
    printf("client dizhi = %d\n", (int)client);
    printf("dizhi33 ttttt == %d\n", (int)((client->backend_handle.data)));
    printf("hehehehhhhhhhhhhhhhhhhhhhhhhh");
    printf("start connect");
    err = uv_tcp_connect(creq, &client->backend_handle, (const struct sockaddr*) res->ai_addr, on_backend_connect);
    if (err != 0) {
        printf("connect error: %s", uv_strerror(err));
    }
}

void get_host_port_from_buf(const uv_buf_t *bfr, char *host, char *port) {
    //strcpy(host, "messyidea.com");
    //strcpy(port, "80");
    //printf("host == %s\n", host);
    //return;
    char * header = bfr->base;
    printf("xixixix\n");
    char * p = strstr(header,"Host:");
    printf("heheheh");
    if(!p) 
    {
        printf("case1");
        return ;
    }
    char * p1 = strchr(p,'\n');
    if(!p1) 
    {
        printf("case2");
        return ; 
    }
    printf("1\n");
    char * p2 = strchr(p + 5,':'); /* 5是指'Host:'的长度 */

    if(p2 && p2 < p1) 
    {
        
        int p_len = (int)(p1 - p2 -1);
        //char s_port[p_len];
        strncpy(port,p2+1,p_len);
        port[p_len] = '\0';
        // remote_port = atoi(s_port);

        int h_len = (int)(p2 - p -5 -1 );
        strncpy(host,p + 5 + 1  ,h_len); //Host:
        //assert h_len < 128;
        host[h_len] = '\0';
    } else 
    {   
        int h_len = (int)(p1 - p - 5 -1 -1); 
        strncpy(host,p + 5 + 1,h_len);
        //assert h_len < 128;
        host[h_len] = '\0';
        strncpy(port, "80", 2);
        port[2] = '\0';
    }
    printf("2\n");
    printf("host == %s\n", host);
    printf("port == %s\n", port);
    return ;
    //memcpy(host, "www.baidu.com", 10);
    //memcpy(port, "1234", 4);
}

void uv_rd_cb( uv_stream_t *strm, ssize_t nrd, const uv_buf_t *bfr) {
    printf("\n do uv_rd_cb");
    if ( nrd <= 0 ) return;
    int err;
    struct client_conn *client = (struct client_conn *)strm;
    //client->backend_handle.data = client;
    printf("read %s\n", bfr->base);
    printf("11111111");
    client->buf.base = bfr->base;
    //memcpy(client->buf.base, bfr->base, nrd);
    printf("22222222");
    client->buf.len = nrd;

    client->host = (char *)malloc(sizeof(char) * 100);
    client->port = (char *)malloc(sizeof(char) * 100);
    get_host_port_from_buf(bfr, client->host, client->port);

    printf("111");
    uv_getaddrinfo_t *resolver = (uv_getaddrinfo_t *)malloc(sizeof(*resolver));
    puts("222");
    resolver->data = client;
    puts("333");
    struct addrinfo hints;
    puts("444");
    memset(&hints, 0, sizeof(hints));
    puts("555");
    hints.ai_family = AF_UNSPEC;
    puts("666");
    hints.ai_socktype = SOCK_STREAM;
    puts("777");
    hints.ai_protocol = IPPROTO_TCP;
    puts("888");
    puts("get addr info");
    puts("999");
    printf("now host == %s\n", client->host);
    err = uv_getaddrinfo(strm->loop,
                       resolver,
                       do_connect,
                       client->host,
                       client->port,
                       &hints);
    if (err != 0) {
        printf("getaddrinfo: %s", uv_strerror(err));
        return ;
    }
    //uv_tls_write((uv_tls_t*)strm, (uv_buf_t*)bfr, on_write);
}


void on_uv_handshake(uv_tls_t *ut, int status) {
    if ( 0 == status )
        uv_tls_read((uv_stream_t*)ut, NULL, uv_rd_cb);
    else
        uv_tls_close((uv_handle_t*)ut, (uv_close_cb)free);
}
void on_connect_cb(uv_stream_t *server, int status) {
    if( status ) return;
    struct client_conn *client = malloc(sizeof(*client));
    assert(client != NULL);

    client->status = 0;
    client->wreq.data = client;
    printf("client dizhi zhengzhengzheng = %d\n", (int) client);
    // client->rreq.data = client;

    //uv_tls_t *sclient = malloc(sizeof(*sclient)); //freed on uv_close callback
    if( uv_tls_init(server->loop, (evt_ctx_t*)server->data, &client->handle) < 0 ) {
        free(client);
        return;
    }
    if (!uv_accept(server, (uv_stream_t*)&((client->handle).skt))) {
        uv_tls_accept(&(client->handle), on_uv_handshake);
    }
}

int main() {
    printf("heheheheheehh33333333");
    uv_loop_t *loop = uv_default_loop();
    int port = 5555, r = 0;
    evt_ctx_t ctx;
    // struct sockaddr_in bind_addr;
    struct sockaddr_in bind_local;

    evt_ctx_init_ex(&ctx, "server-cert.pem", "server-key.pem");
    evt_ctx_set_nio(&ctx, NULL, uv_tls_writer);

    uv_tcp_t listener_local;
    uv_tcp_init(loop, &listener_local);
    listener_local.data = &ctx;
    uv_ip4_addr("0.0.0.0", port, &bind_local);
    if ((r = uv_tcp_bind(&listener_local, (struct sockaddr*)&bind_local, 0)))
        fprintf( stderr, "bind: %s\n", uv_strerror(r));

    if ((r = uv_listen((uv_stream_t*)&listener_local, 128, on_connect_cb)))
        fprintf( stderr, "listen: %s\n", uv_strerror(r));
    printf("Listening on %d\n", port);
    uv_run(loop, UV_RUN_DEFAULT);
    evt_ctx_free(&ctx);
    return 0;
}

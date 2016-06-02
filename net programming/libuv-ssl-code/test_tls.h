#ifndef TEST_UV_TLS_H
#define TEST_UV_TLS_H

static void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void on_write(uv_tls_t *tls, int status);
void after_tls_write(uv_tls_t *tls, int status);
void uv_pure_rd_cb(uv_stream_t *strm, ssize_t nrd, const uv_buf_t *bfr);
void on_remote_read(uv_stream_t* stream, ssize_t nrd, const uv_buf_t* buf);
void after_remote_send(uv_write_t* req, int status);
void on_backend_connect(uv_connect_t* req, int status);
void do_connect(uv_getaddrinfo_t* req, int status, struct addrinfo* res);
void get_host_port_from_buf(const uv_buf_t *bfr, char *host, char *port);
void uv_rd_cb( uv_stream_t *strm, ssize_t nrd, const uv_buf_t *bfr);
void on_uv_handshake(uv_tls_t *ut, int status);
void on_connect_cb(uv_stream_t *server, int status);





#endif
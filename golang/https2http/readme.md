https stream to http stream
===

Since many applications do not support https proxy, it's useful to trans tls/ssl stream to normal stream.

usage
---
```shell
./https2http -listen :8888 -remote your.domain.com:port
```
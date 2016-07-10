Deploy Anyconnect by Docker
===

for docker
```
docker pull treelite/anyconnect
```

create certs
```
cd ~
mkdir certs
cd certs
```

create ca.tmpl， content as follows
```
cn = "Your CA name"
organization = "Your fancy name"
serial = 1
expiration_days = 3650
ca
signing_key
cert_signing_key
crl_signing_key
```

create ca key
```
certtool --generate-privkey --outfile ca-key.pem
```

create ca cert
```
certtool --generate-self-signed --load-privkey ca-key.pem --template ca.tmpl --outfile ca-cert.pem
```

create server.tmpl， content as follows
```
cn = "Your hostname or IP"
organization = "Your fancy name"
expiration_days = 3650
signing_key
encryption_key
tls_www_server
```

create server key
```
certtool --generate-privkey --outfile server-key.pem
```

create server cert
```
certtool --generate-certificate --load-privkey server-key.pem --load-ca-certificate ca-cert.pem --load-ca-privkey ca-key.pem --template server.tmpl --outfile server-cert.pem
```

run anyconnect
```
docker run -d --privileged -p 7000:443 --name anyconnect -v $HOME/certs:/certs treelite/anyconnect
```

For user
===
create user.tmpl, content as follows
```
cn = "some random name"
unit = "some random unit"
expiration_days = 365
signing_key
tls_www_client
```

create user key
```
certtool --generate-privkey --outfile user-key.pem
```

create user cert
```
certtool --generate-certificate --load-privkey user-key.pem --load-ca-certificate ca-cert.pem --load-ca-privkey ca-key.pem --template user.tmpl --outfile user-cert.pem
```

change the key and cert to PKCS12
```
certtool --to-p12 --load-privkey user-key.pem --pkcs-cipher 3des-pkcs12 --load-certificate user-cert.pem --outfile user.p12 --outder
```

now you can just download the file user.p12 and import to your phone.



reference
===
https://github.com/treelite/anyconnect

https://bitinn.net/11084/

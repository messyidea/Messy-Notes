ways of bytetokey
---

加密中有一个需求，就是根据现有的密码来生成新的key和iv。

对于一个固定的加密方法，key和iv的长度是固定的

比如：
```
aes-256-cfb:
key len == 32
iv len == 16

aes-16-cfb:
key len == 16
iv len == 16

rc4:
key len == 16
iv len == 0
```

openssl 提供了 EVP_BytesToKey
```c
EVP_BytesToKey(EVP_des_ede3_cbc,EVP_md5,NULL,passwd,strlen(passwd),key,iv);
```

这个过程是这样的：
```golang
func evpBytesToKey(password string, keyLen int) (key []byte) {
    const md5Len = 16

    cnt := (keyLen-1)/md5Len + 1
    m := make([]byte, cnt*md5Len)
    copy(m, md5sum([]byte(password)))

    // Repeatedly call md5 until bytes generated is enough.
    // Each call to md5 uses data: prev md5 sum + password.
    d := make([]byte, md5Len+len(password))
    start := 0
    for i := 1; i < cnt; i++ {
        start += md5Len
        copy(d, m[start-md5Len:start])
        copy(d[md5Len:], password)
        copy(m[start:], md5sum(d))
    }
    return m[:keyLen]
}
// iv = make([]byte, c.info.ivLen)
// _, err := io.ReadFull(rand.Reader, iv);
```

当然这个是可选的，也可以自己随意来，只要key和iv足够随机就行，下面就是qtunnel生成key和iv的方法
```golang
func secretToKey(secret []byte, size int) []byte {
    // size mod 16 must be 0
    h := md5.New()
    buf := make([]byte, size)
    count := size / md5.Size
    // repeatly fill the key with the secret
    for i := 0; i < count; i++ {
        h.Write(secret)
        copy(buf[md5.Size*i:md5.Size*(i+1)-1], h.Sum(nil))
    }
    return buf
}
//iv ：= key[:block.BlockSize()]
```


关于md5函数的一些用法
---

# for golang
```golang
package main

import (
    "crypto/md5"
    "encoding/hex"
    "fmt"
)

func main() {
    h := md5.New()
    h.Write([]byte("123456123456")) // 需要加密的字符串为 123456123456
    cipherStr := h.Sum(nil)
    fmt.Println(cipherStr)
    hh := md5.New()
    hh.Write([]byte("123456")) // 需要加密的字符串为 123456
    cipherStr2 := hh.Sum(nil)
    fmt.Println(cipherStr2)
    hh.Write([]byte("123456")) // 在原来的123456后面添加123456
    cipherStr3 := hh.Sum(nil)
    fmt.Println(cipherStr3)
    fmt.Printf("%s\n", hex.EncodeToString(cipherStr)) // 输出加密结果
}

// output
// [234 72 87 111 48 190 22 105 151 22 153 192 154 208 92 148]
// [225 10 220 57 73 186 89 171 190 86 224 87 242 15 136 62]
// [234 72 87 111 48 190 22 105 151 22 153 192 154 208 92 148]
```

# for c
```
byte* secretToKey(char* sec, int size) {
    byte *buf = malloc(sizeof(char) * 16);
    byte *buf2 = malloc(sizeof(char) * 16);
    MD5_CTX h;
    MD5_Init(&h);
    int count = size / 16;
    int i,j;
    for(i = 0; i < count; ++i) {
        MD5_Update(&h, sec, strlen(sec));
        MD5_Final(buf2, &h);
        strncpy(buf, buf2, 15);
    }
    buf[15]=0;
    return buf;
}
```

# [openssl evp_bytetokey 过程](http://blog.csdn.net/kkxgx/article/details/12879367)


openssl 使用的一般过程
---

```
1.定义变量
char key[EVP_MAX_KEY_LENGTH];
char iv[EVP_MAX_IV_LENGTH];
EVP_CIPHER_CTX ctx;
unsigned char out[512+8];
int outl;

2.byte to key
EVP_BytesToKey(EVP_des_ede3_cbc,EVP_md5,NULL,passwd,strlen(passwd),key,iv);

3.设置key和iv，初始化EVP_CIPHER_CTX
EVP_EncryptInit_ex(&ctx, EVP_des_ede3_cbc(), NULL, key, iv);

4.加解密
while (....){
    EVP_EncryptUpdate(ctx,out,&outl,in,512);
}

5.结束加密
EVP_EncryptFinal_ex(&ctx, out, &outl)
```

[一些有用链接](http://blog.csdn.net/jasenwan88/article/details/7718901)

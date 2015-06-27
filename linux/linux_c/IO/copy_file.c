#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 1024    //读写缓存大小
#define SRC_FILE_NAME "src_file"
#define DEST_FILE_NAME "dest_file"
#define OFFSET 10240    //复制数据大小

int main()
{
    int src_file, dest_file;
    unsigned char buff[BUFFER_SIZE];
    int real_read_len;

    src_file = open(SRC_FILE_NAME, O_RDONLY);
    //只读打开文件,如果不存在则创建,访问权限值为644
    dest_file = open(DEST_FILE_NAME, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

    if(src_file < 0 || dest_file < 0) {
        printf("open file error\n");
        exit(1);
    }
    //读写指针移到最后10KB起始位置
    lseek(src_file, -OFFSET, SEEK_END);

    while((real_read_len = read(src_file, buff, sizeof(buff))) > 0) {
        write(dest_file, buff, real_read_len);
    }

    close(dest_file);
    close(src_file);

}

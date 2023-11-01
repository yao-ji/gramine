#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define FILE_SIZE 3 + 1024 * 64 // in KB

int main(void) {
    char buf[1024];
    int fd = open("/dev/urandom", O_RDONLY);
    read(fd, buf, sizeof(buf));
    close(fd);

    FILE* fp = fopen("test.enc", "w");
    if (!fp) {
        printf("fopen err, %d\n", errno);
        return -1;
    }
    memset(buf, 0x41, sizeof(buf));
    for (int i = 0; i < FILE_SIZE; i++) {
        size_t wb = fwrite(buf, 1, sizeof(buf), fp);
        if (wb != sizeof(buf))  {
            printf("fwrite err in round %d, %ld\n", i, wb);
            fclose(fp);
            return -1;
        }
    }
    if (fclose(fp))  {
        printf("fclose err, %d\n", errno);
        return -1;
    }
    
    printf("test.enc write success\n");

    fp = fopen("test.enc", "r");
    if (!fp) {
        printf("fopen err, %d\n", errno);
        return -1;
    }
    memset(buf, 0, sizeof(buf));
    for (int i = 0; i < FILE_SIZE; i++) {
        size_t rb = fread(buf, 1, sizeof(buf), fp);
        if (rb != sizeof(buf))  {
            printf("fread err in round %d, %ld\n", i, rb);
            fclose(fp);
            return -1;
        }
    }
    if (fclose(fp))  {
        printf("fclose err, %d\n", errno);
        return -1;
    }
    
    printf("test.enc read success\n");

    return 0;
}

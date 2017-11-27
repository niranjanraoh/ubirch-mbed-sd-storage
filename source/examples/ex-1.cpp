//
// Created by nirao on 21.11.17.
//


#include "mbed.h"
#include "../SDStorage.h"


//SDSTORAGE sd(PTE3, PTE1, PTE2, PTE4);
//
//const char * path = "/ub/myfile.bin";
//int main(){
//    int ret = sd.init();
//
//    sd.write(path, (void *)"hello world123456\r\n", 19, 1);
//    char buf[20] = {0};
//    sd.read(path, buf, 13, 1);
//
//
//
//    printf("buf: %s", buf);
//}
//
//
//








































#if 0


#include "mbed.h"
#include "FATFileSystem.h"
#include "SDBlockDevice.h"
#include <stdio.h>
#include <errno.h>
/* mbed_retarget.h is included after errno.h so symbols are mapped to
 * consistent values for all toolchains */
#include "platform/mbed_retarget.h"


SDBlockDevice sd(SDHC_CMD, SDHC_D0, SDHC_DCLK, SDHC_D3);
FATFileSystem fs("sd", &sd);

void return_error(int ret_val){
    if (ret_val)
        printf("Failure. %d\r\n", ret_val);
    else
        printf("done.\r\n");
}

void errno_error(void* ret_val){
    if (ret_val == NULL)
        printf(" Failure. %d \r\n", errno);
    else
        printf(" done.\r\n");
}

int main()
{

    int error = 0;
//    error = fs.mount(&sd);
    printf("Welcome to the filesystem example.\r\n");

    printf("Opening a new file, numbers.txt.");
    FILE* fd = fopen("/sd/numbers.txt", "w+");
    errno_error(fd);

    for (int i = 0; i < 20; i++){
        printf("Writing decimal numbers to a file (%d/20)\r", i);
        fprintf(fd, "%d\r\n", i);
    }
    printf("Writing decimal numbers to a file (20/20) done.\r\n");

    printf("Closing file.");
    fclose(fd);
    printf(" done.\r\n");

    printf("Re-opening file read-only.");
    fd = fopen("/sd/numbers.txt", "r");
    errno_error(fd);

    printf("Dumping file to screen.\r\n");
    char buff[16] = {0};
    while (!feof(fd)){
        int size = fread(&buff[0], 1, 15, fd);
        fwrite(&buff[0], 1, size, stdout);
    }
    printf("EOF.\r\n");

    printf("Closing file.");
    fclose(fd);

    FILE *fd1 = fopen("/sd/numbers.txt", "a");
    for (int j = 0; j < 10; ++j) {
        fprintf(fd1, "hwllo world %d\r\n", j*121);
    }
    printf("Closing file.");
    fclose(fd1);
    printf(" done.\r\n");

    printf("Re-opening file read-only.");
    fd1 = fopen("/sd/numbers.txt", "r");
    errno_error(fd1);

    char readbuff[256] = {0};
    while (!feof(fd1)){
        int size = fread(&buff[0], 1, 15, fd1);
        fwrite(&buff[0], 1, size, stdout);
    }

    fclose(fd1);

    printf(" done.\r\n");

    printf("Opening root directory.");
    DIR* dir = opendir("/sd/");
    errno_error(fd);

    struct dirent* de;
    printf("Printing all filenames:\r\n");
    while((de = readdir(dir)) != NULL){
        printf("  %s\r\n", &(de->d_name)[0]);
    }

    printf("Closeing root directory. ");
    error = closedir(dir);
    return_error(error);
    printf("Filesystem Demo complete.\r\n");

    while (true) {}
}




#endif
//
// Created by nirao on 21.11.17.
//
#if 0
#include "mbed.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

SDBlockDevice sdBlockDevice(PTE3, PTE1, PTE2, PTE4);
FATFileSystem fs("fs");

static const char * sd_file_path = "/fs/out.txt";

#define MAX_DATA_SIZE 10
uint8_t dataWritten[MAX_DATA_SIZE] = {0};

DigitalOut    led1(LED1);

int main() {
    int error;
    printf("Mounting the filesystem\r\n");
    error = fs.mount(&sdBlockDevice);

    if (error) {
        printf("Filesystem mounting failed, reformatting the SD card\r\n");
        error = FATFileSystem::format(&sdBlockDevice);

        if (error) {
            printf("Failed to format the SD card. Exiting.\r\n");
            mbed_die();
        } else {
            printf("SD card formatted, retrying mount\r\n");
            error = fs.mount(&sdBlockDevice);

            if (error) {
                printf("Filesystem mounting failed after SD card format. Exiting.\r\n");
                mbed_die();
            }
        }
    }

    printf("Filesystem mounted\r\n");

    FILE *f = fopen(sd_file_path, "w");
    printf("open file %s\r\n", sd_file_path);

    if (f){
        printf("open file %s\r\n", sd_file_path);
        printf("ff\r\n");
//        for (int i = 0; i < MAX_DATA_SIZE; i++) {
//            fwri
////            dataWritten[i] = (uint8_t) (rand() % 0xFF);
////            fprintf(f, "%c", i);
//            printf("writing data %d\r\n", dataWritten[i]);
//        }
        int data[MAX_DATA_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        fwrite(data, sizeof(data), 1, f);
        printf("data wrtie done \r\n");
        fclose(f);
    }else printf("failed to open file : w");

    FILE *fo = fopen(sd_file_path, "w");

    fseek(fo, 0, SEEK_END); // seek to end of file
    long size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET);
    printf("size: %ld\r\n", size);
    fclose(fo);
//    if (f){
//        printf("open file %s\r\n", sd_file_path);
//        printf("ff\r\n");
//        for (int i = 0; i < MAX_DATA_SIZE; i++) {
//            dataWritten[i] = (uint8_t) (rand() % 0xFF);
//            fprintf(f, "%c", dataWritten[i]);
//            printf("writing data %d\r\n", dataWritten[i]);
//        }
//        printf("data wrtie done \r\n");
//        fclose(f);
//    }else printf("failed to open file : w");



//    fo = fopen(sd_file_path, "r");
//
//    if (f) {
//        for (int i = 0; i < MAX_DATA_SIZE; ++i) {
//            uint8_t data = fgetc(f);
//            if (data != dataWritten[i]) {
//                printf("read data false: %d:: %d\r\n", dataWritten[i], data);
//            }
//            printf("read data: %d:: %d\r\n", dataWritten[i], data);
//        }
//        fclose(f);
//    } else {
//        printf("failed to open file : r");
//    }

//    fo = fopen(sd_file_path, "a");
//    if (f){
//        printf("open file %s\r\n", sd_file_path);
//        printf("ff\r\n");
//        for (int i = 0; i < MAX_DATA_SIZE; i++) {
//            dataWritten[i] = (uint8_t) (rand() % 0xFF);
//            fprintf(f, "%c", dataWritten[i]);
//            printf("writing data %d\r\n", dataWritten[i]);
//        }
//        fprintf(f, "helloworld\r\n");
//
//        printf("data wrtie done \r\n");
//        fclose(f);
//    }else printf("failed to open file : w");
//
//    fo = fopen(sd_file_path, "r");

//    if (fo) {
//        for (int i = 0; i < 15; ++i) {
//            uint8_t data = fgetc(fo);
//            if (data != dataWritten[i]) {
//                printf("read data false: %d:: %d\r\n", dataWritten[i], data);
//            }
//            printf("read data: %d:: %d\r\n", dataWritten[i], data);
//        }
////        fclose(fo);
//    } else {
//        printf("failed to open file : r");
//    }
//
//    printf("read 2nd time \r\n");
//    fo = fopen(sd_file_path, "r");
//
//    if (fo) {
//        for (int i = 0; i < 15; ++i) {
//            uint8_t data = fgetc(fo);
//            if (data != dataWritten[i]) {
//                printf("read data false: %d:: %d\r\n", dataWritten[i], data);
//            }
//            printf("read data: %d:: %d\r\n", dataWritten[i], data);
//        }
//        fclose(fo);
//    } else {
//        printf("failed to open file : r");
//    }

//    char buff[20] = {0};
//    fread(&buf[0], 1, 20, fo);
//    printf("buf: %s\r\n", buf);

//    FILE *fd = fopen(sd_file_path, "r");
//    while (!feof(fd)){
//        int size = fread(&buff[0], 1, 15, fd);
//        fwrite(&buff[0], 1, size, stdout);
//    }

//    fseek(fo, 0, SEEK_END); // seek to end of file
//    long size = ftell(f); // get current file pointer
//    fseek(f, 0, SEEK_SET);
//    printf("size: %ld\r\n", size);
//    fclose(fo);
//    struct stat st;

//    fs.stat(sd_file_path, &st);

//    printf("the size %d", st.st_size);




}

#endif

//
// Created by nirao on 22.11.17.
//

#if 0

#include "mbed.h"
#include "SDBlockDevice.h"
#include "FATFileSystem.h"

SDBlockDevice sd (PTE3, PTE1, PTE2, PTE4);
FATFileSystem fs("ub");

DigitalOut led(LED1);

static const char * measureFilePath = "/ub/measure.bin";
static const char * sendFilePath = "/ub/send.bin";


#define MEASURE_ARRAY 12
struct Measure {
    time_t ts;
    double  tValue;
};

Measure m[MEASURE_ARRAY] = {0};

void ledblink(int count);

void ledblink(int count) {
    for (int i = 0; i < count; ++i) {
        led = !led;
        Thread::wait(600);
        led = !led;
        Thread::wait(600);
    }
}

int main (){
    printf("Fire up the device.. \r\n");
    ledblink(2);

    set_time(1511364758);

    printf("Mount the filesystem\r\n");
    int error = fs.mount(&sd);

    if (error){
        printf("FileSystem mounting failed, format the SD card\r\n");
        error = FATFileSystem::format(&sd);

        if (error){
            printf("Failed to format\r\n");
            ledblink(5);
        } else {
            printf("SD card formatted, retyr mount\r\n");
            error = fs.mount(&sd);
            if (error){
                printf("Remount failed \r\n");
                ledblink(5);
            }
        }
    }
    /*++++++++++++++++++++++++++++++++++*/

    printf("Mounted\r\n");

    /* WRITE*/
    FILE *f = fopen(measureFilePath, "w");
    printf("Open file %s\r\n", measureFilePath);

    size_t cin = 0;
    size_t  size = 0;

    if (f){
        for (int i = 0; i < MEASURE_ARRAY; ++i) {
            m[i].ts = time(NULL);
            m[i].tValue = 2.3 * i;
            Thread::wait(500);
        }
        cin = fwrite(m, sizeof(Measure), MEASURE_ARRAY, f);
        printf("\r\n\r\ncin: %u\r\n\r\n", cin);
    } else {
        printf("Failed to open file \r\n");
        ledblink(2);
    }
    /*++++++++++++++++++++++++++++++++++*/

    /*GET FILE SIZE*/
    fseek(f, 0, SEEK_END); // seek to end of file
    long fsize = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET);
    printf("size: %ld, %d\r\n", fsize, static_cast<int>(fsize / cin));
    fclose(f);
    /*++++++++++++++++++++++++++++++++++*/

    /* READ*/
    f = fopen(measureFilePath, "r+");
    Measure measure[30] ={0};
    while (!feof(f)){
        size = fread(&measure[0], sizeof(Measure), (fsize/cin), f);
//        fwrite(&measure[0], sizeof(Measure), size, stdout);
        printf("read- size %d\r\n", static_cast<int>(size));
    }
    printf("\r\n");
    for (int j = 0; j < size; ++j) {
        printf("ts: %lu T-> %f\r\n", measure[j].ts, measure[j].tValue);
    }
//    fclose(f);
    /*++++++++++++++++++++++++++++++++++*/

// start reading after 5 values
    fseek(f, sizeof(Measure) * 5, SEEK_SET);
    Measure smes = {
            .ts = 0,
            .tValue = 99.99
    };
    fwrite(&smes, sizeof(Measure), 1, f);
    while (!feof(f)){
        size = fread(&measure[0], sizeof(Measure), 30, f);
//        fwrite(&measure[0], sizeof(Measure), size, stdout);
        printf("read- size %d\r\n", static_cast<int>(size));
    }
    printf("\r\n");
    for (int j = 0; j < size; ++j) {
        printf("ts: %lu T-> %f\r\n", measure[j].ts, measure[j].tValue);
//        printf("ts: %lu : %lu :: T-> %f:%f\r\n", m[j].ts, measure[j].ts, m[j].tValue, measure[j].tValue);
    }

//    measure = {0};

    fseek(f, sizeof(Measure) * 2, SEEK_CUR);
    Measure mes = {
            .ts = 0,
            .tValue = 99.99
    };
    fwrite(&mes, sizeof(Measure), 1, f);
    size = fread(&measure[0], sizeof(Measure), 4, f);
    for (int j = 0; j < size; ++j) {
        printf("seekcurts: %lu T-> %f\r\n", measure[j].ts, measure[j].tValue);
//        printf("ts: %lu : %lu :: T-> %f:%f\r\n", m[j].ts, measure[j].ts, m[j].tValue, measure[j].tValue);
    }

    fflush(f);

//    while (!feof(f)){
        size = fread(&measure[0], sizeof(Measure), 30, f);
//        fwrite(&measure[0], sizeof(Measure), size, stdout);
        printf("read- size %d\r\n", static_cast<int>(size));
//    }


    fclose(f);
//    remove(measureFilePath);
//    int pos = fgetpos


    /* APPEND*/
/*    f = fopen(measureFilePath, "a");
    printf("Open file %s\r\n", measureFilePath);
    if (f){
        for (int i = 0; i < MEASURE_ARRAY; ++i) {
            m[i].ts = time(NULL);
            m[i].tValue = 2.3 * i;
            Thread::wait(500);
        }
        fwrite(m, sizeof(Measure), MEASURE_ARRAY, f);
    } else {
        printf("Failed to open file \r\n");
        ledblink(2);
    }*/
    /*++++++++++++++++++++++++++++++++++*/

    /*GET FILE SIZE*/
/*    fseek(f, 0, SEEK_END); // seek to end of file
    fsize = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET);
    printf("size: %ld\r\n", fsize);
    fclose(f);*/
    /*++++++++++++++++++++++++++++++++++*/

    /*READ+*/
/*    FILE *fa = fopen(measureFilePath, "r+");
    if (!feof(fa)){
        size = fread(&measure[0], sizeof(Measure), 24, fa);
        fwrite(&measure[0], sizeof(Measure), size, stdout);
    }
    printf("\r\n");
    for (int j = 0; j < size; ++j) {
        printf("ts: %lu : %lu :: T-> %f:%f\r\n", m[j].ts, measure[j].ts, m[j].tValue, measure[j].tValue);
    }*/
    /*++++++++++++++++++++++++++++++++++*/


    /*RENAME / REMOVE*/
    printf("file name : %s\r\n", fs.getName());

    printf("Opening root directory.");
    DIR* dir = opendir("/ub/");
//    errno_error(fd);

    struct dirent* de;
    printf("Printing all filenames:\r\n");
    while((de = readdir(dir)) != NULL){
        printf("  %s\r\n", &(de->d_name)[0]);
    }

    printf("Closeing root directory. ");
    error = closedir(dir);
//    return_error(error);
    printf("Filesystem Demo complete.\r\n");

    printf("close the file\r\n");
}


#endif
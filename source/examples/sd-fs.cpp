//
// Created by nirao on 20.11.17.
//
#if 0

#include <FATFileSystem.h>
#include <fsfat_debug.h>
#include <unity/unity.h>
#include "mbed.h"
#include "sd-driver/SDBlockDevice.h"

static const char *sd_mount_point = "sd";
//static const char * sd_file_path = "/sd/out.txt";
SDBlockDevice sd(SDHC_CMD, SDHC_D0, SDHC_DCLK, SDHC_D3);
FATFileSystem fs(sd_mount_point, &sd);

static const char *sd_file_path = "/sd/out.txt";
#define MAX_DATA_SIZE  256

int main() {

    int error = fs.mount(&sd);
    uint8_t dataWritten[MAX_DATA_SIZE] = {0};

    FILE *f = fopen(sd_file_path, "w");
    printf("open file %s\r\n", sd_file_path);

    if (f){
        printf("open file %s\r\n", sd_file_path);
        printf("ff\r\n");
        for (int i = 0; i < MAX_DATA_SIZE; i++) {
            dataWritten[i] = (uint8_t) (rand() % 0xFF);
            fprintf(f, "%c", dataWritten[i]);
            printf("writing data %d\r\n", dataWritten[i]);
        }
        printf("data wrtie done \r\n");
        fclose(f);
    }else printf("failed to open file : w");

    FILE *fo = fopen(sd_file_path, "r");
    if (fo) {
        for (int i = 0; i < MAX_DATA_SIZE; ++i) {
            uint8_t data = fgetc(fo);
            if (data != dataWritten[i]) {
                printf("read data false: %d:: %d\r\n", dataWritten[i], data);
            }
            printf("read data : %d:: %d\r\n", dataWritten[i], data);
        }
        fclose(fo);
    } else {
        printf("failed to open file : r");
    }
}



//
////FSFAT_FENTRYLOG("%s:entered\n", __func__);
////{
////FSFAT_DBGLOG("%s:SD: Writing ... ", __func__);
//    {
//        FILE *f = fopen(sd_file_path, "w+");
//        Thread::wait(1000);
//        printf("open file:: %s\r\n", sd_file_path);
//        /*if (f)*/ {
//            printf("ff\r\n");
//            for (int i = 0; i < MAX_DATA_SIZE; i++) {
//                data_written[i] = rand() % 0XFF;
//                fprintf(f, "%c", data_written[i]);
//            }
//            write_result = true;
//            fclose(f);
//        } /*else printf("open failed w\r\n");*/
//    }
//FSFAT_DBGLOG("[%s]\n", write_result ? "OK" : "FAIL");
//}
//TEST_ASSERT_MESSAGE(write_result == true, "Error: write_result is set to false.");
//
//// Read back the data from the file and store them in data_read
//    {
////FSFAT_DBGLOG("%s:SD: Reading data ... ", __func__);
//        {
//            FILE *f = fopen(sd_file_path, "r");
//            /*if (f)*/ {
//                read_result = true;
//                for (int i = 0; i < MAX_DATA_SIZE; i++) {
//                    uint8_t data = fgetc(f);
//                    if (data != data_written[i]) {
//                        read_result = false;
//                printf("read data false: %d:: %d\r\n", dataWritten[i], data);
//                        break;
//                    }
//                }
//                fclose(f);
//            } /*else */printf("open failed r\r\n");
////        FSFAT_DBGLOG("[%s]\n", read_result ? "OK" : "FAIL");
//        }
//
//
//    }
//
//
//printf("from issue\r\n");
//
////SDBlockDevice sd(MOSI, MISO, SCLK, PJ1);
//   FATFileSystem fs("sd", &sd);
////    int main() {
//	FATFS FatFs;
//	FIL pFilePtr;
//        f_mount(&FatFs, " ",1);
//	f_open(&pFilePtr, "0:/URC/CONFIG/WebResponse1.txt", FA_WRITE  FA_OPEN_ALWAYS);
//        f_write(&pFilePtr, "test2", 5, &uiValue);
//	f_close(&pFilePtr);
////   }


#endif
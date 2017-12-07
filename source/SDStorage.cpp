//
// Created by nirao on 21.11.17.
//

#include "SDStorage.h"

#if NDEBUG
#define PRINTF(...)
#else
#define PRINTF printf
#endif

SDStorage::SDStorage(PinName mosi, PinName miso, PinName sclk, PinName cs):
                    sd(mosi, miso, sclk, cs),
                    fs(MOUNT_ADDRESS)
{

}

int SDStorage::init() {
    return mount(&sd);
}

int SDStorage::format() {
    return FATFileSystem::format(&sd);
}

int SDStorage::mount(SDBlockDevice *sd) {
    int error = fs.mount(sd);

    if (error) {
        PRINTF("FileSystem mounting failed, format the SD card\r\n");
        error = format();
        if (error) {
            PRINTF("Failed to format\r\n");
        } else {
            PRINTF("SD card formatted, retyr mount\r\n");
            error = fs.mount(sd);
            if (error) {
                PRINTF("Remount failed \r\n");
            }
        }
    } else
        PRINTF("Mounted\r\n");
    return error;
}

size_t SDStorage::read(const char *filePath, void *dataBuf, size_t size, size_t count) {
    lock();
    FILE *f = fopen(filePath, SD_READ);
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    size_t ret = 0;
    ret = fread(dataBuf, size, count, f);
    fclose(f);
    unlock();
    return ret;
}

size_t SDStorage::write(const char *filePath, void *data, size_t size, size_t count) {
    lock();
    FILE *f = fopen(filePath, SD_WRITE);
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    size_t ret = fwrite(data, size, count, f);
    fclose(f);
    unlock();
    return ret;
}

size_t SDStorage::update(const char *filePath, void *data, size_t size, size_t count) {
    lock();
    FILE *f = fopen(filePath, SD_APPEND);
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    size_t ret = fwrite(data, size, count, f);
    fclose(f);
    unlock();
    return ret;
}

int SDStorage::seek(const char *filePath, long offSet, int whence) {
    lock();
    FILE *f = fopen(filePath, "r+");
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    int ret = fseek(f, offSet, whence);
    unlock();
    return ret;
}

uint16_t SDStorage::getFileSize(const char *filePath) {
    lock();
    FILE *f = fopen(filePath, "r+");
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    fseek(f, 0, SEEK_END); // seek to end of file
    long fsize = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET);
    fclose(f);
    unlock();
    return static_cast<uint16_t>(fsize);
}

dirent * SDStorage::ls(const char *dirPath) {
    lock();
    DIR* dir = opendir(dirPath);
    struct dirent* de;
    while((de = readdir(dir)) != NULL){
        PRINTF("  %s\r\n", &(de->d_name)[0]);
    }
    closedir(dir);
    unlock();
    return de;
}

int SDStorage::copyFile(const char *sourceFilePath, const char *destinationFilePath) {
    lock();
    FILE *srcF, *destF;

    srcF = fopen(sourceFilePath, SD_READ);
    if (srcF == NULL) {
        PRINTF("Failed to open %s\r\n", sourceFilePath);
        return 1;
    }

    destF = fopen(destinationFilePath, SD_WRITE);
    if (destF == NULL) {
        PRINTF("Failed to open %s \r\n", destinationFilePath);
        fclose(srcF);
        return 1;
    }

    int a = 0;
    while (a != EOF) {
        a = fgetc(srcF);
        fputc(a, destF);
    } /*while (a != EOF);*/

    fseek(srcF, 0, SEEK_END); // seek to end of file
    long srcSize = ftell(srcF); // get current file pointer
    fseek(srcF, 0, SEEK_SET);
    fclose(srcF);

    fseek(destF, 0, SEEK_END); // seek to end of file
    long destSize = ftell(destF); // get current file pointer
    fseek(destF, 0, SEEK_SET);
    fclose(destF);

    PRINTF("SRC_SIZE %lu, %lu DEST_SIZE", srcSize, destSize);

//    if (srcSize == destSize){
    remove(sourceFilePath);
//    }

    unlock();
    return 0;
}

int SDStorage::flush(const char *filePath)  {
    lock();
    FILE *f = fopen(filePath, SD_READ);
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    int ret = fflush(f);
    fclose(f);
    unlock();
    return ret;
}

void SDStorage::open(const char *filePath) {
    lock();
    FILE *f = fopen(filePath, SD_WRITE);
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return;
    }
    fclose(f);
    unlock();
}

int SDStorage::isEOF(const char *filePath){
    FILE *f = fopen(filePath, SD_READ);
    if (f == NULL){
        PRINTF("Failed to open %s\r\n", filePath);
        return 0;
    }
    int ret = feof(f);
    fclose(f);
    return ret;
}

void SDStorage::lock() {
    _mutex.lock();
}

void SDStorage::unlock() {
    _mutex.unlock();
}
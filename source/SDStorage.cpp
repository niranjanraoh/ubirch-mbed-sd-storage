//
// Created by nirao on 21.11.17.
//

#include "SDStorage.h"

#if NDEBUG
#define PRINTF (...)
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
//    return 0;
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
    size_t ret = 0;
    ret = fread(dataBuf, size, count, f);
    fclose(f);
    unlock();
    return ret;
}

size_t SDStorage::onlyRead(FILE *f, void *dataBuf, size_t size, size_t count) {
    lock();
    size_t ret = fread(dataBuf, size, count, f);
    unlock();
    return ret;
}

size_t SDStorage::write(const char *filePath, void *data, size_t size, size_t count) {
    lock();
    FILE *f = fopen(filePath, SD_WRITE);
    size_t ret = fwrite(data, size, count, f);
    fclose(f);
    unlock();
    return ret;
}

size_t SDStorage::update(const char *filePath, void *data, size_t size, size_t count) {
    lock();
    FILE *f = fopen(filePath, SD_APPEND);
    size_t ret = fwrite(data, size, count, f);
    fclose(f);
    unlock();
    return ret;
}

int SDStorage::seek(const char *filePath, long offSet, int whence) {
    lock();
    FILE *f = fopen(filePath, "r+");
    int ret = fseek(f, offSet, whence);
    unlock();
    return ret;
}

uint16_t SDStorage::getFileSize(const char *filePath) {
    lock();
    FILE *f = fopen(filePath, SD_READ);
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

size_t SDStorage::uprintf(const char *filePath, const char *data) {
    lock();
    FILE *f = fopen(filePath, SD_APPEND);
    size_t ret = static_cast<size_t>(fprintf(f, data));/*"S1: %f, S2: %f, S3: %f\r\n", data.v1, data.v2, data.v3);*/
    fclose(f);
    unlock();
    return ret;
}

FILE * SDStorage::open(const char *path) {
    FILE *f = fopen(path, SD_WRITE);
    return f;
}

void SDStorage::lock() {
    _mutex->lock();
}

void SDStorage::unlock() {
    _mutex->unlock();
}

//
// Created by nirao on 21.11.17.
//

#ifndef UBIRCH_SD_DRIVER_SDSTORAGE_H
#define UBIRCH_SD_DRIVER_SDSTORAGE_H


#include <SDBlockDevice.h>
#include <FATFileSystem.h>

#define MOUNT_ADDRESS "ub"
#define SD_READ       "r"
#define SD_WRITE      "w"
#define SD_APPEND     "a"


class SDStorage {

public:
//    struct HumidityVal {
//        float v1;
//        float v2;
//        float v3;
//    };

    SDStorage(PinName mosi, PinName miso, PinName sclk, PinName cs);

    int init();

    int format();

    int mount(SDBlockDevice *sd);

    FILE *open(const char *path);

    inline int close(FILE *f){
        fclose(f);
    }

    // return number size if read sucessful
    size_t read(const char *filePath, void *dataBuf, size_t size, size_t count);

    size_t onlyRead(FILE *f, void *dataBuf, size_t size, size_t count);

    /*@return number of elements of size size written (which is equal to count)*/
    size_t write(const char *filePath, void *data, size_t size, size_t count);

    size_t update(const char *filePath, void *data, size_t size, size_t count);

    size_t uprintf(const char *filePath, const char *data);

    int seek(const char *filePath, long offSet, int whence);

    uint16_t getFileSize(const char *filePath);

    dirent *ls(const char *dirPath);

    inline int flush(const char *filePath) {
        FILE *f = fopen(filePath, SD_READ);
        int ret = fflush(f);
        fclose(f);
        return ret;
    }

    inline int rmFile(const char *filePath) {
        return remove(filePath);
    }

    inline int isEOF(const char *filePath) {
        FILE *f = fopen(filePath, SD_READ);
        int ret = feof(f);
        fclose(f);
        return ret;
    }

    inline void doRewind(const char *filePath) {
        FILE *f = fopen(filePath, SD_READ);
        rewind(f);
//        fclose(f);
//        return ret;
    }

    virtual void lock();


    virtual void unlock();

private:

    SDBlockDevice sd;
    FATFileSystem fs;

    static SingletonPtr<PlatformMutex> _mutex;


};
#endif //UBIRCH_SD_DRIVER_SDSTORAGE_H

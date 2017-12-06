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
#define SD_READ_WRITE "r+"

class SDStorage {

public:

    SDStorage(PinName mosi, PinName miso, PinName sclk, PinName cs);

    int init();

    int format();

    int mount(SDBlockDevice *sd);

    void open(const char *filePath);

    inline int close(FILE *f){
        return fclose(f);
    }

    // return number size if read sucessful
    size_t read(const char *filePath, void *dataBuf, size_t size, size_t count);

    /*@return number of elements of size size written (which is equal to count)*/
    size_t write(const char *filePath, void *data, size_t size, size_t count);

    size_t update(const char *filePath, void *data, size_t size, size_t count);

    /*TODO change this to seek and read*/
    int seek(const char *filePath, long offSet, int whence);

    uint16_t getFileSize(const char *filePath);

    dirent *ls(const char *dirPath);

    /*TODO implement copy file -> x-measure.bin, x-send.bin*/
    int copyFile(const char *sourceFilePath, const char * destinationFilePath);

    int flush(const char *filePath);

    inline int rmFile(const char *filePath) {
        return remove(filePath);
    }

    int isEOF(const char *filePath);

    virtual void lock();

    virtual void unlock();

private:

    SDBlockDevice sd;
    FATFileSystem fs;

//    static SingletonPtr<PlatformMutex> _mutex;
    Mutex _mutex;

};
#endif //UBIRCH_SD_DRIVER_SDSTORAGE_H

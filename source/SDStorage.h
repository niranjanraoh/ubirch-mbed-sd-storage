/*!
 * @file
 * @brief Interface to access SDStorage driver.
 *
 * @author Niranjan Rao
 * @date   2017-12-13
 *
 * @copyright &copy; 2017 ubirch GmbH (https://ubirch.com)
 *
 * ```
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ```
 */

#ifndef UBIRCH_SD_DRIVER_SDSTORAGE_H
#define UBIRCH_SD_DRIVER_SDSTORAGE_H


#include <SDBlockDevice.h>
#include <FATFileSystem.h>

#define DEFAULT_MOUNT_ADDRESS "ub"
#define SD_READ       "r"
#define SD_WRITE      "w"
#define SD_APPEND     "a"
#define SD_READ_WRITE "r+"


/**
 * @class SDStorage
 *
 *
 * This class creates an interface to access SDBlockDevice and FILE functions
 */
class SDStorage {

public:

    /**
     * Initialize the SDBlockDevice and FILE objects
     */
    SDStorage(PinName mosi, PinName miso, PinName sclk, PinName cs, const char *mountAddress = NULL);

    /**
     * Mount a file system to the SD card
     * @return 0 on sucess
     */
    int init();

    /**
     * Format logical drive, FDISK partitioning rule.
     * @return 0 on sucess
     */
    int format();

    /**
     * Mount a file system to the SD card
     * @param sd SDBlockDevice to mount to
     * @return 0 on sucess, negative error code on failure
     */
    int mount(SDBlockDevice *sd);

    /**
     * Read array of elements from a file on the SD card
     * @param filePath path of the file to perform R/W operations on
     * @param dataBuf buffer to read the data into from the file
     * @param size size in bytes of each elements to be read
     * @param count number of size sized elements to read
     * @return number of elements read
     */
    size_t read(const char *filePath, void *dataBuf, size_t size, size_t count);

    /**
     * Write array of elements into a file on SD card
     * @param filePath path of the file to perform R/W operations on
     * @param dataBuf pointer to the array of elements to be written
     * @param size size in bytes of each elements to be read
     * @param count number of size sized elements to read
     * @return number of elements read
     */
    size_t write(const char *filePath, void *data, size_t size, size_t count);

    /**
     * Append array of elements to an existing file on SD card
     * @param filePath path of the file to perform R/W operations on
     * @param dataBuf pointer to the array of elements to be appended
     * @param size size in bytes of each elements to be read
     * @param count number of size sized elements to read
     * @return number of elements read
     */
    size_t update(const char *filePath, void *data, size_t size, size_t count);

    /*TODO change this to seek and read
      int seek(const char *filePath, long offSet, int whence);
     */

    /**
     * Return the size of the fiel
     * @param filePath path of the file to perform R/W operations on
     * @return size of the file
     */
    uint16_t getFileSize(const char *filePath);

    /**
     * List all the files and directories in the given directory
     * struct dirent {
     *      char d_name[NAME_MAX+1]; ///< Name of file
     *      uint8_t d_type;          ///< Type of file
     *      };
     * @return dirent
     */
    dirent *ls(const char *dirPath);

    /**
     * Copy elements of one file to another
     * @param sourceFilePath file path of the sourch file
     * @param destinationFilePath file path of the destination file
     * @return 0 on sucess and 1 on failure
     */
    int copyFile(const char *sourceFilePath, const char *destinationFilePath);

    /**
     * Copy elements of one file to another
     * @param sourceFilePath file path of the sourch file
     * @param destinationFilePath file path of the destination file
     * @return 0 on sucess and 1 on failure
     */
    int renameFile(const char *sourceFilePath, const char *destinationFilePath);

    /**
     * Delete all the elements in the file
     * @param filePath path of the file to perform R/W operations on
     */
    int flush(const char *filePath);

    /**
     * Remove file
     * @param filePath path of the file to perform R/W operations on
     */
    int rmFile(const char *filePath);

    /*int isEOF(const char *filePath);*/

    /**
     * Lock mutex
     */
    virtual void lock();

    /**
     * Unlock mutex
     */
    virtual void unlock();

private:

    SDBlockDevice sd;
    FATFileSystem fs;

    /*static SingletonPtr<PlatformMutex> _mutex;*/
    Mutex _mutex;
};
#endif //UBIRCH_SD_DRIVER_SDSTORAGE_H

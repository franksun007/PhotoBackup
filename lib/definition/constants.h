

#ifndef _PHOTOBACKUP_DEFINITION_CONSTANTS_H_
#define _PHOTOBACKUP_DEFINITION_CONSTANTS_H_

#include <string>

namespace photobackup {

// Default destination folder
constexpr char DEFAULT_DESTINATION[] = "/Volumes/PHOTO_BACKUP_128G/";
constexpr char MISC_FOLDER[] = "MISC";      // MISC folder

#ifdef __linux__
// Command line to populate boost property tree
constexpr char CMD_IO_DISK_INFO_ALL[] = "lsblk -O -J";
#endif

enum class PhotoType {
  JPEG = 1,
  CR2,
  ALL
};

}

#endif // _PHOTOBACKUP_DEFINITION_CONSTANTS_H_

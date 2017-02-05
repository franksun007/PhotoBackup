//
// Created by Chenfan Sun on 2/5/17.
//

#ifndef PHOTOBACKUP_COMMANDLINEESSENTIAL_H
#define PHOTOBACKUP_COMMANDLINEESSENTIAL_H

#include <string>

using namespace std;

class CommandLineEssential {

public:
    CommandLineEssential(int argc, char ** argv);
    static void argsUsage();

    string getDeviceName() { return device_name; };
    string getDestinationFolder() { return destination_folder; };
    int getFileType() { return file_type; };
    int getCopyRecent() { return copy_recent; };
    int getMisc() { return misc; };
    int getVerbose() { return verbose; };

private:
    string device_name;         // SD/CF card of the camera
    string destination_folder;  // Destination folder to backup the image
    int file_type;              // Whether want to copy CR2 or JPG or everything
    int copy_recent;            // Whether copy from the most recent or copy everything
    int misc;               // Whether skipping the misc folder or not
    int verbose;

};

#endif //PHOTOBACKUP_COMMANDLINEESSENTIAL_H

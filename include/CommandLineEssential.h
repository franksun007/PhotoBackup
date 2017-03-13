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

    string getDeviceName() const { return device_name; };
    string getDestinationFolder() const { return destination_folder; };
    int getFileType() const { return file_type; };
    int getCopyRecent() const { return copy_recent; };
    int getMisc() const { return misc; };
    int getVerbose() const { return verbose; };

private:
    string device_name;         // SD/CF card of the camera
    string destination_folder;  // Destination folder to backup the image
    int file_type;              // Whether want to copy CR2 or JPG or everything
    int copy_recent;            // Whether copy from the most recent or copy everything
    int misc;               // Whether skipping the misc folder or not
    string device_id;       // The serial number id of SD/CF card of the camera
    int verbose;

    // The function will help to get the serial id of the source
    // TODO: Finish the test framework and then think about the implementation
    string getSerialIDOfSource();

};

#endif //PHOTOBACKUP_COMMANDLINEESSENTIAL_H

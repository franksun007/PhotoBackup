//
// Created by Chenfan Sun on 2/5/17.
//


#include <boost/program_options.hpp>
#include <iostream>

#include "../include/CommandLineEssential.h"
#include "../include/Definition.h"
#include "../include/Shared.h"

using namespace std;
namespace po = boost::program_options;  // For boost command line options

const string USB_SYSTEM_PROFILER = "system_profiler -xml -detailLevel mini SPUSBDataType";

CommandLineEssential::CommandLineEssential(int argc, char **argv) {
    // Section to process command line arguments
    try {
        po::options_description desc("Allowed options");

        desc.add_options()
                ("help", "produce help message")
                ("source", po::value<string>(), "the source folder to copy from")
                ("type", po::value<int>(), "the file type you want to copy")
                ("copy-recent", po::value<int>(),
                 "if you only want to copy the recent files, the new files after the last backup")
                ("misc", po::value<int>(), "to enable copy misc folder")
                ("dest", po::value<string>(), "the destination folder to copy to")
                ("verbose", po::value<int>(), "for debug");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (argc == 1 || vm.count("help")) {
            cout << desc << "\n";
            argsUsage();
            exit(1);
        }

        cout << "<============= SET UP ARGS ============>" << endl;


        if (vm.count("source")) {
            device_name = vm["source"].as<string>();
            cout << "The device to copy from is:\t\t" << vm["source"].as<string>() << endl;
        } else {
            cout << "The device to copy from is undefined." << endl;
            cout << "Please specify the name of the device." << endl;
            cout << "eg. --source EOS_DIGITAL" << endl;
            exit(1);
        }

        if (vm.count("type")) {
            file_type = vm["type"].as<int>();
            cout << "The file type you want to copy is:\t";
            if (file_type == JPEG)
                cout << "JPEG" << endl;
            else if (file_type == CR2)
                cout << "CR2" << endl;
            else if (file_type == ALL)
                cout << "Everything, JPEG and CR2 and other stuff." << endl;
            else {
                cout << "unspecified filetype" << endl;
                cout << "Please specify a valid filetype" << endl;
                cout << "eg. 1 for JPEG, 2 for CR2, 3 for all" << endl;
                exit(1);
            }
        } else {
            cout << "The filetype to copy is undefined." << endl;
            cout << "Please specify a valid filetype to copy." << endl;
            cout << "eg. 1 for JPEG, 2 for CR2, 3 for all" << endl;
            exit(1);
        }

        if (vm.count("copy-recent")) {
            copy_recent = vm["copy-recent"].as<int>();
            cout << "You want to copy files:\t\t\t";
            if (copy_recent == 1) {
                cout << "since the last backup." << endl;
            } else if (copy_recent == 0) {
                cout << "since the beginning of the time." << endl;
            } else {
                cout << "undefined copy-recent parameter" << endl;
                cout << "Please specify a valid copy-recent parameter." << endl;
                cout << "eg. 1 = \"since the last backup\", 0 = \"the beginning of the time\"" << endl;
                exit(1);
            }
        } else {
            cout << "The copy-recent parameter is undefined" << endl;
            cout << "Please specify a valid copy-recent parameter." << endl;
            cout << "eg. 1 = \"since the last backup\", 0 = \"the beginning of the time\"" << endl;
            exit(1);
        }

        if (vm.count("misc")) {
            misc = vm["misc"].as<int>();
            if (misc == 1) {
                cout << "Set to copy " << MISC_FOLDER << endl;
            } else if (misc == 0) {
                cout << "Set to skip " << MISC_FOLDER << endl;
            } else {
                cout << "Undefined parameter --misc" << endl;
                cout << "Valid options are 1 or 0." << endl;
                cout << "Default to 0" << endl;
            }


        } else {
            // do nothing
        }

        if (vm.count("dest")) {
            destination_folder = vm["dest"].as<string>();
            cout << "Destination Folder is: " << destination_folder << endl;
        } else {
            cout << "Destination Folder unspecified." << endl;
            destination_folder = DEST;
            cout << "Destination Folder defaults to: " << DEST << endl;
        }

        if (vm.count("verbose")) {
            verbose = vm["verbose"].as<int>();
            cout << "Verbose mode is on. " << endl;
        } else {
            verbose = 0;
        }

        cout << "<=========== SET UP ARGS DONE =========>" << endl;


    } catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
    } catch(...) {
        cerr << "Exception of unknown type!\n";
    }
}

string CommandLineEssential::getSerialIDOfSource() {
    string usbinfo = exec(USB_SYSTEM_PROFILER.c_str());
    cout << usbinfo << endl;
    return usbinfo;
}


void CommandLineEssential::argsUsage() {

    cout << "This program is aiming to copy images from one folder to another." << endl;
    cout << "It is not designed to copy images, but not limit to." << endl;
    cout << "The target file copied will be rename as <Creation Date>_<Original Filename>" << endl;
    cout << "Eg. example.jpg  ->>   2017_03_21_example.jpg." << endl;

    cout << "Mandatory Parameters: " << endl;
    cout << "--source " << endl;
    cout << "\tThe name of the device. eg. EOS_DIGITAL" << endl;
    cout << "--type" << endl;
    cout << "\tFiltype to copy" << endl;
    cout << "\t\t 1: only copy JPEG" << endl;
    cout << "\t\t 2: only copy CR2" << endl;
    cout << "\t\t 3: copy both JPEG and CR2" << endl;
    cout << "--copy-recent" << endl;
    cout << "\tThe parameter that specify which files you want to copy from." << endl;
    cout << "\t\t 1: copy files since the last backup" << endl;
    cout << "\t\t 2: copy files since the beginning of the time (all files in the device)" << endl;
    cout << endl;
    cout << "Optional Parameters: " << endl;
    cout << "--misc" << endl;
    cout << "\tChoose whether MISC folder should be copied or not, usually contains movies" << endl;
    cout << "\t\t Default value: 0" << endl;
    cout << "\t\t 0: not copy MISC folder" << endl;
    cout << "\t\t 1: copy MISC folder" << endl;
    cout << "--dest" << endl;
    cout << "\tChoose what destination folder to backup the images" << endl;
    cout << "\t\t Default value: " << DEST << endl;
    cout << "\t\t Pass in an absolute path to specify." << endl;
    cout << endl;
}

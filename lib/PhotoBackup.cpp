#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
// #include <boost/program_options.hpp>

#include "CommandLineEssential.h"
#include "PhotoHolder.h"
#include "Definition.h"
#include "utils/utils.h"
#include "PhotoBackup.h"

using namespace std;                    // For eveything
namespace fs = boost::filesystem;       // For boost fs
namespace algo = boost::algorithm;      // For boost string algo
// namespace po = boost::program_options;  // For boost command line options

namespace pholder = PhotoHolder;

const string LS_COMMAND_WITH_DATE = "gls -1ogh --time-style=long-iso "; // ls command
const string POSTPROCESS_COMMAND_SRC = "awk '{print $4\"_\"$6}' | tr - '_'";  // process command
const string LS_COMMAND_NORMAL = "gls -1h ";    // normal ls command


// Verbose for debug
int verbose = 0;

int main(int argc, char ** argv) {

    CommandLineEssential cmd_args (argc, argv);
    verbose = cmd_args.getVerbose();

    cout << "<=========== VALIDATING FOLDERS =========>" << endl;

    if (verbose) {
        cout << "device_name:\t" << cmd_args.getDeviceName() << endl;
        cout << "file_type:\t" << cmd_args.getFileType() << endl;
        cout << "copy_recent:\t" << cmd_args.getCopyRecent() << endl;
    }

    // To construct and validate source directory
    string src_folder("/Volumes/" + cmd_args.getDeviceName() + "/DCIM/");
    fs::path src_dir = validateDirectory(src_folder);

    if (verbose) {
        cout << "src_dir: " << src_dir << endl;
    }

    // To construct/get and validate all the subfolders of the source directory
    vector<fs::path> src_folder_subfolders = getAllSubFolders(src_dir);
    processMisc(src_folder_subfolders, cmd_args.getMisc());

    if (verbose) {
        cout << "src_folder_subfolders: " << endl;
//        for (auto src_folder_subfolder : src_folder_subfolders) {
//            cout << "\t" << src_folder_subfolder << endl;
//        }
    }

    // To construct and validate the destination directory
    fs::path dest_dir = validateDirectory(cmd_args.getDestinationFolder());

    if (verbose) {
        cout << "dest_dir: " << dest_dir << endl;
    }

    // To process the most recent file that's been backed in the destination directory

    pholder::PhotoHolder pholder;
    if (verbose) {
        cout << "Empty pholder constructed" << endl;
    }
    if (cmd_args.getCopyRecent()) {
        processCopyRecent(pholder, dest_dir, cmd_args.getCopyRecent());
        cout << "The last file you copied since last backup with prefix: " << pholder.getPhotoFilename() << endl;
    }

    cout << "<=========== VALIDATING FOLDERS DONE =========>" << endl;

    cout << endl;

    cout << "<========== GENERATING COPY LIST =========>" << endl;


    int total_files = 0;
    // Data structure that will map subfolder to a map that :
    //      maps the original image name to the target image name with
    //      file date as prefix
    map<fs::path, map<string, string>> folder_to_image_list;
    // Get the list of the photos that we gonna copy
    for (auto src_folder_subfolder : src_folder_subfolders) {
        map<string, string> result;
        if (verbose) {
            cout << "Generating copy list ... " << endl;
        }
        // Generate the list of photos we want to copy in a specific subdirectory
        generateCopyList(src_folder_subfolder, cmd_args.getFileType(), cmd_args.getCopyRecent(), pholder, result);
        folder_to_image_list[src_folder_subfolder] = result;

        if (verbose) {
            cout << "Folder: " << src_folder_subfolder << endl;
            cout << "Num Files: " << result.size() << endl;
        }

        // Increment the number of files we want to process
        total_files += result.size();
    }

    cout << "<========== COPY LIST GENERATED =========>" << endl;

    cout << endl;

    cout << "<===============  COPYING  ==============>" << endl;

    // Process the copying
    copyPhotos(folder_to_image_list, dest_dir, total_files);

    cout << "<===============    DONE   ==============>" << endl;

    return 0;
}

// Compare two pathes.
bool compareFilenames(fs::path i, fs::path j) { return (i.string() < j.string()); }

void processCopyRecent(pholder::PhotoHolder &pholder, const fs::path &dest_dir, int file_type) {

    string extension = ".";
    if (file_type == JPEG) {
        extension = ".JPG";
    } else if (file_type == CR2) {
        extension = ".CR2";
    }

    // Iterate through the folder and add all the files matching the extension a vector
    vector<fs::path> files;
    fs::directory_iterator end_itr; // default construction yields past-the-end
    for ( fs::directory_iterator itr( dest_dir );
          itr != end_itr;
          ++itr ) {
        if ( !fs::is_directory(itr->status()) ) {
            if ((itr->path().filename()).string().find(extension) != string::npos) {
                files.push_back(itr->path().filename());
            }
        }
    }

    // Sort the vector by name
    sort(files.begin(), files.end(), compareFilenames);

    if (verbose) {
        cout << "Last File: " << files[files.size() - 1].string() << endl;
    }

    // Grab the last element in the vector
    string component = files[files.size() - 1].stem().string();

    if (verbose) {
        cout << "After stem: " << component << endl;
    }

    pholder.setArgumentsLastFile(component);


    if (verbose) {
        cout << "pholder construction succeed: " << endl;
        cout << "\tSTATIC: " << pholder.getDeviceID() << endl;
        cout << "\tPUBLIC: " << endl;
        cout << "\tyear: \t" << pholder.getYear() << endl;
        cout << "\tmonth:\t" << pholder.getMonth() << endl;
        cout << "\tday:  \t" << pholder.getDay() << endl;
        cout << "\td_id: \t" << pholder.getFilenameDeviceID() << endl;
        cout << "\tid:   \t" << pholder.getPhotoID() << endl;
        cout << "\tfilename: " << pholder.getPhotoFilename() << endl;
    }

}

fs::path validateDirectory(const string &dir) {
    // check if a folder exists
    fs::path src_dir(dir);
    if (fs::exists(src_dir) && fs::is_directory(src_dir)) {
        if (verbose) {
            cout << "Directory exists: " << dir << endl;
        }
    } else {
        cout << "The folder does not exist:" << endl;
        cout << "\t" << dir << endl;
        cout << "Please make sure it's a valid folder/directory and try again." << endl;
        exit(1);
    }
    return src_dir;
}


vector<fs::path> getAllSubFolders(const fs::path &dir) {
    if ( !exists( dir ) ) {
        cout << "getAllSubFolders: the target folder does not exists: " << dir << endl;
        exit(1);
    }

    // Iterator that will add all the directory to a vector
    vector<fs::path> result;
    fs::directory_iterator end_itr; // default construction yields past-the-end
    for ( fs::directory_iterator itr( dir );
          itr != end_itr;
          ++itr ) {
        if ( fs::is_directory(itr->status()) ) {
            result.push_back(itr->path());
        }
    }
    return result;
}

void processMisc(vector<fs::path> &src_folder_subfolders, const int misc) {
    if (!misc) {

        // Get rid of the MISC folder
        // Don't know a better way to do this

        vector<int> indicies;
        for (int i = 0 ; i < src_folder_subfolders.size(); i++) {
            if (verbose) {
                cout << "From processMisc: " << endl;
                cout << src_folder_subfolders[i].string() << endl;
            }
            if (src_folder_subfolders[i].string().find(MISC_FOLDER) != string::npos) {
                cout << MISC_FOLDER << " folder is skipped, to enable " << MISC_FOLDER << ", pass --misc 1" << endl;
                indicies.push_back(i);
            }
        }

        for (int i = indicies.size() - 1; i >= 0; i--) {
            src_folder_subfolders.erase(src_folder_subfolders.begin() + indicies[i]);
        }

    } else {
        cout << "Copying " << MISC_FOLDER << ". " << endl;
    }
}

void generateCopyList(const fs::path &src_subfolder,
                                const int file_type, const int copy_recent, const pholder::PhotoHolder &pholder,
                                map<string, string> &result) {

    std::string extension = "*.*";
    if (file_type == JPEG) {
        extension = "*.JPG";
    } else if (file_type == CR2) {
        extension = "*.CR2";
    }

    string result_from_exec;

    // Get a list of the original images.
    vector<string> original_images;
    result_from_exec = cmd_execute(("(cd " + src_subfolder.string() + " && " + LS_COMMAND_NORMAL + extension + ")" ).c_str());

    if (verbose) {
        cout << "Command result: " << endl;
//        cout << result_from_exec << endl;
    }

    // If the subfolder contains nothing
    if (result_from_exec.size() == 0 || result_from_exec.find("total 0") != string::npos) {
        cout << "No files found in directory: " << endl;
        cout << "\t" << src_subfolder << endl;
        cout << "Skipping... " << endl;
        return;
    }
    // Otherwise split by lines.
    boost::split(original_images, result_from_exec, boost::is_any_of("\n"));

    // Get a list of the original images, that the name has been changed to the target filename
    vector<string> modified_images;
    result_from_exec = cmd_execute(("(cd " + src_subfolder.string() + " && " + LS_COMMAND_WITH_DATE +
            extension + ") | " + POSTPROCESS_COMMAND_SRC).c_str());
    boost::split(modified_images, result_from_exec, boost::is_any_of("\n"));

    assert(original_images.size() == modified_images.size());


    for (int i = 0; i < original_images.size(); i++) {
        auto original_image = original_images[i];
        // modified image only contains year, month, day + image_id
        auto modified_image = modified_images[i];

        if (original_image.size() == 0)
            continue;

        pholder::PhotoHolder img;
        img.setArgumentsModifiedImage(modified_image);

        // If matching the copy_recent condition, add to the map.
        if (copy_recent) {
            if (img > pholder) {

                result[original_image] = img.getPhotoFilename();

                if (verbose) {
                    cout << "Generating CopyList: " << endl;
                    cout << original_image << endl;
                    cout << modified_image << endl;
                }
            }
        } else {    // else we add everything
            result[original_image] = modified_image;
        }
    }
    return;
}


void printProgress(int processed, int total) {
    // Fancy way to print the progress
    float progress = float(1.0 * processed / total);
    int barWidth = 70;

    std::cout << "[";
    int pos = int(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << processed << "\t/" << total << "\t" << int(progress * 100.0) << " %\r";
    std::cout.flush();
}


void copyPhotos(const map<fs::path, map<string, string>> &folder_to_image_list, const fs::path &dest_dir, int total) {

    cout << "Total number of files to copy: " << total << endl;

    int progress = 0;
    // Iterate through the map / key
    for (auto itr = folder_to_image_list.begin();
            itr != folder_to_image_list.end();
            itr++) {

        fs::path src_prefix = itr->first;

        map<string, string> value = itr->second;

        // Iterate through the map / value
        for (auto itr_val = value.begin();
                itr_val != value.end();
                itr_val++) {

            auto original_filename = itr_val->first;
            auto destination_filename = itr_val->second;

            if (verbose) {
                cout << "Copying: " << src_prefix / original_filename << endl;
            }

            fs::copy(src_prefix / original_filename, dest_dir / destination_filename);
            progress++;
            printProgress(progress, total);
        }

    }

    cout << endl;
}

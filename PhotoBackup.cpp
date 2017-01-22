#include <cstdio>
#include <iostream>
#include <unordered_set>
#include <iterator>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>


#define JPEG    1
#define CR2     2
#define ALL     (JPEG | CR2)

#define MISC_FOLDER "MISC"

using namespace std;                // For eveything 
namespace fs = boost::filesystem;   // For boost fs
namespace algo = boost::algorithm;  // For boost string algo
namespace po = boost::program_options;  // boost program_options

#define DEST  "/Volumes/PHOTO_BACKUP_128G/"  // Destination folder
const string LS_COMMAND_WITH_DATE = "gls -1ogh --time-style=long-iso "; // ls command
const string POSTPROCESS_COMMAND_SRC = "awk '{print $4\"_\"$6}' | tr - '_'";  // process command
const string LS_COMMAND_NORMAL = "gls -1h ";

// Custom structure that contains the information about the most recent file backed up
typedef struct {
    int year;
    int month;
    int day;
    int id;
    string filename;
} copy_recent_details;

void usage();  // Print out the usage of the program
// Execute command line
std::string exec(const char* cmd);
// Copy the photo from src_dir to dest_dir
void copyPhotos(const map<fs::path, map<string, string>> &folder_to_image_list, const fs::path &dest_dir, int total);
// Get all the subfolders of the pass in folder
vector<fs::path> getAllSubFolders(const fs::path &dir);
// Validate if the directory is a directory (exit program if not)
fs::path validateDirectory(const string &dir);
// Choose whether to skip the MISC folder or not
void processMisc(vector<fs::path> &src_folder_subfolders, const int misc);
// Find out the most recent file backed up
void processCopyRecent(copy_recent_details &crd, const fs::path &dest_dir, int file_type);
// Skip all the photos that are already in the pool, just copy that ones that matters
void generateCopyList(const fs::path &src_subfolder,
                      const int file_type, const int copy_recent, const copy_recent_details &crd,
                      map<string, string> &result);

// Verbose for debug
int verbose = 0;

int main(int argc, char ** argv) {

    string device_name;         // SD/CF card of the camera
    string destination_folder;  // Destination folder to backup the image
    int file_type;              // Whether want to copy CR2 or JPG or everything
    int copy_recent;            // Whether copy from the most recent or copy everything
    int misc = 0;               // Whether skipping the misc folder or not

    cout << "<============= SET UP REPORT ============>" << endl;

    try {
        po::options_description desc("Allowed options");

        desc.add_options()
                ("help", "produce help message")
                ("source", po::value<string>(), "the source folder to copy from")
                ("type",   po::value<int>(), "the file type you want to copy")
                ("copy-recent", po::value<int>(), "if you only want to copy the recent files, the new files after the last backup")
                ("misc", po::value<int>(), "to enable copy misc folder")
                ("dest", po::value<string>(), "the destination folder to copy to")
                ("verbose", po::value<int>(), "for debug")
                ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (argc == 1 || vm.count("help")) {
            cout << desc << "\n";
            usage();
            return 1;
        }

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
                cout << "CR2"  << endl;
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
        }
    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }



    if (verbose) {
        cout << "device_name:\t" << device_name << endl;
        cout << "file_type:\t" << file_type << endl;
        cout << "copy_recent:\t" << copy_recent << endl;
    }

    string src_folder("/Volumes/" + device_name + "/DCIM/");
    fs::path src_dir = validateDirectory(src_folder);
    if (verbose) {
        cout << "src_dir: " << src_dir << endl;
    }
    vector<fs::path> src_folder_subfolders = getAllSubFolders(src_dir);
    processMisc(src_folder_subfolders, misc);
    if (verbose) {
        cout << "src_folder_subfolders: " << endl;
        for (auto src_folder_subfolder : src_folder_subfolders) {
            cout << "\t" << src_folder_subfolder << endl;
        }
    }

    fs::path dest_dir = validateDirectory(destination_folder);
    if (verbose) {
        cout << "dest_dir: " << dest_dir << endl;
    }

    copy_recent_details crd = {0, 0, 0, 0};
    if (copy_recent) {
        processCopyRecent(crd, dest_dir, file_type);
    }
    cout << "The last file you copied since last backup with prefix: " << crd.filename << endl;

    cout << "<=========== SET UP REPORT DONE =========>" << endl;

    cout << endl;

    cout << "<========== GENERATING COPY LIST =========>" << endl;

    int total_files;
    map<fs::path, map<string, string>> folder_to_image_list;
    // Get the list of the photos that we gonna copy
    for (auto src_folder_subfolder : src_folder_subfolders) {
        map<string, string> result;
        generateCopyList(src_folder_subfolder, file_type, copy_recent, crd, result);
        folder_to_image_list[src_folder_subfolder] = result;

        if (verbose) {
            cout << "Folder: " << src_folder_subfolder << endl;
            cout << "Num Files: " << result.size() << endl;
        }

        total_files += result.size();
    }

    cout << "<========== COPY LIST GENERATED =========>" << endl;

    cout << endl;

    cout << "<===============  COPYING  ==============>" << endl;

    copyPhotos(folder_to_image_list, dest_dir, total_files);

    cout << "<===============    DONE   ==============>" << endl;

    return 0;
}

// Compare two pathes.
bool compareFilenames(fs::path i, fs::path j) { return (i.string() < j.string()); }

void processCopyRecent(copy_recent_details &crd, const fs::path &dest_dir, int file_type) {

    string extension = ".";
    if (file_type == 1) {
        extension = ".JPG";
    } else if (file_type == 2) {
        extension = ".CR2";
    }

    vector<fs::path> files;
    fs::directory_iterator end_itr; // default construction yields past-the-end
    for ( fs::directory_iterator itr( dest_dir );
          itr != end_itr;
          ++itr ) {
        if ( !fs::is_directory(itr->status()) ) {
            if ((itr->path().filename()).string().find(extension) != string::npos)
                files.push_back(itr->path().filename());
        }
    }

    sort(files.begin(), files.end(), compareFilenames);

    if (verbose) {
        cout << "Last File: " << files[files.size() - 1].string() << endl;
    }

    string component = files[files.size() - 1].stem().string();

    if (verbose) {
        cout << "After stem: " << component << endl;
    }

    vector<string> components;
    boost::split(components, component, boost::is_any_of("_"));
    assert(components.size() == 5);

    try {
        crd.year = atoi(components[0].c_str());
        crd.month = atoi(components[1].c_str());
        crd.day = atoi(components[2].c_str());
        crd.id = atoi(components[4].c_str());
        crd.filename = component;
    } catch (exception& e) {
        cerr << "error: " << e.what() << "\n";
        cerr << "When Constructing Copy Recent Details. " << endl;
        exit(1);
    } catch(...) {
        cerr << "Exception of unknown type!\n";
        exit(1);
    }

    if (verbose) {
        cout << "CRD construction succeed: " << endl;
        cout << "\tyear: \t" << crd.year << endl;
        cout << "\tmonth:\t" << crd.month << endl;
        cout << "\tday:  \t" << crd.day << endl;
        cout << "\tid:   \t" << crd.id << endl;
        cout << "\tfilename: " << crd.filename << endl;
    }

}

fs::path validateDirectory(const string &dir) {
    // check if source folder exists
    fs::path src_dir(dir);
    if (fs::exists(src_dir) && fs::is_directory(src_dir)) {
        if (verbose)
            cout << "Directory exists: " << dir << endl;
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


void usage() {
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

void processMisc(vector<fs::path> &src_folder_subfolders, const int misc) {
    if (!misc) {
        int index = -1;
        for (int i = 0 ; i < src_folder_subfolders.size(); i++) {
            if (src_folder_subfolders[i].string().find(MISC_FOLDER) != string::npos) {
                cout << MISC_FOLDER << " folder is skipped, to enable " << MISC_FOLDER << ", pass --misc 1" << endl;
                index = i;
            }
        }
        if (index != -1) {
            src_folder_subfolders.erase(src_folder_subfolders.begin() + index);
        }
    } else {
        cout << "Copying " << MISC_FOLDER << ". " << endl;
    }
}

// Exec a command line command and return the value to the parent function
std::string exec(const char* cmd) {
    if (verbose) {
        cout << "Executing command: " << endl;
        cout << "\t" << cmd << endl;
    }
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

void generateCopyList(const fs::path &src_subfolder,
                                const int file_type, const int copy_recent, const copy_recent_details &crd,
                                map<string, string> &result) {

    std::string extension = "*.*";
    if (file_type == 1) {
        extension = "*.JPG";
    } else if (file_type == 2) {
        extension = "*.CR2";
    }

    string result_from_exec;

    vector<string> original_images;
    result_from_exec = exec(("(cd " + src_subfolder.string() + " && " + LS_COMMAND_NORMAL + extension + ")" ).c_str());

    if (verbose) {
        cout << "Command result: " << endl;
        cout << result_from_exec << endl;
    }

    if (result_from_exec.size() == 0 || result_from_exec.find("total 0") != string::npos) {
        cout << "No files found in directory: " << endl;
        cout << "\t" << src_subfolder << endl;
        cout << "Skipping... " << endl;
        return;
    }
    boost::split(original_images, result_from_exec, boost::is_any_of("\n"));

    vector<string> modified_images;
    result_from_exec = exec(("(cd " + src_subfolder.string() + " && " + LS_COMMAND_WITH_DATE +
            extension + ") | " + POSTPROCESS_COMMAND_SRC).c_str());
    boost::split(modified_images, result_from_exec, boost::is_any_of("\n"));

    assert(original_images.size() == modified_images.size());


    for (int i = 0; i < original_images.size(); i++) {
        auto original_image = original_images[i];
        auto modified_image = modified_images[i];

        if (copy_recent) {
            if ((modified_image > crd.filename) && modified_image.find(crd.filename) == string::npos) {
                result[original_image] = modified_image;
                if (verbose) {
                    cout << original_image << endl;
                    cout << modified_image << endl;
                }
            }
        }
    }
    return;
}

void printProgress(int processed, int total) {
    float progress = float(1.0 * processed / total);
    int barWidth = 70;

    std::cout << "[";
    int pos = int(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << "\t" << processed << "\t/\t" << total << " %\r";
    std::cout.flush();
}


void copyPhotos(const map<fs::path, map<string, string>> &folder_to_image_list, const fs::path &dest_dir, int total) {


    cout << "Total number of files to copy: " << total << endl;

    int progress = 0;
    for (auto itr = folder_to_image_list.begin();
            itr != folder_to_image_list.end();
            itr++) {

        fs::path src_prefix = itr->first;

        map<string, string> value = itr->second;

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


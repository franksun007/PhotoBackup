//
// Created by Chenfan Sun on 2/6/17.
//

#ifndef PHOTOBACKUP_PHOTOBACKUP_H
#define PHOTOBACKUP_PHOTOBACKUP_H

namespace fs = boost::filesystem;       // For boost fs
namespace algo = boost::algorithm;      // For boost string algo
// namespace po = boost::program_options;  // For boost command line options

namespace pholders = PhotoHolder;


// Copy the photo from src_dir to dest_dir
void copyPhotos(const map<fs::path, map<string, string>> &folder_to_image_list, const fs::path &dest_dir, int total);
// Get all the subfolders of the pass in folder
vector<fs::path> getAllSubFolders(const fs::path &dir);
// Validate if the directory is a directory (exit program if not)
fs::path validateDirectory(const string &dir);
// Choose whether to skip the MISC folder or not
void processMisc(vector<fs::path> &src_folder_subfolders, const int misc);
// Find out the most recent file backed up
void processCopyRecent(pholders::PhotoHolder &pholder, const fs::path &dest_dir, int file_type);
// Skip all the photos that are already in the pool, just copy that ones that matters
void generateCopyList(const fs::path &src_subfolder,
                      const int file_type, const int copy_recent, const pholders::PhotoHolder &pholder,
                      map<string, string> &result);


#endif //PHOTOBACKUP_PHOTOBACKUP_H

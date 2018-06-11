//
// Created by Chenfan Sun on 2/5/17.
//

#ifndef PHOTOBACKUP_PHOTOHOLDER_H
#define PHOTOBACKUP_PHOTOHOLDER_H

#include <string>
#include <iostream>

using namespace std;

namespace PhotoHolder {

    class PhotoHolder {

    public:

        PhotoHolder() : year(0), month(0), day(0), id(""), filename(""), filename_device_id("") { }
        PhotoHolder(const PhotoHolder &);
        PhotoHolder& operator=(const PhotoHolder &);
        PhotoHolder(string lastFile);

        void setArgumentsLastFile(string lastFile);
        void setArgumentsModifiedImage(string image);

        int getYear() const { return year; };
        int getMonth() const { return month; };
        int getDay() const { return day; };
        string getPhotoID() const { return id; };
        string getPhotoFilename() const { return filename; };
        string getFilenameDeviceID() const { return filename_device_id; };
        string getDeviceID() const { return device_id; };
        string getTruncatedPhotoFilename() const { return truncated_filename; };

        friend bool operator> (const PhotoHolder &lhs, const PhotoHolder &rhs);
        friend bool operator< (const PhotoHolder &lhs, const PhotoHolder &rhs) { return rhs > lhs; }
        friend bool operator<=(const PhotoHolder &lhs, const PhotoHolder &rhs) { return !(lhs > rhs); }
        friend bool operator>=(const PhotoHolder &lhs, const PhotoHolder &rhs) { return !(lhs < rhs); }


    private:
        int year;
        int month;
        int day;
        string id;
        string filename;        // The original filename of the file
        string truncated_filename;
        string filename_device_id;

        static string device_id;
        static string identifyDeviceId();
    };
}

#endif //PHOTOBACKUP_PHOTOHOLDER_H

//
// Created by Chenfan Sun on 2/5/17.
//

#ifndef PHOTOBACKUP_COPYRECENTDETAILS_H
#define PHOTOBACKUP_COPYRECENTDETAILS_H

#include <string>

using namespace std;

class CopyRecentDetails {

public:
    CopyRecentDetails();
    CopyRecentDetails(string lastFile);
    void setArguments(string lastFile);

    int getYear() { return year; };
    int getMonth() { return month; };
    int getDay() { return day; };
    int getPhotoID() { return id; };
    string getPhotoFilename() const { return filename; };

private:
    int year;
    int month;
    int day;
    int id;
    string filename;        // The original filename of the file
};

#endif //PHOTOBACKUP_COPYRECENTDETAILS_H

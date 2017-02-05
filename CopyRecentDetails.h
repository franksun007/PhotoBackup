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

    int getYear() const { return year; };
    int getMonth() const { return month; };
    int getDay() const { return day; };
    int getPhotoID() const { return id; };
    string getPhotoFilename() const { return filename; };

private:
    int year;
    int month;
    int day;
    int id;
    string filename;        // The original filename of the file
};

#endif //PHOTOBACKUP_COPYRECENTDETAILS_H

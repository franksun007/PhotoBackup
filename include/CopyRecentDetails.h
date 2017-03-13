//
// Created by Chenfan Sun on 2/5/17.
//

#ifndef PHOTOBACKUP_COPYRECENTDETAILS_H
#define PHOTOBACKUP_COPYRECENTDETAILS_H

#include <string>
#include <iostream>

using namespace std;

namespace CopyRecentDetails {

    class CopyRecentDetails {

    public:
        CopyRecentDetails() : year(0), month(0), day(0), id(""), filename(""), device_id("") { }
        CopyRecentDetails(const CopyRecentDetails &);
        CopyRecentDetails& operator=(const CopyRecentDetails &);
        CopyRecentDetails(string lastFile);

        void setArguments(string lastFile);
        int getYear() const { return year; };
        int getMonth() const { return month; };
        int getDay() const { return day; };
        string getPhotoID() const { return id; };
        string getPhotoFilename() const { return filename; };
        string getDeviceID() const { return device_id; };

        friend bool operator> (const CopyRecentDetails &rhs, const CopyRecentDetails &lhs);
        friend bool operator< (const CopyRecentDetails &rhs, const CopyRecentDetails &lhs){ return rhs > lhs; }
        friend bool operator<=(const CopyRecentDetails &rhs, const CopyRecentDetails &lhs){ return !(lhs > rhs); }
        friend bool operator>=(const CopyRecentDetails &rhs, const CopyRecentDetails &lhs){ return !(lhs < rhs); }

    private:
        int year;
        int month;
        int day;
        string id;
        string filename;        // The original filename of the file
        string device_id;
    };
}

#endif //PHOTOBACKUP_COPYRECENTDETAILS_H

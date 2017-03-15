//
// Created by Chenfan Sun on 2/5/17.
//

#include "../include/CopyRecentDetails.h"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

using namespace std;

namespace CopyRecentDetails {

    CopyRecentDetails::CopyRecentDetails(const CopyRecentDetails &other) {
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
        this->id = other.id;
        this->filename = other.filename;
        this->device_id = other.device_id;
    }

    CopyRecentDetails &CopyRecentDetails::operator=(const CopyRecentDetails &other) {
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
        this->id = other.id;
        this->filename = other.filename;
        this->device_id = other.device_id;
        return *this;
    }


    CopyRecentDetails::CopyRecentDetails(string lastFile) {
        setArguments(lastFile);
    }

    void CopyRecentDetails::setArguments(string lastFile) {

        assert(lastFile.size() != 0);

        // Decompose the last element
        vector<string> components;
        boost::split(components, lastFile, boost::is_any_of("_"));

        // Pretty unnecessary, but fill in the blanks.
        try {
            this->year = atoi(components[0].c_str());
            this->month = atoi(components[1].c_str());
            this->day = atoi(components[2].c_str());

//            if (strcmp(components[3].c_str(), "IMG") == 0 || components.size() == 4) {
//                id = components[3];
//                for (int i = 4; i < components.size(); i++) {
//                    id = "_" + components[i];
//                }
//                device_id = "";
//            } else {
//                device_id = components[3];
//                id = components[4];
//                for (int i = 5; i < components.size(); i++) {
//                    id = "_" + components[i];
//                }
//            }
            this->filename = lastFile;
        } catch (exception &e) {
            cerr << "error: " << e.what() << "\n";
            cerr << "When Constructing Copy Recent Details. " << endl;
            exit(1);
        } catch (...) {
            cerr << "Exception of unknown type!\n";
            exit(1);
        }
    }

    bool operator>(const CopyRecentDetails &lhs, const CopyRecentDetails &rhs) {
        if (lhs.device_id == rhs.device_id) {
            if (lhs.year - rhs.year > 0) {
                if (lhs.month - rhs.month > 0) {
                    return (lhs.day - rhs.day) > 0;
                }
            }
        }
        return false;
    }
}

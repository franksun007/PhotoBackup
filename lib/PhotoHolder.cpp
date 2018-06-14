//
// Created by Chenfan Sun on 2/5/17.
//

#include "PhotoHolder.h"
#include "utils/utils.h"

#include <vector>
#include <iostream>

#include <boost/algorithm/string.hpp>

#define USB_DEVICE_COMMAND "system_profiler -detailLevel mini SPUSBDataType"
#define CARD_READER_IDENTIFIER "Card Reader"
#define SERIAL_NUMBER_IDENTIFIED "Serial Number"

#define FILENAME_DELINIMATOR "_"

using namespace std;

namespace PhotoHolder {

    PhotoHolder::PhotoHolder(const PhotoHolder &other) {
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
        this->id = other.id;
        this->filename = other.filename;
        this->filename_device_id = other.filename_device_id;
    }

    PhotoHolder &PhotoHolder::operator=(const PhotoHolder &other) {
        this->year = other.year;
        this->month = other.month;
        this->day = other.day;
        this->id = other.id;
        this->filename = other.filename;
        this->filename_device_id = other.filename_device_id;
        return *this;
    }


    PhotoHolder::PhotoHolder(string lastFile) {

    }

    void PhotoHolder::setArgumentsLastFile(string lastFile) {

        assert(lastFile.size() != 0);

        // Decompose the last element
        vector<string> components;
        vector<string> image_id;
        boost::split(components, lastFile, boost::is_any_of("_"));

        // Pretty unnecessary, but fill in the blanks.
        try {
            this->year = atoi(components[0].c_str());
            this->month = atoi(components[1].c_str());
            this->day = atoi(components[2].c_str());
            this->filename_device_id = components[3];

            this->filename = lastFile;

            vector<string> temp;
            boost::split(temp, this->filename, boost::is_any_of("."));
            this->truncated_filename = temp[0];
        } catch (exception &e) {
            cerr << "error: " << e.what() << "\n";
            cerr << "When Constructing Copy Recent Details. " << endl;
            exit(1);
        } catch (...) {
            cerr << "Exception of unknown type!\n";
            exit(1);
        }
    }

    void PhotoHolder::setArgumentsModifiedImage(string image) {

        assert(image.size() != 0);

        // Decompose the last element
        vector<string> components;
        boost::split(components, image, boost::is_any_of("_"));

        // Pretty unnecessary, but fill in the blanks.
        try {
            this->year = atoi(components[0].c_str());
            this->month = atoi(components[1].c_str());
            this->day = atoi(components[2].c_str());
            this->filename_device_id = this->device_id;

            this->filename = components[0] + FILENAME_DELINIMATOR
                             + components[1] + FILENAME_DELINIMATOR
                             + components[2] + FILENAME_DELINIMATOR
                             + this->device_id;

            for (int i = 3; i < components.size(); i++) {
                this->filename += FILENAME_DELINIMATOR + components[i];
            }

            vector<string> temp;
            boost::split(temp, this->filename, boost::is_any_of("."));
            this->truncated_filename = temp[0];
        } catch (exception &e) {
            cerr << "error: " << e.what() << "\n";
            cerr << "When Constructing Copy Recent Details. " << endl;
            exit(1);
        } catch (...) {
            cerr << "Exception of unknown type!\n";
            exit(1);
        }
    }

    bool operator>(const PhotoHolder &lhs, const PhotoHolder &rhs) {
        if (boost::equals(lhs.filename_device_id, rhs.filename_device_id)) {
            return (lhs.truncated_filename > rhs.truncated_filename);
        }
        return false;
    }

    string PhotoHolder::identifyDeviceId() {
        string info = cmd_execute(USB_DEVICE_COMMAND);

        vector<string> splitted;
        boost::split(splitted, info, boost::is_any_of("\n"));

        bool card_reader = false;

        for (string line : splitted) {
            if (line.find(CARD_READER_IDENTIFIER) != string::npos) {
                card_reader = true;
            }

            if (card_reader) {
                if (line.find(SERIAL_NUMBER_IDENTIFIED) != string::npos) {
                    vector<string> temp;
                    boost::split(temp, line, boost::is_any_of(": "));
                    assert(temp[temp.size() - 1].size() != 0);
                    return temp[temp.size() - 1];
                }
            }
        }
        return nullptr;
    }

    // TODO FIXME: this is a bug that should be fixed, will cause SIGFAULT
//    string PhotoHolder::device_id = PhotoHolder::identifyDeviceId();
    string PhotoHolder::device_id = "";
}

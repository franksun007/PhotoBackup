//
// Created by Chenfan Sun on 2/5/17.
//

#include "CopyRecentDetails.h"

#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

using namespace std;

CopyRecentDetails::CopyRecentDetails() {
    year = -1;
    month = -1;
    day = -1;
    id = -1;
    filename = "-1";
}

CopyRecentDetails::CopyRecentDetails(string lastFile) {
    setArguments(lastFile);
}

void CopyRecentDetails::setArguments(string lastFile) {
    // Decompose the last element
    vector<string> components;
    boost::split(components, lastFile, boost::is_any_of("_"));
    assert(components.size() == 5);

    // Pretty unnecessary, but fill in the blanks.
    try {
        year = atoi(components[0].c_str());
        month = atoi(components[1].c_str());
        day = atoi(components[2].c_str());
        id = atoi(components[4].c_str());
        filename = lastFile;
    } catch (exception& e) {
        cerr << "error: " << e.what() << "\n";
        cerr << "When Constructing Copy Recent Details. " << endl;
        exit(1);
    } catch(...) {
        cerr << "Exception of unknown type!\n";
        exit(1);
    }
}

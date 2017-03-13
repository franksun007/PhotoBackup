//
// Created by Chenfan Sun on 2/23/17.
//

#include <iostream>

#include "../include/Shared.h"

#define BUFFER_SIZE 128

// Exec a command line command and return the value to the parent function
std::string exec(const char* cmd) {

    std::cout << "****************************************" << std::endl;
    std::cout << "\tExecuting command: " << std::endl;
    std::cout << "\t\t" << cmd << std::endl;
    std::cout << "****************************************" << std::endl;

    char buffer[BUFFER_SIZE];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, BUFFER_SIZE, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
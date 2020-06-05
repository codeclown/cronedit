#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

// Source:
// https://stackoverflow.com/a/478960/239527

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

std::string exec(std::string cmd) {
    return exec(cmd.c_str());
}

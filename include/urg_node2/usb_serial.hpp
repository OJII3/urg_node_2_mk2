#pragma once
#include <dirent.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>

class USB {
   public:
    USB();

    static auto GetSerialPortFromSearch(const std::string& search_string,
                                        std::string& serial_port) -> bool;
};

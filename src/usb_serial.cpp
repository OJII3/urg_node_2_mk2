#include "urg_node2/usb_serial.hpp"

auto USB::GetSerialPortFromSearch(const std::string &match_string,
                                  std::string &serial_port) -> bool {
    DIR *dir = opendir("/dev/serial/by-id");
    if (dir == nullptr) {
        std::cerr << "Error opening directory: " << strerror(errno)
                  << std::endl;
        return false;
    }

    struct dirent *entry = nullptr;
    while ((entry = readdir(dir)) != nullptr) {
        if (static_cast<std::string>(entry->d_name).find(match_string) !=
            std::string::npos) {
            std::string linkname =
                "/dev/serial/by-id/" + std::string(entry->d_name);
            char target[PATH_MAX];
            int nbytes = readlink(linkname.c_str(), target, PATH_MAX);
            if (nbytes > 0) {
                target[nbytes] = '\0';
                std::string productname = target;
                productname.replace(0, 5, "/dev");
                serial_port = productname;
                closedir(dir);
                return true;
            }
        }
    }

    closedir(dir);
    return false;
}

#include "urg_node2/usb_serial.hpp"

#include <iostream>

USB::USB() {
    libusb_init(&ctx_);
    libusb_set_option(ctx_, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);
    device_count_ = libusb_get_device_list(ctx_, &devices_);
}

USB::~USB() {
    libusb_free_device_list(devices_, 1);
    libusb_exit(ctx_);
}

auto USB::GetDeviceList() -> std::vector<Device> {
    std::vector<Device> devices{};
    for (int i = 0; i < device_count_; ++i) {
        libusb_get_device_descriptor(devices_[i], &desc_);

        auto ret = libusb_open(devices_[i], &handle_);
        if (ret != LIBUSB_SUCCESS) {
            continue;
        }

        Device device{};
        device.device = devices_[i];
        device.device_address = libusb_get_device_address(device.device);
        device.bus_number = libusb_get_bus_number(device.device);
        device.port_number = libusb_get_port_number(device.device);
        device.vendor_id = desc_.idVendor;
        device.product_id = desc_.idProduct;
        device.manufacturer_index = desc_.iManufacturer;
        device.manufacturer.resize(128);
        device.product.resize(128);
        libusb_get_string_descriptor_ascii(
            handle_, desc_.iManufacturer,
            (unsigned char *)device.manufacturer.data(),
            device.manufacturer.size());
        libusb_get_string_descriptor_ascii(
            handle_, desc_.iProduct, (unsigned char *)device.product.data(),
            device.product.size());

        devices.push_back(device);
        libusb_close(handle_);
    }
    return devices;
}

auto USB::GetSerialPortFromSearch(const std::string &match_string,
                                  std::string &serial_port) -> bool {
    DIR *dir = opendir("/dev/serial/by-id");
    if (dir == NULL) {
        std::cerr << "Error opening directory: " << strerror(errno)
                  << std::endl;
        return false;
    }

    struct dirent *entry = nullptr;
    while ((entry = readdir(dir)) != NULL) {
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

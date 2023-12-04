#pragma once
#include <dirent.h>
#include <libusb-1.0/libusb.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

class USB {
   public:
    struct Device {
        libusb_device* device;
        int device_address;
        int bus_number;
        int port_number;
        int vendor_id;
        int product_id;
        int manufacturer_index;
        std::string manufacturer;
        std::string product;
    };

    USB();

    USB(const USB&) = default;
    USB(USB&&) = delete;
    auto operator=(const USB&) -> USB& = default;
    auto operator=(USB&&) -> USB& = delete;
    ~USB();

    auto GetDeviceList() -> std::vector<Device>;
    static auto GetSerialPortFromSearch(const std::string& search_string,
                                        std::string& serial_port) -> bool;

   private:
    libusb_context* ctx_ = nullptr;
    libusb_device** devices_ = nullptr;
    ssize_t device_count_ = 0;
    libusb_device_descriptor desc_{};
    libusb_device_handle* handle_ = nullptr;
};

/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <cutils/log.h>
#include <cutils/properties.h>
#include <hardware_brillo/wifi_driver_hal.h>

#include <string>

namespace {

const char kWcnssWlanEnableFile[] = "/dev/wcnss_wlan";
const char kDevicePathPrefix[] = "/sys/class/net/";
const char kApDeviceName[] = "softap.0";
const char kStationDeviceName[] = "wlan0";
const char kDefaultSerialNumber[] = "00000000";
const char kSerialNumberProperty[] = "ro.boot.serialno";
const int kSerialNumberSize = sizeof(kDefaultSerialNumber) - 1;
const char kApConnectionMode[] = "1";
const char kStationConnectionMode[] = "0";
const char kConnectionModeFile[] = "/sys/module/wlan/parameters/con_mode";

// Writing the 32-bit serial number of the device to this file will cause
// the driver to compute the correct MAC address for the wireless interface.
// TODO(pstew): This device path might be different per reference board.
const char kSerialNumberCtrl[] =
    "/sys/devices/soc.0/a000000.qcom,wcnss-wlan/serial_number";


static bool open_and_close_file(const std::string& filename) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd < 0) {
    ALOGE("Cannot open %s", filename.c_str());
    return false;
  }
  close(fd);
  return true;
}

static bool write_file(
    const std::string& filename, const std::string& content) {
  int fd = open(filename.c_str(), O_WRONLY);
  if (fd < 0) {
    ALOGE("Cannot open %s for writing", filename.c_str());
    return false;
  }
  ssize_t write_count = content.size();
  ssize_t actual_count = write(fd, content.c_str(), write_count);
  close(fd);

  if (actual_count != write_count) {
    ALOGE("Expected to write %d bytes to %s but write returns %d",
          write_count, filename.c_str(), actual_count);
    return false;
  }
  return true;
}

static bool read_file(const std::string& filename, std::string* buffer,
                      size_t buffer_size) {
  int fd = open(filename.c_str(), O_RDONLY);
  if (fd < 0) {
    ALOGE("Cannot open %s for reading", filename.c_str());
    return false;
  }
  char buffer_data[buffer_size];
  ssize_t count = read(fd, buffer_data, buffer_size);
  close(fd);

  if (count <= 0) {
    ALOGE("Cannot read any data from %s", filename.c_str());
    return false;
  }

  buffer->assign(buffer_data, count);
  return true;
}

static bool interface_exists(const char* interface_name) {
  std::string interface_path = std::string(kDevicePathPrefix) + interface_name;
  return access(interface_path.c_str(), F_OK) == 0;
}

static wifi_driver_error wifi_driver_initialize_qcwcn() {
  // Simply opening the file enables it.
  if (!open_and_close_file(kWcnssWlanEnableFile))
    return WIFI_ERROR_UNKNOWN;

  // Although we may not be able to get or set the serial number below,
  // we may be able to have a working device, so do not return an error
  // for failures below.
  std::string serial_number;
  if (!read_file(kSerialNumberCtrl, &serial_number, kSerialNumberSize)) {
    ALOGE("Could not check device serial number");
  } else if (serial_number == kDefaultSerialNumber) {
    char system_serial_number_buf[PROPERTY_VALUE_MAX];
    property_get("ro.serialno", system_serial_number_buf, "");
    std::string system_serial_number(system_serial_number_buf);
    if (system_serial_number.empty()) {
      ALOGE("Could not retrieve device serial number");
    } else if (!write_file(kSerialNumberCtrl, system_serial_number)) {
      ALOGE("Could not set device serial number");
    }
  }

  return WIFI_SUCCESS;
}

static wifi_driver_error wifi_driver_set_mode_qcwcn(
    wifi_driver_mode mode,
    char* wifi_device_name,
    size_t wifi_device_name_size) {
  const char* connection_mode = nullptr;
  const char* device_name = nullptr;
  switch (mode) {
    case WIFI_MODE_AP:
      connection_mode = kApConnectionMode;
      device_name = kApDeviceName;
      break;

    case WIFI_MODE_STATION:
      connection_mode = kStationConnectionMode;
      device_name = kStationDeviceName;
      break;

    default:
      ALOGE("Unkonwn WiFi driver mode %d", mode);
      return WIFI_ERROR_INVALID_ARGS;
  }

  strlcpy(wifi_device_name, device_name, wifi_device_name_size);
  if (interface_exists(device_name))
    return WIFI_SUCCESS;

  // It's been observed that writing to the connection mode file toggles
  // between modes.  Therefore we try a few times.
  for (int retry = 0; retry < 5; ++retry) {
    if (!write_file(kConnectionModeFile, connection_mode))
      return WIFI_ERROR_UNKNOWN;

    // The write above is blocked until the operation completes so there
    // is no need to sleep before checking the result.
    if (interface_exists(device_name))
      return WIFI_SUCCESS;
  }

  return WIFI_ERROR_TIMED_OUT;
}

static int close_wcnss_driver(struct hw_device_t* device) {
  wifi_driver_device_t* dev = reinterpret_cast<wifi_driver_device_t*>(device);
  if (dev)
    free(dev);
  return 0;
}

static int open_wcnss_driver(const struct hw_module_t* module, const char*,
                       struct hw_device_t** device) {
  wifi_driver_device_t* dev = reinterpret_cast<wifi_driver_device_t*>(
      calloc(1, sizeof(wifi_driver_device_t)));

  dev->common.tag = HARDWARE_DEVICE_TAG;
  dev->common.version = WIFI_DRIVER_DEVICE_API_VERSION_0_1;
  // We're forced into this cast by the existing API.  This pattern is
  // common among users of the HAL.
  dev->common.module = const_cast<hw_module_t*>(module);
  dev->common.close = close_wcnss_driver;
  dev->wifi_driver_initialize = wifi_driver_initialize_qcwcn;
  dev->wifi_driver_set_mode = wifi_driver_set_mode_qcwcn;

  *device = &dev->common;

  return 0;
}

static struct hw_module_methods_t wcnss_driver_module_methods = {
  open: open_wcnss_driver
};

}  // namespace {}

hw_module_t HAL_MODULE_INFO_SYM = {
  tag: HARDWARE_MODULE_TAG,
  version_major: 1,
  version_minor: 0,
  id: WIFI_DRIVER_HARDWARE_MODULE_ID,
  name: "WCNSS / PRIMA module",
  author: "Prima",
  methods: &wcnss_driver_module_methods,
  dso: NULL,
  reserved: {0},
};

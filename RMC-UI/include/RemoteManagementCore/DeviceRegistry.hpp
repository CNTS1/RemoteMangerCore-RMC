#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace RMC
{
  enum class DeviceStatus
  {
    Offline,
    Online,
    Unauthorized
  };

  struct DeviceInfo
  {
   std::string deviceID;
   std::string deviceName;
   std::string operatingSystem;
   std::string clientVersion;
   DeviceStatus status = DeviceStatus::Offline;
  };

  enum class RegistryResult
  {
   Success,
   AlreadyExists,
   NotFound,
   InvalidData
  };

  class DeviceRegistry
  {
    public:

    RegistryResult RegistryDevice(const DeviceInfo& device);

    RegistryResult UnregisterDevice(const std::string& deviceID);

    std::optional<DeviceInfo>GetDevice(const std::string& deviceID) const;

    std::vector<DeviceInfo>GetAllDevices() const;

    RegistryResult UpdateDeviceInfo(const DeviceInfo& device);

    RegistryResult UpdateDeviceStatus(const std::string& deviceID, DeviceStatus newStatus);

    bool IsDeviceRegistered(const std::string& deviceID) const;

    private:
    std::unordered_map<std::string,DeviceInfo> devices;
  };
}
#include "../include/RemoteManagementCore/DeviceRegistry.hpp"


namespace RMC
{
    RegistryResult DeviceRegistry::RegistryDevice(const DeviceInfo& device)
    {
     if(device.deviceID.empty() || device.deviceName.empty())
     {
        return RegistryResult::InvalidData;
     }

     if(devices.find(device.deviceID) != devices.end())
     {
        return RegistryResult::AlreadyExists;
     }

     devices.emplace(device.deviceID, device);

     return RegistryResult::Success;
    }


    RegistryResult DeviceRegistry::UnregisterDevice(const std::string& deviceID)
    {
        auto it = devices.find(deviceID);
        if(it == devices.end())
        {
            return RegistryResult::NotFound;
        }

        devices.erase(it);

        return RegistryResult::Success;
    }


    std::optional<DeviceInfo>DeviceRegistry::GetDevice(const std::string& deviceID) const
    {
        auto it = devices.find(deviceID);
        if(it == devices.end())
        {
            return std::nullopt;
        }

        return it->second;
    }

    std::vector<DeviceInfo>DeviceRegistry::GetAllDevices() const
    {
        std::vector<DeviceInfo>result;

        result.reserve(devices.size());

        for(const auto& entry : devices)
        {
            result.push_back(entry.second);
        }

        return result;
    }


    RegistryResult DeviceRegistry::UpdateDeviceInfo(const DeviceInfo& device)
    {
        if(device.deviceID.empty() || device.deviceName.empty())
        {
            return RegistryResult::InvalidData;
        }

        auto it = devices.find(device.deviceID);
        if(it == devices.end())
        {
            return RegistryResult::NotFound;
        }

        it->second.deviceName = device.deviceName;
        it->second.operatingSystem = device.operatingSystem;
        it->second.clientVersion = device.clientVersion;

        return RegistryResult::Success;
    }


    RegistryResult DeviceRegistry::UpdateDeviceStatus(const std::string& deviceID, DeviceStatus newStatus)
    {
        auto it = devices.find(deviceID);
        if(it == devices.end())
        {
            return RegistryResult::NotFound;
        }

        it->second.status = newStatus;

        return RegistryResult::Success;
    }


    bool DeviceRegistry::IsDeviceRegistered(const std::string& deviceID) const
    {
        return devices.find(deviceID) != devices.end();
    }
}
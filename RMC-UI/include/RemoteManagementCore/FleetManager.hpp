#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

#include "DeviceRegistry.hpp"

namespace RMC
{
    struct DeviceGroup
    {
      std::string groupID;
      std::string groupName;
      std::vector<std::string>deviceIDs;
    };

    enum class GroupResult
    {
      Success,
      AlreadyExists,
      NotFound,
      DeviceNotRegistered,
      AlreadyMember,
      NotMember,
      InvalidData
    };

    class FleetManager
    {
    public:

      GroupResult CreateGroup(const std::string& groupID, const std::string& groupName); 
      
      GroupResult DeleteGroup(const std::string& groupID);

      GroupResult AddDeviceToGroup(const std::string& groupID, const std::string& deviceID, const DeviceRegistry& registry);

      GroupResult RemoveDeviceFromGroup(const std::string& groupID, const std::string& deviceID);

      std::optional<std::vector<std::string>> GetGroupDevices(const std::string& groupID) const;

      std::vector<DeviceGroup> GetAllGroups() const;

      bool IsDeviceInGroup(const std::string& groupID, const std::string& deviceID) const;
    
    private:
       
      std::unordered_map<std::string,DeviceGroup> groups;
    };
}
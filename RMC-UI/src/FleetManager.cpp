#include "../include/RemoteManagementCore/FleetManager.hpp"

#include <algorithm>
#include <utility>

namespace RMC
{
    GroupResult FleetManager::CreateGroup(const std::string& groupID, const std::string& groupName)
    {
        if(groupID.empty() || groupName.empty())
        {
            return GroupResult::InvalidData;
        }

        if(groups.find(groupID) != groups.end())
        {
            return GroupResult::AlreadyExists;
        }

        DeviceGroup group;

        group.groupID = groupID;
        group.groupName = groupName;

        groups.emplace(groupID, std::move(group));

        return GroupResult::Success;
    }


    GroupResult FleetManager::DeleteGroup(const std::string& groupID)
    {
        auto it = groups.find(groupID);
        if(it == groups.end())
        {
            return GroupResult::NotFound;
        }

        groups.erase(it);

        return GroupResult::Success;
    }

    GroupResult FleetManager::AddDeviceToGroup(const std::string& groupID, const std::string& deviceID, const DeviceRegistry& registry)
    {
        auto groupIt = groups.find(groupID);
        if(groupIt == groups.end())
        {
            return GroupResult::NotFound;
        }

        if(!registry.IsDeviceRegistered(deviceID))
        {
            return GroupResult::DeviceNotRegistered;
        }

        auto& deviceIDs = groupIt->second.deviceIDs;

        if(std::find(deviceIDs.begin(), deviceIDs.end(), deviceID) != deviceIDs.end())
        {
            return GroupResult::AlreadyMember;
        }

        deviceIDs.push_back(deviceID);

        return GroupResult::Success;
    }


    GroupResult FleetManager::RemoveDeviceFromGroup(const std::string& groupID, const std::string& deviceID)
    {
        auto groupIt = groups.find(groupID);
        if(groupIt == groups.end())
        {
            return GroupResult::NotFound;
        }

        auto& deviceIDs = groupIt->second.deviceIDs;

        auto deviceIt = std::find(deviceIDs.begin(),deviceIDs.end(),deviceID);
        if(deviceIt == deviceIDs.end())
        {
            return GroupResult::NotMember;
        }

        deviceIDs.erase(deviceIt);

        return GroupResult::Success;
    }


    std::optional<std::vector<std::string>> FleetManager::GetGroupDevices(const std::string& groupID) const
    {
        auto it = groups.find(groupID);
        if(it == groups.end())
        {
            return std::nullopt;
        }

        return it->second.deviceIDs;
    }


    std::vector<DeviceGroup> FleetManager::GetAllGroups() const
    {
        std::vector<DeviceGroup> result;

        result.reserve(groups.size());

        for(const auto& entry : groups)
        {
            result.push_back(entry.second);
        }

        return result;
    }


    bool FleetManager::IsDeviceInGroup(const std::string& groupID, const std::string& deviceID) const
    {
        auto groupIt = groups.find(groupID);
        if(groupIt == groups.end())
        {
            return false;
        }

        const auto& deviceIDs = groupIt->second.deviceIDs;

        return std::find(deviceIDs.begin(),deviceIDs.end(), deviceID) != deviceIDs.end();
    }
}
#include "../include/RemoteManagementCore/ConnectionCoordinator.hpp"
#include <utility>

namespace RMC
{
    ConnectionCoordinator::ConnectionCoordinator(
        DeviceRegistry& registryRef, 
        ConnectHandler onConnect, 
        DisconnectHandler onDisconnect
    ) 
    : registry(registryRef), 
      connectHandler(std::move(onConnect)), 
      disconnectHandler(std::move(onDisconnect))
    {
    }

    ConnectionResult ConnectionCoordinator::StartConnection(const std::string& deviceID, const ConnectionTarget& target)
    {
        if(!registry.IsDeviceRegistered(deviceID))
        {
            return ConnectionResult::DeviceNotRegistered;
        }

        if(!connectHandler)
        {
            return ConnectionResult::HandlerNotConfigured;
        }

        if(target.address.empty())
        {
            return ConnectionResult::InvalidAddress;
        }

        if(target.port == 0)
        {
            return ConnectionResult::InvalidPort;
        }

        ConnectionState currentState = GetConnectionState(deviceID);
        if(currentState == ConnectionState::Connected)
        {
            return ConnectionResult::AlreadyConnected;
        }

        if(currentState == ConnectionState::Connecting)
        {
            return ConnectionResult::AlreadyConnecting;
        }

        connectionStates[deviceID] = ConnectionState::Connecting;

        bool requestAccepted = connectHandler(deviceID, target);

        if(!requestAccepted)
        {
            connectionStates[deviceID] = ConnectionState::Failed;
            return ConnectionResult::ConnectionFailed;
        }

        return ConnectionResult::Success;
    }


    ConnectionResult ConnectionCoordinator::DisconnectDevice(const std::string& deviceID)
    {
        if(!registry.IsDeviceRegistered(deviceID))
        {
            return ConnectionResult::DeviceNotRegistered;
        }

        if(!disconnectHandler)
        {
            return ConnectionResult::HandlerNotConfigured;
        }

        ConnectionState currentState = GetConnectionState(deviceID);
        if(currentState != ConnectionState::Connected)
        {
            return ConnectionResult::NotConnected;
        }

        connectionStates[deviceID] = ConnectionState::Disconnecting;

        bool requestAccepted = disconnectHandler(deviceID);
        if(!requestAccepted)
        {
            connectionStates[deviceID] = ConnectionState::Connected;

            return ConnectionResult::ConnectionFailed;
        }

        return ConnectionResult::Success;
    }

    ConnectionResult ConnectionCoordinator::HandleDeviceConnected(const std::string& deviceID)
    {
        if(!registry.IsDeviceRegistered(deviceID))
        {
            return ConnectionResult::DeviceNotRegistered;
        }

        ConnectionState currentState = GetConnectionState(deviceID);

        if(currentState != ConnectionState::Connecting)
        {
            return ConnectionResult::ConnectionFailed;
        }


        connectionStates[deviceID] = ConnectionState::Connected;

        registry.UpdateDeviceStatus(deviceID, DeviceStatus::Online);
        
        return ConnectionResult::Success;
    }


    ConnectionResult ConnectionCoordinator::HandleDeviceDisconnected(const std::string& deviceID)
    {
        if(!registry.IsDeviceRegistered(deviceID))
        {
            return ConnectionResult::DeviceNotRegistered;
        }

        ConnectionState currentState = GetConnectionState(deviceID);
        if(currentState != ConnectionState::Connected && currentState != ConnectionState::Disconnecting && currentState != ConnectionState::Connecting)
        {
            return ConnectionResult::NotConnected;
        }

        connectionStates[deviceID] = ConnectionState::Disconnected;

        registry.UpdateDeviceStatus(deviceID, DeviceStatus::Offline);

        return ConnectionResult::Success;
    }


    ConnectionState ConnectionCoordinator::GetConnectionState(const std::string& deviceID) const
    {
       auto it = connectionStates.find(deviceID);
       if(it == connectionStates.end())
       {
        return ConnectionState::Disconnected;
       }

       return it->second;
    }

    bool ConnectionCoordinator::IsConnected(const std::string& deviceID) const
    {
        return GetConnectionState(deviceID) == ConnectionState::Connected;
    }
}
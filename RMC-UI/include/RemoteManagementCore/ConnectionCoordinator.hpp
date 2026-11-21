#pragma once



#include <string>

#include <functional>

#include <unordered_map>

#include <cstdint>



#include "DeviceRegistry.hpp"



namespace RMC

{

    enum class ConnectionState

    {

        Disconnected,

        Connecting,

        Connected,

        Disconnecting,

        Failed

    };





    enum class ConnectionResult

    {

        Success,

        DeviceNotRegistered,

        AlreadyConnected,

        AlreadyConnecting,

        NotConnected,

        ConnectionFailed,

        InvalidAddress,

        InvalidPort,

        HandlerNotConfigured

    };



    struct ConnectionTarget

    {

        std::string address;

        std::uint16_t port = 0;

    };





    class ConnectionCoordinator

    {

     public:

       

        using ConnectHandler = std::function<bool(const std::string&, const ConnectionTarget&)>;



        using DisconnectHandler = std::function<bool(const std::string&)>;



        ConnectionCoordinator(DeviceRegistry& registry,ConnectHandler connectHandler, DisconnectHandler disconnectHandler);

     

        ConnectionResult StartConnection(const std::string& deviceID, const ConnectionTarget& target);



        ConnectionResult DisconnectDevice(const std::string& deviceID);



        ConnectionResult HandleDeviceConnected(const std::string& deviceID);
        


        ConnectionResult HandleDeviceDisconnected(const std::string& deviceID);


        
        ConnectionState GetConnectionState(const std::string& deviceID) const;



        bool IsConnected(const std::string& deviceID) const;



      private:

       

        DeviceRegistry& registry;



        ConnectHandler connectHandler;



        DisconnectHandler disconnectHandler;



        std::unordered_map<std::string,ConnectionState> connectionStates;

    };

} 
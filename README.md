# RemoteManagementCore (RMC)

A lightweight, dependency-free C++17 core library for managing device registrations, device groups, and connection states.

## Overview

RemoteManagementCore (RMC) provides a modular foundation for organizing and managing registered devices.

The library separates device registration, fleet organization, and connection-state coordination into three independent components. These components can be integrated into a larger application through a simple C++ interface.

RMC focuses on core management logic rather than implementing a specific network transport or communication protocol.

## Core Components

### 1. DeviceRegistry

`DeviceRegistry` manages registered device information using an internal `std::unordered_map`.

Each device is represented by a `DeviceInfo` structure containing:

- `deviceID` — Unique device identifier.
- `deviceName` — Device name.
- `operatingSystem` — Operating system information.
- `clientVersion` — Client version.
- `status` — Current device status.

Supported operations:

| Function | Description |
|---|---|
| `RegistryDevice()` | Registers a new device. |
| `UnregisterDevice()` | Removes a registered device. |
| `GetDevice()` | Retrieves a device by its identifier. |
| `GetAllDevices()` | Returns all registered devices. |
| `UpdateDeviceInfo()` | Updates device metadata. |
| `UpdateDeviceStatus()` | Changes the device status. |
| `IsDeviceRegistered()` | Checks whether a device exists. |

Available device statuses:

- `Offline`
- `Online`
- `Unauthorized`

Registry operations return a `RegistryResult` describing the outcome, including successful operations, duplicate registrations, missing devices, and invalid input.

### 2. FleetManager

`FleetManager` organizes registered devices into logical groups.

Each group is represented by a `DeviceGroup` structure containing:

- `groupID`
- `groupName`
- `deviceIDs`

Supported operations:

| Function | Description |
|---|---|
| `CreateGroup()` | Creates a new device group. |
| `DeleteGroup()` | Deletes an existing group. |
| `AddDeviceToGroup()` | Adds a registered device to a group. |
| `RemoveDeviceFromGroup()` | Removes a device from a group. |
| `GetGroupDevices()` | Retrieves the device identifiers belonging to a group. |
| `GetAllGroups()` | Returns all existing groups. |
| `IsDeviceInGroup()` | Checks group membership. |

The manager validates group existence, device registration, and duplicate membership before adding devices.

Operations return a `GroupResult` describing the result of each operation.

### 3. ConnectionCoordinator

`ConnectionCoordinator` manages connection states for registered devices.

It uses callback-based handlers to delegate connection and disconnection requests to an external implementation.

Available connection states:

- `Disconnected`
- `Connecting`
- `Connected`
- `Disconnecting`
- `Failed`

Connection requests use the `ConnectionTarget` structure:

- `address` — Target address.
- `port` — Target port.

The component exposes the following operations:

| Function | Description |
|---|---|
| `StartConnection()` | Validates and initiates a connection request. |
| `DisconnectDevice()` | Initiates a disconnection request. |
| `HandleDeviceConnected()` | Processes a successful connection notification. |
| `HandleDeviceDisconnected()` | Processes a disconnection notification. |
| `GetConnectionState()` | Retrieves the current connection state. |
| `IsConnected()` | Checks whether a device is connected. |

#### Callback interface

Two callback types are used:

- `ConnectHandler`: `bool(const std::string&, const ConnectionTarget&)`
- `DisconnectHandler`: `bool(const std::string&)`

The callbacks return a boolean indicating whether the external operation accepted the request.

RMC does not implement the underlying network connection itself.

#### Connection lifecycle

A typical connection lifecycle is:

1. `StartConnection()` validates the registered device and target.
2. The connection state changes to `Connecting`.
3. The configured connection handler is invoked.
4. If the handler rejects the request, the state becomes `Failed`.
5. If the request is accepted, the coordinator waits for an external connection notification.
6. `HandleDeviceConnected()` changes the state to `Connected` and updates the registry status to `Online`.

Disconnection follows a similar callback-based process. A successful request moves the state to `Disconnecting` until `HandleDeviceDisconnected()` confirms the disconnection.

## Project Structure

```text
RemoteMangerCore-RMC/
│
├── README.md
│
└── RMC-UI/
    │
    ├── CMakeLists.txt
    │
    ├── include/
    │   └── RemoteManagementCore/
    │       ├── DeviceRegistry.hpp
    │       ├── FleetManager.hpp
    │       └── ConnectionCoordinator.hpp
    │
    └── src/
        ├── DeviceRegistry.cpp
        ├── FleetManager.cpp
        └── ConnectionCoordinator.cpp
```

## Requirements

- C++17 compatible compiler.
- CMake 3.16 or later.
- Standard C++ library.

No external C++ libraries are required by the core implementation.

## Integration Example

The following example demonstrates device registration and group management using the public interfaces.

```cpp
#include <iostream>

#include "RemoteManagementCore/DeviceRegistry.hpp"
#include "RemoteManagementCore/FleetManager.hpp"

int main()
{
    RMC::DeviceRegistry registry;
    RMC::FleetManager fleet;

    RMC::DeviceInfo device;

    device.deviceID = "DEVICE-001";
    device.deviceName = "Workstation";
    device.operatingSystem = "Windows";
    device.clientVersion = "1.0.0";

    auto registration = registry.RegistryDevice(device);

    if (registration != RMC::RegistryResult::Success)
    {
        std::cout << "Device registration failed.\n";
        return 1;
    }

    auto group = fleet.CreateGroup("GROUP-001", "Workstations");

    if (group != RMC::GroupResult::Success)
    {
        std::cout << "Group creation failed.\n";
        return 1;
    }

    auto result = fleet.AddDeviceToGroup(
        "GROUP-001",
        "DEVICE-001",
        registry
    );

    if (result == RMC::GroupResult::Success)
    {
        std::cout << "Device successfully added to group.\n";
    }

    return 0;
}
```

## Current Scope and Limitations

The current implementation provides in-memory device management and connection-state coordination.

The following capabilities are not implemented in the current source code:

- Network transport or socket communication.
- Remote command execution.
- Authentication or authorization protocols.
- Encryption or secure channel establishment.
- Persistent storage.
- Automatic device discovery.
- Multithreaded synchronization.
- A graphical user interface.

Connection handlers must be supplied by the integrating application.

Device information, groups, and connection states are maintained in memory and are not persisted between application runs.

## Build Configuration

The project includes a CMake configuration targeting a static library named `RemoteManagementCore` and an executable named `RemoteManagementCoreTestUI`.

**Current repository limitation:** `CMakeLists.txt` references `src/TestUI.cpp`, but that source file is not currently present in the repository. Consequently, the existing CMake configuration requires that file to be added or the executable target to be removed before configuration can complete successfully.

## License

No license file is currently included in the repository. Refer to the repository's licensing information before redistributing or incorporating the code into other projects.

--	This file will contain
--	components that are needed
--	for the network

-- Network Component
local NetworkComponent = ComponentType()
NetworkComponent.Name = "Network"
NetworkComponent.SyncNetwork = true
NetworkComponent.TableType = TableType.Map
worldCreator:AddComponentType(NetworkComponent)

-- Username Component
local NameComponent = ComponentType()
NameComponent.Name = "Username"
NameComponent.SyncNetwork = true
NameComponent.TableType = TableType.Map
NameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(NameComponent)

-- NetConnection Component
local NetConnection = ComponentType()
NetConnection.Name = "NetConnection"
NetConnection.TableType = TableType.Map
NetConnection:AddVariable("IpAddress", ByteSize.Text)
NetConnection:AddVariable("Port", ByteSize.Int)
worldCreator:AddComponentType(NetConnection)

-- Active NetConnection Component
local ActiveNetConnection = ComponentType()
ActiveNetConnection.Name = "ActiveNetConnection"
ActiveNetConnection.TableType = TableType.Map
worldCreator:AddComponentType(ActiveNetConnection)

-- Sync Network Component
local SyncNetworkComponent = ComponentType()
SyncNetworkComponent.Name = "SyncNetwork"
SyncNetworkComponent.SyncNetwork = true
SyncNetworkComponent.TableType = TableType.Array
worldCreator:AddComponentType(SyncNetworkComponent)

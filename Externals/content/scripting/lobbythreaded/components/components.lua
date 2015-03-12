
-- Position Component
local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", ByteSize.Float)
PositionComponent:AddVariable("Y", ByteSize.Float)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

-- Scale Component
local ScaleComponent = ComponentType()
ScaleComponent.Name = "Scale"
ScaleComponent.TableType = TableType.Array
ScaleComponent:AddVariable("X", ByteSize.Float)
ScaleComponent:AddVariable("Y", ByteSize.Float)
ScaleComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ScaleComponent)

-- Rotation Component
local RotationComponent = ComponentType()
RotationComponent.Name = "Rotation"
RotationComponent.TableType = TableType.Array
RotationComponent:AddVariable("X", ByteSize.Float)
RotationComponent:AddVariable("Y", ByteSize.Float)
RotationComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(RotationComponent)

-- Color Component
local ColorComponent = ComponentType()
ColorComponent.Name = "Color"
ColorComponent.TableType = TableType.Array
ColorComponent:AddVariable("X", ByteSize.Float)
ColorComponent:AddVariable("Y", ByteSize.Float)
ColorComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ColorComponent)

-- Velocity Component
local VelocityComponent = ComponentType()
VelocityComponent.Name = "Velocity"
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", ByteSize.Float)
VelocityComponent:AddVariable("Y", ByteSize.Float)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)

-- Render Component
local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", ByteSize.Int)
RenderComponent:AddVariable("Mat", ByteSize.Matrix)
RenderComponent:AddVariable("ColorX", ByteSize.Float)
RenderComponent:AddVariable("ColorY", ByteSize.Float)
RenderComponent:AddVariable("ColorZ", ByteSize.Float)
worldCreator:AddComponentType(RenderComponent)

--	Model Component
local ModelComponent = ComponentType()
ModelComponent.Name = "Model"
ModelComponent.SyncNetwork = true
ModelComponent.TableType = TableType.Map
ModelComponent:AddVariable("ModelName", ByteSize.Text)
ModelComponent:AddVariable("ModelPath", ByteSize.Text)
ModelComponent:AddVariable("RenderType", ByteSize.Int)
ModelComponent:AddVariable("RenderShadow", ByteSize.Int)
worldCreator:AddComponentType(ModelComponent)

-- Username Component
local NameComponent = ComponentType()
NameComponent.Name = "Username"
NameComponent.TableType = TableType.Map
NameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(NameComponent)

-- NetConnection Component
local NetConnection = ComponentType()
NetConnection.Name = "NetConnection"
NetConnection.TableType = TableType.Map
NetConnection:AddVariable("IpAddress", ByteSize.Text)
NetConnection:AddVariable("Port", ByteSize.Int)
NetConnection:AddVariable("Active", ByteSize.Bool)
worldCreator:AddComponentType(NetConnection)

-- Slerp Rotation Component
local SlerpRotationComponent = ComponentType()
SlerpRotationComponent.Name = "SlerpRotation"
SlerpRotationComponent.SyncNetwork = true
SlerpRotationComponent.TableType = TableType.Map
SlerpRotationComponent:AddVariable("fromX", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromY", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromZ", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromW", ByteSize.Float)
SlerpRotationComponent:AddVariable("time", ByteSize.Float)
SlerpRotationComponent:AddVariable("toX", ByteSize.Float)
SlerpRotationComponent:AddVariable("toY", ByteSize.Float)
SlerpRotationComponent:AddVariable("toZ", ByteSize.Float)
SlerpRotationComponent:AddVariable("toW", ByteSize.Float)
worldCreator:AddComponentType(SlerpRotationComponent)


-- Available Spawnpoint
local AvailableSpawnpoint = ComponentType()
AvailableSpawnpoint.Name = "AvailableSpawnpoint"
AvailableSpawnpoint.TableType = TableType.Map
AvailableSpawnpoint:AddVariable("X", ByteSize.Int)
AvailableSpawnpoint:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(AvailableSpawnpoint)

--	Player Component
local PlayerComponent = ComponentType()
PlayerComponent.Name = "Player"
PlayerComponent.TableType = TableType.Map
worldCreator:AddComponentType(PlayerComponent)

local GameRunningComponent = ComponentType()
GameRunningComponent.Name = "GameRunning"
GameRunningComponent.TableType = TableType.Map
worldCreator:AddComponentType(GameRunningComponent)

-- Sync Network Component
local SyncNetworkComponent = ComponentType()
SyncNetworkComponent.Name = "SyncNetwork"
SyncNetworkComponent.SyncNetwork = true
SyncNetworkComponent.TableType = TableType.Array
worldCreator:AddComponentType(SyncNetworkComponent)


-- Hide Component
local HideComponent = ComponentType()
HideComponent.Name = "Hide"
HideComponent.SyncNetwork = true
HideComponent.TableType = TableType.Map
worldCreator:AddComponentType(HideComponent)


--	PlayerName Component
NewComponent = ComponentType()
NewComponent.Name = "PlayerName"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Name", ByteSize.String)
worldCreator:AddComponentType(NewComponent)


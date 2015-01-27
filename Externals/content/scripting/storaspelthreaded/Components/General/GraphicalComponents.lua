--	This file will contain
--	components that are needed
--	to render graphical objects

--	Position
local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.SyncNetwork = true
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", ByteSize.Float)
PositionComponent:AddVariable("Y", ByteSize.Float)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

local ScaleComponent = ComponentType()
ScaleComponent.Name = "Scale"
ScaleComponent.SyncNetwork = true
ScaleComponent.TableType = TableType.Array
ScaleComponent:AddVariable("X", ByteSize.Float)
ScaleComponent:AddVariable("Y", ByteSize.Float)
ScaleComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ScaleComponent)

local RotationComponent = ComponentType()
RotationComponent.Name = "Rotation"
RotationComponent.SyncNetwork = true
RotationComponent.TableType = TableType.Array
RotationComponent:AddVariable("X", ByteSize.Float)
RotationComponent:AddVariable("Y", ByteSize.Float)
RotationComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(RotationComponent)

-- Render Component
local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", ByteSize.Int)
RenderComponent:AddVariable("Mat", ByteSize.Matrix)
worldCreator:AddComponentType(RenderComponent)

--	Model Component
local ModelComponent = ComponentType()
ModelComponent.Name = "Model"
ModelComponent.SyncNetwork = true
ModelComponent.TableType = TableType.Map
ModelComponent:AddVariable("ModelName", ByteSize.Text)
ModelComponent:AddVariable("ModelPath", ByteSize.Text)
ModelComponent:AddVariable("RenderType", ByteSize.Int)
worldCreator:AddComponentType(ModelComponent)

-- Hide Component
local HideComponent = ComponentType()
HideComponent.Name = "Hide"
HideComponent.SyncNetwork = true
HideComponent.TableType = TableType.Map
worldCreator:AddComponentType(HideComponent)

-- Camera Free Look Component
local CameraSystemComponent = ComponentType()
CameraSystemComponent.Name = "CameraSystemComponent"
CameraSystemComponent.TableType = TableType.Map
worldCreator:AddComponentType(CameraSystemComponent)


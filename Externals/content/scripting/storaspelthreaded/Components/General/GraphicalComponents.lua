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

local ColorComponent = ComponentType()
ColorComponent.Name = "Color"
ColorComponent.SyncNetwork = true
ColorComponent.TableType = TableType.Array
ColorComponent:AddVariable("X", ByteSize.Float)
ColorComponent:AddVariable("Y", ByteSize.Float)
ColorComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ColorComponent)

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
worldCreator:AddComponentType(ModelComponent)

--	AnimationModel Component
local AModelComponent = ComponentType()
AModelComponent.Name = "AModel"
AModelComponent.SyncNetwork = true
AModelComponent.TableType = TableType.Map
AModelComponent:AddVariable("ModelName", ByteSize.Text)
AModelComponent:AddVariable("ModelPath", ByteSize.Text)
AModelComponent:AddVariable("RenderType", ByteSize.Int)
worldCreator:AddComponentType(AModelComponent)

-- Hide Component
local NoShadowComponent = ComponentType()
NoShadowComponent.Name = "NoShadow"
NoShadowComponent.SyncNetwork = true
NoShadowComponent.TableType = TableType.Map
worldCreator:AddComponentType(NoShadowComponent)

-- Hide Component
local HideComponent = ComponentType()
HideComponent.Name = "Hide"
HideComponent.SyncNetwork = true
HideComponent.TableType = TableType.Map
worldCreator:AddComponentType(HideComponent)

-- Static Model Component
local StaticModelComponent = ComponentType()
StaticModelComponent.Name = "StaticModel"
StaticModelComponent.SyncNetwork = true
StaticModelComponent.TableType = TableType.Map
worldCreator:AddComponentType(StaticModelComponent)
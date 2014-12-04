local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", 4)
PositionComponent:AddVariable("Y", 4)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

local VelocityComponent = ComponentType()
VelocityComponent.Name = "Velocity"
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", 4)
VelocityComponent:AddVariable("Y", 4)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)

local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", 4)
RenderComponent:AddVariable("Mat", 64)
worldCreator:AddComponentType(RenderComponent)
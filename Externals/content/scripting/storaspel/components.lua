local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", 4)
PositionComponent:AddVariable("Y", 4)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

local VelocityComponent = ComponentType()
VelocityComponent.Name = "VelocityComponent"
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", 4)
VelocityComponent:AddVariable("Y", 4)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)


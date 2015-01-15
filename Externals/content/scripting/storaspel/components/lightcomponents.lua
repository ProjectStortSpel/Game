
--	RotateAround Component
local RotateAroundComponent = ComponentType()
RotateAroundComponent.Name = "RotateAround"
RotateAroundComponent.TableType = TableType.Array
RotateAroundComponent:AddVariable("PosX", ByteSize.Float)
RotateAroundComponent:AddVariable("PosY", ByteSize.Float)
RotateAroundComponent:AddVariable("PosZ", ByteSize.Float)

RotateAroundComponent:AddVariable("Radius", ByteSize.Float)
RotateAroundComponent:AddVariable("Factor", ByteSize.Float)
worldCreator:AddComponentType(RotateAroundComponent)

-- Pointlight Component
local PointlightComponent = ComponentType()
PointlightComponent.Name = "Pointlight"
PointlightComponent.SyncNetwork = true
PointlightComponent.TableType = TableType.Array
PointlightComponent:AddVariable("PosX", ByteSize.Float)
PointlightComponent:AddVariable("PosY", ByteSize.Float)
PointlightComponent:AddVariable("PosZ", ByteSize.Float)

PointlightComponent:AddVariable("AmbientInt", ByteSize.Float)
PointlightComponent:AddVariable("DiffuseInt", ByteSize.Float)
PointlightComponent:AddVariable("SpecularInt", ByteSize.Float)

PointlightComponent:AddVariable("ColorRed", ByteSize.Float)
PointlightComponent:AddVariable("ColorGreen", ByteSize.Float)
PointlightComponent:AddVariable("ColorBlue", ByteSize.Float)

PointlightComponent:AddVariable("Range", ByteSize.Float)
worldCreator:AddComponentType(PointlightComponent)

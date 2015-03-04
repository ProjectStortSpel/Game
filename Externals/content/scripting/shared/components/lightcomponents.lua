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

FadeInLight = ComponentType()
FadeInLight.Name = "FadeInLight"
FadeInLight.SyncNetwork = true
FadeInLight.TableType = TableType.Array
FadeInLight:AddVariable("CurrentTime", ByteSize.Float)
FadeInLight:AddVariable("FadeTime", ByteSize.Float)
worldCreator:AddComponentType(FadeInLight)

-- DirectionalLight Component
local DirectionalLightComponent = ComponentType()
DirectionalLightComponent.Name = "DirectionalLight"
DirectionalLightComponent.SyncNetwork = true
DirectionalLightComponent.TableType = TableType.Array
DirectionalLightComponent:AddVariable("DirX", ByteSize.Float)
DirectionalLightComponent:AddVariable("DirY", ByteSize.Float)
DirectionalLightComponent:AddVariable("DirZ", ByteSize.Float)

DirectionalLightComponent:AddVariable("AmbientInt", ByteSize.Float)
DirectionalLightComponent:AddVariable("DiffuseInt", ByteSize.Float)
DirectionalLightComponent:AddVariable("SpecularInt", ByteSize.Float)

DirectionalLightComponent:AddVariable("ColorRed", ByteSize.Float)
DirectionalLightComponent:AddVariable("ColorGreen", ByteSize.Float)
DirectionalLightComponent:AddVariable("ColorBlue", ByteSize.Float)

DirectionalLightComponent:AddVariable("Range", ByteSize.Float)
worldCreator:AddComponentType(DirectionalLightComponent)

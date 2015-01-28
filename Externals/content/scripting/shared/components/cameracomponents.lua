-- Camera Free Look Component
local CameraSystemComponent = ComponentType()
CameraSystemComponent.Name = "CameraSystemComponent"
CameraSystemComponent.TableType = TableType.Map
worldCreator:AddComponentType(CameraSystemComponent)

-- Camera Component
local CameraComponent = ComponentType()
CameraComponent.Name = "Camera"
CameraComponent.SyncNetwork = true
CameraComponent.TableType = TableType.Map
CameraComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(CameraComponent)
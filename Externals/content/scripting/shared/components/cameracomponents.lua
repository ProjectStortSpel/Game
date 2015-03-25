-- Camera Free Look Component
local CameraSystemComponent = ComponentType()
CameraSystemComponent.Name = "CameraSystemComponent"
CameraSystemComponent.TableType = TableType.Map
worldCreator:AddComponentType(CameraSystemComponent)

-- Camera Component
local CameraInterestPoint = ComponentType()
CameraInterestPoint.Name = "CameraInterestPoint"
CameraInterestPoint.TableType = TableType.Map
CameraInterestPoint:AddVariable("AtX", ByteSize.Float)
CameraInterestPoint:AddVariable("AtZ", ByteSize.Float)
CameraInterestPoint:AddVariable("UpX", ByteSize.Float)
CameraInterestPoint:AddVariable("UpZ", ByteSize.Float)
CameraInterestPoint:AddVariable("Distance", ByteSize.Float)
CameraInterestPoint:AddVariable("Time", ByteSize.Float)
worldCreator:AddComponentType(CameraInterestPoint)

-- Camera Component
local CameraOnPlayer = ComponentType()
CameraOnPlayer.Name = "CameraOnPlayer"
CameraOnPlayer.TableType = TableType.Map
worldCreator:AddComponentType(CameraOnPlayer)

-- Camera Component
local CameraOnPlayer = ComponentType()
CameraOnPlayer.Name = "CameraElement"
CameraOnPlayer.TableType = TableType.Map
worldCreator:AddComponentType(CameraOnPlayer)

-- Camera Component
local CameraComponent = ComponentType()
CameraComponent.Name = "Camera"
CameraComponent.SyncNetwork = true
CameraComponent.TableType = TableType.Map
CameraComponent:AddVariable("EntityId", ByteSize.Reference)
worldCreator:AddComponentType(CameraComponent)
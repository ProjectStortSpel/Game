-- Camera Free Look Component
local FreeLookCamera = ComponentType()
FreeLookCamera.Name = "FreeLookCamera"
worldCreator:AddComponentType(FreeLookCamera)

-- Camera Free Look Component
local LerpCamera = ComponentType()
LerpCamera.Name = "LerpCamera"
worldCreator:AddComponentType(LerpCamera)

-- Camera Start Position
local CameraStartPosition = ComponentType()
CameraStartPosition.Name = "CameraStartPosition"
CameraStartPosition.TableType = TableType.Map
CameraStartPosition:AddVariable("X", ByteSize.Float)
CameraStartPosition:AddVariable("Y", ByteSize.Float)
CameraStartPosition:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(CameraStartPosition)

-- Camera Target Position
local CameraTargetPosition = ComponentType()
CameraTargetPosition.Name = "CameraTargetPosition"
CameraTargetPosition.TableType = TableType.Map
CameraTargetPosition:AddVariable("X", ByteSize.Float)
CameraTargetPosition:AddVariable("Y", ByteSize.Float)
CameraTargetPosition:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(CameraTargetPosition)

-- Lerp Time
local CameraLerpTime = ComponentType()
CameraLerpTime.Name = "CameraLerpTime"
CameraLerpTime.TableType = TableType.Map
CameraLerpTime:AddVariable("Time", ByteSize.Float)
CameraLerpTime:AddVariable("Timer", ByteSize.Float)
worldCreator:AddComponentType(CameraLerpTime)
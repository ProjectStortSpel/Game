--	This file will contain
--	components that are needed
--	for LERPING

-- Lerp Start Position
local LerpStartPosition = ComponentType()
LerpStartPosition.Name = "LerpStartPosition"
LerpStartPosition.TableType = TableType.Map
LerpStartPosition:AddVariable("X", ByteSize.Float)
LerpStartPosition:AddVariable("Y", ByteSize.Float)
LerpStartPosition:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(LerpStartPosition)

-- Lerp Target Position
local LerpTargetPosition = ComponentType()
LerpTargetPosition.Name = "LerpTargetPosition"
LerpTargetPosition.TableType = TableType.Map
LerpTargetPosition:AddVariable("X", ByteSize.Float)
LerpTargetPosition:AddVariable("Y", ByteSize.Float)
LerpTargetPosition:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(LerpTargetPosition)

-- Lerp Time
local LerpTime = ComponentType()
LerpTime.Name = "LerpTime"
LerpTime.TableType = TableType.Map
LerpTime:AddVariable("Time", ByteSize.Float)
LerpTime:AddVariable("Timer", ByteSize.Float)
worldCreator:AddComponentType(LerpTime)


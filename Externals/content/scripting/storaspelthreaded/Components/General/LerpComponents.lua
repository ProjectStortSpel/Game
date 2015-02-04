--	This file will contain
--	components that are needed
--	for LERPING

local LerpPosition = ComponentType()
LerpPosition.Name = "LerpPosition"
LerpPosition.TableType = TableType.Map
LerpPosition:AddVariable("X", ByteSize.Float)
LerpPosition:AddVariable("Y", ByteSize.Float)
LerpPosition:AddVariable("Z", ByteSize.Float)
LerpPosition:AddVariable("Time", ByteSize.Float)
LerpPosition:AddVariable("Algorithm", ByteSize.Text)
worldCreator:AddComponentType(LerpPosition)


-- Lerp Start Position
local LerpingPosition = ComponentType()
LerpingPosition.Name = "LerpingPosition"
LerpingPosition.TableType = TableType.Map
LerpingPosition:AddVariable("sX", ByteSize.Float)
LerpingPosition:AddVariable("sY", ByteSize.Float)
LerpingPosition:AddVariable("sZ", ByteSize.Float)
LerpingPosition:AddVariable("tX", ByteSize.Float)
LerpingPosition:AddVariable("tY", ByteSize.Float)
LerpingPosition:AddVariable("tZ", ByteSize.Float)
LerpingPosition:AddVariable("Time", ByteSize.Float)
LerpingPosition:AddVariable("Timer", ByteSize.Float)
LerpingPosition:AddVariable("Algorithm", ByteSize.Text)
worldCreator:AddComponentType(LerpingPosition)

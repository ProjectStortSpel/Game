-- Lerp Position
local LerpPosition = ComponentType()
LerpPosition.Name = "LerpPosition"
LerpPosition.TableType = TableType.Map
LerpPosition:AddVariable("X", ByteSize.Float)
LerpPosition:AddVariable("Y", ByteSize.Float)
LerpPosition:AddVariable("Z", ByteSize.Float)
LerpPosition:AddVariable("Time", ByteSize.Float)
LerpPosition:AddVariable("Algorithm", ByteSize.Text)
worldCreator:AddComponentType(LerpPosition)

-- Lerping Position
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


-- Lerp Scale
local LerpScale = ComponentType()
LerpScale.Name = "LerpScale"
LerpScale.TableType = TableType.Map
LerpScale:AddVariable("X", ByteSize.Float)
LerpScale:AddVariable("Y", ByteSize.Float)
LerpScale:AddVariable("Z", ByteSize.Float)
LerpScale:AddVariable("Time", ByteSize.Float)
LerpScale:AddVariable("Algorithm", ByteSize.Text)
worldCreator:AddComponentType(LerpScale)

-- Lerping Scale
local LerpingScale = ComponentType()
LerpingScale.Name = "LerpingScale"
LerpingScale.TableType = TableType.Map
LerpingScale:AddVariable("sX", ByteSize.Float)
LerpingScale:AddVariable("sY", ByteSize.Float)
LerpingScale:AddVariable("sZ", ByteSize.Float)
LerpingScale:AddVariable("tX", ByteSize.Float)
LerpingScale:AddVariable("tY", ByteSize.Float)
LerpingScale:AddVariable("tZ", ByteSize.Float)
LerpingScale:AddVariable("Time", ByteSize.Float)
LerpingScale:AddVariable("Timer", ByteSize.Float)
LerpingScale:AddVariable("Algorithm", ByteSize.Text)
worldCreator:AddComponentType(LerpingScale)

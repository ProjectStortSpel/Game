-- Lerp Position
local LerpPosition = ComponentType()
LerpPosition.Name = "LerpPosition"
LerpPosition.TableType = TableType.Map
LerpPosition:AddVariable("Algorithm", ByteSize.Text)
LerpPosition:AddVariable("Time", ByteSize.Float)
LerpPosition:AddVariable("X", ByteSize.Float)
LerpPosition:AddVariable("Y", ByteSize.Float)
LerpPosition:AddVariable("Z", ByteSize.Float)
LerpPosition:AddVariable("KillWhenFinished", ByteSize.Bool)
worldCreator:AddComponentType(LerpPosition)

-- Lerping Position
local LerpingPosition = ComponentType()
LerpingPosition.Name = "LerpingPosition"
LerpingPosition.SyncNetwork = true
LerpingPosition.TableType = TableType.Map
LerpingPosition:AddVariable("Algorithm", ByteSize.Text)
LerpingPosition:AddVariable("Time", ByteSize.Float)
LerpingPosition:AddVariable("Timer", ByteSize.Float)
LerpingPosition:AddVariable("sX", ByteSize.Float)
LerpingPosition:AddVariable("sY", ByteSize.Float)
LerpingPosition:AddVariable("sZ", ByteSize.Float)
LerpingPosition:AddVariable("tX", ByteSize.Float)
LerpingPosition:AddVariable("tY", ByteSize.Float)
LerpingPosition:AddVariable("tZ", ByteSize.Float)
LerpingPosition:AddVariable("KillWhenFinished", ByteSize.Bool)
worldCreator:AddComponentType(LerpingPosition)

-- Lerp Scale
local LerpScale = ComponentType()
LerpScale.Name = "LerpScale"
LerpScale.TableType = TableType.Map
LerpScale:AddVariable("Algorithm", ByteSize.Text)
LerpScale:AddVariable("Time", ByteSize.Float)
LerpScale:AddVariable("X", ByteSize.Float)
LerpScale:AddVariable("Y", ByteSize.Float)
LerpScale:AddVariable("Z", ByteSize.Float)
LerpScale:AddVariable("KillWhenFinished", ByteSize.Bool)
worldCreator:AddComponentType(LerpScale)

-- Lerping Scale
local LerpingScale = ComponentType()
LerpingScale.Name = "LerpingScale"
LerpingScale.SyncNetwork = true
LerpingScale.TableType = TableType.Map
LerpingScale:AddVariable("Algorithm", ByteSize.Text)
LerpingScale:AddVariable("Time", ByteSize.Float)
LerpingScale:AddVariable("Timer", ByteSize.Float)
LerpingScale:AddVariable("sX", ByteSize.Float)
LerpingScale:AddVariable("sY", ByteSize.Float)
LerpingScale:AddVariable("sZ", ByteSize.Float)
LerpingScale:AddVariable("tX", ByteSize.Float)
LerpingScale:AddVariable("tY", ByteSize.Float)
LerpingScale:AddVariable("tZ", ByteSize.Float)
LerpingScale:AddVariable("KillWhenFinished", ByteSize.Bool)
worldCreator:AddComponentType(LerpingScale)

-- Lerp Rotation
local LerpRotation = ComponentType()
LerpRotation.Name = "LerpRotation"
LerpRotation.TableType = TableType.Map
LerpRotation:AddVariable("Algorithm", ByteSize.Text)
LerpRotation:AddVariable("Time", ByteSize.Float)
LerpRotation:AddVariable("X", ByteSize.Float)
LerpRotation:AddVariable("Y", ByteSize.Float)
LerpRotation:AddVariable("Z", ByteSize.Float)
LerpRotation:AddVariable("KillWhenFinished", ByteSize.Bool)
worldCreator:AddComponentType(LerpRotation)

-- Lerping Rotation
local LerpingRotation = ComponentType()
LerpingRotation.Name = "LerpingRotation"
LerpingRotation.SyncNetwork = true
LerpingRotation.TableType = TableType.Map
LerpingRotation:AddVariable("Algorithm", ByteSize.Text)
LerpingRotation:AddVariable("Time", ByteSize.Float)
LerpingRotation:AddVariable("Timer", ByteSize.Float)
LerpingRotation:AddVariable("sX", ByteSize.Float)
LerpingRotation:AddVariable("sY", ByteSize.Float)
LerpingRotation:AddVariable("sZ", ByteSize.Float)
LerpingRotation:AddVariable("tX", ByteSize.Float)
LerpingRotation:AddVariable("tY", ByteSize.Float)
LerpingRotation:AddVariable("tZ", ByteSize.Float)
LerpingRotation:AddVariable("KillWhenFinished", ByteSize.Bool)
worldCreator:AddComponentType(LerpingRotation)

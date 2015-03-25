--	This file will contain
--	components that are needed
--	for map components

-- Create Map Component
local NewComponent = ComponentType()
NewComponent.Name = "CreateMap"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("MapName", ByteSize.Text)
worldCreator:AddComponentType(NewComponent)

-- Map Position Component
NewComponent = ComponentType()
NewComponent.Name = "MapPosition"
NewComponent.TableType = TableType.Map
NewComponent.SyncNetwork = true
NewComponent:AddVariable("X", ByteSize.Int)
NewComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Map Position Component
NewComponent = ComponentType()
NewComponent.Name = "TileOffset"
NewComponent.TableType = TableType.Map
NewComponent.SyncNetwork = true
NewComponent:AddVariable("Offset", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)

-- Water Component
NewComponent = ComponentType()
NewComponent.Name = "River"
NewComponent.TableType = TableType.Map
NewComponent.SyncNetwork = true
NewComponent:AddVariable("DirX", ByteSize.Int)
NewComponent:AddVariable("DirZ", ByteSize.Int)
NewComponent:AddVariable("Speed", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Checkpoint Component
NewComponent = ComponentType()
NewComponent.Name = "Checkpoint"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Number", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- CheckpointId Component
NewComponent = ComponentType()
NewComponent.Name = "CheckpointId"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Id", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Tile Component
NewComponent = ComponentType()
NewComponent.Name = "TileComp"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Finishpoint Component
NewComponent = ComponentType()
NewComponent.Name = "Finishpoint"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Edge?
-- Hole and Out Component
NewComponent = ComponentType()
NewComponent.Name = "Void"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Not Walkable Component
NewComponent = ComponentType()
NewComponent.Name = "NotWalkable"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--River end component
NewComponent = ComponentType()
NewComponent.Name = "RiverEnd"
NewComponent.TableType = TableType.Map
NewComponent.SyncNetwork = true
worldCreator:AddComponentType(NewComponent)

-- Available Spawnpoint
NewComponent = ComponentType()
NewComponent.Name = "AvailableSpawnpoint"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("X", ByteSize.Int)
NewComponent:AddVariable("Z", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

-- Map Size Component, added to exactly entity to signal how big the map is.
NewComponent = ComponentType()
NewComponent.Name = "MapSpecs"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("NoOfCheckpoints", ByteSize.Int)
NewComponent:AddVariable("NoOfSpawnpoints", ByteSize.Int)
NewComponent:AddVariable("SizeX", ByteSize.Int)
NewComponent:AddVariable("SizeY", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	GenerateMap
NewComponent = ComponentType()
NewComponent.Name = "GenerateMap"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	RightCorner
NewComponent = ComponentType()
NewComponent.Name = "RiverCornerDir"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Dir", ByteSize.Text)
worldCreator:AddComponentType(NewComponent)

-- Tile Walkability Has Changed
NewComponent = ComponentType()
NewComponent.Name = "TileWalkabilityHasChanged"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Time", ByteSize.Int)
NewComponent:AddVariable("X", ByteSize.Int)
NewComponent:AddVariable("Y", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

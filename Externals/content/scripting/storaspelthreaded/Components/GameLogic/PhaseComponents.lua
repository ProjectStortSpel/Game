--	This file will contain
--	components that are needed
--	for different phases

-- PickingPhase Component
local NewComponent = ComponentType()
NewComponent.Name = "PickingPhase"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Time", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)

-- OnPickingPhase Component
NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "OnPickingPhase"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Auto Pick Cards
NewComponent = ComponentType()
NewComponent.Name = "AutoPickCards"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Game Running
NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "GameRunning"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Create Deck
NewComponent = ComponentType()
NewComponent.Name = "CreateDeck"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Deal Cards
NewComponent = ComponentType()
NewComponent.Name = "DealCards"
NewComponent:AddVariable("NumCards", ByteSize.Int)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Notify Start New Round
NewComponent = ComponentType()
NewComponent.Name = "NotifyStartNewRound"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("IsAI", ByteSize.Bool)
worldCreator:AddComponentType(NewComponent)

--	New Round
NewComponent = ComponentType()
NewComponent.Name = "NewRound"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	New Step
NewComponent = ComponentType()
NewComponent.Name = "NewStep"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Play Cards
NewComponent = ComponentType()
NewComponent.Name = "PlayCard"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Step", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	Post Move
NewComponent = ComponentType()
NewComponent.Name = "PostMove"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Move River
NewComponent = ComponentType()
NewComponent.Name = "MoveRiver"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Respawn Units
NewComponent = ComponentType()
NewComponent.Name = "RespawnUnits"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Check Void
NewComponent = ComponentType()
NewComponent.Name = "CheckVoid"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Check Checkpoint
NewComponent = ComponentType()
NewComponent.Name = "CheckCheckpoint"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	CheckCheckpointForEntity
NewComponent = ComponentType()
NewComponent.Name = "CheckCheckpointForEntity"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("EntityId", ByteSize.Reference)
NewComponent:AddVariable("PosX", ByteSize.Int)
NewComponent:AddVariable("PosZ", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	Check Finish point
NewComponent = ComponentType()
NewComponent.Name = "CheckFinishpoint"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	PostSpawn check
NewComponent = ComponentType()
NewComponent.Name = "PostSpawn"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)
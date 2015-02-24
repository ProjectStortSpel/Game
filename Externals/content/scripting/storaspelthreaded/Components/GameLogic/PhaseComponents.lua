--	This file will contain
--	components that are needed
--	for different phases

-- PickingPhase Component
local PickingPhaseComponent = ComponentType()
PickingPhaseComponent.Name = "PickingPhase"
PickingPhaseComponent.TableType = TableType.Map
PickingPhaseComponent:AddVariable("Time", ByteSize.Float)
worldCreator:AddComponentType(PickingPhaseComponent)

-- OnPickingPhase Component
local OnPickingPhaseComponent = ComponentType()
OnPickingPhaseComponent.SyncNetwork = true
OnPickingPhaseComponent.Name = "OnPickingPhase"
OnPickingPhaseComponent.TableType = TableType.Map
worldCreator:AddComponentType(OnPickingPhaseComponent)

--	Auto Pick Cards
local AutoPickCardsComponent = ComponentType()
AutoPickCardsComponent.Name = "AutoPickCards"
AutoPickCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(AutoPickCardsComponent)

--	Game Running
local GameRunningComponent = ComponentType()
GameRunningComponent.SyncNetwork = true
GameRunningComponent.Name = "GameRunning"
GameRunningComponent.TableType = TableType.Map
worldCreator:AddComponentType(GameRunningComponent)

--	Create Deck
local CreateDeckComponent = ComponentType()
CreateDeckComponent.Name = "CreateDeck"
CreateDeckComponent.TableType = TableType.Map
worldCreator:AddComponentType(CreateDeckComponent)

--	Deal Cards
local DealCardsComponent = ComponentType()
DealCardsComponent.Name = "DealCards"
DealCardsComponent:AddVariable("NumCards", ByteSize.Int)
DealCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(DealCardsComponent)

--	Notify Start New Round
local NotifyStartNewRoundComponent = ComponentType()
NotifyStartNewRoundComponent.Name = "NotifyStartNewRound"
NotifyStartNewRoundComponent.TableType = TableType.Map
NotifyStartNewRoundComponent:AddVariable("IsAI", ByteSize.Bool)
worldCreator:AddComponentType(NotifyStartNewRoundComponent)

--	New Round
local NewRoundComponent = ComponentType()
NewRoundComponent.Name = "NewRound"
NewRoundComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewRoundComponent)

--	New Step
local NewStepComponent = ComponentType()
NewStepComponent.Name = "NewStep"
NewStepComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewStepComponent)

--	Play Cards
local PlayCardsComponent = ComponentType()
PlayCardsComponent.Name = "PlayCard"
PlayCardsComponent.TableType = TableType.Map
PlayCardsComponent:AddVariable("Step", ByteSize.Int)
worldCreator:AddComponentType(PlayCardsComponent)

--	Post Move
local PostMoveComponent = ComponentType()
PostMoveComponent.Name = "PostMove"
PostMoveComponent.TableType = TableType.Map
worldCreator:AddComponentType(PostMoveComponent)

--	Move River
local MoveRiverComponent = ComponentType()
MoveRiverComponent.Name = "MoveRiver"
MoveRiverComponent.TableType = TableType.Map
worldCreator:AddComponentType(MoveRiverComponent)

--	Respawn Units
local RespawnUnitsComponent = ComponentType()
RespawnUnitsComponent.Name = "RespawnUnits"
RespawnUnitsComponent.TableType = TableType.Map
worldCreator:AddComponentType(RespawnUnitsComponent)

--	Check Void
local CheckVoidComponent = ComponentType()
CheckVoidComponent.Name = "CheckVoid"
CheckVoidComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckVoidComponent)

--	Check Checkpoint
local CheckCheckpointComponent = ComponentType()
CheckCheckpointComponent.Name = "CheckCheckpoint"
CheckCheckpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckCheckpointComponent)

--	CheckCheckpointForEntity
local CheckCheckpointForEntity = ComponentType()
CheckCheckpointForEntity.Name = "CheckCheckpointForEntity"
CheckCheckpointForEntity.TableType = TableType.Map
CheckCheckpointForEntity:AddVariable("EntityId", ByteSize.Reference)
CheckCheckpointForEntity:AddVariable("PosX", ByteSize.Int)
CheckCheckpointForEntity:AddVariable("PosZ", ByteSize.Int)
worldCreator:AddComponentType(CheckCheckpointForEntity)

--	Check Finish point
local CheckFinishpointComponent = ComponentType()
CheckFinishpointComponent.Name = "CheckFinishpoint"
CheckFinishpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckFinishpointComponent)

--	PostSpawn check
local PostSpawnComponent = ComponentType()
PostSpawnComponent.Name = "PostSpawn"
PostSpawnComponent.TableType = TableType.Map
worldCreator:AddComponentType(PostSpawnComponent)
local GameRunningComponent = ComponentType()
GameRunningComponent.SyncNetwork = false
GameRunningComponent.Name = "GameRunning"
GameRunningComponent.TableType = TableType.Map
worldCreator:AddComponentType(GameRunningComponent)

local CreateDeckComponent = ComponentType()
CreateDeckComponent.SyncNetwork = false
CreateDeckComponent.Name = "CreateDeck"
CreateDeckComponent.TableType = TableType.Map
worldCreator:AddComponentType(CreateDeckComponent)

local DealCardsComponent = ComponentType()
DealCardsComponent.SyncNetwork = false
DealCardsComponent.Name = "DealCards"
DealCardsComponent:AddVariable("NumCards", ByteSize.Int)
DealCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(DealCardsComponent)


local NotifyStartNewRoundComponent = ComponentType()
NotifyStartNewRoundComponent.Name = "NotifyStartNewRound"
NotifyStartNewRoundComponent.TableType = TableType.Map
worldCreator:AddComponentType(NotifyStartNewRoundComponent)

local NewRoundComponent = ComponentType()
NewRoundComponent.Name = "NewRound"
NewRoundComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewRoundComponent)

local NewStepComponent = ComponentType()
NewStepComponent.Name = "NewStep"
NewStepComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewStepComponent)

local PlayCardsComponent = ComponentType()
PlayCardsComponent.Name = "PlayCard"
PlayCardsComponent.TableType = TableType.Map
PlayCardsComponent:AddVariable("Step", ByteSize.Int)
worldCreator:AddComponentType(PlayCardsComponent)

local PostMoveComponent = ComponentType()
PostMoveComponent.Name = "PostMove"
PostMoveComponent.TableType = TableType.Map
worldCreator:AddComponentType(PostMoveComponent)

local MoveRiverComponent = ComponentType()
MoveRiverComponent.Name = "MoveRiver"
MoveRiverComponent.TableType = TableType.Map
worldCreator:AddComponentType(MoveRiverComponent)

local RespawnUnitsComponent = ComponentType()
RespawnUnitsComponent.Name = "RespawnUnits"
RespawnUnitsComponent.TableType = TableType.Map
worldCreator:AddComponentType(RespawnUnitsComponent)

local CheckVoidComponent = ComponentType()
CheckVoidComponent.Name = "CheckVoid"
CheckVoidComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckVoidComponent)

local TestMoveComponent = ComponentType()
TestMoveComponent.Name = "TestMove"
TestMoveComponent.TableType = TableType.Map
TestMoveComponent:AddVariable("Unit", ByteSize.Reference)
TestMoveComponent:AddVariable("PosX", ByteSize.Int)
TestMoveComponent:AddVariable("PosZ", ByteSize.Int)
TestMoveComponent:AddVariable("DirX", ByteSize.Int)
TestMoveComponent:AddVariable("DirZ", ByteSize.Int)
worldCreator:AddComponentType(TestMoveComponent)


local CheckCheckpointComponent = ComponentType()
CheckCheckpointComponent.Name = "CheckCheckpoint"
CheckCheckpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckCheckpointComponent)

local UpdateSpawnpointComponent = ComponentType()
UpdateSpawnpointComponent.Name = "UpdateSpawnpoint"
UpdateSpawnpointComponent:AddVariable("Unit", ByteSize.Reference)
UpdateSpawnpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(UpdateSpawnpointComponent)

local StepTimerComponent = ComponentType()
StepTimerComponent.Name = "StepTimer"
StepTimerComponent:AddVariable("Time", ByteSize.Float)
StepTimerComponent.TableType = TableType.Map
worldCreator:AddComponentType(StepTimerComponent)

local PlayCardTimerComponent = ComponentType()
PlayCardTimerComponent.Name = "PlayCardTimer"
PlayCardTimerComponent:AddVariable("Time", ByteSize.Float)
PlayCardTimerComponent:AddVariable("Step", ByteSize.Int)
PlayCardTimerComponent.TableType = TableType.Map
worldCreator:AddComponentType(PlayCardTimerComponent)

local EndRoundComponent = ComponentType()
EndRoundComponent.Name = "EndRound"
EndRoundComponent.TableType = TableType.Map
worldCreator:AddComponentType(EndRoundComponent)
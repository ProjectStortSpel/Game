local GameRunningComponent = ComponentType()
GameRunningComponent.Name = "GameRunning"
GameRunningComponent.TableType = TableType.Map
worldCreator:AddComponentType(GameRunningComponent)

local CreateDeckComponent = ComponentType()
CreateDeckComponent.Name = "CreateDeck"
CreateDeckComponent.TableType = TableType.Map
worldCreator:AddComponentType(CreateDeckComponent)

-- Added to exactly one entity to notify start dealing cards to all players.
local DealCardsComponent = ComponentType()
DealCardsComponent.Name = "DealCards"
DealCardsComponent:AddVariable("NumCards", ByteSize.Int)
DealCardsComponent.TableType = TableType.Map
worldCreator:AddComponentType(DealCardsComponent)

local NotifyStartNewRoundComponent = ComponentType()
NotifyStartNewRoundComponent.Name = "NotifyStartNewRound"
NotifyStartNewRoundComponent.TableType = TableType.Map
NotifyStartNewRoundComponent:AddVariable("IsAI", ByteSize.Bool)
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
TestMoveComponent:AddVariable("Steps", ByteSize.Int)
worldCreator:AddComponentType(TestMoveComponent)

local CheckCheckpointComponent = ComponentType()
CheckCheckpointComponent.Name = "CheckCheckpoint"
CheckCheckpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckCheckpointComponent)

local CheckFinishpointComponent = ComponentType()
CheckFinishpointComponent.Name = "CheckFinishpoint"
CheckFinishpointComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckFinishpointComponent)

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

local TestRiverComponent = ComponentType()
TestRiverComponent.Name = "TestRiver"
TestRiverComponent.TableType = TableType.Map
TestRiverComponent:AddVariable("Unit", ByteSize.Reference)
TestRiverComponent:AddVariable("PosX", ByteSize.Int)
TestRiverComponent:AddVariable("PosZ", ByteSize.Int)
worldCreator:AddComponentType(TestRiverComponent)

local TakeCardsFromPlayerComponent = ComponentType()
TakeCardsFromPlayerComponent.Name = "TakeCardsFromPlayer"
TakeCardsFromPlayerComponent.TableType = TableType.Map
TakeCardsFromPlayerComponent:AddVariable("Player", ByteSize.Reference)
worldCreator:AddComponentType(TakeCardsFromPlayerComponent)

local TakeCardStepsFromUnitComponent = ComponentType()
TakeCardStepsFromUnitComponent.Name = "TakeCardStepsFromUnit"
TakeCardStepsFromUnitComponent.TableType = TableType.Map
TakeCardStepsFromUnitComponent:AddVariable("Unit", ByteSize.Reference)
worldCreator:AddComponentType(TakeCardStepsFromUnitComponent)

local AddTotemPieceComponent = ComponentType()
AddTotemPieceComponent.Name = "AddTotemPiece"
AddTotemPieceComponent.TableType = TableType.Map
worldCreator:AddComponentType(AddTotemPieceComponent)

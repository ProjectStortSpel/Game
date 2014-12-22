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
NewRoundComponent.TableType = TableType.Array
worldCreator:AddComponentType(NewRoundComponent)

local NewStepComponent = ComponentType()
NewStepComponent.Name = "NewStep"
NewStepComponent.TableType = TableType.Array
worldCreator:AddComponentType(NewStepComponent)

local PlayCardsComponent = ComponentType()
PlayCardsComponent.Name = "PlayCards"
PlayCardsComponent.TableType = TableType.Array
PlayCardsComponent:AddVariable("Step", ByteSize.Int)
worldCreator:AddComponentType(PlayCardsComponent)

local MoveRiverComponent = ComponentType()
MoveRiverComponent.Name = "MoveRiver"
MoveRiverComponent.TableType = TableType.Array
worldCreator:AddComponentType(MoveRiverComponent)

local StepTimerComponent = ComponentType()
StepTimerComponent.Name = "StepTimer"
StepTimerComponent:AddVariable("Time", ByteSize.Float)
StepTimerComponent.TableType = TableType.Map
worldCreator:AddComponentType(StepTimerComponent)

local EndRoundComponent = ComponentType()
EndRoundComponent.Name = "EndRound"
EndRoundComponent.TableType = TableType.Map
worldCreator:AddComponentType(EndRoundComponent)
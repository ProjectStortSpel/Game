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


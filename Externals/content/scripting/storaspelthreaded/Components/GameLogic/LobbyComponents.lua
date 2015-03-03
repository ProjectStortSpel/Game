--	LobbyMenu Component
local NewComponent = ComponentType()
NewComponent.Name = "LobbyMenu"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyMenuElement Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyMenuElement"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyMenuPlayer Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyMenuPlayer"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyPlayerReady Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyPlayerReady"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyPlayerStart Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyPlayerStart"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyPlayerReadyMSG Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyPlayerReadyMSG"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyPlayerStartMSG Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyPlayerStartMSG"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	LobbyMenuActive Component
NewComponent = ComponentType()
NewComponent.Name = "LobbyMenuActive"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)
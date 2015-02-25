--	LobbyMenu Component
local LobbyMenu = ComponentType()
LobbyMenu.Name = "LobbyMenu"
LobbyMenu.TableType = TableType.Map
worldCreator:AddComponentType(LobbyMenu)

--	LobbyMenuElement Component
local LobbyMenuElement = ComponentType()
LobbyMenuElement.Name = "LobbyMenuElement"
LobbyMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(LobbyMenuElement)

--	LobbyMenuPlayer Component
local LobbyMenuPlayer = ComponentType()
LobbyMenuPlayer.Name = "LobbyMenuPlayer"
LobbyMenuPlayer.TableType = TableType.Map
worldCreator:AddComponentType(LobbyMenuPlayer)

--	LobbyPlayerReady Component
local LobbyPlayerReady = ComponentType()
LobbyPlayerReady.Name = "LobbyPlayerReady"
LobbyPlayerReady.TableType = TableType.Map
worldCreator:AddComponentType(LobbyPlayerReady)

--	LobbyPlayerStart Component
local LobbyPlayerStart = ComponentType()
LobbyPlayerStart.Name = "LobbyPlayerStart"
LobbyPlayerStart.TableType = TableType.Map
worldCreator:AddComponentType(LobbyPlayerStart)

--	LobbyPlayerReadyMSG Component
local LobbyPlayerReadyMSG = ComponentType()
LobbyPlayerReadyMSG.Name = "LobbyPlayerReadyMSG"
LobbyPlayerReadyMSG.TableType = TableType.Map
worldCreator:AddComponentType(LobbyPlayerReadyMSG)

--	LobbyPlayerStartMSG Component
local LobbyPlayerStartMSG = ComponentType()
LobbyPlayerStartMSG.Name = "LobbyPlayerStartMSG"
LobbyPlayerStartMSG.TableType = TableType.Map
worldCreator:AddComponentType(LobbyPlayerStartMSG)

--	LobbyMenuActive Component
local LobbyMenuActive = ComponentType()
LobbyMenuActive.Name = "LobbyMenuActive"
LobbyMenuActive.SyncNetwork = true
LobbyMenuActive.TableType = TableType.Map
worldCreator:AddComponentType(LobbyMenuActive)
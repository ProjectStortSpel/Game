--	This file will contain
--	components that are needed
--	for menus

--  GameInterfaceElement Component
local GameInterfaceElement = ComponentType()
GameInterfaceElement.Name = "GameInterfaceElement"
GameInterfaceElement.TableType = TableType.Map
worldCreator:AddComponentType(GameInterfaceElement)

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

--	GameMenu Component
local GameMenu = ComponentType()
GameMenu.Name = "GameMenu"
GameMenu.TableType = TableType.Map
worldCreator:AddComponentType(GameMenu)

--	GameMenuElement Component
local GameMenuElement = ComponentType()
GameMenuElement.Name = "GameMenuElement"
GameMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(GameMenuElement)

--	OptionMenu Component
local OptionMenu = ComponentType()
OptionMenu.Name = "OptionMenu"
OptionMenu.TableType = TableType.Map
worldCreator:AddComponentType(OptionMenu)

--	OptionMenuElement Component
local OptionMenuElement = ComponentType()
OptionMenuElement.Name = "OptionMenuElement"
OptionMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(OptionMenuElement)

--	RconMenu Component
local RconMenu = ComponentType()
RconMenu.Name = "RconMenu"
RconMenu.TableType = TableType.Map
worldCreator:AddComponentType(RconMenu)

--	RconMenuElement Component
local RconMenuElement = ComponentType()
RconMenuElement.Name = "RconMenuElement"
RconMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(RconMenuElement)

-- Menu ConsoleCommand
local MenuConsoleCommand = ComponentType()
MenuConsoleCommand.Name = "MenuConsoleCommand"
MenuConsoleCommand.TableType = TableType.Map
MenuConsoleCommand:AddVariable("Command", ByteSize.Text)
worldCreator:AddComponentType(MenuConsoleCommand)

-- Menu EntityCommand
local MenuEntityCommand = ComponentType()
MenuEntityCommand.Name = "MenuEntityCommand"
MenuEntityCommand.TableType = TableType.Map
MenuEntityCommand:AddVariable("ComponentName", ByteSize.Text)
worldCreator:AddComponentType(MenuEntityCommand)
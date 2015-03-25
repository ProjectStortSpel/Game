--	This file will contain
--	components that are needed
--	for menus

--  GameInterfaceElement Component
local GameInterfaceElement = ComponentType()
GameInterfaceElement.Name = "GameInterfaceElement"
GameInterfaceElement.TableType = TableType.Map
worldCreator:AddComponentType(GameInterfaceElement)

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

-- Menu EntityCommand
local MenuSlider = ComponentType()
MenuSlider.Name = "MenuSlider"
MenuSlider.TableType = TableType.Map
MenuSlider:AddVariable("ConsoleCommand", ByteSize.Text)
MenuSlider:AddVariable("Min", ByteSize.Float)
MenuSlider:AddVariable("Max", ByteSize.Float)
MenuSlider:AddVariable("Current", ByteSize.Float)
MenuSlider:AddVariable("Step", ByteSize.Float)
worldCreator:AddComponentType(MenuSlider)

-- Menu EntityCommand
local MenuSliderButton = ComponentType()
MenuSliderButton.Name = "MenuSliderButton"
MenuSliderButton.TableType = TableType.Map
MenuSliderButton:AddVariable("ButtonId", ByteSize.Int)
worldCreator:AddComponentType(MenuSliderButton)

-- NotificationPopup
local NotificationPopup = ComponentType()
NotificationPopup.Name = "NotificationPopup"
NotificationPopup.TableType = TableType.Map
NotificationPopup.SyncNetwork = true
NotificationPopup:AddVariable("Text", ByteSize.Text)
worldCreator:AddComponentType(NotificationPopup)
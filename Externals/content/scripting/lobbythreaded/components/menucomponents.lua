--	MainMenu Component
local MainMenu = ComponentType()
MainMenu.Name = "MainMenu"
MainMenu.TableType = TableType.Map
worldCreator:AddComponentType(MainMenu)
--	MainMenuElement Component
local MainMenuElement = ComponentType()
MainMenuElement.Name = "MainMenuElement"
MainMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(MainMenuElement)

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

--	OptionMenuActivate Component
local OptionMenuActivate = ComponentType()
OptionMenuActivate.Name = "OptionMenuActivate"
OptionMenuActivate.TableType = TableType.Map
worldCreator:AddComponentType(OptionMenuActivate)

--	ConnectMenu Component
local ConnectMenu = ComponentType()
ConnectMenu.Name = "ConnectMenu"
ConnectMenu.TableType = TableType.Map
worldCreator:AddComponentType(ConnectMenu)
--	ConnectMenuElement Component
local ConnectMenuElement = ComponentType()
ConnectMenuElement.Name = "ConnectMenuElement"
ConnectMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(ConnectMenuElement)

--	HostMenu Component
local HostMenu = ComponentType()
HostMenu.Name = "HostMenu"
HostMenu.TableType = TableType.Map
worldCreator:AddComponentType(HostMenu)
--	HostMenuElement Component
local HostMenuElement = ComponentType()
HostMenuElement.Name = "HostMenuElement"
HostMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(HostMenuElement)

--	HostMenuActivate Component
local HostMenuActivate = ComponentType()
HostMenuActivate.Name = "HostMenuActivate"
HostMenuActivate.TableType = TableType.Map
worldCreator:AddComponentType(HostMenuActivate)

--	HowToMenu Component
local HowToMenu = ComponentType()
HowToMenu.Name = "HowToMenu"
HowToMenu.TableType = TableType.Map
worldCreator:AddComponentType(HowToMenu)
local HowToMenuElement = ComponentType()
HowToMenuElement.Name = "HowToMenuElement"
HowToMenuElement.TableType = TableType.Map
worldCreator:AddComponentType(HowToMenuElement)

-- Menu ConsoleCommand
local MenuConsoleCommand = ComponentType()
MenuConsoleCommand.Name = "MenuConsoleCommand"
MenuConsoleCommand.TableType = TableType.Map
MenuConsoleCommand:AddVariable("Command", ByteSize.String)
worldCreator:AddComponentType(MenuConsoleCommand)
-- Menu EntityCommand
local MenuEntityCommand = ComponentType()
MenuEntityCommand.Name = "MenuEntityCommand"
MenuEntityCommand.TableType = TableType.Map
MenuEntityCommand:AddVariable("ComponentName", ByteSize.Text)
worldCreator:AddComponentType(MenuEntityCommand)

-- Hover Size
local HoverSize = ComponentType()
HoverSize.Name = "HoverSize"
HoverSize.TableType = TableType.Map
HoverSize:AddVariable("X", ByteSize.Float)
HoverSize:AddVariable("Y", ByteSize.Float)
HoverSize:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(HoverSize)

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
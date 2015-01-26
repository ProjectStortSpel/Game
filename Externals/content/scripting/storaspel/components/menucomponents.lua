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

-- Hover Size
local HoverSize = ComponentType()
HoverSize.Name = "HoverSize"
HoverSize.TableType = TableType.Map
HoverSize:AddVariable("X", ByteSize.Float)
HoverSize:AddVariable("Y", ByteSize.Float)
HoverSize:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(HoverSize)
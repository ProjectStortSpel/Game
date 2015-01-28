--	This file will contain
--	components that are needed
--	for buttons

--	ButtonCommand Component
local ButtonCommandComponent = ComponentType()
ButtonCommandComponent.Name = "ButtonCommand"
ButtonCommandComponent.TableType = TableType.Map
ButtonCommandComponent:AddVariable("Command", ByteSize.Text)
ButtonCommandComponent:AddVariable("Button", ByteSize.Reference)
ButtonCommandComponent:AddVariable("Index", ByteSize.Int)
worldCreator:AddComponentType(ButtonCommandComponent)

--	Button Component
local ButtonComponent = ComponentType()
ButtonComponent.Name = "Button"
ButtonComponent.TableType = TableType.Map
ButtonComponent:AddVariable("NumCommands", ByteSize.Int)
worldCreator:AddComponentType(ButtonComponent)

-- Hover Size
local HoverSize = ComponentType()
HoverSize.Name = "HoverSize"
HoverSize.TableType = TableType.Map
HoverSize:AddVariable("X", ByteSize.Float)
HoverSize:AddVariable("Y", ByteSize.Float)
HoverSize:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(HoverSize)
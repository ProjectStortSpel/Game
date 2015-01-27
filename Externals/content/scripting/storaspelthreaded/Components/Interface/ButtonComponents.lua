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

-- PickBox Component
local PickBox = ComponentType()
PickBox.Name = "PickBox"
PickBox.TableType = TableType.Map
PickBox:AddVariable("SizeX", ByteSize.Float)
PickBox:AddVariable("SizeY", ByteSize.Float)
worldCreator:AddComponentType(PickBox)

-- OnPickBoxHit
local OnPickBoxHit = ComponentType()
OnPickBoxHit.Name = "OnPickBoxHit"
OnPickBoxHit.TableType = TableType.Map
worldCreator:AddComponentType(OnPickBoxHit)

-- ReadyButton
local ReadyButton = ComponentType()
ReadyButton.Name = "ReadyButton"
ReadyButton.TableType = TableType.Map
worldCreator:AddComponentType(ReadyButton)

--	InterfaceElement Component
local InterfaceElement = ComponentType()
InterfaceElement.Name = "InterfaceElement"
InterfaceElement.TableType = TableType.Map
worldCreator:AddComponentType(InterfaceElement)
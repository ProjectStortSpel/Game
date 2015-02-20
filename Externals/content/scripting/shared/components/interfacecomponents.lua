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

--	PickingPhaseTimer Component
local PickingTimer = ComponentType()
PickingTimer.Name = "PickingTimer"
PickingTimer.TableType = TableType.Map
worldCreator:AddComponentType(PickingTimer)

--	PickingPhaseTimer Component
local SetPickingTimer = ComponentType()
SetPickingTimer.Name = "SetPickingTimer"
SetPickingTimer:AddVariable("Time", ByteSize.Float)
SetPickingTimer.TableType = TableType.Map
worldCreator:AddComponentType(SetPickingTimer)

-- Checkbox Component
local CheckboxComponent = ComponentType()
CheckboxComponent.Name = "Checkbox"
CheckboxComponent:AddVariable("Checked", ByteSize.Bool)
CheckboxComponent.TableType = TableType.Map
worldCreator:AddComponentType(CheckboxComponent)

--	NotificationBox Component
local NotificationBox = ComponentType()
NotificationBox.Name = "NotificationBox"
NotificationBox.TableType = TableType.Map
NotificationBox:AddVariable("Text", ByteSize.Text)
NotificationBox:AddVariable("ParentMenu", ByteSize.Text)
worldCreator:AddComponentType(NotificationBox)

--	NotificationBoxElement Component
local NotificationBoxElement = ComponentType()
NotificationBoxElement.Name = "NotificationBoxElement"
NotificationBoxElement.TableType = TableType.Map
worldCreator:AddComponentType(NotificationBoxElement)
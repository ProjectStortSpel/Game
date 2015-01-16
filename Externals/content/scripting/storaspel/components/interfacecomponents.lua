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
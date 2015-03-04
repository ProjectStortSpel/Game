-- Slerp Rotation Component
local NewComponent = ComponentType()
NewComponent.Name = "SlerpRotation"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("fromX", ByteSize.Float)
NewComponent:AddVariable("fromY", ByteSize.Float)
NewComponent:AddVariable("fromZ", ByteSize.Float)
NewComponent:AddVariable("fromW", ByteSize.Float)
NewComponent:AddVariable("time", ByteSize.Float)
NewComponent:AddVariable("toX", ByteSize.Float)
NewComponent:AddVariable("toY", ByteSize.Float)
NewComponent:AddVariable("toZ", ByteSize.Float)
NewComponent:AddVariable("toW", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)
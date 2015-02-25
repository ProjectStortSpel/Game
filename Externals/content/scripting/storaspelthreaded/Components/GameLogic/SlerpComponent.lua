-- Slerp Rotation Component
local SlerpRotationComponent = ComponentType()
SlerpRotationComponent.Name = "SlerpRotation"
SlerpRotationComponent.SyncNetwork = true
SlerpRotationComponent.TableType = TableType.Map
SlerpRotationComponent:AddVariable("fromX", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromY", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromZ", ByteSize.Float)
SlerpRotationComponent:AddVariable("fromW", ByteSize.Float)
SlerpRotationComponent:AddVariable("time", ByteSize.Float)
SlerpRotationComponent:AddVariable("toX", ByteSize.Float)
SlerpRotationComponent:AddVariable("toY", ByteSize.Float)
SlerpRotationComponent:AddVariable("toZ", ByteSize.Float)
SlerpRotationComponent:AddVariable("toW", ByteSize.Float)
worldCreator:AddComponentType(SlerpRotationComponent)
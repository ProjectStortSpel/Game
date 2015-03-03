--	This file will contain
--	components that are needed
--	for timers

-- Picking Timer
local NewComponent = ComponentType()
NewComponent.Name = "PickingPhaseTimer"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Timer", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)

--	Set Picking Phase Timer
NewComponent = ComponentType()
NewComponent.Name = "SetPickingPhaseTimer"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Amount", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)

--	Add To Picking Phase Timer
NewComponent = ComponentType()
NewComponent.Name = "AddToPickingPhaseTimer"
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Amount", ByteSize.Float)
worldCreator:AddComponentType(NewComponent)
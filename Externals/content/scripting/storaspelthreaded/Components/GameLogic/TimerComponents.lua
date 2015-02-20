--	This file will contain
--	components that are needed
--	for timers

-- Picking Timer
local PickingPhaseTimerComponent = ComponentType()
PickingPhaseTimerComponent.Name = "PickingPhaseTimer"
PickingPhaseTimerComponent.TableType = TableType.Map
PickingPhaseTimerComponent:AddVariable("Timer", ByteSize.Float)
worldCreator:AddComponentType(PickingPhaseTimerComponent)

--	Set Picking Phase Timer
local SetPickingPhaserComponent = ComponentType()
SetPickingPhaserComponent.Name = "SetPickingPhaseTimer"
SetPickingPhaserComponent.TableType = TableType.Map
SetPickingPhaserComponent:AddVariable("Amount", ByteSize.Float)
worldCreator:AddComponentType(SetPickingPhaserComponent)

--	Add To Picking Phase Timer
local AddToPickingPhaseTimerComponent = ComponentType()
AddToPickingPhaseTimerComponent.Name = "AddToPickingPhaseTimer"
AddToPickingPhaseTimerComponent.TableType = TableType.Map
AddToPickingPhaseTimerComponent:AddVariable("Amount", ByteSize.Float)
worldCreator:AddComponentType(AddToPickingPhaseTimerComponent)
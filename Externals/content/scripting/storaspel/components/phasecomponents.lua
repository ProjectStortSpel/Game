-- PickingPhase Component
local PickingPhaseComponent = ComponentType()
PickingPhaseComponent.Name = "PickingPhase"
PickingPhaseComponent.TableType = TableType.Map
PickingPhaseComponent:AddVariable("Time", ByteSize.Float)
worldCreator:AddComponentType(PickingPhaseComponent)
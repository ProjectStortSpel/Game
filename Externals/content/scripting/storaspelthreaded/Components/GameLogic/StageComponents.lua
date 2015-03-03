--	This file will contain
--	components that are needed
--	for different game stages

-- GameStage Component
local NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "GameStage"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- PickingStage Component
NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "PickingStage"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- MoveStage Component
NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "MoveStage"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- PreStartStage Component
NewComponent = ComponentType()
NewComponent.SyncNetwork = true
NewComponent.Name = "JoiningStage"
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

-- Stage Switch
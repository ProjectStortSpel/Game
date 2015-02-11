--	This file will contain
--	components that are needed
--	for different game stages

-- GameStage Component
local GameStageComponent = ComponentType()
GameStageComponent.SyncNetwork = true
GameStageComponent.Name = "GameStage"
GameStageComponent.TableType = TableType.Map
worldCreator:AddComponentType(GameStageComponent)

-- PickingStage Component
local PickingStageComponent = ComponentType()
PickingStageComponent.SyncNetwork = true
PickingStageComponent.Name = "PickingStage"
PickingStageComponent.TableType = TableType.Map
worldCreator:AddComponentType(PickingStageComponent)

-- MoveStage Component
local MoveStageComponent = ComponentType()
MoveStageComponent.SyncNetwork = true
MoveStageComponent.Name = "MoveStage"
MoveStageComponent.TableType = TableType.Map
worldCreator:AddComponentType(MoveStageComponent)

-- PreStartStage Component
local JoiningStageComponent = ComponentType()
JoiningStageComponent.SyncNetwork = true
JoiningStageComponent.Name = "JoiningStage"
JoiningStageComponent.TableType = TableType.Map
worldCreator:AddComponentType(JoiningStageComponent)

-- Stage Switch
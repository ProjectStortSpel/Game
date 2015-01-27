
--	Map Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/MapSystems/?.lua"
require "sv_CreateMapSystem"
require "sv_CheckpointSystem"
require "sv_RiverSystem"
require "sv_VoidSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CreateMapSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CheckpointSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(RiverSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(VoidSystem)

--	Light Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/LightSystems/?.lua"
require "sv_DirectionalLightSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(DirectionalLightSystem)

--	Move Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/MoveSystems/?.lua"
require "sv_PostMoveSystem"
require "sv_TestMoveSystem"
require "sv_TestMoveRiverSystem"

require "sv_MoveForwardSystem"
require "sv_MoveBackwardSystem"
require "sv_TurnAroundSystem"
require "sv_TurnLeftSystem"
require "sv_TurnRightSystem"


worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PostMoveSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TestMoveSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TestMoveRiverSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MoveForwardSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(MoveBackwardSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TurnAroundSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TurnLeftSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TurnRightSystem)

--	Card Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/CardSystems/?.lua"
require "sv_CreateDeckSystem"
require "sv_AddCardPickTimerSystem"
require "sv_SetCardPickTimerSystem"
require "sv_UpdateCardPickTimerSystem"
require "sv_AutoPickCardsSystem"

require "sv_TakeCardStepFromUnitSystem"
require "sv_TakeCardsFromPlayerSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(CreateDeckSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddCardPickTimer)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SetCardPickTimer)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(UpdateCardPickTimer)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AutoPickCards)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TakeCardStepsFromUnitSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TakeCardsFromPlayerSystem)

--	Card Action Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/CardActionSystems/?.lua"
require "sv_ActionMoveForwardSystem"
require "sv_ActionMoveBackwardSystem"
require "sv_ActionTurnLeftSystem"
require "sv_ActionTurnRightSystem"
require "sv_ActionTurnAroundSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ActionMoveForwardSystem)
worldCreator:AddSystemToCurrentGroup(ActionMoveBackwardSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnLeftSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnRightSystem)
worldCreator:AddSystemToCurrentGroup(ActionTurnAroundSystem)

--	Spawn Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/SpawnSystems/?.lua"
require "sv_SpawnSystem"
require "sv_RespawnSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(SpawnSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(RespawnSystem)

--	Round Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Server/RoundSystems/?.lua"
require "sv_StepTimerSystem"
require "sv_NewStepSystem"
require "sv_StartNewRoundSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StartNewRoundSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StepTimerSystem)
worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(NewStepSystem)


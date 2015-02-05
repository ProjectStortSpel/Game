--	Lerping Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Shared/LerpSystems/?.lua"
require "sh_MovePlayerSystem"
require "sh_AddLerpPositionSystem"
require "sh_UpdateLerpPositionSystem"
require "sh_AddLerpScaleSystem"
require "sh_UpdateLerpScaleSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TrueTestMoveSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddLerpPositionSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(UpdateLerpPositionSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddLerpScaleSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(UpdateLerpScaleSystem)


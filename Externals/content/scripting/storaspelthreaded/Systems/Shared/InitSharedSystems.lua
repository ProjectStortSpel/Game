--	Lerping Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Shared/LerpSystems/?.lua"
require "sh_MovePlayerSystem"
require "sh_AddLerpSystem"
require "sh_UpdateLerpSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TrueTestMoveSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddLerpSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(UpdateLerpSystem)



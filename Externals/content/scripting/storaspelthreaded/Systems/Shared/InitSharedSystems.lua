--	Lerping Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/Systems/Shared/LerpSystems/?.lua"
require "sh_NetworkMessagesSystem"
require "sh_MovePlayerSystem"
require "sh_LerpSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(NetworkMessagesSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TrueTestMoveSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LerpSystem)



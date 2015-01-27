--	Lerping Systems
package.path = package.path .. ";../../../Externals/content/scripting/storaspelthreaded/systems/Shared/LerpSystems/?.lua"
require "sh_MovePlayerSystem"
require "sh_LerpSystem"
require "sh_NetworkMessagesSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TrueTestMoveSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LerpSystem)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(NetworkMessagesSystem)


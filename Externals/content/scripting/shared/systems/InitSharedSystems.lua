--	Lerping Systems
package.path = package.path .. ";../../../Externals/content/scripting/shared/systems/?.lua"
require "sh_PickboxSystem"
require "sh_HoverSizeSystem"
--require "sh_AddTextToTextureSystem"
require "sh_ParentSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ParentSystem)

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PickBoxSystem)

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)

--worldCreator:AddSystemGroup() << Helt removed 
--worldCreator:AddSystemToCurrentGroup(AddTextToTextureSystem) << Helt removed
--	Lerping Systems
package.path = package.path .. ";../shared/systems/?.lua"
require "sh_PickboxSystem"
require "sh_HoverSizeSystem"
require "sh_HoverSoundSystem"
--require "sh_AddTextToTextureSystem"
require "sh_ParentSystem"
require "sh_NotificationBoxSystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(ParentSystem)

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(PickBoxSystem)

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(HoverSizeSystem)

worldCreator:AddSystemToCurrentGroup(HoverSoundSystem)

worldCreator:AddSystemToCurrentGroup(NotificationBoxSystem)

--worldCreator:AddSystemGroup() << Helt removed 
--worldCreator:AddSystemToCurrentGroup(AddTextToTextureSystem) << Helt removed
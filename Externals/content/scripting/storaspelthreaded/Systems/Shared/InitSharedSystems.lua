--	Lerping Systems
package.path = package.path .. ";Systems/Shared/LerpSystems/?.lua"
require "sh_AddLerpPositionSystem"
require "sh_UpdateLerpPositionSystem"
require "sh_AddLerpScaleSystem"
require "sh_UpdateLerpScaleSystem"
require "sh_AddLerpRotationSystem"
require "sh_UpdateLerpRotationSystem"
require "sh_KillAfterLerpSystem"
require "sh_AddEntityAfterLerpSystem"

package.path = package.path .. ";Systems/Shared/AnimationSystems/?.lua"
require "sh_AddAnimationSystem"
require "sh_UpdateAnimationSystem"

package.path = package.path .. ";Systems/Shared/LoadingScreenSystems/?.lua"
require "sh_LoadingScreenSystem"

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddLerpPositionSystem)
worldCreator:AddSystemToCurrentGroup(UpdateLerpPositionSystem)
worldCreator:AddSystemToCurrentGroup(AddLerpScaleSystem)
worldCreator:AddSystemToCurrentGroup(UpdateLerpScaleSystem)
worldCreator:AddSystemToCurrentGroup(AddLerpRotationSystem)
worldCreator:AddSystemToCurrentGroup(UpdateLerpRotationSystem)
worldCreator:AddSystemToCurrentGroup(KillAfterLerpSystem)
worldCreator:AddSystemToCurrentGroup(AddEntityAfterLerpSystem)
--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddAnimationSystem)
worldCreator:AddSystemToCurrentGroup(UpdateAnimationSystem)

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(LoadingScreenSystem)

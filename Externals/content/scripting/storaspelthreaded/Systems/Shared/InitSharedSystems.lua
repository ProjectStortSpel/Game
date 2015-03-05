--	Lerping Systems
package.path = package.path .. ";Systems/Shared/LerpSystems/?.lua"
require "sh_AddLerpPositionSystem"
require "sh_UpdateLerpPositionSystem"
require "sh_AddLerpScaleSystem"
require "sh_UpdateLerpScaleSystem"
require "sh_AddLerpRotationSystem"
require "sh_UpdateLerpRotationSystem"
require "sh_KillAfterLerpSystem"

package.path = package.path .. ";Systems/Shared/AnimationSystems/?.lua"
require "sh_AddAnimationSystem"
require "sh_UpdateAnimationSystem"

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddLerpPositionSystem)
worldCreator:AddSystemToCurrentGroup(UpdateLerpPositionSystem)
worldCreator:AddSystemToCurrentGroup(AddLerpScaleSystem)
worldCreator:AddSystemToCurrentGroup(UpdateLerpScaleSystem)
worldCreator:AddSystemToCurrentGroup(AddLerpRotationSystem)
worldCreator:AddSystemToCurrentGroup(UpdateLerpRotationSystem)
worldCreator:AddSystemToCurrentGroup(KillAfterLerpSystem)

--worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(AddAnimationSystem)
worldCreator:AddSystemToCurrentGroup(UpdateAnimationSystem)

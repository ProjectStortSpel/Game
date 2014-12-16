-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobby/components/?.lua"
require "components"

-- Systems
package.path = package.path .. ";../../../Externals/content/scripting/lobby/systems/?.lua"
require "systems"

-- Templates
package.path = package.path .. ";../../../Externals/content/scripting/lobby/templates/?.lua"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StartUpSystem)

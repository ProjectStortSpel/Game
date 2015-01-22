GameRunning = false;


-- Components
package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/components/?.lua"
require "components"
require "interfacecomponents"
require "buttoncomponents"

package.path = package.path .. ";../../../Externals/content/scripting/lobbythreaded/systems/?.lua"
require "testsystem"

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(TestSystem)
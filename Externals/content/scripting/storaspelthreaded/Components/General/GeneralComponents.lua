--	This is like the init.lua but
--	for all General Components.
--	Use require in this file
--	and they will be loaded
package.path = package.path .. ";Components/General/?.lua"

require "GraphicalComponents"

require "NetworkComponents"

require "LerpComponents"

require "AnimationComponents"

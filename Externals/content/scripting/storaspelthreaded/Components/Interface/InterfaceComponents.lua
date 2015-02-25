--	This is like the init.lua but
--	for all Interface Components.
--	Use require in this file
--	and they will be loaded
package.path = package.path .. ";Components/Interface/?.lua"

require "ButtonComponents"
require "MenuComponents"

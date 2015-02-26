
--	General Components
package.path = package.path .. ";Components/General/?.lua"
require "GeneralComponents"

package.path = package.path .. ";Components/GameLogic/?.lua"
require "GameLogicComponents"

package.path = package.path .. ";Components/Interface/?.lua"
require "InterfaceComponents"
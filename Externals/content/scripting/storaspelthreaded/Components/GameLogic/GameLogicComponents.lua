--	This is like the init.lua but
--	for all Game Logic Components.
--	Use require in this file
--	and they will be loaded
package.path = package.path .. ";Components/GameLogic/?.lua"

require "LobbyComponents"

require "StageComponents"

require "RulesComponents"

require "AIComponents"

require "CardActionComponents"

require "CardSettingsComponents"

require "AbilityComponents"

require "TimerComponents"

require "PhaseComponents"

require "MapComponents"

require "TotemComponents"

require "TriggerComponents"

require "PlayerComponents"

require "UnitComponents"

require "SlerpComponent"

require "MapRaterComponents"

require "WeatherComponents"

require "MiscComponents"

require "ScoreboardComponents"
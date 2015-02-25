
--	Wind
local WeatherComponent = ComponentType()
WeatherComponent.Name = "Weather"
WeatherComponent.SyncNetwork = true
WeatherComponent.TableType = TableType.Map
WeatherComponent:AddVariable("StepToHappen",	ByteSize.Int)	--	Which step during the round
WeatherComponent:AddVariable("StageToHappen", 	ByteSize.Int)	--	When during this step it should happen (Beginning/End of step)
worldCreator:AddComponentType(WeatherComponent)

--	Wind
local WeatherWind = ComponentType()
WeatherWind.Name = "WeatherWind"
WeatherWind.SyncNetwork = true
WeatherWind.TableType = TableType.Map
WeatherWind:AddVariable("Force", ByteSize.Int)
worldCreator:AddComponentType(WeatherWind)

--	Tornado
local WeatherTornado = ComponentType()
WeatherTornado.Name = "WeatherTornado"
WeatherTornado.SyncNetwork = true
WeatherTornado.TableType = TableType.Map
worldCreator:AddComponentType(WeatherTornado)
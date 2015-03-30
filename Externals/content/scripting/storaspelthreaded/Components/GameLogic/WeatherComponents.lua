--	Weather Step
NewComponent = ComponentType()
NewComponent.Name = "WeatherStep"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	WeatherStep Timer
NewComponent = ComponentType()
NewComponent.Name = "WeatherStepTimer"
NewComponent:AddVariable("Time", ByteSize.Float)
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Wind
local NewComponent = ComponentType()
NewComponent.Name = "Weather"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("StepToHappen",	ByteSize.Int)	--	Which step during the round
NewComponent:AddVariable("StageToHappen", 	ByteSize.Int)	--	When during this step it should happen (Beginning/End of step)
worldCreator:AddComponentType(NewComponent)

--	Wind
NewComponent = ComponentType()
NewComponent.Name = "WeatherWind"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
NewComponent:AddVariable("Force", ByteSize.Int)
worldCreator:AddComponentType(NewComponent)

--	Tornado
NewComponent = ComponentType()
NewComponent.Name = "WeatherTornado"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)

--	Rain
NewComponent = ComponentType()
NewComponent.Name = "WeatherRain"
NewComponent.SyncNetwork = true
NewComponent.TableType = TableType.Map
worldCreator:AddComponentType(NewComponent)
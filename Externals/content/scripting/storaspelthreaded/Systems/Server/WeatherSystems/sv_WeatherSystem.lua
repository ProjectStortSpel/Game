WeatherSystem = System()
WeatherSystem.WeatherChance			=	1
WeatherSystem.WeatherList			=	{}
WeatherSystem.WeatherList.__mode	=	"k"

WeatherSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealCards", FilterType.Mandatory)
end

WeatherSystem.PostInitialize = function(self)
	self.WeatherList[#self.WeatherList+1]	=	"Tornado"
	self.WeatherList[#self.WeatherList+1]	=	"Wind"
	

end

WeatherSystem.EntitiesAdded = function(self, dt, newEntities)

	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "DealCards") then
				
			local	actualWeatherChance	=	math.random(1, 100)
			self.WeatherChance	=	(self.WeatherChance+1)*1.5
			print("Weather forecast, " .. self.WeatherChance .. "% chance of weather")
			if actualWeatherChance <= self.WeatherChance then
				self:SpawnWeather()
				self.WeatherChance	=	0
			end
			
		end
		
	end
	
end


WeatherSystem.SpawnWeather = function(self)
	
	if #self.WeatherList == 0 then
		return
	end
	local	newWeather	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Weather", newWeather)
	world:CreateComponentAndAddTo("SyncNetwork", newWeather)
	
	local	weatherType	=	math.random(1, #self.WeatherList)
	local	weatherString	=	self.WeatherList[weatherType]
	print("Weather: " .. weatherString)
	world:CreateComponentAndAddTo("Weather" .. weatherString, newWeather)
end










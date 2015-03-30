WeatherSystem = System()
WeatherSystem.WeatherChance			=	1
WeatherSystem.WeatherList			=	{}
WeatherSystem.WeatherList.__mode	=	"k"
WeatherSystem.Timer = -1
WeatherSystem.WeaterIsActive = false

WeatherSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("WeatherStepTimer", FilterType.RequiresOneOf)
end

WeatherSystem.PostInitialize = function(self)
	self.WeatherList[#self.WeatherList+1]	=	"Tornado"
	self.WeatherList[#self.WeatherList+1]	=	"Wind"
	self.WeatherList[#self.WeatherList+1]	=	"Rain"
end

WeatherSystem.Update = function(self, dt)
	
	if self.Timer >= 0 then
		
		self.Timer = self.Timer - dt
		if self.Timer < 0 then
			
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("WeatherStep", id)
			world:CreateComponentAndAddTo("SyncNetwork", id)
			
			id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("StepTimer", id)
			world:GetComponent(id, "StepTimer", "Time"):SetFloat(1)
			
		end

	end
end

WeatherSystem.EntitiesAdded = function(self, dt, newEntities)

	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "DealCards") then
			self.WeaterIsActive = false
			local	actualWeatherChance	=	math.random(1, 100)
			self.WeatherChance	=	(self.WeatherChance+1)*1.5
			print("Weather forecast, " .. self.WeatherChance .. "% chance of weather")
			if actualWeatherChance <= self.WeatherChance then
				self.WeaterIsActive = true
				self:SpawnWeather()
				self.WeatherChance	=	0
			end
			
		elseif world:EntityHasComponent(tEntity, "WeatherStepTimer") then
			
			if self.WeaterIsActive then
				self.Timer = world:GetComponent(tEntity, "WeatherStepTimer", "Time"):GetFloat()
			else
				self.Timer = 0
			end
			world:KillEntity(tEntity)
			
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










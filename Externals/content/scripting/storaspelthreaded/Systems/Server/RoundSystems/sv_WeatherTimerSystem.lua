WeatherTimerSystem = System()
WeatherTimerSystem.Timer = -1

WeatherTimerSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherTimerSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("WeatherStepTimer", FilterType.Mandatory)
end

WeatherTimerSystem.Update = function(self, dt)
	
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

WeatherTimerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		self.Timer = world:GetComponent(entity, "WeatherStepTimer", "Time"):GetFloat()
		world:KillEntity(entity)
	end
end
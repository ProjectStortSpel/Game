StepTimerSystem = System()
StepTimerSystem.Timer = -1

StepTimerSystem.Initialize = function(self)
	--	Set Name
	self:SetName("StepTimerSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("StepTimer", FilterType.Mandatory)
end

StepTimerSystem.Update = function(self, dt)
	
	if self.Timer >= 0 then
		
		self.Timer = self.Timer - dt
		if self.Timer < 0 then
			
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NewStep", id)
			world:CreateComponentAndAddTo("SyncNetwork", id)
		end

	end
end

StepTimerSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		self.Timer = world:GetComponent(entity, "StepTimer", "Time"):GetFloat()
		world:KillEntity(entity)
	end
end
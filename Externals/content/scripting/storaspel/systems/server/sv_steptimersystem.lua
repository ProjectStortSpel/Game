StepTimerSystem = System()
StepTimerSystem.Timer = -1

StepTimerSystem.Initialize = function(self)
	self:SetName("StepTimerSystem")
	
	self:AddComponentTypeToFilter("StepTimer", FilterType.Mandatory)
end

StepTimerSystem.Update = function(self, dt)
	
	if self.Timer >= 0 then
		
		self.Timer = self.Timer - dt

		if self.Timer < 0 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NewStep", id)
		end

	end
end

StepTimerSystem.OnEntityAdded = function(self, entity)
	
	self.Timer = world:GetComponent(entity, "StepTimer", "Time"):GetFloat()
	world:KillEntity(entity)

end
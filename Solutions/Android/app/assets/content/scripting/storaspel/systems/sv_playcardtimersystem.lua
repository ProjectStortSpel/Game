PlayCardTimerSystem = System()
PlayCardTimerSystem.Timer = -1
PlayCardTimerSystem.Step = 0

PlayCardTimerSystem.Initialize = function(self)
	self:SetName("PlayCardTimerSystem")
	
	self:AddComponentTypeToFilter("PlayCardTimer", FilterType.Mandatory)
		
	print("PlayCardTimerSystem initialized!")
end

PlayCardTimerSystem.Update = function(self, dt)
	
	if self.Timer >= 0 then
		
		self.Timer = self.Timer - dt

		if self.Timer < 0 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PlayCard", id)

			world:SetComponent(id, "PlayCard", "Step", self.Step)
		end

	end
end

PlayCardTimerSystem.OnEntityAdded = function(self, entity)
	
	self.Timer = world:GetComponent(entity, "PlayCardTimer", "Time"):GetFloat()
	self.Step  = world:GetComponent(entity, "PlayCardTimer", "Step"):GetInt()
	world:KillEntity(entity)

end
PlayCardTimerSystem = System()
PlayCardTimerSystem.Timer = -1
PlayCardTimerSystem.Step = 0

PlayCardTimerSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PlayCardTimerSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("PlayCardTimer", FilterType.Mandatory)
end

PlayCardTimerSystem.Update = function(self, dt, taskIndex, taskCount)
	
	if self.Timer >= 0 then
		
		self.Timer = self.Timer - dt

		if self.Timer < 0 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PlayCard", id)

			world:SetComponent(id, "PlayCard", "Step", self.Step)
		end

	end
end

PlayCardTimerSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		self.Timer = world:GetComponent(entity, "PlayCardTimer", "Time"):GetFloat()
		self.Step  = world:GetComponent(entity, "PlayCardTimer", "Step"):GetInt()
		world:KillEntity(entity)
	end
end
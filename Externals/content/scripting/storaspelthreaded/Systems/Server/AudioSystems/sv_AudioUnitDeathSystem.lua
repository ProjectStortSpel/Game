AudioUnitDeathSystem = System()

AudioUnitDeathSystem.AudioDeathPosY = -1.0

AudioUnitDeathSystem.PrevPosYTable = {}
AudioUnitDeathSystem.PrevPosYTable.__mode = "k"

AudioUnitDeathSystem.Initialize = function(self)
	self:SetName("AudioUnitDeathSystem")
	
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitDead", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
end

AudioUnitDeathSystem.Update = function(self, dt, taskIndex, taskCount)
	local entities = self:GetEntities("Unit")
	if entities ~= nil then
		for i = 1, #entities do
			local entity = entities[i]
			local y = world:GetComponent(entity, "Position", "Y"):GetFloat(0)
			
			if self.PrevPosYTable[entity] ~= nil then
				if y <= self.AudioDeathPosY and self.PrevPosYTable[entity] > self.AudioDeathPosY then
					--local audioId = Net.StartPack("Client.PlaySound")
					--Net.WriteString(audioId, "No")
					--Net.WriteBool(audioId, false)
					--Net.Broadcast(audioId)
				end
			end
		  
			self.PrevPosYTable[entity] = y
		end
	end
end

AudioUnitDeathSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	if entities ~= nil then
		for i = 1, #entities do
			local entity = entities[i]
			local y = world:GetComponent(entity, "Position", "Y"):GetFloat(0)
			self.PrevPosYTable[entity] = y
		end
	end
end
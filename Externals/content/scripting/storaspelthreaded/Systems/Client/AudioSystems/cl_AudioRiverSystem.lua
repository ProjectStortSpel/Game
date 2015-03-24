AudioRiverSystem = System()

AudioRiverSystem.Initialize = function(self)
	self:SetName("AudioRiverSystem")
	
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	self:AddComponentTypeToFilter("River", FilterType.RequiresOneOf)
end

AudioRiverSystem.EntitiesAdded = function(self, dt, entities)
	if entities ~= nil then
		for i = 1, #entities do
			local entity = entities[i]
			local x, y, z = world:GetComponent(entity, "Position", "X"):GetFloat3()
			local soundName = "RiverWaterLoop" .. entity
			
			if world:EntityHasComponent(entity, "RiverEnd") then
				Audio.PlaySound("WaterfallLoop", soundName, true)
				Audio.SetSoundVolume(soundName, 18)
			else
				Audio.PlaySound("RiverWaterLoop", soundName, true)
				Audio.SetSoundVolume(soundName, 10)
			end
			
			Audio.SetSoundPosition(soundName, x, y, z)
		end
	end
end

AudioRiverSystem.EntitiesRemoved = function(self, dt, entities)
	if entities ~= nil then
		for i = 1, #entities do
			local entity = entities[i]
			local soundName = "RiverWaterLoop" .. entity
			
			Audio.StopSound(soundName)
		end
	end
end
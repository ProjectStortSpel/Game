HoverSoundSystem = System()

HoverSoundSystem.Initialize = function(self)
	--	Set Name
	self:SetName("HoverSoundSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("HoverSound", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
end


HoverSoundSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		local sound = world:GetComponent(entityId, "HoverSound", "Sound"):GetText()
		
		local soundname = sound .. entityId
		Audio.PlaySound(sound, soundname, false)
		Audio.SetSoundVolume(soundname, 12)
	end
end


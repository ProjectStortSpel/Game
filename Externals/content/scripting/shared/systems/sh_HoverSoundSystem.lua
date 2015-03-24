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
		
		Audio.PlaySound(sound, sound .. entityId, false)
	end
end


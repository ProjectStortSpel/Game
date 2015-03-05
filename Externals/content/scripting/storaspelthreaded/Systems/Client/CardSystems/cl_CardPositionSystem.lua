CardPositionSystem = System()
CardPositionSystem.UpOffset = -1.65

CardPositionSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CardPositionSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
	self:AddComponentTypeToFilter("SelectCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("CardHolding", FilterType.Excluded)
	
end

CardPositionSystem.EntitiesAdded = function(self, dt, newEntities)
	local entities = self:GetEntities()
	local entityCount = #entities
	for i = 1, entityCount do
		local entity = entities[i]
		
		local halfentities = entityCount * 0.5
		local px = (-halfentities + i - 0.5) * 0.6
		local py = self.UpOffset
		local pz = -4.0
		
		if px > 2.2 or px < -2.2 then
			print("2 : 1 - Very very wrong! px: ", px, i, entityCount)
		end
		
		if not world:EntityHasComponent(entity, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", entity)
		end
		
		world:GetComponent(entity, "LerpPosition", "Time"):SetFloat4(0.1, px, py, pz)
		world:GetComponent(entity, "LerpPosition", "Algorithm"):SetText("SmootherLerp")
	end
end

CardPositionSystem.EntitiesRemoved = function(self, dt, newEntities)
	local entities = self:GetEntities()
	local entityCount = #entities
	for i = 1, entityCount do
		local entity = entities[i]
		
		local halfentities = entityCount * 0.5
		local px = (-halfentities + i - 0.5) * 0.6
		local py = self.UpOffset
		local pz = -4.0
		
		if not world:EntityHasComponent(entity, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", entity)
		end
		world:GetComponent(entity, "LerpPosition", "Time"):SetFloat4(0.1, px, py, pz)
		world:GetComponent(entity, "LerpPosition", "Algorithm"):SetText("SmootherLerp")
	end
end
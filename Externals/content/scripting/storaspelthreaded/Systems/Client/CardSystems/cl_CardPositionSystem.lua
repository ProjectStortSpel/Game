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

CardPositionSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, newEntities)
	print("added")
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local index = world:GetComponent(entity, "CardIndex", "Index"):GetInt()
		
		local halfentities = #entities/2
		local px = (-halfentities + i - 0.5) * 0.6
		local py = self.UpOffset
		local pz = -4
		
		if not world:EntityHasComponent(entity, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", entity)
		end
		world:GetComponent(entity, "LerpPosition", "X"):SetFloat(px)
		world:GetComponent(entity, "LerpPosition", "Y"):SetFloat(py)
		world:GetComponent(entity, "LerpPosition", "Z"):SetFloat(pz)
		world:GetComponent(entity, "LerpPosition", "Time"):SetFloat(0.1)
		world:GetComponent(entity, "LerpPosition", "Algorithm"):SetString("SmootherLerp")
	end
end

CardPositionSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, newEntities)
	print("removed")
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local index = world:GetComponent(entity, "CardIndex", "Index"):GetInt()
		
		local halfentities = #entities/2
		local px = (-halfentities + i - 0.5) * 0.6
		local py = self.UpOffset
		local pz = -4
		
		if not world:EntityHasComponent(entity, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", entity)
		end
		world:GetComponent(entity, "LerpPosition", "X"):SetFloat(px)
		world:GetComponent(entity, "LerpPosition", "Y"):SetFloat(py)
		world:GetComponent(entity, "LerpPosition", "Z"):SetFloat(pz)
		world:GetComponent(entity, "LerpPosition", "Time"):SetFloat(0.1)
		world:GetComponent(entity, "LerpPosition", "Algorithm"):SetString("SmootherLerp")
	end
end
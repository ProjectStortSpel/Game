CardPickedPositionSystem = System()
CardPickedPositionSystem.UpOffset = -1

CardPickedPositionSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CardPickedPositionSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardSelected", FilterType.Mandatory)
end

CardPickedPositionSystem.EntitiesAdded = function(self, dt, newEntities)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local card = entities[i]
	
		local data = world:GetComponent(card, "SelectCard", "Index"):GetInt()
		
		local halfentities = #entities/2
		local px = (-halfentities + data - 0.5) * 0.7
		local py = self.UpOffset
		local pz = -6
		
		if not world:EntityHasComponent(card, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", card)
		end
		world:GetComponent(card, "LerpPosition", "X"):SetFloat(px)
		world:GetComponent(card, "LerpPosition", "Y"):SetFloat(py)
		world:GetComponent(card, "LerpPosition", "Z"):SetFloat(pz)
		world:GetComponent(card, "LerpPosition", "Time"):SetFloat(0.1)
		world:GetComponent(card, "LerpPosition", "Algorithm"):SetText("SmoothLerp")
		world:GetComponent(card, "LerpPosition", "KillWhenFinished"):SetBool(false)
	end
end

CardPickedPositionSystem.EntitiesRemoved = function(self, dt, newEntities)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local card = entities[i]
	
		local data = world:GetComponent(card, "SelectCard", "Index"):GetInt()
		
		local halfentities = #entities/2
		local px = (-halfentities + data - 0.5) * 0.7
		local py = self.UpOffset
		local pz = -6
		
		if not world:EntityHasComponent(card, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", card)
		end
		world:GetComponent(card, "LerpPosition", "X"):SetFloat(px)
		world:GetComponent(card, "LerpPosition", "Y"):SetFloat(py)
		world:GetComponent(card, "LerpPosition", "Z"):SetFloat(pz)
		world:GetComponent(card, "LerpPosition", "Time"):SetFloat(0.1)
		world:GetComponent(card, "LerpPosition", "Algorithm"):SetText("SmoothLerp")
		world:GetComponent(card, "LerpPosition", "KillWhenFinished"):SetBool(false)
	end
end

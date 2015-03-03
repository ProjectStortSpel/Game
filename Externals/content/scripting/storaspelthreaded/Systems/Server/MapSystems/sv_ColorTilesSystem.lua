ColorTilesSystem = System()

ColorTilesSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ColorTilesSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()

	--	Set Filter
	self:AddComponentTypeToFilter("NotWalkable", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Model", FilterType.Mandatory)
end

ColorTilesSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		if not world:EntityHasComponent(entity, "Color") then
			world:CreateComponentAndAddTo("Color", entity)
		end
		
		world:GetComponent(entity, "Color", "X"):SetFloat(0.001)
		world:GetComponent(entity, "Color", "Y"):SetFloat(0.001)
		world:GetComponent(entity, "Color", "Z"):SetFloat(0.001)
		
	end
end

ColorTilesSystem.EntitiesRemoved = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		if not world:EntityHasComponent(entity, "Color") then
			world:CreateComponentAndAddTo("Color", entity)
		end

		local X, Z = world:GetComponent(entity, "MapPosition", "X"):GetInt2(0)
		local check = ((X + Z) % 2)
		world:GetComponent(entity, "Color", "X"):SetFloat(check*0.75)
		world:GetComponent(entity, "Color", "Y"):SetFloat(check)
		world:GetComponent(entity, "Color", "Z"):SetFloat(check*0.75)
		
	end
end
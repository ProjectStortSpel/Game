ActionGuardSystem = System()

ActionGuardSystem.Initialize = function(self)

	self:SetName("ActionGuardSystem")
		
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("UnitGuard",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ActionGuard",FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewStep", FilterType.RequiresOneOf)
	
end

ActionGuardSystem.EntitiesAdded = function(self, dt, entities)

	local uGuard = self:GetEntities("UnitGuard")
	local aGuard = self:GetEntities("ActionGuard")

	for n = 1, #entities do
		local entity = entities[n]

		if world:EntityHasComponent( entity, "NewRound") or world:EntityHasComponent( entity, "NewStep") then -- If it's a new round or new step
	
			for i = 1, #aGuard do
				world:RemoveComponentFrom("ActionGuard", aGuard[i])
			end
	
		else -- If a guard card was played
		
			for i = 1, #uGuard do
				world:CreateComponentAndAddTo("ActionGuard", entity)
				world:RemoveComponentFrom("UnitGuard", entity)
			end
			
		end	
		
	end
	
end
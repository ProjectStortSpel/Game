ActionGuardSystem = System()

ActionGuardSystem.Initialize = function(self)

	self:SetName("ActionGuardSystem")
		
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitGuard",FilterType.Mandatory)
	
end

ActionGuardSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("AbilityGuard", id)
		world:RemoveComponentFrom("UnitSprint", entity)
		
	end
end
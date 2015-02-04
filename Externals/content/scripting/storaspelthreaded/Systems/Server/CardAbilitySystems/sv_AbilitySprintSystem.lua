AbilitySprintSystem = System()

AbilitySprintSystem.Initialize = function(self)

	self:SetName("AbilitySprintSystem")
		
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitSprint",FilterType.Mandatory)
	
	--self:AddComponentTypeToFilter("Stunned",FilterType.Excluded)
end

AbilitySprintSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		
		local dirX, dirZ = world:GetComponent(entity, "Direction", 0):GetInt2()
		local mapPosX, mapPosZ = world:GetComponent(entity, "MapPosition", 0):GetInt2()

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("TestMove", id)
		world:SetComponent(id, "TestMove", "Unit", entity)
		world:SetComponent(id, "TestMove", "PosX", mapPosX+dirX)
		world:SetComponent(id, "TestMove", "PosZ", mapPosZ+dirZ)
		world:SetComponent(id, "TestMove", "DirX", dirX)
		world:SetComponent(id, "TestMove", "DirZ", dirZ)
		world:SetComponent(id, "TestMove", "Steps", 2)

		world:RemoveComponentFrom("UnitSprint", entity)
	end
end
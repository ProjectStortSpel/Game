ActionMoveForwardSystem = System()

ActionMoveForwardSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ActionMoveForwardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("Unit",			FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitForward",	FilterType.Mandatory)
end

ActionMoveForwardSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local dirX, dirZ 		= 	world:GetComponent(entity, "Direction", 0):GetInt2()
		local mapPosX, mapPosZ 	= 	world:GetComponent(entity, "MapPosition", 0):GetInt2()

	
		if dirX ~= 0 then
			dirX = dirX / math.abs(dirX)
		end

		if dirZ ~= 0 then
			dirZ = dirZ / math.abs(dirZ)
		end

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("TestMove", id)
		world:SetComponent(id, "TestMove", "Unit", entity)
		world:SetComponent(id, "TestMove", "PosX", mapPosX + dirX)
		world:SetComponent(id, "TestMove", "PosZ", mapPosZ + dirZ)
		world:SetComponent(id, "TestMove", "DirX", dirX)
		world:SetComponent(id, "TestMove", "DirZ", dirZ)
		world:SetComponent(id, "TestMove", "Steps", 1)
	
		world:RemoveComponentFrom("UnitForward", entity)
	end
end
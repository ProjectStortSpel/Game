
TestMoveRiverSystem = System()

TestMoveRiverSystem.Initialize = function(self)
	--	Set Name
	self:SetName("TestMoveRiverSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("TestRiver", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable", 	FilterType.RequiresOneOf)
end

TestMoveRiverSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "TestRiver") then
			
			local notWalkable = self:GetEntities("NotWalkable")

			local unit = world:GetComponent(entity, "TestRiver", "Unit"):GetInt()
			local posX = world:GetComponent(entity, "TestRiver", "PosX"):GetInt()
			local posZ = world:GetComponent(entity, "TestRiver", "PosZ"):GetInt()

			local canMove = true

			for i = 1, #notWalkable do
				local X2, Z2 = world:GetComponent(notWalkable[i], "MapPosition", 0):GetInt2()
				if posX == X2 and posZ == Z2 then
					canMove = false
					break
				end
			end 

			if canMove then
				world:GetComponent(unit, "MapPosition", 0):SetInt2(posX, posZ)
			end

			world:KillEntity(entity)
		end
	end
end
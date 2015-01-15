RiverSystem = System()

RiverSystem.Initialize = function(self)
	self:SetName("RiverSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MoveRiver", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("River", FilterType.RequiresOneOf)

	print("RiverSystem initialized!")
end

RiverSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "MoveRiver") then
		print("Move River")
		local units = self:GetEntities("Unit")
		local rivers = self:GetEntities("River")

		for i = 1, #units do
			
			local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2() 

			for j = 1, #rivers do
				
				local riverX, riverZ = world:GetComponent(rivers[j], "MapPosition", 0):GetInt2() 

				if unitX == riverX and unitZ == riverZ then

					local riverDirX, riverDirZ = world:GetComponent(rivers[j], "River", 0):GetInt2()
					local posXs = unitX + riverDirX
					local posZs = unitZ + riverDirZ

					--world:GetComponent(units[i], "MapPosition", 0):SetInt2(posX, posZ)

					--print(entity)

					local id = world:CreateNewEntity()
					world:CreateComponentAndAddTo("TestRiver", id)
					world:SetComponent(id, "TestRiver", "Unit", units[i])
					world:SetComponent(id, "TestRiver", "PosX", posXs)
					world:SetComponent(id, "TestRiver", "PosZ", posZs)

					break

				end

			end

		end

		world:KillEntity( entity )

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PostMove", id)

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("StepTimer", id)
		world:GetComponent(id, "StepTimer", "Time"):SetFloat(1)


	end
end


TestMoveRiverSystem = System()

TestMoveRiverSystem.Initialize = function(self)
	self:SetName("TestMoveRiverSystem System")

	self:AddComponentTypeToFilter("TestRiver", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable", FilterType.RequiresOneOf)

	print("TestMoveRiverSystem initialized!")
end

TestMoveRiverSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "TestRiver") then
		
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

TestMoveSystem = System()

TestMoveSystem.Initialize = function(self)
	--	Set Name
	self:SetName("TestMoveSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TestMove",FilterType.RequiresOneOf)

end

TestMoveSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent( entity, "TestMove") then
			
			local units = self:GetEntities("Unit")
			local notWalkable = self:GetEntities("NotWalkable")

			local unit = world:GetComponent(entity, "TestMove", "Unit"):GetInt()
			local posX = world:GetComponent(entity, "TestMove", "PosX"):GetInt()
			local posZ = world:GetComponent(entity, "TestMove", "PosZ"):GetInt()
			local dirX = world:GetComponent(entity, "TestMove", "DirX"):GetInt()
			local dirZ = world:GetComponent(entity, "TestMove", "DirZ"):GetInt()

			local moveUnits = { }
			
			local X1, Z1 = posX, posZ

			local bla = true

			while bla do
				
				bla = false
				for i = 1, #units do
				
					local X2, Z2 = world:GetComponent(units[i], "MapPosition", 0):GetInt2()

					if X1 == X2 and Z1 == Z2 then
					
						moveUnits[#moveUnits + 1] = units[i]
						X1 = X1 + dirX
						Z1 = Z1 + dirZ
						bla = true
					end
				end

			end

			local isWalkable = true
			for i = 1, #notWalkable do
				
				local X2, Z2 = world:GetComponent(notWalkable[i], "MapPosition", 0):GetInt2()

				if X1 == X2 and Z1 == Z2 then
					
					isWalkable = false
					break

				end
			end

			if isWalkable then
				
				print("Push units: " .. #moveUnits)

				for i = 1, #moveUnits do
					
					local mapPos = world:GetComponent(moveUnits[i], "MapPosition", 0)
					local pos = world:GetComponent(moveUnits[i], "Position", 0)

					local mapPosX, mapPosZ = mapPos:GetInt2()
					local newPosX, newPosY, newPosZ = pos:GetFloat3()

					mapPos:SetInt2(mapPosX + dirX, mapPosZ + dirZ)
					--pos:SetFloat3(newPosX + dirX, newPosY, newPosZ + dirZ)

				end

				--local posY = world:GetComponent(unit, "Position", "Y"):GetFloat()
				world:GetComponent(unit, "MapPosition", 0):SetInt2(posX, posZ)		
				
				--world:GetComponent(unit, "Position", 0):SetFloat3(posX, posY, posZ)


				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("PostMove", id)

			end


			world:KillEntity(entity)

		end
	end
end
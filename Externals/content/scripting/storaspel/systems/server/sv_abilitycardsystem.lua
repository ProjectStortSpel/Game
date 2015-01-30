AbilityNudgeSystem = System()
AbilityNudgeSystem.Power = 2
AbilityNudgeSystem.Range = 1

AbilityNudgeSystem.Initialize = function(self)
	self:SetName("AbilityNudgeSystem")
	
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)
	
end

AbilityNudgeSystem.Update = function(self, dt)

	local entites = self:GetEntities("UnitNudge")
	
	for i = 1, #entites do
		-- Direction and Position of the unit using the ability
		local dirX, dirZ = world:GetComponent(entites[i], "Direction", 0):GetInt2()
		local mapPosX, mapPosZ = world:GetComponent(entites[i], "MapPosition", 0):GetInt2()
	
		if dirX ~= 0 then
			dirX = dirX / math.abs(dirX)
		end

		if dirZ ~= 0 then
			dirZ = dirZ / math.abs(dirZ)
		end
	
		-- Get the tile the ability will be cast on
		-- Range is how far away from the unit the ability will reach
		local aimPosX = mapPosX + (dirX * self.Range)
		local aimPosZ = mapPosZ + (dirZ * self.Range)
		
		-- Go through all players on the map
		local units = self:GetEntities("Unit")
		for j = 1, #units do
		
			-- If the target is not the same as the unit which used the ability
			--if not units[j] == entites[i] then
			
				-- Get the other units position
				local targetPosX, targetPosZ = world:GetComponent(units[j], "MapPosition", 0):GetInt2()
				-- If the targeted tile is the same as the units position
				if aimPosX == targetPosX and aimPosZ == targetPosZ then
					--print("HIT")
					local id = world:CreateNewEntity()
					world:CreateComponentAndAddTo("TestMove", id)
					world:SetComponent(id, "TestMove", "Unit", units[j])
					world:SetComponent(id, "TestMove", "PosX", targetPosX)
					world:SetComponent(id, "TestMove", "PosZ", targetPosZ)
					world:SetComponent(id, "TestMove", "DirX", dirX)
					world:SetComponent(id, "TestMove", "DirZ", dirZ)
					world:SetComponent(id, "TestMove", "Steps", self.Power)
				else
					--print("MISS")
				end
				
			
			--end
		
		end
		world:RemoveComponentFrom("UnitNudge", entites[i])
	end

end

AbilityNudgeSystem.OnEntityAdded = function(self, entity)
end


AbilitySlingShotSystem = System()
AbilitySlingShotSystem.Range = 50

AbilitySlingShotSystem.Initialize = function(self)
	self:SetName("AbilitySlingShotSystem")
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SlingShotComponent", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSize", FilterType.RequiresOneOf)
	
end

AbilitySlingShotSystem.Update = function(self, dt)

	local entites = self:GetEntities("UnitSlingShot")
	local mapSize = self:GetEntities("MapSize")
	local mapSizeX, mapSizeZ = world:GetComponent(mapSize[1], "MapSize", 0):GetInt2()
	for i = 1, #entites do
	
		local hitSomething = false
		-- Direction and Position of the unit using the ability
		local dirX, dirZ = world:GetComponent(entites[i], "Direction", 0):GetInt2()
		local mapPosX, mapPosZ = world:GetComponent(entites[i], "MapPosition", 0):GetInt2()
		
		-- Go through all tiles and check if it collides with something
		for j = 1, self.Range do

			-- If we hit something not walkable we should stop checking tiles
			if hitSomething then
				break
			end
		
			-- Get the current tile position
			local currentPosX = mapPosX + (dirX * j)
			local currentPosZ = mapPosZ + (dirZ * j)
			
			-- Make sure we are not outside the map
			-- Get the map size somehow
			
			if currentPosX < 1 or currentPosZ < 1
			or currentPosX > mapSizeX or currentPosZ > mapSizeZ then
			
				--print("currentPosX: " .. currentPosX)
				--print("currentPosZ: " .. currentPosZ)
				--print("mapSizeX: " .. mapSizeX)
				--print("mapSizeZ: " .. mapSizeZ)
			
				local bullet = world:CreateNewEntity("SlingShot")
				world:CreateComponentAndAddTo("LerpTargetPosition", bullet)
				world:CreateComponentAndAddTo("LerpTime", bullet)
				world:GetComponent(bullet, "Position", 0):SetFloat3(mapPosX, 1, mapPosZ)
				world:GetComponent(bullet, "LerpTargetPosition", 0):SetFloat3(currentPosX, 1, currentPosZ)
				self:GetComponent(bullet, "LerpTime", 0):SetFloat2(0.5, 0)
				break
				
			else

				-- loop through all units and not walkable entities
				local units = self:GetEntities()
				for u = 1, #units do
				
					-- Get the position of all entities
					local targetPosX, targetPosZ = world:GetComponent(units[u], "MapPosition", 0):GetInt2()
					
					-- If the sling shot hit something
					if targetPosX == currentPosX and targetPosZ == currentPosZ then
					
						local bullet = world:CreateNewEntity("SlingShot")
						world:CreateComponentAndAddTo("LerpTargetPosition", bullet)
						world:CreateComponentAndAddTo("LerpTime", bullet)
						world:GetComponent(bullet, "Position", 0):SetFloat3(mapPosX, 1, mapPosZ)
						world:GetComponent(bullet, "LerpTargetPosition", 0):SetFloat3(targetPosX, 1, targetPosZ)
						self:GetComponent(bullet, "LerpTime", 0):SetFloat2(0.5, 0)
						
						-- if the hit entity is a unit
						if world:EntityHasComponent(units[u], "Unit") then
						
							hitSomething = true
							if not world:EntityHasComponent(units[u], "Stunned") then
								world:CreateComponentAndAddTo("Stunned", units[u])
							end
							break
							
						-- Else the hit entity is a NotWalkable object
						-- Remove the component and return
						else
							hitSomething = true
							break
						end
							
					end
	
				end
			
			end
			
		
		end
		
		-- Remvoe the ability component
		world:RemoveComponentFrom("UnitSlingShot", entites[i])
	end

	
	local slingshots = self:GetEntities("SlingShotComponent")
	for i = 1, #slingshots do
	
		local posX, _, posZ = world:GetComponent(slingshots[i], "Position", 0):GetFloat3()
		local tarX, _, tarZ = self:GetComponent(slingshots[i], "LerpTargetPosition", 0):GetFloat3()
		if posX == tarX and posZ == tarZ then
			world:KillEntity(slingshots[i])
		end
	end
	
end

AbilitySlingShotSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent(entity, "DealCards") then
		
		--print("world:EntityHasComponent(entity, \"DealCards\")")
		local units = self:GetEntities("Unit")
		for i = 1, #units do
			if world:EntityHasComponent(units[i], "Stunned") then
				world:RemoveComponentFrom("Stunned", units[i])
			end
		end
	end
end

AbilitySlingShotSystem.OnEntityRemoved = function(self, entity)
end
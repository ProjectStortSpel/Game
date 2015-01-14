MapSystem = System()
MapSystem.entities = { }
MapSystem.mapX = 0
MapSystem.mapY = 0

MapSystem.PostInitialize = function(self)
	local map
    self.mapX, self.mapY, map = File.LoadMap("content/maps/map.txt")
    local posX, posZ
	
	for x = 0, self.mapX+1 do
		self:AddTile(x, 1, 111) -- 111 = void
	end
	
	for y = 1, self.mapY do
		self:AddTile(0, y, 111) -- 111 = void
		for x = 1, self.mapX do
			self:AddTile(x, y, map[(y - 1) * self.mapX + x])
        end
		self:AddTile(self.mapX + 1, y, 111) -- 111 = void
    end
	
	for x = 0, self.mapX+1 do
		self:AddTile(x, self.mapY, 111) -- 111 = void
	end

	local activeEntities = MapSystem.entities
	local waterTiles = {}
	for i = 1, #activeEntities do
		if world:EntityHasComponent(activeEntities[i], "River") then
			waterTiles[#waterTiles + 1] = activeEntities[i]
		end
	end

	print("Water size: " .. #waterTiles)

	for waterA = 1, #waterTiles do
		
		local waterPosA = world:GetComponent(waterTiles[waterA], "MapPosition", 0)
		local waterDirA = world:GetComponent(waterTiles[waterA], "River", 0)
		local posAX, posAY = waterPosA:GetInt2()
		local dirAX, dirAY = waterDirA:GetInt2()

		--print("Water[ " .. posAX .. ", " .. posAY .. "] with direction [" .. dirAX .. ", " .. dirAY .. "]")

		for waterB = 1, #waterTiles do

			if waterA ~= waterB then
				local waterPosB = world:GetComponent(waterTiles[waterB], "MapPosition", 0)
				local waterDirB = world:GetComponent(waterTiles[waterB], "River", 0)
				local posBX, posBY = waterPosB:GetInt2()
				local dirBX, dirBY = waterDirB:GetInt2()

				if posAX + dirAX == posBX and posAY + dirAY == posBY then
					if dirAX ~= dirBX and dirAY ~= dirBY then

						local comp = self:GetComponent(waterTiles[waterB], "Model", 0)
						comp:SetModel("rivercorner", "rivercorner", 0, 0)


						--	LEFT TURN (Correct rotation)
						--if dirAX == 1 and dirBY == -1 then
						--elseif dirAX == -1 and dirBY == 1 then
						--elseif dirAY == -1 and dirBX == -1 then
						--elseif dirAY == 1 and dirBX == 1 then

						local comp = self:GetComponent(waterTiles[waterB], "Rotation", 0)
						local currentRotation = comp:GetFloat(1)

						--	RIGHT TURN
						if dirAX == 1 and dirBY == 1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
						elseif dirAX == -1 and dirBY == -1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
						elseif dirAY == 1 and dirBX == -1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
						elseif dirAY == -1 and dirBX == 1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
						end



					end
				end
			end
		end

	end

    print("Init map done!")
end

MapSystem.AddTile = function(self, posX, posZ, tiletype)
	
    local entity = world:CreateNewEntity("Tile")
    local posComp = self:GetComponent(entity, "Position", 0)
    posComp:SetFloat3(posX, 0.0, posZ)
	
    local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
    mapPosComp:SetInt2(posX, posZ)

    if tiletype == 111 then -- 111 = o = void
        world:CreateComponentAndAddTo("Void", entity)
		
    elseif tiletype == 104 then -- 104 = h = hole
        world:CreateComponentAndAddTo("Void", entity)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("hole_test", "hole", 0)
--  No need???
--    elseif tiletype == 46 then -- 46 = . = grass
--        world:CreateComponentAndAddTo("", entity)

    elseif tiletype == 120 then -- 120 = x = stone
		world:CreateComponentAndAddTo("NotWalkable", entity)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("stone", "stone", 0)
		posComp:SetFloat3(posX, 1.0, posZ)
		
		self:AddGroundTileBelow(posX, posZ)

    elseif tiletype >= 49 and tiletype <= 57 then -- 49 = 1 = first checkpoint, 47 = 9 = 9th checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(tiletype - 48)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("checkpoint", "checkpoint", 0)
		--posComp:SetFloat3(posX, 1.0, posZ)

    elseif tiletype == 102 then -- 102 = f = finish
        world:CreateComponentAndAddTo("Finish", entity)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("finish", "finish", 0)
		--posComp:SetFloat3(posX, 1.0, posZ)

    elseif tiletype == 117 then -- 117 = u = water up
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(0, -1)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, -math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)

    elseif tiletype == 100 then -- 100 = d = water down
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(0, 1)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)

    elseif tiletype == 108 then -- 108 = l = water left
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(-1, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)

    elseif tiletype == 114 then -- 114 = r = water right
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(1, 0)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, math.pi, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)
		
	elseif tiletype == 115 then -- 115 = s = Available spawn point
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("grass", "grass", 0, 0)
		
		
		local newSpawnId = world:CreateNewEntity()
		world:CreateComponentAndAddTo("AvailableSpawnpoint", newSpawnId)
		local newSpawn = self:GetComponent(newSpawnId, "AvailableSpawnpoint", 0)
		newSpawn:SetInt2(posX, posZ)
		
	else
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("grass", "grass", 0)
		
    end

    table.insert(self.entities, entity)
end

MapSystem.AddGroundTileBelow = function(self, posX, posZ)

	local groundEntity = world:CreateNewEntity("Tile")
	local posComp = self:GetComponent(groundEntity, "Position", 0)
	posComp:SetFloat3(posX, 0.0, posZ)
	
	local mapPosComp = self:GetComponent(groundEntity, "MapPosition", 0)
	mapPosComp:SetInt2(posX, posZ)
	
	world:CreateComponentAndAddTo("Model", groundEntity)
	local comp = self:GetComponent(groundEntity, "Model", 0)
	comp:SetModel("grass", "grass", 0)
	
	--table.insert(self.entities, groundEntity)
end 

MapSystem.GetCheckPointId = function(self, posX, posY)
	
	local index = self.mapX * posY + posX + 1
	entity = self.entities[index]
	
	if self:EntityHasComponent(entity, "Checkpoint") then
		local comp = self:GetComponent(entity, "Checkpoint", 0)
		
		return comp:GetInt()
	else
		return -1
	end
	
end

MapSystem.TileIsWalkable = function(self, posX, posY)
	
	return not self:TileHasComponent("NotWalkable", posX, posY)
end

MapSystem.TileIsVoid = function(self, posX, posY)

	return self:TileHasComponent("Void", posX, posY)
end

MapSystem.TileHasComponent = function(self, component, posX, posY)
	local index = self.mapX * posY + posX + 1
	entity = self.entities[index]
	
	local returnValue = self:EntityHasComponent(entity, component)
	return returnValue
end
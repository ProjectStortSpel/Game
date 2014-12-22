MapSystem = System()
MapSystem.entities = { }
MapSystem.mapX = 0
MapSystem.mapY = 0

MapSystem.PostInitialize = function(self)
	local map
    self.mapX, self.mapY, map = File.LoadMap("content/maps/map.txt")
    local posX, posZ
	
	
	for y = 0, self.mapY-1 do
        for x = 0, self.mapX-1 do
            --posX = x - self.mapX/2
            --posZ = y - self.mapY/2
            self:AddTile(x, y, map[y * self.mapX + x + 1])
        end
    end

    print("Init map done!")
end

MapSystem.AddTile = function(self, posX, posZ, tiletype)
	
    local entity = world:CreateNewEntity("Tile")
    local posComp = self:GetComponent(entity, "Position", 0)
    posComp:SetFloat3(posX, 0.0, posZ)
	
    world:CreateComponentAndAddTo("MapPosition", entity)
    local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
    mapPosComp:SetInt2(posX, posZ)

    if tiletype == 111 then -- 111 = o = out
        world:CreateComponentAndAddTo("Void", entity)
		
    elseif tiletype == 104 then -- 104 = h = hole
        world:CreateComponentAndAddTo("Void", entity)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("hole_test", "hole")
--  No need???
--    elseif tiletype == 46 then -- 46 = . = grass
--        world:CreateComponentAndAddTo("", entity)

    elseif tiletype == 120 then -- 120 = x = stone
		world:CreateComponentAndAddTo("NotWalkable", entity)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("stone", "stone")
		posComp:SetFloat3(posX, 1.0, posZ)
		
		self:AddGroundTileBelow(posX, posZ)

    elseif tiletype == 49 then -- 49 = 1 = first checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(1)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("checkpoint", "checkpoint")
		--posComp:SetFloat3(posX, 1.0, posZ)

    elseif tiletype == 50 then -- 50 = 2 = second checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(2)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("checkpoint", "checkpoint")
		--posComp:SetFloat3(posX, 1.0, posZ)

    elseif tiletype == 51 then -- 51 = 3 = third checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(3)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("checkpoint", "checkpoint")
		--posComp:SetFloat3(posX, 1.0, posZ)

    elseif tiletype == 52 then -- 52 = 4 = fourth checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(4)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("checkpoint", "checkpoint")
		--posComp:SetFloat3(posX, 1.0, posZ)

    elseif tiletype == 117 then -- 117 = u = water up
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(0, -1)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, -math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")

    elseif tiletype == 100 then -- 100 = d = water down
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(0, 1)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")

    elseif tiletype == 108 then -- 108 = l = water left
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(-1, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")

    elseif tiletype == 114 then -- 114 = r = water right
        world:CreateComponentAndAddTo("River", entity)
        local comp = self:GetComponent(entity, "River", 0)
        comp:SetInt2(1, 0)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, math.pi, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")
		
	else
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("grass", "grass")
		
    end

    table.insert(self.entities, entity)
end

MapSystem.AddGroundTileBelow = function(self, posX, posZ)

	local groundEntity = world:CreateNewEntity("Tile")
	local posComp = self:GetComponent(groundEntity, "Position", 0)
	posComp:SetFloat3(posX, 0.0, posZ)
	
	world:CreateComponentAndAddTo("MapPosition", groundEntity)
	local mapPosComp = self:GetComponent(groundEntity, "MapPosition", 0)
	mapPosComp:SetInt2(posX, posZ)
	
	world:CreateComponentAndAddTo("Model", groundEntity)
	local comp = self:GetComponent(groundEntity, "Model", 0)
	comp:SetModel("grass", "grass")
	
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
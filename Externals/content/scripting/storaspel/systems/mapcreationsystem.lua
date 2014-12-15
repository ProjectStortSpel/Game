MapCreationSystem = System()
MapCreationSystem.entities = { }
MapCreationSystem.mapX = 0
MapCreationSystem.mapY = 0

MapCreationSystem.PostInitialize = function(self)
	local map
    self.mapX, self.mapY, map = File.LoadMap("content/maps/map.txt")
    local posX, posZ
	
	self:AddPlayers()
	
	for y = 0, self.mapY-1 do
        for x = 0, self.mapX-1 do
            posX = x - self.mapX/2
            posZ = y - self.mapY/2
            self:AddTile(x, y, map[y * self.mapX + x + 1])
        end
    end

    print("Init map done!")
end

MapCreationSystem.AddPlayers = function(self)
	
	for i = 3, 9, 2 do
	--for i = 2, 4, 1 do
		local entity = world:CreateNewEntity("Player")
		world:CreateComponentAndAddTo("Spawn", entity)
		world:CreateComponentAndAddTo("SyncNetwork", entity)
		local mapPos = {i,12}
		--local mapPos = {i, 6}
		self:SetPosition(entity, mapPos[1], 1.0, mapPos[2])
		local comp = self:GetComponent(entity, "Spawn", 0)
		comp:SetInt2(mapPos[1], mapPos[2])
	end
end

MapCreationSystem.AddTile = function(self, posX, posZ, tiletype)
	
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
        world:CreateComponentAndAddTo("Water", entity)
        local comp = self:GetComponent(entity, "Water", 0)
        comp:SetInt2(0, 1)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, -math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")

    elseif tiletype == 100 then -- 100 = d = water down
        world:CreateComponentAndAddTo("Water", entity)
        local comp = self:GetComponent(entity, "Water", 0)
        comp:SetInt2(0, -1)
		local comp = self:GetComponent(entity, "Rotation", 0)
		comp:SetFloat3(0, math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")

    elseif tiletype == 108 then -- 108 = l = water left
        world:CreateComponentAndAddTo("Water", entity)
        local comp = self:GetComponent(entity, "Water", 0)
        comp:SetInt2(-1, 0)
		world:CreateComponentAndAddTo("Model", entity)
		local comp = self:GetComponent(entity, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight")

    elseif tiletype == 114 then -- 114 = r = water right
        world:CreateComponentAndAddTo("Water", entity)
        local comp = self:GetComponent(entity, "Water", 0)
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

MapCreationSystem.AddGroundTileBelow = function(self, posX, posZ)

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

MapCreationSystem.SetPosition = function(self, entity, posX, posY, posZ)
	local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
    local posComp = self:GetComponent(entity, "Position", 0)
    mapPosComp:SetInt2(posX, posZ)
    posComp:SetFloat3(posX, posY, posZ)
	
end
MapCreationSystem = System()

MapCreationSystem.PostInitialize = function(self)
    print("Started to initialize map!")
    local mapX, mapY, map = File.LoadMap("content/maps/map.txt")
    local posX, posZ

    for y = 1, mapY do
        for x = 1, mapX do
            posX = x - mapX/2
            posZ = y - mapY/2
            self:AddTile(posX, posZ, x, y, map[y * mapX + x])
        end
    end

    print("Init map done!")
end

MapCreationSystem.AddTile = function(self, posX, posZ, mapPosX, mapPosY, tiletype)
    local entity = world:CreateNewEntity("Tile")

    local posComp = self:GetComponent(entity, "Position", 0)
    posComp:SetFloat3(posX, 0.0, posZ)

    world:CreateComponentAndAddTo("MapPosition", entity)
    local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
    mapPosComp:SetInt2(mapPosX, mapPosY)


--    if tiletype == 111 then -- 111 = o = out
--        world:CreateComponentAndAddTo("Void", entity)

--    elseif tiletype == 104 then -- 104 = h = hole
--        world:CreateComponentAndAddTo("Void", entity)

    if tileType == 111 or tileType == 104 then
        world:CreateComponentAndAddTo("Void", entity)

--  No need???
--    elseif tiletype == 46 then -- 46 = . = grass
--        world:CreateComponentAndAddTo("", entity)

    elseif tiletype == 120 then -- 120 = x = stone
        world:CreateComponentAndAddTo("NotWalkable", entity)

    elseif tiletype == 49 then -- 49 = 1 = first checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(1)

    elseif tiletype == 50 then -- 50 = 2 = second checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(2)

    elseif tiletype == 51 then -- 51 = 3 = third checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(3)

    elseif tiletype == 52 then -- 52 = 4 = fourth checkpoint
        world:CreateComponentAndAddTo("Checkpoint", entity)
        local comp = self:GetComponent(entity, "Checkpoint", 0)
        comp:SetInt(4)

    elseif tiletype == 117 then -- 117 = u = water up
        world:CreateComponentAndAddTo("WaterDirection", entity)
        local comp = self:GetComponent(entity, "WaterDirection", 0)
        comp:SetInt2(0, 1)

    elseif tiletype == 100 then -- 100 = d = water down
        world:CreateComponentAndAddTo("WaterDirection", entity)
        local comp = self:GetComponent(entity, "WaterDirection", 0)
        comp:SetInt2(0, -1)

    elseif tiletype == 108 then -- 108 = l = water left
        world:CreateComponentAndAddTo("WaterDirection", entity)
        local comp = self:GetComponent(entity, "WaterDirection", 0)
        comp:SetInt2(-1, 0)

    elseif tiletype == 114 then -- 114 = r = water right
        world:CreateComponentAndAddTo("WaterDirection", entity)
        local comp = self:GetComponent(entity, "WaterDirection", 0)
        comp:SetInt2(1, 0)

    end


    table.insert(self.entities, entity)
end
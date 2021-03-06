CreateMapSystem = System()
CreateMapSystem.entities = nil
CreateMapSystem.waterTiles = nil
CreateMapSystem.mapX = 0
CreateMapSystem.mapY = 0
CreateMapSystem.noOfSpawnpoints = 0
CreateMapSystem.noOfCheckpoints = 0
CreateMapSystem.filePath = "content/maps/"

CreateMapSystem.Initialize = function(self)
	self:SetName("CreateMapSystem")
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter("CreateMap", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	Console.AddCommand("loadmap", self.LoadMap)
end

CreateMapSystem.PostInitialize = function(self)
	--self:CreateMap("content/maps/map.txt")
end

CreateMapSystem.EntitiesAdded = function(self, dt, entities)
	for i = 1, #entities do
		local entity = entities[i]
		if world:EntityHasComponent(entity, "CreateMap") then
			local name = world:GetComponent(entity, "CreateMap", 0):GetText()
			self:CreateMap(self.filePath .. name .. ".txt")
			world:KillEntity(entity)
		end
	end
end

CreateMapSystem.AddTile = function(self, posX, posZ, tiletype)
	
    local newTile = world:CreateNewEntity("Tile")
    local posComp = world:GetComponent(newTile, "Position", 0)
    posComp:SetFloat3(posX, 0.0, posZ)
    local mapPosComp = world:GetComponent(newTile, "MapPosition", 0)
	
	local check = ((posX + posZ) % 2)
	world:GetComponent(newTile, "Color", "X"):SetFloat(check*0.75)
	world:GetComponent(newTile, "Color", "Y"):SetFloat(check)
	world:GetComponent(newTile, "Color", "Z"):SetFloat(check*0.75)
	
    mapPosComp:SetInt2(posX, posZ)
    if tiletype == 104 then -- 104 = h = hole
        world:CreateComponentAndAddTo("Void", newTile)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("hole", "hole", 0)
		local rotComp = world:GetComponent(newTile, "Rotation", 0)
		rotComp:SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
		
    elseif tiletype == 120 then -- 120 = x = stone
		world:CreateComponentAndAddTo("NotWalkable", newTile)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("bush", "bush", 0)
		posComp:SetFloat3(posX, 0.5, posZ) --posComp:SetFloat3(posX, 0.8 + 0.1* math.random(-1, 1), posZ)
		local scaleComp = world:GetComponent(newTile, "Scale", 0)
		scaleComp:SetFloat3(0.75,0.75,0.75)
		local rotComp = world:GetComponent(newTile, "Rotation", 0)
		rotComp:SetFloat3(math.pi * 0.01 * math.random(0, 25), math.pi * 0.01 * math.random(0, 100), math.pi * 0.01 * math.random(0, 25))
		self:AddGroundTileBelow(posX, posZ)
    elseif tiletype >= 49 and tiletype <= 57 then -- 49 = 1 = first checkpoint, 57 = 9 = 9th checkpoint

        world:CreateComponentAndAddTo("Checkpoint", newTile)
        local comp = world:GetComponent(newTile, "Checkpoint", 0)
        comp:SetInt(tiletype - 48)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		
		comp:SetModel("checkpoint", "checkpoint", 0)
		
		self.noOfCheckpoints = self.noOfCheckpoints + 1

		--posComp:SetFloat3(posX, 0.5, posZ)

    elseif tiletype == 102 then -- 102 = f = finish
        world:CreateComponentAndAddTo("Finishpoint", newTile)
		world:CreateComponentAndAddTo("Checkpoint", newTile)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("finish", "finish", 0)
		--posComp:SetFloat3(posX, 1.0, posZ)
		
		self.noOfCheckpoints = self.noOfCheckpoints + 1

    elseif tiletype == 117 then -- 117 = u = water up
        world:CreateComponentAndAddTo("River", newTile)
        local comp = world:GetComponent(newTile, "River", 0)
        comp:SetInt3(0, -1, 1)
		local comp = world:GetComponent(newTile, "Rotation", 0)
		comp:SetFloat3(0, -math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)

		world:GetComponent(newTile, "TileOffset", "Offset"):SetFloat(0.2)
		
		self.waterTiles[#self.waterTiles+1]=newTile
		
    elseif tiletype == 100 then -- 100 = d = water down
        world:CreateComponentAndAddTo("River", newTile)
        local comp = world:GetComponent(newTile, "River", 0)
        comp:SetInt3(0, 1, 1)
		local comp = world:GetComponent(newTile, "Rotation", 0)
		comp:SetFloat3(0, math.pi/2, 0)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)

		world:GetComponent(newTile, "TileOffset", "Offset"):SetFloat(0.2)
		
		self.waterTiles[#self.waterTiles+1]=newTile
		
    elseif tiletype == 108 then -- 108 = l = water left
        world:CreateComponentAndAddTo("River", newTile)
        local comp = world:GetComponent(newTile, "River", 0)
        comp:SetInt3(-1, 0, 1)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)

		world:GetComponent(newTile, "TileOffset", "Offset"):SetFloat(0.2)
		
		self.waterTiles[#self.waterTiles+1]=newTile
		
    elseif tiletype == 114 then -- 114 = r = water right
        world:CreateComponentAndAddTo("River", newTile)
        local comp = world:GetComponent(newTile, "River", 0)
        comp:SetInt3(1, 0, 1)
		local comp = world:GetComponent(newTile, "Rotation", 0)
		comp:SetFloat3(0, math.pi, 0)
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("riverstraight", "riverstraight", 0)
		
		world:GetComponent(newTile, "TileOffset", "Offset"):SetFloat(0.2)

		self.waterTiles[#self.waterTiles+1]=newTile
		
	elseif tiletype == 115 then -- 115 = s = Available spawn point
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("grass", "grass", 0, 0)
		
		
		local newSpawnId = world:CreateNewEntity()
		world:CreateComponentAndAddTo("AvailableSpawnpoint", newSpawnId)
		local newSpawn = world:GetComponent(newSpawnId, "AvailableSpawnpoint", 0)
		newSpawn:SetInt2(posX, posZ)
				
		self.noOfSpawnpoints = self.noOfSpawnpoints + 1
		
	elseif tiletype == 46 then -- 46 = . = grass
		world:CreateComponentAndAddTo("Model", newTile)
		local comp = world:GetComponent(newTile, "Model", 0)
		comp:SetModel("grass", "grass", 0)
		self:AddTallGrass(posX, posZ)
		self:AddTinyStone(posX, posZ)
		self:AddTree(posX, posZ)
	elseif tiletype == 111 then
        world:CreateComponentAndAddTo("Void", newTile)
		--world:CreateComponentAndAddTo("Model", entity)
		--local comp = world:GetComponent(entity, "Model", 0)
		--comp:SetModel("grass", "grass", 0)
	else
		print("ERROR: TILETYPE NOT DEFINED IN sv_CreateMapSystem. Ascii = ", tiletype, "at:", posX, posZ)
    end
	
	self.entities[#self.entities+1]=newTile
	local entity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("DirectionalLight", entity)
	world:CreateComponentAndAddTo("SyncNetwork", entity)
    local directionalLight = world:GetComponent(entity, "DirectionalLight", 0)
	directionalLight:SetDirectionalLight(-0.38, -1.0, 0.7, 0.3, 0.7, 0.7, 0.7, 0.75, 0.85)
	return newTile
end

-- Does not use the template as this tile is visual only and therefore should not receive the TileComp component. If it does then the array of entities used in the map will be wrong.
CreateMapSystem.AddGroundTileBelow = function(self, posX, posZ)

	local groundEntity = world:CreateNewEntity()
	
	world:CreateComponentAndAddTo("Position", groundEntity)
	world:CreateComponentAndAddTo("Rotation", groundEntity)
	world:CreateComponentAndAddTo("Scale", groundEntity)
	world:CreateComponentAndAddTo("MapPosition", groundEntity)
	world:CreateComponentAndAddTo("SyncNetwork", groundEntity)
	world:CreateComponentAndAddTo("Model", groundEntity)
	
	world:GetComponent(groundEntity, "Position", 0):SetFloat3(posX, 0.0, posZ)
	world:GetComponent(groundEntity, "MapPosition", 0):SetInt2(posX, posZ)
	world:GetComponent(groundEntity, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(groundEntity, "Scale", 0):SetFloat3(1.0, 1.0, 1.0)
	world:GetComponent(groundEntity, "Model", 0):SetModel("grass", "grass", 0)
	
end 

CreateMapSystem.AddTree = function(self, posX, posZ)
	if (math.random(1, 10) > 1) then
		return
	end
	
	local tinyStone = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", tinyStone)
	world:CreateComponentAndAddTo("Rotation", tinyStone)
	world:CreateComponentAndAddTo("Scale", tinyStone)
	world:CreateComponentAndAddTo("SyncNetwork", tinyStone)
	world:CreateComponentAndAddTo("Model", tinyStone)
	
	local randX = posX+(math.random(0, 1)-0.5)*0.8
	local randZ = posZ+(math.random(0, 1)-0.5)*0.8
	world:GetComponent(tinyStone, "Position", 0):SetFloat3(randX, 0.5, randZ)
	world:GetComponent(tinyStone, "Rotation", 0):SetFloat3(math.pi * 0.01 * math.random(0, 10), math.pi * 0.01 * math.random(0, 100), math.pi * 0.01 * math.random(0, 10))
	local randScale = math.random() + 0.5
	world:GetComponent(tinyStone, "Scale", 0):SetFloat3(1, randScale, 1)
	world:GetComponent(tinyStone, "Model", 0):SetModel("tree", "tree", 8)
end 

CreateMapSystem.AddTallGrass = function(self, posX, posZ)
	if (math.random(1, 4) > 1) then
		return
	end
	
	local tallGrass = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", tallGrass)
	world:CreateComponentAndAddTo("Rotation", tallGrass)
	world:CreateComponentAndAddTo("Scale", tallGrass)
	world:CreateComponentAndAddTo("SyncNetwork", tallGrass)
	world:CreateComponentAndAddTo("Model", tallGrass)
	
	local randX = posX-0.5+math.random()
	local randZ = posZ-0.5+math.random()
	world:GetComponent(tallGrass, "Position", 0):SetFloat3(randX, 0.5, randZ)
	world:GetComponent(tallGrass, "Rotation", 0):SetFloat3(0, 0, 0)
	local randScale = math.random() + 0.5
	world:GetComponent(tallGrass, "Scale", 0):SetFloat3(0.15*randScale, 0.15*randScale, 0.15*randScale)
	world:GetComponent(tallGrass, "Model", 0):SetModel("tallgrass", "tallgrass", 9)
end 

CreateMapSystem.AddTinyStone = function(self, posX, posZ)
	if (math.random(1, 5) > 1) then
		return
	end
	
	local tinyStone = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", tinyStone)
	world:CreateComponentAndAddTo("Rotation", tinyStone)
	world:CreateComponentAndAddTo("Scale", tinyStone)
	world:CreateComponentAndAddTo("SyncNetwork", tinyStone)
	world:CreateComponentAndAddTo("Model", tinyStone)
	
	local randX = posX-0.5+math.random()
	local randZ = posZ-0.5+math.random()
	world:GetComponent(tinyStone, "Position", 0):SetFloat3(randX, 0.5, randZ)
	world:GetComponent(tinyStone, "Rotation", 0):SetFloat3(math.pi * 0.01 * math.random(0, 25), math.pi * 0.01 * math.random(0, 100), math.pi * 0.01 * math.random(0, 25))
	local randScale = math.random() + 0.5
	world:GetComponent(tinyStone, "Scale", 0):SetFloat3(0.15*randScale, 0.15*randScale, 0.15*randScale)
	world:GetComponent(tinyStone, "Model", 0):SetModel("smallstone", "smallstone", 8)
end 

CreateMapSystem.CreateMap = function(self, name)
	self.entities = { }
	self.entities.__mode = "k"
	self.waterTiles = { }
	self.waterTiles.__mode = "k"
	
	local inputData = InputData()
	local map
    self.mapX, self.mapY, map = File.LoadMap(name)

		
	for x = 0, self.mapX + 1 do
		self:AddTile(x, 0, 111) -- 111 = void
		inputData:AddTile( 1, true )
	end
	
	local highestCP = 0
	local finishList = { }
	finishList.__mode = "k"
	
	for y = 1, self.mapY do
		self:AddTile(0, y, 111) -- 111 = void
		inputData:AddTile( 1, true )
		for x = 1, self.mapX do
			local tiletype = map[(y - 1) * self.mapX + x]		
            local entity = self:AddTile(x, y, tiletype)
			if tiletype == 120  or tiletype == 104 or tiletype == 111
			then
				inputData:AddTile( 1, false )
			else
				inputData:AddTile( 1, true )
			end
			if tiletype >= 49 and tiletype <= 57 then -- 49 = 1 = first checkpoint, 57 = 9 = 9th checkpoint

				highestCP = math.max(highestCP, tiletype - 48)

			elseif tiletype == 102 then -- 102 = f = finish
				
				finishList[#finishList + 1] = entity

			end
        end
		self:AddTile(self.mapX + 1, y, 111) -- 111 = void
		inputData:AddTile( 1, true )
    end

	for i = 1, #finishList do

		local comp = world:GetComponent(finishList[i], "Checkpoint", 0)
        comp:SetInt(highestCP + 1)

	end
	
	for x = 0, self.mapX + 1 do
		self:AddTile(x, self.mapY+1, 111) -- 111 = void
		inputData:AddTile( 1, true )
	end
	
	
	-- Create an entity that will keep track of the map size. If it already exist from a previous "loadmap", use that one instead.
	local mapSpecsEntity = self:GetEntities("MapSpecs")
	local mapEntity = nil
	if #mapSpecsEntity == 0 then
		mapEntity = world:CreateNewEntity()
		world:CreateComponentAndAddTo("MapSpecs", mapEntity)
		world:CreateComponentAndAddTo("SyncNetwork", mapEntity)
	else
		mapEntity = mapSpecsEntity[1]
	end
	
	-- Add to the map size as voids have been added around the map.
	self.mapX = self.mapX + 2
	self.mapY = self.mapY + 2
	
	inputData:SetSize(self.mapX, self.mapY)
	
	PathfinderHandler.SetData(inputData)
	PotentialFieldHandler.InitPFHandler(self.mapX, self.mapY, self.noOfSpawnpoints)
	
	-- Set size of the map.
	world:GetComponent(mapEntity, "MapSpecs", 0):SetInt4(self.noOfCheckpoints, self.noOfSpawnpoints, self.mapX, self.mapY)
	
	print("MapSize:", self.mapX, self.mapY)
	print("Spawnpoints:", self.noOfSpawnpoints)
	print("Checkpoints:", self.noOfCheckpoints)
	
	for waterA = 1, #self.waterTiles do
		
		local waterPosA = world:GetComponent(self.waterTiles[waterA], "MapPosition", 0)
		local waterDirA = world:GetComponent(self.waterTiles[waterA], "River", 0)
		local posAX, posAY = waterPosA:GetInt2()
		local dirAX, dirAY = waterDirA:GetInt2()


		for waterB = 1, #self.waterTiles do

			if waterA ~= waterB then
				local waterPosB = world:GetComponent(self.waterTiles[waterB], "MapPosition", 0)
				local waterDirB = world:GetComponent(self.waterTiles[waterB], "River", 0)
				local posBX, posBY = waterPosB:GetInt2()
				local dirBX, dirBY = waterDirB:GetInt2()

				if posAX + dirAX == posBX and posAY + dirAY == posBY then
					if dirAX ~= dirBX and dirAY ~= dirBY then

						local comp = world:GetComponent(self.waterTiles[waterB], "Model", 0)
						comp:SetModel("rivercorner", "rivercorner", 0, 0)

						--	LEFT TURN (Correct rotation)
						--if dirAX == 1 and dirBY == -1 then
						--elseif dirAX == -1 and dirBY == 1 then
						--elseif dirAY == -1 and dirBX == -1 then
						--elseif dirAY == 1 and dirBX == 1 then

						local comp = world:GetComponent(self.waterTiles[waterB], "Rotation", 0)
						local currentRotation = comp:GetFloat(1)
						
						world:CreateComponentAndAddTo("RiverCornerDir", self.waterTiles[waterB])
						
						print("RiverCornerDir Created: " .. self.waterTiles[waterB])
						
						--	RIGHT TURN
						if dirAX == 1 and dirBY == 1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(self.waterTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						elseif dirAX == -1 and dirBY == -1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(self.waterTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						elseif dirAY == 1 and dirBX == -1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(self.waterTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						elseif dirAY == -1 and dirBX == 1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(self.waterTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
							--	LEFT TURN
						else
							world:GetComponent(self.waterTiles[waterB], "RiverCornerDir", "Dir"):SetText("Left")
						end

					end
				end
			end
		end
	end

	finishList = nil
	self.waterTiles = nil
	self.entities = nil
	collectgarbage()
end

CreateMapSystem.LoadMap = function (_command, ...)
	local args = { ... }
	if #args == 1 then
		if type(args[1]) == "string" then
			print( args[1] )
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("CreateMap", id)
			world:GetComponent(id, "CreateMap", 0):SetText(args[1])
		end
	else
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CreateMap", id)
		world:GetComponent(id, "CreateMap", 0):SetText("content/maps/smallmap.txt")
	end
end

MapGenerator = System()
MapGenerator.DebugInfo 			=	false
MapGenerator.MapSizeX 			= 	0
MapGenerator.MapSizeZ 			= 	0
MapGenerator.Players 			= 	0
MapGenerator.Checkpoints 		= 	0
MapGenerator.Rivers				=	0
MapGenerator.MapTiles 			= 	{}
MapGenerator.MapTiles.__mode 	= 	"k"
MapGenerator.TileTypes 			= 	{}
MapGenerator.TileTypes.__mode 	= 	"k"
MapGenerator.abc = 1

--	All types
MapGenerator.Void		=	0
MapGenerator.Grass		=	1
MapGenerator.Stone		=	2
MapGenerator.Hole		=	3
MapGenerator.River		=	4
MapGenerator.Checkpoint	=	5
MapGenerator.Spawnpoint	=	6

MapGenerator.Initialize = function(self)
	self:SetName("MapGenerator")
	
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter("GenerateMap", FilterType.RequiresOneOf)
end

MapGenerator.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	self:GenerateMap(1651654, 4, 4)
end

MapGenerator.PostInitialize = function(self)
	local	lol	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("GenerateMap", lol)
end

MapGenerator.GenerateMap = function(self, randSeed, numberOfPlayers, numberOfCheckpoints)
	--	Set the seed
	math.randomseed(randSeed)
	
	--	Randomize some initial values
	self.MapSizeX		=	math.random(5 + numberOfPlayers, 7 + 2*numberOfPlayers) + 2
	self.MapSizeZ		=	math.random(5 + numberOfPlayers, 7 + 2*numberOfPlayers) + 2
	self.Players		=	numberOfPlayers
	self.Checkpoints	=	numberOfCheckpoints
	self.Rivers			=	math.random(1, math.floor(numberOfPlayers*numberOfPlayers/3))
	print("----- MAP INFO -----")
	print("Players: " .. numberOfPlayers)
	print("Checkpoints: " .. numberOfCheckpoints)
	print("Map Size: " .. self.MapSizeX .. "x" .. self.MapSizeZ)
	
	for X = 1, self.MapSizeX do
		for Z = 1, self.MapSizeZ do
			self.MapTiles[#self.MapTiles+1] 	= 	-1
			self.TileTypes[#self.TileTypes+1]	=	self.Void
		end
	end
	
	--	Generate a empty plane
	self:GeneratePlane()
	
	--	Carve away tiles around the map
	self:CarveEdges()
	
	self:GenerateRivers()
	
	for Z = 1, self.MapSizeZ do
		for X = 1, self.MapSizeX do
			if self:GetTileType(X-1, Z-1) == self.Void then
				self:RemoveTile(X-1, Z-1)
				local	newTile	=	self:GenerateEmptyTile(X-1, Z-1)
				world:CreateComponentAndAddTo("Void", newTile)
			end
		end
	end
	
	for X = 1, self.MapSizeX do
		self:RemoveTile(X-1, 0)
		local	newTile	=	self:GenerateEmptyTile(X-1, 0)
		world:CreateComponentAndAddTo("Void", newTile)
		
		self:RemoveTile(X-1, self.MapSizeZ-1)
		newTile	=	self:GenerateEmptyTile(X-1, self.MapSizeZ-1)
		world:CreateComponentAndAddTo("Void", newTile)
	end
	
	for Z = 1, self.MapSizeZ do
		self:RemoveTile(0, Z-1)
		local	newTile	=	self:GenerateEmptyTile(0, Z-1)
		world:CreateComponentAndAddTo("Void", newTile)
		
		self:RemoveTile(self.MapSizeX-1, Z-1)
		newTile	=	self:GenerateEmptyTile(self.MapSizeX-1, Z-1)
		world:CreateComponentAndAddTo("Void", newTile)
	end
	

	
	--	Remove tiles that are floating alone
	self:DeleteEmptyTiles()
	
	
	
	--	Run it multiple times to fill all gaps
	for n = 1, 5 do
		self:GenerateHoles()
	end
	
	self:PlaceCheckpoints()
	self:PlaceSpawnpoints()
	
	self:PostFixStones()
	
	self:GeneratePathfindingData()
	
	--	Spawn info entity
	local	dataEntity	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("MapSpecs", dataEntity)
	world:CreateComponentAndAddTo("SyncNetwork", dataEntity)
	world:GetComponent(dataEntity, "MapSpecs", "NoOfSpawnpoints"):SetInt(self.Players)
	world:GetComponent(dataEntity, "MapSpecs", "SizeX"):SetInt2(self.MapSizeX, self.MapSizeZ)
	
	self:PrintMap()
	collectgarbage()
end

MapGenerator.PrintMap = function(self)

	for Z = 1, self.MapSizeZ do
		local	rowData	=	""
		
		for X = 1, self.MapSizeX do
			local	tileType	=	self:GetTileType(X-1, Z-1)
			
			if tileType == self.Grass then
				rowData	=	rowData .. "."
			elseif tileType == self.Stone then
				rowData	=	rowData .. "x"
			elseif tileType == self.Hole then
				rowData	=	rowData .. "h"
			elseif tileType == self.River then
				rowData	=	rowData .. "d"
			elseif tileType == self.Checkpoint then
				rowData	=	rowData .. "1"
			elseif tileType == self.Spawnpoint then
				rowData	=	rowData .. "s"
			else
				rowData	=	rowData .. "o"
			end
				
		end
		print(rowData)
	end
end

MapGenerator.GetListIndex = function(self, X, Z)
	return (Z * self.MapSizeX + X + 1)
end

MapGenerator.GetEntityId = function(self, X, Z)
	if X < 0 or X >= self.MapSizeX then
		return -1
	end
	
	if Z < 0 or Z >= self.MapSizeZ then
		return -1
	end
	
	return self.MapTiles[self:GetListIndex(X,Z)]
end

MapGenerator.GetTileType = function(self, X, Z)
	if X < 0 or X >= self.MapSizeX then
		return -1
	end
	
	if Z < 0 or Z >= self.MapSizeZ then
		return -1
	end
	
	return self.TileTypes[self:GetListIndex(X,Z)]
end

MapGenerator.RemoveTile = function(self, X, Z)
	local	tIndex 	= 	self:GetListIndex(X, Z)
	if self.MapTiles[tIndex] ~= -1 then
		if self:GetTileType(X, Z) == self.River then
			print("River: " .. self.abc)
			self.abc = self.abc + 1
		end
		world:KillEntity(self.MapTiles[tIndex])
		self.MapTiles[tIndex] 	= 	-1
		self.TileTypes[tIndex]	=	self.Void
	end
end

MapGenerator.GenerateEmptyTile = function(self, X, Z)

	--	Create the tile entity
	local	newTile	=	world:CreateNewEntity("Tile")
	
	--	Set its starting position
	world:GetComponent(newTile, "Position", "X"):SetFloat3(X, 0.0, Z)
	world:GetComponent(newTile, "MapPosition", "X"):SetInt2(X, Z)
	
	self.MapTiles[self:GetListIndex(X,Z)]	=	newTile
	
	--	Return ID
	return newTile
end

MapGenerator.GeneratePlane = function(self)

	for X = 1, self.MapSizeX do
		
		for Z = 1, self.MapSizeZ do
			local	tileId	=	self:GenerateEmptyTile(X-1, Z-1)
			world:CreateComponentAndAddTo("Model", tileId)
			local comp = world:GetComponent(tileId, "Model", 0)
			comp:SetModel("grass", "grass", 0, 0)

			self.TileTypes[self:GetListIndex(X-1, Z-1)]	=	self.Grass
		end
		
	end

end

MapGenerator.CarveEdges = function(self)
	
	--	Calculate how many tiles to carve away from the island
	local	maxNumberOfCarves	=	math.ceil(math.random(math.ceil(self.MapSizeX*self.MapSizeZ/8), math.ceil(self.MapSizeX*self.MapSizeZ/6)))
	print("Carves: " .. maxNumberOfCarves)
	--	Begin carving...
	local	carveTries	=	0
	while true do
		
		--	Decide what side to carve	-1 is Left/Up, 1 is Right/Down
		local	carveX	=	(-1)^(math.random(1, 10))
		local	carveZ	=	(-1)^(math.random(1, 10))
		
		--	Try a random location
		local	tempX	=	math.random(0, math.random(1, math.ceil(maxNumberOfCarves/4)))
		local	tempZ	=	math.random(0, math.random(1, math.ceil(maxNumberOfCarves/4)))
		if carveX == 1 then
			tempX	=	self.MapSizeX - tempX - 1
		end
		if carveZ == 1 then
			tempZ	=	self.MapSizeZ - tempZ - 1
		end
		
		if self:GetTileType(tempX, tempZ) ~= self.Void then
			self:RemoveTile(tempX, tempZ)
			maxNumberOfCarves = maxNumberOfCarves - 1
			carveTries	=	0
		else
			carveTries	=	carveTries + 1
		end
		
		--	Break if there are too many tries on one carve
		if carveTries > 5 then
			maxNumberOfCarves = maxNumberOfCarves - 1
			carveTries	=	0
		end
		
		--	Break after each carve has been tried/done
		if maxNumberOfCarves <= 0 then
			break
		end
	end
	
	
end

MapGenerator.NumberOfNeighbours = function(self, X, Z)
	local	nNeighbours	=	0
	if X > 0 then
		if self:GetTileType(X-1, Z) ~= self.Void then
			nNeighbours	=	nNeighbours+1
		end
	end
	
	if Z > 0 then
		if self:GetTileType(X, Z-1) ~= self.Void then
			nNeighbours	=	nNeighbours+1
		end
	end
	
	if X < self.MapSizeX-1 then
		if self:GetTileType(X+1, Z) ~= self.Void then
			nNeighbours	=	nNeighbours+1
		end
	end
	
	if Z < self.MapSizeZ-1 then
		if self:GetTileType(X, Z+1) ~= self.Void then
			nNeighbours	=	nNeighbours+1
		end
	end

	
	return nNeighbours
end

MapGenerator.DeleteEmptyTiles = function(self)

	for X = 0, self.MapSizeX-1 do
		for Z = 0, self.MapSizeZ-1 do
			if self:GetTileType(X, Z) ~= self.Void then
				if self:NumberOfNeighbours(X, Z) == 0 then
					self:RemoveTile(X, Z)
				end
			end
		end
	end
end

MapGenerator.GenerateHole = function(self, X, Z)

	local	newHole	=	self:GenerateEmptyTile(X, Z)
	self.TileTypes[self:GetListIndex(X, Z)]	=	self.Hole
	world:CreateComponentAndAddTo("Void", newHole)
	world:CreateComponentAndAddTo("Model", newHole)
	world:GetComponent(newHole, "Model", 0):SetModel("hole", "hole", 0)
	world:GetComponent(newHole, "Rotation", 0):SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
end

MapGenerator.GenerateHoles = function(self)
	
	for X = 0, self.MapSizeX-1 do
		for Z = 0, self.MapSizeZ-1 do
			if self:GetTileType(X, Z) == self.Void then
				if self:NumberOfNeighbours(X, Z) >= 3 then
					self:GenerateHole(X,Z)
				end
			end
		end
	end
end

MapGenerator.DistanceBetween = function(self, X1, Z1, X2, Z2)
	return math.sqrt((X2-X1)^2 + (Z2-Z1)^2)
end

--	RIVER METHODS
--	All methods for creating the rivers will
--	be here
MapGenerator.GetRiverDirection = function(self, DirectionIndex)
	
	--	Up
	if DirectionIndex == 1 then
		return 0, -1
		
	--	Right
	elseif DirectionIndex == 2 then
		return 1, 0
		
	--	Down
	elseif DirectionIndex == 3 then
		return 0, 1
		
	--	Left
	elseif DirectionIndex == 4 then
		return -1, 0
	end
	
	
	--	Undefined
	return 0, 0
end

MapGenerator.CreateRiverTile = function(self, X, Z, DirectionIndex)
	
	--	Get the tile
	local	riverId	=	self:GetEntityId(X, Z)
	if riverId == -1 then
		riverId	=	self:GenerateEmptyTile(X, Z)
	end
	
	self.TileTypes[self:GetListIndex(X, Z)]	=	self.River
	
	--	Get the direction of the river
	local	tRiverDirX, tRiverDirZ	=	self:GetRiverDirection(DirectionIndex)
	
	--	Create the river component
	if not world:EntityHasComponent(riverId, "River") then
		world:CreateComponentAndAddTo("River", riverId)
	end
	world:GetComponent(riverId, "River", 0):SetInt3(tRiverDirX, tRiverDirZ, 1)
	
	
	--	Set the model
	if not world:EntityHasComponent(riverId, "Model") then
		world:CreateComponentAndAddTo("Model", riverId)
	end
	world:GetComponent(riverId, "Model", 0):SetModel("riverstraight", "riverstraight", 0)
	
	--	Set correct rotation
	if DirectionIndex == 1 then
		world:GetComponent(riverId, "Rotation", 0):SetFloat3(0, -math.pi/2, 0)
	elseif DirectionIndex == 2 then
		world:GetComponent(riverId, "Rotation", 0):SetFloat3(0, math.pi, 0)
	elseif DirectionIndex == 3 then
		world:GetComponent(riverId, "Rotation", 0):SetFloat3(0, math.pi/2, 0)
	elseif DirectionIndex == 4 then
		--	Nothing
	end
	
	if self.DebugInfo then
		print("River with direction(" .. DirectionIndex .. ") [" .. tRiverDirX .. ", " .. tRiverDirZ .. "] created!")
	end
end

MapGenerator.FixRiverCorner = function(self, RiverA, RiverB)

	if RiverA == -1 or RiverB == -1 then
		return
	end
	
	--	Set the model for the corner
	world:GetComponent(RiverB, "Model", 0):SetModel("rivercorner", "rivercorner", 0, 0)
	
	local 	dirAX, dirAY 	= 	world:GetComponent(RiverA, "River", 0):GetInt2()
	local 	dirBX, dirBY 	= 	world:GetComponent(RiverB, "River", 0):GetInt2()
	
	local 	rotComp 			= 	world:GetComponent(RiverB, "Rotation", 0)
	local 	currentRotation 	= 	rotComp:GetFloat(1)
	
	if dirAX == 1 and dirBY == 1 then
		rotComp:SetFloat3(0, currentRotation - math.pi/2, 0)
	elseif dirAX == -1 and dirBY == -1 then
		rotComp:SetFloat3(0, currentRotation - math.pi/2, 0)
	elseif dirAY == 1 and dirBX == -1 then
		rotComp:SetFloat3(0, currentRotation - math.pi/2, 0)
	elseif dirAY == -1 and dirBX == 1 then
		rotComp:SetFloat3(0, currentRotation - math.pi/2, 0)
	end
end

MapGenerator.CarveRiver = function(self, X, Z)
	
	if self:GetTileType(X, Z) == self.River then
		return
	end
	
	--	Generate a direction for the water
	local	riverDirection			=	math.random(1, 4)
	local	riverDirX, riverDirZ	=	self:GetRiverDirection(riverDirection)
	self:CreateRiverTile(X, Z, riverDirection)
	
	--	Make a list of all corners
	local	riverCorners	=	{}
	riverCorners.__mode 	= 	"k"
	
	--	Make temporary variables that will
	--	be used in the loops
	local	tempDirection		=	riverDirection
	local	tempDirX, tempDirZ	=	self:GetRiverDirection(riverDirection)
	local	tempX		=	X
	local	tempZ		=	Z
	local	carveChance	=	0
	while true do
		
		--	Move in the waters direction
		tempX	=	tempX+tempDirX
		tempZ	=	tempZ+tempDirZ
		
		--	Check if there is a tile on this position
		local	tempId	=	self:GetEntityId(tempX, tempZ)
		if tempId == -1 then
			break
		end
		
		if self:GetTileType(tempX, tempZ) == self.River then
			
			self:RemoveTile(tempX-tempDirX, tempZ-tempDirZ)
			break
		end
		
		carveChance	=	math.random(1, 100)
		if carveChance < 5 then
		
			self:RemoveTile(tempX, tempZ)
			break
		
		elseif carveChance < 15 then
			tempDirection	=	tempDirection + (-1)^math.random(1, 10)
			if tempDirection > 4 then
				tempDirection	=	1
			elseif tempDirection < 1 then
				tempDirection	=	4
			end
			
			riverCorners[#riverCorners+1]	=	self:GetEntityId(tempX-tempDirX, tempZ-tempDirZ)
			riverCorners[#riverCorners+1]	=	self:GetEntityId(tempX, tempZ)
			
			tempDirX, tempDirZ	=	self:GetRiverDirection(tempDirection)
			
			if self:GetTileType(tempX+tempDirX, tempZ+tempDirZ) == self.River then
			
				self:RemoveTile(tempX, tempZ)
				break
			end
			
		end
		
		self:CreateRiverTile(tempX, tempZ, tempDirection)
	end
	
	--	Reset to initial river and go backwards
	tempDirection	=	riverDirection
	tempDirX		=	riverDirX
	tempDirZ		=	riverDirZ
	tempX			=	X
	tempZ			=	Z
	while true do
		
		--	Move in the waters direction
		tempX	=	tempX-tempDirX
		tempZ	=	tempZ-tempDirZ
		
		--	Check if there is a tile on this position
		if self:GetEntityId(tempX, tempZ) == -1 then
			break
		end
		
		if self:GetTileType(tempX, tempZ) == self.River then
			
			self:RemoveTile(tempX+tempDirX, tempZ+tempDirZ)
			self:GenerateStone(tempX+tempDirX, tempZ+tempDirZ)
			break
		end
		
		self:CreateRiverTile(tempX, tempZ, tempDirection)
		
		carveChance	=	math.random(1, 100)
		if carveChance < 10 then
			
			self:RemoveTile(tempX, tempZ)
			self:GenerateStone(tempX, tempZ)
			break
		
		elseif carveChance < 20 then
			tempDirection	=	tempDirection + (-1)^math.random(1, 10)
			if tempDirection > 4 then
				tempDirection	=	1
			elseif tempDirection < 1 then
				tempDirection	=	4
			end

			
			tempDirX, tempDirZ	=	self:GetRiverDirection(tempDirection)
			
			if self:GetEntityId(tempX-tempDirX, tempZ-tempDirZ) == -1 then
				self:CreateRiverTile(tempX-tempDirX, tempZ-tempDirZ, tempDirection)
				riverCorners[#riverCorners+1]	=	self:GetEntityId(tempX-tempDirX, tempZ-tempDirZ)
				riverCorners[#riverCorners+1]	=	self:GetEntityId(tempX, tempZ)
				break
			else
				riverCorners[#riverCorners+1]	=	self:GetEntityId(tempX-tempDirX, tempZ-tempDirZ)
				riverCorners[#riverCorners+1]	=	self:GetEntityId(tempX, tempZ)
			end
			
		end
		
		
	end

	
	if self.DebugInfo then
		print(#riverCorners .. " river corners exists!")
	end
	
	--	
	for n = 1, #riverCorners, 2 do
		self:FixRiverCorner(riverCorners[n], riverCorners[n+1])
	end
	
end

MapGenerator.GenerateRivers = function(self)	
	
	for n = 1, self.Rivers do
		local	rX	=	math.random(math.floor(self.MapSizeX/10), math.ceil(self.MapSizeX-math.ceil(self.MapSizeX/10)))
		local	rZ	=	math.random(math.floor(self.MapSizeZ/10), math.ceil(self.MapSizeZ-math.ceil(self.MapSizeZ/10)))
		self:CarveRiver(rX, rZ)
	end
end

--	STONE METHODS
--	All methods for creating stones will 
--	be here
MapGenerator.GenerateStone = function(self, X, Z)

	local	newStone	=	self:GenerateEmptyTile(X, Z)
	self.TileTypes[self:GetListIndex(X, Z)]	=	self.Stone
	world:CreateComponentAndAddTo("NotWalkable", newStone)
	world:CreateComponentAndAddTo("Model", newStone)
	world:GetComponent(newStone, "Model", 0):SetModel("stone", "stone", 0)
	world:GetComponent(newStone, "Position", 0):SetFloat3(X, 0.8 + 0.1* math.random(-1, 1), Z)
	world:GetComponent(newStone, "Rotation", 0):SetFloat3
	(
		math.pi * 0.01 * math.random(0, 25), 
		math.pi * 0.01 * math.random(0, 100), 
		math.pi * 0.01 * math.random(0, 25)
	)

end

MapGenerator.PostFixStones = function(self)
	for X = 1, self.MapSizeX do
		for Z = 1, self.MapSizeZ do
		
			if self:GetTileType(X-1, Z-1) == self.Stone then
			
				local grassTile = world:CreateNewEntity("Tile")
				world:GetComponent(grassTile, "Position", 0):SetFloat3(X-1, 0.0, Z-1)
				
				world:GetComponent(grassTile, "MapPosition", 0):SetInt2(X-1, Z-1)
				
				world:CreateComponentAndAddTo("Model", grassTile)
				world:GetComponent(grassTile, "Model", 0):SetModel("grass", "grass", 0)
			end
		end
	end
end

--	CHECKPOINT METHODS
--	All methods for creating checkpoints will 
--	be here
MapGenerator.CreateCheckPoint = function(self, X, Z, Index)
	self:RemoveTile(X, Z)
	
	local	newCheckpoint	=	self:GenerateEmptyTile(X, Z)
	self.TileTypes[self:GetListIndex(X, Z)]	=	self.Checkpoint
	
	world:CreateComponentAndAddTo("Checkpoint", newCheckpoint)
	world:GetComponent(newCheckpoint, "Checkpoint", 0):SetInt(Index)
	world:CreateComponentAndAddTo("Model", newCheckpoint)
	world:GetComponent(newCheckpoint, "Model", 0):SetModel("checkpoint" .. Index, "checkpoint", 0)
	
end

MapGenerator.PlaceCheckpoints = function(self)

	local	checkpointsLeft	=	self.Checkpoints
	
	local	lastCheckpointX	=	math.random(math.floor(self.MapSizeX/10), math.ceil(self.MapSizeX-math.ceil(self.MapSizeX/10)))
	local	lastCheckpointZ	=	math.random(math.floor(self.MapSizeZ/10), math.ceil(self.MapSizeZ-math.ceil(self.MapSizeZ/10)))
	
	local	tPosX		=	0
	local	tPosZ		=	0
	local	minDistance	=	math.ceil(math.sqrt(self.MapSizeX*self.MapSizeZ*self.Checkpoints)/math.max(2, self.Players))
	local	randomTries	=	0
	while checkpointsLeft > 0 do
		
		tPosX	=	math.random(math.floor(self.MapSizeX/10), math.ceil(self.MapSizeX-math.ceil(self.MapSizeX/10)))
		tPosZ	=	math.random(math.floor(self.MapSizeZ/10), math.ceil(self.MapSizeZ-math.ceil(self.MapSizeZ/10)))
		
		if self:GetTileType(tPosX, tPosZ) == self.Grass then
			if self:DistanceBetween(lastCheckpointX, lastCheckpointZ, tPosX, tPosZ) >= minDistance then
				
				lastCheckpointX	=	tPosX
				lastCheckpointZ	=	tPosZ
				
				checkpointsLeft	=	checkpointsLeft - 1
				self:CreateCheckPoint(tPosX, tPosZ, self.Checkpoints-checkpointsLeft)
			else
				randomTries	=	randomTries + 1
				
				if randomTries >= 10 then
					minDistance	=	math.floor(minDistance*0.8)
				end
			end
		end
	end

end

--	SPAWNPOINT METHODS
--	All methods for creating spawnpoints will 
--	be here
MapGenerator.CreateSpawnpoint = function(self, X, Z)
	self:RemoveTile(X, Z)
	
	local	newCheckpoint	=	self:GenerateEmptyTile(X, Z)
	world:CreateComponentAndAddTo("Model", newCheckpoint)
	world:GetComponent(newCheckpoint, "Model", 0):SetModel("grass", "grass", 0, 0)
	
	local newSpawnId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AvailableSpawnpoint", newSpawnId)
	world:GetComponent(newSpawnId, "AvailableSpawnpoint", 0):SetInt2(X, Z)
	
	self.TileTypes[self:GetListIndex(X, Z)]	=	self.Spawnpoint
end

MapGenerator.PlaceSpawnNear = function(self, X, Z)
	
	local	randomDirX	=	0
	local	randomDirZ	=	0
	local	randomTries	=	50
	while true do
	
		randomDirX	=	randomDirX + (-1)^(math.random(1, 10)) * math.random(0, 1)
		randomDirZ	=	randomDirZ + (-1)^(math.random(1, 10)) * math.random(0, 1)
		
		if self:GetTileType(X+randomDirX, Z+randomDirZ) == self.Grass then
			self:CreateSpawnpoint(X+randomDirX, Z+randomDirZ)
			break
		end
		
		randomTries	=	randomTries - 1
		if randomTries <= 0 then
			return false
		end
	end
	
	return true
end

MapGenerator.PlaceSpawnpoints = function(self)

	local	distanceX	=	0
	local	distanceZ	=	0
	local	factorX		=	0
	local	factorZ		=	0
	local	posX		=	0
	local	posZ		=	0
	local	spawnsLeft	=	self.Players
	
	while true do
		distanceX	=	math.random(math.floor(self.MapSizeX/4), 2*math.floor(self.MapSizeX/4))
		distanceZ	=	math.random(math.floor(self.MapSizeZ/4), 2*math.floor(self.MapSizeZ/4))
		
		factorX	=	(-1)^(math.random(1, 10)) * math.random(0, 1)
		factorZ	=	(-1)^(math.random(1, 10)) * math.random(0, 1)
		posX	=	math.floor(self.MapSizeX/2) + factorX * distanceX
		posZ	=	math.floor(self.MapSizeZ/2) + factorZ * distanceZ
		
		--if self:DistanceBetween(
		if self:GetTileType(posX, posZ) == self.Grass then
		
			self:CreateSpawnpoint(posX, posZ)
			spawnsLeft	=	spawnsLeft - 1
			while true do 
				if self:PlaceSpawnNear(posX, posZ) then
					spawnsLeft	=	spawnsLeft - 1
				end
				if spawnsLeft == 0 then
					break
				end
			end
			break
		end
		
	
	end
	
end


MapGenerator.IsWalkable = function(self, X, Z)
	
	local	tileType	=	self:GetTileType(X, Z)
	
	if tileType == self.Hole or tileType == self.Stone or tileType == self.Void then
		return false
	end
	
	return true
end

MapGenerator.GeneratePathfindingData = function(self)

	local inputData = InputData()
	
	for Z = 1, self.MapSizeZ do
		for X = 1, self.MapSizeX do
			inputData:AddTile( 1, self:IsWalkable(X-1, Z-1) )
		end
	end
		
	
	inputData:SetSize(self.MapSizeX, self.MapSizeZ)
	PathfinderHandler.SetData(inputData)
	--PathfinderHandler.SetTileWalkable( 4, 7, true );
end




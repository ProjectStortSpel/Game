--	 __  __          _____  	  _____ ______ _   _ ______ _____         _______ ____  _____  
--	|  \/  |   /\   |  __ \ 	 / ____|  ____| \ | |  ____|  __ \     /\|__   __/ __ \|  __ \ 
--	| \  / |  /  \  | |__) |	| |  __| |__  |  \| | |__  | |__) |   /  \  | | | |  | | |__) |
--	| |\/| | / /\ \ |  ___/ 	| | |_ |  __| | . ` |  __| |  _  /   / /\ \ | | | |  | |  _  / 
--	| |  | |/ ____ \| |     	| |__| | |____| |\  | |____| | \ \  / ____ \| | | |__| | | \ \ 
--	|_|  |_/_/    \_\_|     	 \_____|______|_| \_|______|_|  \_\/_/    \_\_|  \____/|_|  \_\
MapGenerator = System()
MapGenerator.DebugInfo 				=	true	--	Will print debug info about the map 
MapGenerator.VoidMargin				=	1		--	How many layers of Void it should be around the map
MapGenerator.MapSizeX 				= 	0		--	The width of the map, with the MARGIN included (2xVoidMargin)
MapGenerator.MapSizeZ 				= 	0		--	The height of the map, with the MARGIN included (2xVoidMargin)
MapGenerator.Players 				= 	0		--	How many players that can play this map (Number of Spawnpoints)
MapGenerator.Checkpoints 			= 	0		--	How many checkpoints the map will create
MapGenerator.Rivers					=	0		--	How many rivers the map should contain
MapGenerator.RiverTiles				=	0		--	How many rivers tiles the map should contain
MapGenerator.TileTypes 				= 	{}
MapGenerator.TileTypes.__mode 		= 	"k"


--	 _______ _____ _      ______ 	 _________     _______  ______  _____ 
--	|__   __|_   _| |    |  ____|	|__   __\ \   / /  __ \|  ____|/ ____|
--	   | |    | | | |    | |__   	   | |   \ \_/ /| |__) | |__  | (___  
--	   | |    | | | |    |  __|  	   | |    \   / |  ___/|  __|  \___ \ 
--	   | |   _| |_| |____| |____ 	   | |     | |  | |    | |____ ____) |
--	   |_|  |_____|______|______|	   |_|     |_|  |_|    |______|_____/ 
MapGenerator.UNDEFINED		=	-1
--	Standard tiles
MapGenerator.Void			=	 0
MapGenerator.Grass			=	 1
MapGenerator.Stone			=	 2
MapGenerator.Hole			=	 3

--	River tiles	(MUST BE IN Up,Right,Down,Left ORDER!)
MapGenerator.RiverUp		=	 4
MapGenerator.RiverRight		=	 5
MapGenerator.RiverDown		=	 6
MapGenerator.RiverLeft		=	 7

--	Other
MapGenerator.Spawnpoint		=	 8

--	Checkpoints (MUST BE THE LAST TILE TYPE DEFINED!)
MapGenerator.Checkpoint		=	 100
--	The logic for this is the following
--	Checkpoint with number
--	ONE			=	self.Checkpoint
--	TWO			=	self.Checkpoint+1
--	...
--	ONE HUNDRED	=	self.Checkpoint+101

--	  _____ ______ _______ _    _ _____  
--	 / ____|  ____|__   __| |  | |  __ \ 
--	| (___ | |__     | |  | |  | | |__) |
--	 \___ \|  __|    | |  | |  | |  ___/ 
--	 ____) | |____   | |  | |__| | |     
--	|_____/|______|  |_|   \____/|_|     
MapGenerator.Initialize = function(self)
	self:SetName("MapGenerator")
	
	self:UsingEntitiesAdded()
	self:AddComponentTypeToFilter("GenerateMap", FilterType.RequiresOneOf)
end

MapGenerator.EntitiesAdded = function(self, dt, entities)
	--self:GenerateMap(os.time()%29181249, 4, 4)
	--self:GenerateMap(23246299, 8, 4)
	self:GenerateMap(1579125, 8, 5)
	--self:GenerateMap(23239474, 4, 4)
	--self:GenerateMap(5747, 4, 4)
	--self:GenerateMap(1338, 2, 4)
end

MapGenerator.PostInitialize = function(self)
	local	newMap	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("GenerateMap", newMap)
	
	--local	newParticle	=	world:CreateNewEntity()
	--world:CreateComponentAndAddTo("Position", newParticle)
	--world:CreateComponentAndAddTo("Color", newParticle)
	--world:CreateComponentAndAddTo("Particle", newParticle)
	--
	--world:GetComponent(newParticle, "Position", "X"):SetFloat3(10, 1, 7)
	--world:GetComponent(newParticle, "Color", "X"):SetFloat3(1.0, 0.0, 0.0)
	--
	--world:GetComponent(newParticle, "Particle", "Name"):SetText("fire")
	--world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/firewhite.png")
	--world:GetComponent(newParticle, "Particle", "Particles"):SetInt(1000)
	--world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(7000)
	--world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.05)
	--world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.6)
	--world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
end

MapGenerator.GenerateMap = function(self, MapSeed, NumberOfPlayers, NumberOfCheckpoints)

	--	Seed the random
	math.randomseed(MapSeed)
	--print("SEEEED " .. MapSeed)
	
	--	Randomize initial values
	local	tMax, tMin				=	math.max(NumberOfPlayers, NumberOfCheckpoints), math.min(NumberOfPlayers, NumberOfCheckpoints)
	local	overallFactor			=	tMax/tMin + tMin/tMax
	print("factor: " .. overallFactor)
	local	playerFactor			=	math.ceil(NumberOfPlayers*0.85*overallFactor)
	local	checkpointFactor		=	math.ceil(NumberOfCheckpoints*0.45*overallFactor)
	local	defaultSize				=	3
	local	lowerLimit				=	defaultSize + math.ceil(math.sqrt(playerFactor*checkpointFactor)) + 2*self.VoidMargin--math.ceil(defaultSize + math.sqrt((playerFactor+checkpointFactor)/(NumberOfPlayers/2))*math.exp((playerFactor+checkpointFactor)/NumberOfPlayers))--math.ceil((playerFactor+checkpointFactor)*2.0), math.ceil((playerFactor+checkpointFactor+1)*2.0)
	local	upperLimit				=	defaultSize + math.ceil(2 + math.sqrt(playerFactor*checkpointFactor)) + 2*self.VoidMargin--math.ceil(defaultSize + math.sqrt((playerFactor+checkpointFactor)/(NumberOfPlayers/2))*math.exp((playerFactor+checkpointFactor)/NumberOfPlayers) + 2)
	
	self.MapSizeX		=	math.random(lowerLimit, upperLimit)--math.random(math.ceil((18+NumberOfPlayers)/2), math.ceil((20+NumberOfPlayers)/2)) + 2*self.VoidMargin
	self.MapSizeZ		=	math.random(lowerLimit, upperLimit)--math.random(math.ceil((18+NumberOfPlayers)/2), math.ceil((20+NumberOfPlayers)/2)) + 2*self.VoidMargin
	self.Players		=	NumberOfPlayers
	self.Checkpoints	=	NumberOfCheckpoints
	self.Rivers			=	0--math.ceil(math.sqrt(self.MapSizeX*self.MapSizeZ)/10)--math.random(NumberOfPlayers, 2*NumberOfPlayers-NumberOfPlayers)
	
	if self.MapSizeX % 2 == 0 then
		self.MapSizeX	=	self.MapSizeX + (-1)^math.random(1, 10)
	end
	if self.MapSizeZ % 2 == 0 then
		self.MapSizeZ	=	self.MapSizeZ + (-1)^math.random(1, 10)
	end
	
	--	Print debug info
	if self.DebugInfo then
		print("----- MAP INFO -----")
		print("Seed: " .. MapSeed)
		print("Players: " .. self.Players)
		print("Checkpoints: " .. self.Checkpoints)
		print("Map Size: " .. self.MapSizeX .. "x" .. self.MapSizeZ)
		print("----- END OF MAP INFO -----")
	end
	
	--	Create the Pathfinding object
	local	tInputData	= 	InputData()
	
	for Z = 1, self.MapSizeZ do
		for X = 1, self.MapSizeX do
			tInputData:AddTile( 1, false )
		end
	end
			
	
	--	Fill the list with starting values
	for Z = 1, self.MapSizeZ do
		for X = 1, self.MapSizeX do
			self.TileTypes[#self.TileTypes+1]	=	self.Void
		end
	end
	tInputData:SetSize(self.MapSizeX, self.MapSizeZ)
	PathfinderHandler.SetData(tInputData)
	
	--	Create a empty grass plane
	self:PrintDebugMessage("Creating grass plane")
	self:CreateGrassPlane()
	self:PrintDebugMessage("Carving void")
	self:CarveVoidMargin()
	self:PrintDebugMessage("Carving corners")
	self:CarveVoidCorners()
	
	
	
	--	Create all rivers
	local	tNumberOfTiles	=	self:GetPlayableTiles()
	self.Rivers				=	math.ceil((self.MapSizeX*self.MapSizeZ)/tNumberOfTiles)
	self.RiverTiles			=	math.ceil(math.sqrt(tNumberOfTiles)*1.90)
	self:PrintDebugMessage("Carving " .. self.RiverTiles .. " river tiles")
	self:PrintDebugMessage("Carving " .. self.Rivers .. " rivers")
	self:CreateRivers()
	
	self:CarveVoidHoles()
	
	local	numberOfStones	=	math.floor(self:GetPlayableTiles() / (math.ceil(2*math.sqrt(self.MapSizeX*self.MapSizeZ))))
	self:PrintDebugMessage("Placing " .. numberOfStones .. " stones")
	self:PlaceStones(numberOfStones)
	--	Place spawnpoints
	self:PrintDebugMessage("Placing checkpoints")
	self:PlaceCheckpoints()
	self:PrintDebugMessage("Placing spawnpoints")
	self:PlaceAllSpawnpoints()
	self:PrintDebugMessage("Replacing empty tiles")
	self:FixEmptyTiles()
	--	Create the actual map
	self:PrintDebugMessage("Creating map")
	self:CreateMap()
end


--	  _____ _____             _____ _____ 
--	 / ____|  __ \     /\    / ____/ ____|
--	| |  __| |__) |   /  \  | (___| (___  
--	| | |_ |  _  /   / /\ \  \___ \\___ \ 
--	| |__| | | \ \  / ____ \ ____) |___) |
--	 \_____|_|  \_\/_/    \_\_____/_____/ 
--	Grass generating methods should be here

MapGenerator.CreateGrassPlane = function(self)
	for X = 1, self.MapSizeX do
		for Z = 1, self.MapSizeZ do
			self:SetTileType(X-1, Z-1, self.Grass)
		end
	end
end 


--	 _____  _______      ________ _____  
--	|  __ \|_   _\ \    / /  ____|  __ \ 
--	| |__) | | |  \ \  / /| |__  | |__) |
--	|  _  /  | |   \ \/ / |  __| |  _  / 
--	| | \ \ _| |_   \  /  | |____| | \ \ 
--	|_|  \_\_____|   \/   |______|_|  \_\
--	River generating methods should be here

--	Create all rivers
MapGenerator.CreateRivers = function(self)

	local	tempX,	tempZ		=	self:GetRandomPositionWithinMargin(self.VoidMargin, self.VoidMargin)
	local	centerX, centerZ	=	self:GetCenterOfMap()
	local	centerDistance		=	math.ceil(self:GetDistanceBetween(centerX, centerZ, self.VoidMargin, self.VoidMargin)*0.5)
	local	riversCarved		=	0
	local	riverMargin			=	math.ceil(1.2*self.VoidMargin)
	local	nTries				=	self.MapSizeX*self.MapSizeZ
	local	riverTilesLeft		=	self.RiverTiles
	
	while riverTilesLeft>= 0 do
		
		tempX, tempZ	=	self:GetRandomPositionWithinMargin(riverMargin, riverMargin)
		local	isNearRiver	=	false
		for tempRiver = self.RiverUp, self.RiverLeft do
			if self:HasAdjacentTileOfType(tempX, tempZ, tempRiver) then
				isNearRiver	=	true
				break
			end
		end
		if not isNearRiver then
			local	tilesCarved	=	self:CarveRiverAt(tempX, tempZ)
			if tilesCarved > 0 then
				riversCarved	=	riversCarved+1
				
				centerDistance	=	centerDistance+1
				riverTilesLeft	=	riverTilesLeft - tilesCarved
				if riversCarved == self.Rivers or riverTilesLeft <= 0 then
					break
				end
			end
		else
			nTries	=	nTries-1
			if nTries <= 0 then
				break
			end
		end
	end
end

MapGenerator.CanPlaceRiverAt = function(self, X, Z)
	
	if not self:IsInsideWorld(X, Z) then
		return false
	end
	
	if self:IsRiver(X, Z) then
		return false
	end
	
	local	tType	=	self:GetTileType(X, Z)
	
	if tType == self.Hole or tType == self.Void then
		return false
	end
	
	if tType == self.Stone then
		return false
	end
	
	return true
end

MapGenerator.GetRandomRiverDirection = function(self, currentDirection)
	currentDirection	=	currentDirection + (-1)^math.random(1, 10)
	if currentDirection > self.RiverLeft then
		currentDirection	=	self.RiverUp
	elseif currentDirection < self.RiverUp then
		currentDirection	=	self.RiverLeft
	end
	
	return currentDirection
end

MapGenerator.CarveRiver = function(self, TilesLeft)

	local	posX, posZ			=	self:GetRandomPositionWithinMargin(math.floor(self.VoidMargin), math.floor(self.VoidMargin))
	local	tempTileCounter		=	0
	local	riverDirection		=	math.random(self.RiverUp, self.RiverLeft)
	local	dirX, dirZ			=	self:GetRiverDirection(riverDirection)
	
	for tZ = -1, 1 do
		for tX = -1, 1 do
			if self:IsRiver(posX+tX, posZ+tZ) then
				return	0
			end
		end
	end
	self:SetTileType(posX, posZ, riverDirection)
	tempTileCounter	=	tempTileCounter+1
		
	while true do
	
		posX	=	posX+dirX
		posZ	=	posZ+dirZ
		
		--	If we can't place a river
		--	at the location we break
		if not self:CanPlaceRiverAt(posX, posZ) then
			break
		end
		
		--	Place the river
		if math.random(1, 100) < 25 then
			riverDirection	=	self:GetRandomRiverDirection(riverDirection)
			dirX, dirZ		=	self:GetRiverDirection(riverDirection)
		end
		
		
		if math.random(1, 1000) < 90 then
			self:SetTileType(posX, posZ, self.Hole)
			break
		end
		
		if self:CanPlaceRiverAt(posX+dirX, posZ+dirZ) then
			self:SetTileType(posX, posZ, riverDirection)
			tempTileCounter	=	tempTileCounter+1
		else
			self:SetTileType(posX, posZ, self.Hole)
			break
		end



		

		
		if tempTileCounter >= TilesLeft then
			break
		end
	end
	
	--	Return how many rivertiles the river
	--	carved
	return tempTileCounter
end

--	Carves a river at the given location
MapGenerator.CarveRiverAt = function(self, X, Z)
	
	--	Checks so the tile is OK
	if self:IsRiver(X, Z) then
		return	0
	end
	
	--	Create a random direction
	local	tempDirection	=	math.random(self.RiverUp, self.RiverLeft)
	local	backupDirection	=	tempDirection
	local	dirX,	dirZ	=	self:GetRiverDirection(tempDirection)
	local	tempX,	tempZ	=	X, Z
	local	tilesCarved		=	0
	
	if self:IsRiver(X+dirX, Z+dirZ) then
		return	tilesCarved
	elseif self:IsRiver(X-dirX, Z-dirZ) then
		return	tilesCarved
	end
	
	--	Create the initial river
	self:SetTileType(tempX, tempZ, tempDirection)
	tilesCarved	=	tilesCarved + 1
	
	--	Begin carving forward
	tilesCarved	=	tilesCarved + self:CarveRiverForward(tempX, tempZ, dirX, dirZ, tempDirection)

	--	Reset variables
	tempDirection	=	backupDirection
	dirX,	dirZ	=	self:GetRiverDirection(tempDirection)
	tempX,	tempZ	=	X, Z
	
	--	Begin carving backwards
	tilesCarved	=	tilesCarved + self:CarveRiverBackwards(tempX, tempZ, dirX, dirZ, tempDirection)
	
	return	tilesCarved
end

--	Carves the river in its direction forward
MapGenerator.CarveRiverForward = function(self, tempX, tempZ, dirX, dirZ, tempDirection)
	
	local	carveStatus		=	0
	local	riversX			=	{}
			riversX.__mode	=	"k"
	local	riversZ			=	{}
			riversZ.__mode	=	"k"
	
	--	Begin the carving!
	while true do
	
		tempX	=	tempX+dirX
		tempZ	=	tempZ+dirZ
		
		--	Get the tile type of the current location
		tileType	=	self:GetTileType(tempX, tempZ)
		
		--	If there is a void or another river
		--	stop the carving
		if tileType == self.Void or tileType == self.Hole or tileType == self.Stone then
			break
		end
		if self:IsRiver(tempX, tempZ) then
			self:SetTileType(tempX-dirX, tempZ-dirZ, self.Hole)
			break
		end
		
		--	Randomize what will happen with this river
		carveStatus	=	math.random(1, 100)
		
		--	Stop the river
		if carveStatus <= 5 then
			self:SetTileType(tempX, tempZ, self.Hole)
			break
		end
		
		--	Turn the river
		if carveStatus <= 25 then
			
			--	Change the direction
			tempDirection	=	tempDirection + (-1)^math.random(1, 10)
			
			--	Fix the direction if it goes out of scope
			if tempDirection > self.RiverLeft then
				tempDirection	=	self.RiverUp
			elseif tempDirection < self.RiverUp then
				tempDirection	=	self.RiverLeft
			end
			
			--	Get the direction of the current river
			dirX, dirZ	=	self:GetRiverDirection(tempDirection)
			
			--	If there is a void or another river
			--	stop the carving
			if self:IsRiver(tempX+dirX, tempZ+dirZ) then
				self:SetTileType(tempX, tempZ, self.Hole)
				break
			end
			
		end
		
		--	Get the direction of the current river
		dirX, dirZ	=	self:GetRiverDirection(tempDirection)
		
		self:SetTileType(tempX, tempZ, tempDirection)
		riversX[#riversX+1]	=	tempX
		riversZ[#riversZ+1]	=	tempZ
	end
	
	if #riversX == 1 and #riversZ == 1 then
		self:SetTileType(riversX[1], riversZ[1], self.Grass)
		return 0
	end
	
	
	return	#riversX
end

--	Carves the river in its direction backwards
MapGenerator.CarveRiverBackwards = function(self, tempX, tempZ, dirX, dirZ, tempDirection)
	
	local	carveStatus		=	0
	local	riversX			=	{}
			riversX.__mode	=	"k"
	local	riversZ			=	{}
			riversZ.__mode	=	"k"
			
	--	Begin the carving!
	while true do
	
		tempX	=	tempX-dirX
		tempZ	=	tempZ-dirZ
		
		--	Get the tile type of the current location
		tileType	=	self:GetTileType(tempX, tempZ)
		
		--	If there is a void or another river
		--	stop the carving
		if tileType == self.Void then
			self:SetTileType(tempX+dirX, tempZ+dirZ, self.Stone)
			break
		end
		if self:IsRiver(tempX, tempZ) then
			self:SetTileType(tempX+dirX, tempZ+dirZ, self.Stone)
			break
		end
		
		self:SetTileType(tempX, tempZ, tempDirection)
		riversX[#riversX+1]	=	tempX
		riversZ[#riversZ+1]	=	tempZ
		
		--	Randomize what will happen with this river
		carveStatus	=	math.random(1, 100)
		
		--	Stop the river
		if carveStatus <= 5 then
			self:SetTileType(tempX, tempZ, self.Stone)
			break
		end
		
		--	Turn the river
		if carveStatus <= 25 then
			
			--	Change the direction
			tempDirection	=	tempDirection + (-1)^math.random(1, 10)
			
			--	Fix the direction if it goes out of scope
			if tempDirection > self.RiverLeft then
				tempDirection	=	self.RiverUp
			elseif tempDirection < self.RiverUp then
				tempDirection	=	self.RiverLeft
			end
			
			--	Get the direction of the current river
			dirX, dirZ	=	self:GetRiverDirection(tempDirection)
			
			--	If there is a void or another river
			--	stop the carving
			if self:IsRiver(tempX-dirX, tempZ-dirZ) then
				self:SetTileType(tempX, tempZ, self.Stone)
				break
			elseif self:GetTileType(tempX-dirX, tempZ-dirZ) == self.Void then
				self:SetTileType(tempX+dirX, tempZ+dirZ, self.Stone)
				break
			end
			
		end
	end
	
	if #riversX == 1 and #riversZ == 1 then
		self:SetTileType(riversX[1], riversZ[1], self.Grass)
		return 0
	end
	
	return	#riversX
end

--	Returns the direction of a given River Type
MapGenerator.GetRiverDirection = function(self, RiverType)
	
	if RiverType == self.RiverUp then
		return	0, -1
	elseif RiverType == self.RiverRight then
		return	1, 0
	elseif RiverType == self.RiverDown	then
		return	0, 1
	elseif RiverType == self.RiverLeft	then
		return	-1, 0
	end
		
	return	0, 0
end

--	  _____ _______ ____  _   _ ______ 
--	 / ____|__   __/ __ \| \ | |  ____|
--	| (___    | | | |  | |  \| | |__   
--	 \___ \   | | | |  | | . ` |  __|  
--	 ____) |  | | | |__| | |\  | |____ 
--	|_____/   |_|  \____/|_| \_|______|
--	Stone generation should be here

MapGenerator.PlaceStones = function(self, nStonesToPlace)
	
	--local	nStonesToPlace	=	math.random(self.Players, 2*self.Players-self.Players)
	local	tX, tZ			=	0, 0
	local	tileType		=	self.UNDEFINED
	
	for n = 1, nStonesToPlace do
		
		while true do
		
			tX, tZ		=	self:GetRandomPositionWithinMargin(self.VoidMargin, self.VoidMargin)
			tileType	=	self:GetTileType(tX, tZ)
			
			if tileType ~= self.Void and tileType ~= self.Stone and not self:IsRiver(tX, tZ) then
				self:SetTileType(tX, tZ, self.Stone)
				break
			end
		
		end
	end
	
end

MapGenerator.PlaceStonesNear = function(self, X, Z, nStonesToPlace, Distance)

	for n = 1, nStonesToPlace do
		self:PlaceStoneNear(X, Z, Distance)
	end

end

MapGenerator.PlaceStoneNear = function(self, X, Z, Distance)

	local	tX, tZ				=	X, Z
	local	tileType			=	self.UNDEFINED
	local	directionX			=	0
    local	directionZ			=	0
	local	stoneTempDistance	=	0
	while true do
	
		directionX		=	(-1)^math.random(1, 10) * math.random(0, 1)
		directionZ		=	(-1)^math.random(1, 10) * math.random(0, 1)
		stoneTempDistance	=	math.random(0, Distance)
		
		tX	=	X + stoneTempDistance*directionX
		tZ	=	Z + stoneTempDistance*directionZ
		
		tileType	=	self:GetTileType(tX, tZ)
		
		if tileType ~= self.Void and tileType ~= self.Stone and tileType < self.Checkpoint then
			
			if self:NumberOfAdjacentTiles(tX, tZ) == 4 then
				self:SetTileType(tX, tZ, self.Stone)
				return tX, tZ
			end
		end
	
	end
end


--	  _____          _______      _______ _   _  _____ 
--	 / ____|   /\   |  __ \ \    / /_   _| \ | |/ ____|
--	| |       /  \  | |__) \ \  / /  | | |  \| | |  __ 
--	| |      / /\ \ |  _  / \ \/ /   | | | . ` | | |_ |
--	| |____ / ____ \| | \ \  \  /   _| |_| |\  | |__| |
--	 \_____/_/    \_\_|  \_\  \/   |_____|_| \_|\_____|
--	Void carving

MapGenerator.CarveVoidCorners = function(self)
	
	for n = 1, 20 do
		--	Randomize what corner to carve
		local	cornerX	=	(-1)^math.random(1, 10)-- * math.random(0, 1)
		local	cornerZ	=	(-1)^math.random(1, 10)-- * math.random(0, 1)
		
		
		
		local	locationX		=	math.floor(self.MapSizeX/2) + cornerX * (math.floor(self.MapSizeX/2) - self.VoidMargin)
		local	locationZ		=	math.floor(self.MapSizeZ/2) + cornerZ * (math.floor(self.MapSizeZ/2) - self.VoidMargin)
		local	cornerDepth		=	math.sqrt(1+1)
		local	nIterationsX	=	math.random(1, 4)
		local	nIterationsZ	=	math.random(1, 4)
		local	nDepth			=	1
		local	maxDepth		=	math.floor((self.MapSizeX*self.MapSizeZ+self.Players)/(4*math.sqrt(self.MapSizeX*self.MapSizeZ)))
		for X = 0, nIterationsX do
			for Z = 0, nIterationsZ do
				
				if self:GetDistanceBetween(locationX, locationZ, locationX-X*cornerX, locationZ-Z*cornerZ) < nDepth*cornerDepth then
					self:RemoveTile(locationX-X*cornerX, locationZ-Z*cornerZ)
					nDepth	=	(nDepth + (-1)^math.random(2, 9)) % maxDepth
				end
			end
		end
	end
	
	
end

--	Carve the void around the map
MapGenerator.CarveVoidMargin = function(self)
	for Z = 0, self.VoidMargin-1 do
		for X = 0, self.MapSizeX-1 do
			self:RemoveTile(X, Z)
			self:RemoveTile(X, self.MapSizeZ-Z-1)
		end
	end
	
	for X = 0, self.VoidMargin-1 do
		for Z = 0, self.MapSizeZ-1 do
			self:RemoveTile(X, Z)
			self:RemoveTile(self.MapSizeX-X-1, Z)
		end
	end
end


MapGenerator.CarveVoidHoleNear = function(self, X, Z, Radius)

	local	voidCarved			=	0
	local	tIterations			=	math.ceil(Radius/2)
	local	offsetX, offsetZ	=	math.random(0, 10)*0.1, math.random(0, 10)*0.1
	for tZ = -tIterations, tIterations do
		for tX = -tIterations, tIterations do
			if self:IsInsideWorld(X+tX, Z+tZ) then
				if self:GetDistanceBetween(X+offsetX, Z+offsetZ, X+tX, Z+tZ) < Radius then
					self:SetTileType(X+tX, Z+tZ, self.Void)
					voidCarved	=	voidCarved+1
				end
			end
		end
	end
	
	
	return voidCarved
end

MapGenerator.CarveVoidHoles = function(self)
	
	local	tNumberOfTiles	=	self:GetPlayableTiles()
			tNumberOfTiles	=	math.floor(0.50*math.sqrt(tNumberOfTiles))
	if tNumberOfTiles == 0 then
		return
	end
	self:PrintDebugMessage("Trying to carve void holes " .. tNumberOfTiles)
	
	
	local	tX, tZ	=	self:GetRandomPositionWithinMargin(2*self.VoidMargin, 2*self.VoidMargin)
	while tNumberOfTiles > 0 do
		
		if math.random(1, 100) < 10 then
			tX, tZ	=	self:GetRandomPositionWithinMargin(2*self.VoidMargin, 2*self.VoidMargin)
		end
		
		if self:GetTileType(tX, tZ) ~= self.Void then
			tNumberOfTiles	=	tNumberOfTiles - self:CarveVoidHoleNear(tX, tZ, math.random(10, 18)*0.1)
		end
	end
	
end


--	 _    _  ____  _      ______ 
--	| |  | |/ __ \| |    |  ____|
--	| |__| | |  | | |    | |__   
--	|  __  | |  | | |    |  __|  
--	| |  | | |__| | |____| |____ 
--	|_|  |_|\____/|______|______|
--	Fixing holes

--	Replaces all empty "holes" with actual holes
MapGenerator.FixEmptyTiles = function(self)
	
	for X = self.VoidMargin, self.MapSizeX-self.VoidMargin-1 do
		for Z = self.VoidMargin, self.MapSizeZ-self.VoidMargin-1 do
			
			if self:GetTileType(X, Z) == self.Void then
				if self:NumberOfAdjacentTiles(X, Z) == 4 then
					self:SetTileType(X, Z, self.Hole)
				end
			end
			
		end
	end
	
end

--	  _____ _____   __          ___   _ _____   ____ _____ _   _ _______ _____ 
--	 / ____|  __ \ /\ \        / / \ | |  __ \ / __ \_   _| \ | |__   __/ ____|
--	| (___ | |__) /  \ \  /\  / /|  \| | |__) | |  | || | |  \| |  | | | (___  
--	 \___ \|  ___/ /\ \ \/  \/ / | . ` |  ___/| |  | || | | . ` |  | |  \___ \ 
--	 ____) | |  / ____ \  /\  /  | |\  | |    | |__| || |_| |\  |  | |  ____) |
--	|_____/|_| /_/    \_\/  \/   |_| \_|_|     \____/_____|_| \_|  |_| |_____/ 
--	Methods for creating spawnpoints


MapGenerator.ForcePlaceSpawnpoints = function(self)

	while true do
		
		local	tX, tZ	=	self:GetRandomTileOfType(self.Grass)
		
		if tX ~= -1 and tZ ~= -1 then
			self:SetTileType(tX, tZ, self.Spawnpoint)
			return true
		end
	
	end
	
	return false
end

MapGenerator.PlaceSpawnpoints = function(self)

	local	centerX, centerZ	=	self:GetCenterOfMap()
	local	tSpawns	=	math.ceil(self.Players/2)
	local	tX,	tZ	=	0, 0 --self:GetPositionXDistanceAwayFrom(centerX, centerZ, 2*tSpawns)

	local	canSpawnX	=	true
	local	canSpawnZ	=	true
	for n = 1, 100 do
		tX,	tZ	=	self:GetRandomTileOfType(self.Grass)
		canSpawnX	=	true
		canSpawnZ	=	true
		for X = -tSpawns, tSpawns-1 do
			if self:IsInsideWorld(tX, tZ) then
				if self:GetTileType(tX+X, tZ) ~= self.Grass then
					canSpawnX	=	false
					break
				end
			end
		end
		for Z = -tSpawns, tSpawns-1 do
			if self:IsInsideWorld(tX, tZ) then
				if self:GetTileType(tX, tZ+Z) ~= self.Grass then
					canSpawnZ	=	false
					break
				end
			end
		end
		
		if canSpawnX or canSpawnZ then
			break
		end
	end

	
	if canSpawnX then
		for X = -tSpawns, tSpawns-1 do
			self:SetTileType(tX+X, tZ, self.Spawnpoint)
		end
	elseif canSpawnZ then
		for Z = -tSpawns, tSpawns-1 do
			self:SetTileType(tX, tZ+Z, self.Spawnpoint)
		end
	else
		for n = 1, self.Players do
			self:ForcePlaceSpawnpoints()
		end
	end
end


MapGenerator.PlaceAllSpawnpoints = function(self)
	
	--	originX/Z is the position of the first
	--	checkpoint
	local	originX, originZ	=	-1, -1
	for Z = 0, self.MapSizeZ-1 do
		for X = 0, self.MapSizeX-1 do
			if self:GetTileType(X, Z) == self.Checkpoint then
				originX	=	X
				originZ	=	Z
				break
			end
		end
	end
	
	local	tempDistance	=	math.floor(self.Players/2)
	local	maxDifference	=	1
	while true do
		local	spawnpointsFound	=	0
		for Z = -tempDistance, tempDistance do
			for X = -tempDistance, tempDistance do
				
				if self:GetTileType(originX+X, originZ+Z) == self.Grass then
					local	currentDistance	=	PathfinderHandler.GeneratePath(originX, originZ, originX+X, originZ+Z)
					local	tempDiff		=	math.abs(currentDistance-tempDistance)
					if tempDiff < maxDifference then
						spawnpointsFound	=	spawnpointsFound+1
					end
				end
			end
		end
		
		if spawnpointsFound >= self.Players then
			for Z = -tempDistance, tempDistance do
				for X = -tempDistance, tempDistance do
					
					if self:GetTileType(originX+X, originZ+Z) == self.Grass then
						local	currentDistance	=	PathfinderHandler.GeneratePath(originX, originZ, originX+X, originZ+Z)
						local	tempDiff		=	math.abs(currentDistance-tempDistance)
						if tempDiff < maxDifference then
							self:SetTileType(originX+X, originZ+Z, self.Spawnpoint)
						end
					end
				end
			end
			
			break
		else
			tempDistance	=	tempDistance+1
		end
		
		if tempDistance > self.Players then
			tempDistance	=	0
			maxDifference	=	maxDifference+1
		end
	end
	
end

--	  _____ _    _ ______ _____ _  _______   ____ _____ _   _ _______ _____ 
--	 / ____| |  | |  ____/ ____| |/ /  __ \ / __ \_   _| \ | |__   __/ ____|
--	| |    | |__| | |__ | |    | ' /| |__) | |  | || | |  \| |  | | | (___  
--	| |    |  __  |  __|| |    |  < |  ___/| |  | || | | . ` |  | |  \___ \ 
--	| |____| |  | | |___| |____| . \| |    | |__| || |_| |\  |  | |  ____) |
--	 \_____|_|  |_|______\_____|_|\_\_|     \____/_____|_| \_|  |_| |_____/ 
--	Methods for creating checkpoints

--	Initial method for creating all checkpoints
MapGenerator.PlaceCheckpoints = function(self)

	local	centerX, centerZ	=	self:GetCenterOfMap()
	local	lastX, lastZ		=	self:GetRandomTileOfType(self.Grass)--self:GetRandomPositionWithinMargin(self.VoidMargin, self.VoidMargin)
	local	tempDistance		=	math.ceil(self:GetDistanceBetween(self.VoidMargin, self.VoidMargin, centerX, centerZ))--math.ceil(self:GetDistanceBetween(self.VoidMargin, self.VoidMargin, centerX, centerZ))
	
	for n = 0, self.Checkpoints-1 do
		tempDistance		=	math.ceil(self:GetDistanceBetween(lastX, lastZ, centerX, centerZ))
		while true do
			
			local	tX, tZ	=	self:GetPositionXDistanceAwayFrom(lastX, lastZ, tempDistance)
			if self:GetTileType(tX, tZ) == self.Grass then
				local	isNearCheckpoint	=	false
				for tempCheckpoint = self.Checkpoint, self.Checkpoint+n do
					if self:HasAdjacentTileOfType(tX, tZ, tempCheckpoint) then
						isNearCheckpoint	=	true
						break
					end
				end
				if not isNearCheckpoint then
					if self:CanWalkBetween(tX, tZ, lastX, lastZ) then
						self:SetTileType(tX, tZ, self.Checkpoint+n)
						
						lastX	=	tX
						lastZ	=	tZ
						
						if self:NumberOfAdjacentTiles(tX, tZ) > 3 then
							if math.random(1, 100) < 10 then
								self:PlaceStonesNear(tX, tZ, 1, 4)
							end
						end
						break
					end
				end
			else
				tempDistance	=	tempDistance - 1
				if tempDistance < 0 then
					tempDistance		=	math.ceil(self:GetDistanceBetween(self.VoidMargin, self.VoidMargin, centerX, centerZ))
				end
			end
		end
	end
end


--	 _______ _____ _      ______ 	 _    _ ______ _      _____  
--	|__   __|_   _| |    |  ____|	| |  | |  ____| |    |  __ \ 
--	   | |    | | | |    | |__   	| |__| | |__  | |    | |__) |
--	   | |    | | | |    |  __|  	|  __  |  __| | |    |  ___/ 
--	   | |   _| |_| |____| |____ 	| |  | | |____| |____| |     
--	   |_|  |_____|______|______|	|_|  |_|______|______|_|     
--	Methods that will make it easier to check the lists
--	i.e. like finding what tiletype and which
--	index a certain position is in the lists

--	Checks the given location against all
--	defined River types
MapGenerator.IsRiver = function(self, X, Z)

	--	First check if the position is inside
	--	the world
	if not self:IsInsideWorld(X, Z) then
		return false
	end
	
	--	Get the type and check against all River Types
	local	tempTileType	=	self:GetTileType(X, Z)
	if	
		tempTileType == self.RiverUp 	or
		tempTileType == self.RiverRight	or
		tempTileType == self.RiverDown	or
		tempTileType == self.RiverLeft	
	then
		return	true
	end
	
	--	Not a river tile
	return false
end

--	Checks if the given location(X, Z) is walkable
MapGenerator.IsWalkable = function(self, X, Z)
	--	First check if the position is inside
	--	the world
	if not self:IsInsideWorld(X, Z) then
		return false
	end
	
	--	Get the type and check against all River Types
	local	tempTileType	=	self:GetTileType(X, Z)
	if	
		tempTileType == self.Void	or
		tempTileType == self.Stone	or
		tempTileType == self.Hole
	then
		return	false
	end
	
	--	Walkable
	return true
end

--	Sets the tiletype of the given location
MapGenerator.SetTileType = function(self, X, Z, TileType)
	if not self:IsInsideWorld(X, Z) then
		return
	end
	
	self.TileTypes[self:GetListIndex(X, Z)]	=	TileType
	
	--	Update the pathfinder
	local walkable = self:IsWalkable(X, Z)
	PathfinderHandler.SetTileWalkable(X, Z, self:IsWalkable(X, Z))
	
	--local newEntity = world:CreateNewEntity()
	--world:CreateComponentAndAddTo("TileWalkabilityHasChanged", newEntity)
	--world:GetComponent(newEntity, "MapSpecs", 0):SetInt2(X, Z)
	--world:GetComponent(newEntity, "MapSpecs", "Walkable"):SetBool(walkable)
end

--	Removes the current tile by setting it to Void
MapGenerator.RemoveTile = function(self, X, Z)
	self:SetTileType(X, Z, self.Void)
end

--	Will check if the given location(X, Z) is
--	inside the world's MIN/MAX
MapGenerator.IsInsideWorld = function(self, X, Z)
	if X < 0 or X >= self.MapSizeX then
		return	false
	end
	
	if Z < 0 or Z >= self.MapSizeZ then
		return	false
	end
	
	return 	true
end

--	Will return the tiletype of the given location(X, Z)
--	Returns -1(UNDEFINED) if position is outside the map
MapGenerator.GetTileType = function(self, X, Z)
	--	Check if the position is inside the world
	if not self:IsInsideWorld(X, Z) then
		return	self.UNDEFINED
	end
	
	--	Return the tile type
	return self.TileTypes[self:GetListIndex(X,Z)]
end

--	Returns the current index for a given location(X, Z)
MapGenerator.GetListIndex = function(self, X, Z)
	return (Z * self.MapSizeX + X + 1)
end


--	 __  __          _____  	  _____ _____  ______       _______ _____ ____  _   _ 
--	|  \/  |   /\   |  __ \ 	 / ____|  __ \|  ____|   /\|__   __|_   _/ __ \| \ | |
--	| \  / |  /  \  | |__) |	| |    | |__) | |__     /  \  | |    | || |  | |  \| |
--	| |\/| | / /\ \ |  ___/ 	| |    |  _  /|  __|   / /\ \ | |    | || |  | | . ` |
--	| |  | |/ ____ \| |     	| |____| | \ \| |____ / ____ \| |   _| || |__| | |\  |
--	|_|  |_/_/    \_\_|     	 \_____|_|  \_\______/_/    \_\_|  |_____\____/|_| \_|
--	Methods that will create the actual map, will be run to make
--	sure each tile has its correct settings

--	Called to create the actual map, this
--	method will convert each tile type to 
--	the correct entity
MapGenerator.CreateMap = function(self)

	--	Save all river tile entities
	--	to change the corners
	local	riverTiles			=	{}
			riverTiles.__mode	=	"k"
	for Z = 0, self.MapSizeZ-1 do
		for X = 0, self.MapSizeX-1 do
		
			local	tTileType	=	self:GetTileType(X, Z)
			
			if tTileType == self.Grass then
				self:CreateGrassEntity(X, Z)
			elseif tTileType == self.Hole then
				
				local	hasWaterNear	=	false
				for n = self.RiverUp, self.RiverLeft do
					if self:HasAdjacentTileOfType(X, Z, n) then
						hasWaterNear	=	true
						break
					end
				end
				
				if hasWaterNear then
					self:SetTileType(X, Z, self.Void)
					self:CreateVoidEntity(X, Z)
				else
					self:CreateHoleEntity(X, Z)
				end
				
			elseif tTileType == self.Stone then
				self:CreateStoneEntity(X, Z)
			elseif tTileType == self.Spawnpoint then
				self:CreateSpawnpointEntity(X, Z)
			elseif tTileType >= self.Checkpoint then
				self:CreateCheckpointEntity(X, Z, tTileType-self.Checkpoint+1)
			elseif self:IsRiver(X, Z) then
				riverTiles[#riverTiles+1]	=	self:CreateRiverEntity(X, Z)
			else
				self:CreateVoidEntity(X, Z)
			end
		end
	end
	
	--	Fix the river corners
	self:FixRiverCorners(riverTiles)
	self:FixRiverEffects(riverTiles)
	self:FixEdges()
	self:PlaceJibberish()
	
	--	Spawn info entity
	local	dataEntity	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("MapSpecs", dataEntity)
	world:CreateComponentAndAddTo("SyncNetwork", dataEntity)
	world:GetComponent(dataEntity, "MapSpecs", "NoOfSpawnpoints"):SetInt(self.Players)
	world:GetComponent(dataEntity, "MapSpecs", "NoOfCheckpoints"):SetInt(self.Checkpoints)
	world:GetComponent(dataEntity, "MapSpecs", "SizeX"):SetInt2(self.MapSizeX, self.MapSizeZ)
	
	-- Initialize potential fields
	PotentialFieldHandler.InitPFHandler(self.MapSizeX, self.MapSizeZ, self.Players)
	
	--	Generate island
	self:GenerateIslandBelow()
	
	--	Generate directional light
	local	R,G,B		=	0.8,0.7,0.6
	local 	newLight 	= 	world:CreateNewEntity()
	world:CreateComponentAndAddTo("DirectionalLight", newLight)
	world:CreateComponentAndAddTo("SyncNetwork", newLight)
    world:GetComponent(newLight, "DirectionalLight", 0):SetDirectionalLight(math.sin(math.random(1, 360)), -1.0, math.sin(math.random(1, 360)), 0.45, 0.65, 0.65, R, G, B)
	
	
end

--	Creates a new entity with the Tile template
--	and returns the id
MapGenerator.CreateTileEntity = function(self, X, Z)

	local	newTile	=	world:CreateNewEntity("Tile")
	world:GetComponent(newTile, "Position", "X"):SetFloat3(X, 0, Z)
	world:GetComponent(newTile, "MapPosition", "X"):SetInt2(X, Z)
	
	local check = ((X + Z) % 2)
	world:GetComponent(newTile, "Color", "X"):SetFloat(check*0.75)
	world:GetComponent(newTile, "Color", "Y"):SetFloat(check)
	world:GetComponent(newTile, "Color", "Z"):SetFloat(check*0.75)
	
	return	newTile
end

MapGenerator.CreateGrassEntity = function(self, X, Z)
	
	local	newGrass	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newGrass)
	world:GetComponent(newGrass, "Model", 0):SetModel("grass", "grass", 0)
	world:CreateComponentAndAddTo("NoShadow", newGrass)
	world:GetComponent(newGrass, "Rotation", 0):SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
	
	world:CreateComponentAndAddTo("StaticModel", newGrass)
	
	return	newGrass
end

MapGenerator.CreateHoleEntity = function(self, X, Z)
	
	local	newHole	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newHole)
	world:GetComponent(newHole, "Model", 0):SetModel("hole", "hole", 0)
	world:CreateComponentAndAddTo("NoShadow", newHole)
	
	world:CreateComponentAndAddTo("Void", newHole)
	world:GetComponent(newHole, "Rotation", 0):SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
	
	world:CreateComponentAndAddTo("StaticModel", newHole)
	
	return newHole
end

MapGenerator.CreateRiverEntity = function(self, X, Z)
	
	local	newRiver	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newRiver)
	world:GetComponent(newRiver, "Model", 0):SetModel("riverstraight", "riverstraight", 0)
	
	local	tRiverType				=	self:GetTileType(X, Z)
	local	tRiverDirX, tRiverDirZ	=	self:GetRiverDirection(tRiverType)
	world:CreateComponentAndAddTo("River", newRiver)
	world:GetComponent(newRiver, "River", 0):SetInt3(tRiverDirX, tRiverDirZ, 1)
	world:GetComponent(newRiver, "TileOffset", "Offset"):SetFloat(0.2)
	
	world:CreateComponentAndAddTo("StaticModel", newRiver)
	
	--	Set correct rotation
	if tRiverType == self.RiverUp then
		world:GetComponent(newRiver, "Rotation", 0):SetFloat3(0, -math.pi/2, 0)
	elseif tRiverType == self.RiverRight then
		world:GetComponent(newRiver, "Rotation", 0):SetFloat3(0, math.pi, 0)
	elseif tRiverType == self.RiverDown then
		world:GetComponent(newRiver, "Rotation", 0):SetFloat3(0, math.pi/2, 0)
	elseif tRiverType == self.RiverLeft then
		--	Nothing
	end
	
	return	newRiver
end

MapGenerator.CreateStoneEntity = function(self, X, Z)

	local	newGrass	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("NotWalkable", newGrass)
	world:CreateComponentAndAddTo("Model", newGrass)
	world:GetComponent(newGrass, "Model", 0):SetModel("grass", "grass", 0)
	world:CreateComponentAndAddTo("NoShadow", newGrass)
	world:GetComponent(newGrass, "Rotation", 0):SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
	world:CreateComponentAndAddTo("StaticModel", newGrass)

	local	newStone	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", newStone)
	world:CreateComponentAndAddTo("Rotation", newStone)
	world:CreateComponentAndAddTo("Scale", newStone)
	world:CreateComponentAndAddTo("MapPosition", newStone)
	world:CreateComponentAndAddTo("SyncNetwork", newStone)
	world:CreateComponentAndAddTo("Model", newStone)
	world:CreateComponentAndAddTo("StaticModel", newStone)
	
	world:GetComponent(newStone, "Model", 0):SetModel("smallstone", "smallstone", 0)
	world:GetComponent(newStone, "Position", 0):SetFloat3(X, 0.5 + 0.1* math.random(-1, 1), Z)
	world:GetComponent(newStone, "MapPosition", 0):SetInt2(X, Z)
	world:GetComponent(newStone, "Rotation", 0):SetFloat3
	(
		math.pi * 0.005 * math.random(0, 25), 
		math.pi * 0.01 * math.random(0, 100), 
		math.pi * 0.005 * math.random(0, 25)
	)
	world:GetComponent(newStone, "Scale", 0):SetFloat3
	(
		0.8 + 0.1* math.random(-1, 1), 
		0.8 + 0.1* math.random(-1, 1), 
		0.8 + 0.1* math.random(-1, 1)
	)

	return	newGrass
end

MapGenerator.CreateSpawnpointEntity = function(self, X, Z)
	
	local	newSpawnpoint	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newSpawnpoint)
	world:GetComponent(newSpawnpoint, "Model", 0):SetModel("grass", "grass", 0)
	world:CreateComponentAndAddTo("NoShadow", newSpawnpoint)
	
	local newSpawnId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AvailableSpawnpoint", newSpawnId)
	world:GetComponent(newSpawnId, "AvailableSpawnpoint", 0):SetInt2(X, Z)
	
	world:CreateComponentAndAddTo("StaticModel", newSpawnpoint)
	
	return	newSpawnpoint
end

MapGenerator.CreateCheckpointEntity = function(self, X, Z, Number)
	
	local	newCheckpoint	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Checkpoint", newCheckpoint)
	world:GetComponent(newCheckpoint, "Checkpoint", 0):SetInt(Number)
	world:CreateComponentAndAddTo("Model", newCheckpoint)
	world:GetComponent(newCheckpoint, "Model", 0):SetModel("checkpoint", "checkpoint", 0)
	world:GetComponent(newCheckpoint, "TileOffset", "Offset"):SetFloat(0.60)
	
	world:CreateComponentAndAddTo("StaticModel", newCheckpoint)
	
	return	newCheckpoint
end

MapGenerator.CreateVoidEntity = function(self, X, Z)
	local	newVoid	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Void", newVoid)
	
	return	newVoid
end


--	 _____   ____   _____ _______ 	 ______ _______   ________  _____ 
--	|  __ \ / __ \ / ____|__   __|	|  ____|_   _\ \ / /  ____|/ ____|
--	| |__) | |  | | (___    | |   	| |__    | |  \ V /| |__  | (___  
--	|  ___/| |  | |\___ \   | |   	|  __|   | |   > < |  __|  \___ \ 
--	| |    | |__| |____) |  | |   	| |     _| |_ / . \| |____ ____) |
--	|_|     \____/|_____/   |_|   	|_|    |_____/_/ \_\______|_____/ 
--	Methods that will have to be executed after the actual
--	map have been generated should be here

--	Fix river corners (change model and rotate)
MapGenerator.FixRiverCorners = function(self, riverTiles)

	for waterA = 1, #riverTiles do
		local waterPosA = world:GetComponent(riverTiles[waterA], "MapPosition", 0)
		local waterDirA = world:GetComponent(riverTiles[waterA], "River", 0)
		local posAX, posAY = waterPosA:GetInt2()
		local dirAX, dirAY = waterDirA:GetInt2()

		for waterB = 1, #riverTiles do
			if waterA ~= waterB then
				local waterPosB = world:GetComponent(riverTiles[waterB], "MapPosition", 0)
				local waterDirB = world:GetComponent(riverTiles[waterB], "River", 0)
				local posBX, posBY = waterPosB:GetInt2()
				local dirBX, dirBY = waterDirB:GetInt2()

				if posAX + dirAX == posBX and posAY + dirAY == posBY then
					if dirAX ~= dirBX and dirAY ~= dirBY then

						local comp = world:GetComponent(riverTiles[waterB], "Model", 0)
						comp:SetModel("rivercorner", "rivercorner", 0, 0)

						--	LEFT TURN (Correct rotation)
						--if dirAX == 1 and dirBY == -1 then
						--elseif dirAX == -1 and dirBY == 1 then
						--elseif dirAY == -1 and dirBX == -1 then
						--elseif dirAY == 1 and dirBX == 1 then

						local comp = world:GetComponent(riverTiles[waterB], "Rotation", 0)
						local currentRotation = comp:GetFloat(1)
						
						world:CreateComponentAndAddTo("RiverCornerDir", riverTiles[waterB])
						print("RiverCornerDir Created: " .. riverTiles[waterB])
						--	RIGHT TURN
						if dirAX == 1 and dirBY == 1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(riverTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						elseif dirAX == -1 and dirBY == -1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(riverTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						elseif dirAY == 1 and dirBX == -1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(riverTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						elseif dirAY == -1 and dirBX == 1 then
							comp:SetFloat3(0, currentRotation - math.pi/2, 0)
							world:GetComponent(riverTiles[waterB], "RiverCornerDir", "Dir"):SetText("Right")
						--	LEFT TURN
						else
							world:GetComponent(riverTiles[waterB], "RiverCornerDir", "Dir"):SetText("Left")
						end
						
					end
				end
			end
		end
	end
	
end

--	Fix river starts and ends (change model and particle effects)
MapGenerator.FixRiverEffects = function(self, riverTiles)

	for waterA = 1, #riverTiles do
		local 	waterPosA 		= 	world:GetComponent(riverTiles[waterA], "MapPosition", 0)
		local 	waterDirA 		= 	world:GetComponent(riverTiles[waterA], "River", 0)
		local 	posAX, posAY 	= 	waterPosA:GetInt2()
		local	dirAX, dirAY 	= 	waterDirA:GetInt2()
		
		local	isFirstTile	=	true
		local	isLastTile	=	true
		for waterB = 1, #riverTiles do
			if waterA ~= waterB then
				local	waterPosB 		= 	world:GetComponent(riverTiles[waterB], "MapPosition", 0)
				local	waterDirB 		= 	world:GetComponent(riverTiles[waterB], "River", 0)
				local	posBX, posBY	= 	waterPosB:GetInt2()
				local	dirBX, dirBY 	= 	waterDirB:GetInt2()

				if posBX + dirBX == posAX and posBY + dirBY == posAY then
					isFirstTile	=	false
					break
				end
				

			end
		end
		for waterB = 1, #riverTiles do
			if waterA ~= waterB then
				local	waterPosB 		= 	world:GetComponent(riverTiles[waterB], "MapPosition", 0)
				local	waterDirB 		= 	world:GetComponent(riverTiles[waterB], "River", 0)
				local	posBX, posBY	= 	waterPosB:GetInt2()
				local	dirBX, dirBY 	= 	waterDirB:GetInt2()
				if posAX + dirAX == posBX and posAY + dirAY == posBY then
					isLastTile	=	false
					break
				end
			end
		end
		
		if isFirstTile then

			world:GetComponent(riverTiles[waterA], "Model", 0):SetModel("riverend", "riverend", 0)
			
			local rotComp = world:GetComponent(riverTiles[waterA], "Rotation", 0)
			local currentRotation = rotComp:GetFloat(1)
			rotComp:SetFloat(currentRotation + math.pi, 1)
			
			
			local	newParticle	=	world:CreateNewEntity()
			world:CreateComponentAndAddTo("Position", newParticle)
			world:CreateComponentAndAddTo("Color", newParticle)
			world:CreateComponentAndAddTo("Particle", newParticle)
			world:CreateComponentAndAddTo("SyncNetwork", newParticle)
			
			world:GetComponent(newParticle, "Position", "X"):SetFloat3(posAX, 0.42, posAY)
			world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.7, 0.7, 0.8)
			
			world:GetComponent(newParticle, "Particle", "aName"):SetText("waterspawn")
			world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/wassertmp.png")
			world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(30)
			world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(1.2)
			world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.05)
			world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.0)
			world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.05)
			world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(0)
			world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0)
			world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(0)
			world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(0.40)
			world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
		end
		if isLastTile then
			
			if self:GetTileType(posAX + dirAX, posAY + dirAY) == self.Void then
				local	newParticle	=	world:CreateNewEntity()
				world:CreateComponentAndAddTo("Position", newParticle)
				world:CreateComponentAndAddTo("Color", newParticle)
				world:CreateComponentAndAddTo("Particle", newParticle)
				world:CreateComponentAndAddTo("SyncNetwork", newParticle)
				
				world:GetComponent(newParticle, "Position", "X"):SetFloat3(posAX + 0.35*dirAX, 0.43, posAY + 0.35*dirAY)
				
				world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.0)
				
				world:GetComponent(newParticle, "Particle", "aName"):SetText("waterfall")
				world:GetComponent(newParticle, "Particle", "bTexture"):SetText("content/textures/wassertmp.png")
				world:GetComponent(newParticle, "Particle", "cParticles"):SetInt(50)
				world:GetComponent(newParticle, "Particle", "dLifetime"):SetFloat(3.0)
				world:GetComponent(newParticle, "Particle", "eScaleX"):SetFloat(0.09*dirAY)
				world:GetComponent(newParticle, "Particle", "eScaleY"):SetFloat(0.12)
				world:GetComponent(newParticle, "Particle", "eScaleZ"):SetFloat(0.09*dirAX)
				world:GetComponent(newParticle, "Particle", "fVelocityX"):SetFloat(dirAX*0.3)
				world:GetComponent(newParticle, "Particle", "fVelocityY"):SetFloat(0.055)
				world:GetComponent(newParticle, "Particle", "fVelocityZ"):SetFloat(dirAY*0.3)
				world:GetComponent(newParticle, "Particle", "gSpriteSize"):SetFloat(1.2)
				world:GetComponent(newParticle, "Particle", "hId"):SetInt(-1)
			else
				if self:GetTileType(posAX + dirAX, posAY + dirAY) ~= self.Hole then 
					if self:IsRiver(posAX - dirAX, posAY - dirAY) then
						world:GetComponent(riverTiles[waterA], "Model", 0):SetModel("riverend", "riverend", 0)
					end
				end
			end
		end
		
	end
	
end

MapGenerator.FixEdges = function(self)
	
	local	edgeTiles			=	{}
			edgeTiles.__mode	=	"k"
			
	for Z = 0, self.MapSizeZ do
		for X = 0, self.MapSizeX do
			local	nAdjacent	=	self:NumberOfAdjacentTiles(X, Z)
			local	tType		=	self:GetTileType(X, Z)
			if tType ~= self.Void and tType ~= self.UNDEFINED then
				if nAdjacent >= 1 and nAdjacent <= 3 then
					edgeTiles[#edgeTiles+1]	=	self:GetListIndex(X, Z)
					
					self:PlaceEdgeAt(X, Z)
				end
			end
		end
	end
	
	if #edgeTiles == 0 then
		return
	end
	
end

MapGenerator.CreateEdgePiece = function(self, X, Z, isCorner, EdgeDirection)
	local	newEdge	=	world:CreateNewEntity()
	
	world:CreateComponentAndAddTo("Position", newEdge)
	world:CreateComponentAndAddTo("Rotation", newEdge)
	world:CreateComponentAndAddTo("Scale", newEdge)
	world:CreateComponentAndAddTo("SyncNetwork", newEdge)
	world:CreateComponentAndAddTo("Model", newEdge)
	world:CreateComponentAndAddTo("StaticModel", newEdge)
	
	world:GetComponent(newEdge, "Position", 0):SetFloat3(0, 0.0, 0)
	world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(newEdge, "Scale", 0):SetFloat3(1.0, 1.0, 1.0)
	
	if isCorner then
		world:GetComponent(newEdge, "Model", 0):SetModel("edgeoutercorner", "edgeoutercorner", 1)
	else
		if self:IsRiver(X, Z) then
			
			local	dirX, dirZ	=	self:GetRiverDirection(self:GetTileType(X, Z))
			
			if self:GetTileType(X+dirX, Z+dirZ) == self.Void then
				
				local	tDirX, tDirZ	=	self:GetRiverDirection(self.RiverUp + EdgeDirection)
				if tDirX == dirX and tDirZ == dirZ then
					world:GetComponent(newEdge, "Model", 0):SetModel("edgeriver", "edgeriver", 1)
				else
					world:GetComponent(newEdge, "Model", 0):SetModel("edgeflat", "edgeflat", 1)
				end
			else
				world:GetComponent(newEdge, "Model", 0):SetModel("edgeflat", "edgeflat", 1)
			end
		else
			world:GetComponent(newEdge, "Model", 0):SetModel("edgeflat", "edgeflat", 1)
		end
	end
	
	return	newEdge
end

MapGenerator.PlaceEdgeAt = function(self, X, Z)
	
	local	tUp, tRight, tDown, tLeft	=	self:GetEmptyAdjacentTiles(X, Z)
	
	local	newEdge		=	0
	local	tEdgeOffset	=	0.509
	
	if tUp then
		newEdge	=	self:CreateEdgePiece(X, Z, false, 0)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X, 0.0, Z-tEdgeOffset)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, math.pi, 0.0)
		
	end
	
	if tRight then
		newEdge	=	self:CreateEdgePiece(X, Z, false, 1)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X+tEdgeOffset, 0.0, Z)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, math.pi/2, 0.0)
	end
	
	if tDown then
		newEdge	=	self:CreateEdgePiece(X, Z, false, 2)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X, 0.0, Z+tEdgeOffset)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	end
	
	if tLeft then
		newEdge	=	self:CreateEdgePiece(X, Z, false, 3)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X-tEdgeOffset, 0.0, Z)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, -math.pi/2, 0.0)
	end
	
	if tDown and tLeft then
		newEdge	=	self:CreateEdgePiece(X, Z, true, -1)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X-0.5014, 0.0, Z+0.5102)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, -math.pi/2, 0.0)
	end
	
	if tDown and tRight then
		newEdge	=	self:CreateEdgePiece(X, Z, true, -1)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X+0.5101, 0.0, Z+0.5018)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, 0, 0.0)
	end
	
	if tUp and tLeft then
		newEdge	=	self:CreateEdgePiece(X, Z, true, -1)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X-0.5101, 0.0, Z-0.5018)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, math.pi, 0.0)
	end
	
	if tUp and tRight then
		newEdge	=	self:CreateEdgePiece(X, Z, true, -1)
		world:GetComponent(newEdge, "Position", 0):SetFloat3(X+0.5014, 0.0, Z-0.5102)
		world:GetComponent(newEdge, "Rotation", 0):SetFloat3(0.0, math.pi-math.pi/2, 0.0)
	end
	

end

MapGenerator.PlaceJibberish = function(self)
	
	--	Place stones
	local	stonesToSpawn	=	self:GetPlayableTiles()/7.0
	for n = 1, stonesToSpawn do
	
		local	tX, tZ	=	self:GetRandomTileOfType(self.Grass)
		if tX + tZ >= 0 then
			local 	newStone 	= 	world:CreateNewEntity()
			world:CreateComponentAndAddTo("Position", newStone)
			world:CreateComponentAndAddTo("Rotation", newStone)
			world:CreateComponentAndAddTo("Scale", newStone)
			world:CreateComponentAndAddTo("SyncNetwork", newStone)
			world:CreateComponentAndAddTo("Model", newStone)
			world:CreateComponentAndAddTo("StaticModel", newStone)
			
			local randX = tX-0.5+math.random()
			local randZ = tZ-0.5+math.random()
			world:GetComponent(newStone, "Position", 0):SetFloat3(randX, 0.5, randZ)
			world:GetComponent(newStone, "Rotation", 0):SetFloat3(math.pi * 0.01 * math.random(0, 25), math.pi * 0.01 * math.random(0, 100), math.pi * 0.01 * math.random(0, 25))
			local randScale = (math.random() + 0.5)*0.15
			world:GetComponent(newStone, "Scale", 0):SetFloat3(randScale, randScale, randScale)
			world:GetComponent(newStone, "Model", 0):SetModel("smallstone", "smallstone", 8)
			world:CreateComponentAndAddTo("NoShadow", newStone)
			
			
			--[[world:CreateComponentAndAddTo("LerpScale", newStone)
			world:GetComponent(newStone, "LerpScale", "X"):SetFloat(randScale)
			world:GetComponent(newStone, "LerpScale", "Y"):SetFloat(randScale)
			world:GetComponent(newStone, "LerpScale", "Z"):SetFloat(randScale)
			world:GetComponent(newStone, "LerpScale", "Time"):SetFloat(0.8)
			world:GetComponent(newStone, "LerpScale", "Algorithm"):SetText("OvershotLerp")--]]
		end
	end
	
	
	--	Place grass
	local	grassToSpawn	=	self:GetPlayableTiles()/7.0
	for n = 1, grassToSpawn do
	
		local	tX, tZ	=	self:GetRandomTileOfType(self.Grass)
		if tX + tZ >= 0 then
			local 	newGrass 	= 	world:CreateNewEntity()
			world:CreateComponentAndAddTo("Position", newGrass)
			world:CreateComponentAndAddTo("Rotation", newGrass)
			world:CreateComponentAndAddTo("Scale", newGrass)
			world:CreateComponentAndAddTo("SyncNetwork", newGrass)
			world:CreateComponentAndAddTo("Model", newGrass)
			world:CreateComponentAndAddTo("StaticModel", newGrass)
			
			local randX = tX-0.5+math.random()
			local randZ = tZ-0.5+math.random()
			world:GetComponent(newGrass, "Position", 0):SetFloat3(randX, 0.45, randZ)
			world:GetComponent(newGrass, "Rotation", 0):SetFloat3(0, math.pi * 0.01 * math.random(0, 100),0)
			local randScale = (math.random() + 0.5)*0.5
			world:GetComponent(newGrass, "Scale", 0):SetFloat3(randScale, randScale, randScale)
			world:GetComponent(newGrass, "Model", 0):SetModel("tallgrass", "tallgrass", 9)
			world:CreateComponentAndAddTo("NoShadow", newGrass)
			
			--[[world:CreateComponentAndAddTo("LerpScale", newGrass)
			world:GetComponent(newGrass, "LerpScale", "X"):SetFloat(randScale)
			world:GetComponent(newGrass, "LerpScale", "Y"):SetFloat(randScale)
			world:GetComponent(newGrass, "LerpScale", "Z"):SetFloat(randScale)
			world:GetComponent(newGrass, "LerpScale", "Time"):SetFloat(0.8)
			world:GetComponent(newGrass, "LerpScale", "Algorithm"):SetText("OvershotLerp")--]]
		end
	end
	
	
	
	self:PlaceTrees()
end


MapGenerator.PlaceTrees = function(self)

	local	allTiles		=	{}
			allTiles.__mode	=	"k"
	for Z = 0, self.MapSizeZ-1 do
		for X = 0, self.MapSizeX-1 do
			if self:GetTileType(X, Z) ~= self.Grass then
				allTiles[#allTiles+1]	=	true
			else
				allTiles[#allTiles+1]	=	false
			end
		end
	end
	
	
	
	--	Place trees
	local	treesToSpawn	=	math.ceil(math.sqrt(self.MapSizeX*self.MapSizeZ*self:GetPlayableTiles())*0.1)
	local	tX,	tZ			=	0, 0
	local	nTries			=	0
	while treesToSpawn >= 0 do
		
		--	Randomize a position
		tX	=	math.random(self.VoidMargin, self.MapSizeX-self.VoidMargin)
		tZ	=	math.random(self.VoidMargin, self.MapSizeZ-self.VoidMargin)
		
		if not allTiles[self:GetListIndex(tX, tZ)] then
			
			local	newTree 	= 	world:CreateNewEntity()
			world:CreateComponentAndAddTo("Position", newTree)
			world:CreateComponentAndAddTo("Rotation", newTree)
			world:CreateComponentAndAddTo("Scale", newTree)
			world:CreateComponentAndAddTo("Color", newTree)
			world:CreateComponentAndAddTo("SyncNetwork", newTree)
			world:CreateComponentAndAddTo("Model", newTree)
			world:CreateComponentAndAddTo("IsTree", newTree)
			
			local randX = tX+(math.random(0, 1)-0.5)*0.8
			local randZ = tZ+(math.random(0, 1)-0.5)*0.8
			world:GetComponent(newTree, "Position", 0):SetFloat3(randX, 0.40, randZ)
			world:GetComponent(newTree, "Rotation", 0):SetFloat3(math.pi * 0.01 * math.random(0, 10), math.pi * 0.01 * math.random(0, 100), math.pi * 0.01 * math.random(0, 10))
			local randScale = 0.9 - math.sin(math.random(0, 360)) * 0.2
			world:GetComponent(newTree, "Scale", 0):SetFloat3(0, 0, 0)
			world:GetComponent(newTree, "Color", 0):SetFloat3(math.random(), math.random(), math.random())
			world:GetComponent(newTree, "Model", 0):SetModel("tree", "tree", 1)
			
			world:CreateComponentAndAddTo("LerpScale", newTree)
			world:GetComponent(newTree, "LerpScale", "X"):SetFloat(randScale)
			world:GetComponent(newTree, "LerpScale", "Y"):SetFloat(randScale)
			world:GetComponent(newTree, "LerpScale", "Z"):SetFloat(randScale)
			world:GetComponent(newTree, "LerpScale", "Time"):SetFloat(0.8)
			world:GetComponent(newTree, "LerpScale", "Algorithm"):SetText("OvershotLerp")
			
			for Z = -1, 1 do
				for X = -1, 1 do
					
					if self:GetDistanceBetween(tX, tZ, tX+X, tZ+Z) < 1.8 then
						allTiles[self:GetListIndex(tX+X, tZ+Z)]	=	true
					end
				end
			end
			
			allTiles[self:GetListIndex(tX, tZ)]	=	true
			
			treesToSpawn	=	treesToSpawn-1
		else
			nTries	=	nTries+1
			if nTries >= 10 then
				treesToSpawn	=	treesToSpawn-1
			end
		end
	end
end


MapGenerator.GenerateIslandBelow = function(self)

	local	stringMap	=	""
	
	for Z = 0, self.MapSizeZ-1 do
		for X = 0, self.MapSizeX-1 do
			if self:GetTileType(X, Z) == self.Void then
				stringMap	=	stringMap .. "o"
			else
				stringMap	=	stringMap .. "."
			end
		end
	end
	
	--MapCreation.GenerateIslandMesh(self.MapSizeX, self.MapSizeZ, stringMap)
	local generateIsland = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SyncNetwork", generateIsland)
	world:CreateComponentAndAddTo("GenerateIsland", generateIsland)
	world:GetComponent(generateIsland, "GenerateIsland", "Map"):SetString(stringMap)
	world:GetComponent(generateIsland, "GenerateIsland", "SizeX"):SetInt(self.MapSizeX)
	world:GetComponent(generateIsland, "GenerateIsland", "SizeZ"):SetInt(self.MapSizeZ)
end


--	 _    _ ______ _      _____  	 __  __ ______ _______ _    _  ____  _____   _____ 
--	| |  | |  ____| |    |  __ \ 	|  \/  |  ____|__   __| |  | |/ __ \|  __ \ / ____|
--	| |__| | |__  | |    | |__) |	| \  / | |__     | |  | |__| | |  | | |  | | (___  
--	|  __  |  __| | |    |  ___/ 	| |\/| |  __|    | |  |  __  | |  | | |  | |\___ \ 
--	| |  | | |____| |____| |     	| |  | | |____   | |  | |  | | |__| | |__| |____) |
--	|_|  |_|______|______|_|     	|_|  |_|______|  |_|  |_|  |_|\____/|_____/|_____/ 
--	Methods for making things easier

--	Returns the distance between two locations
MapGenerator.GetDistanceBetween = function(self, X1, Z1, X2, Z2)
	return math.sqrt((X2-X1)^2 + (Z2-Z1)^2)
end

--	Returns a position that is within a certain margin of the edges
--	MarginX/MarginZ is defined in number of tiles away from the edges
MapGenerator.GetRandomPositionWithinMargin = function(self, MarginX, MarginZ)
	
	MarginX	=	math.abs(MarginX)
	MarginZ	=	math.abs(MarginZ)
	
	--	Set the bounds of the random location
	local	minX,	maxX	=	self.VoidMargin+MarginX,	self.MapSizeX-MarginX-self.VoidMargin-1
	local	minZ,	maxZ	=	self.VoidMargin+MarginZ,	self.MapSizeZ-MarginZ-self.VoidMargin-1	
	
	--	Generate a random location within these bounds
	return	math.random(minX, maxX),	math.random(minZ, maxZ)
end

--	Returns a position that is Distance away from the given location
--	Returns -1, -1 if it failed
MapGenerator.GetPositionXDistanceAwayFrom = function(self, X, Z, Distance)
	
	local	newX, newZ		=	0, 0
	local	nTries			=	0
	local	nMaxTries		=	1000
	while true do
		
		newX, newZ	=	self:GetRandomPositionWithinMargin(0, 0)
		
		if self:GetDistanceBetween(X, Z, newX, newZ) >= Distance then
			return newX, newZ
		else
			nTries = nTries+1
			
			if nTries >= nMaxTries then
				if self.DebugInfo then
					print("!!! ERROR OCCURED IN GetPositionXDistanceAwayFrom(" .. X .. ", " .. Z .. ", " .. Distance .. ") !!!")
					print("!!! Max number of tries reached and no position found !!!")
				end
				
				break
			end
		end
	end
	
	return	-1, -1
end

--	Returns a position of the given tile type
--	Returns -1, -1 if it failed
MapGenerator.GetRandomTileOfType = function(self, Type)
	
	local	tempX, tempZ	=	0, 0
	local	nTries			=	1
	local	nMaxTries		=	1000
	while true do
		
		tempX, tempZ	=	self:GetRandomPositionWithinMargin(0, 0)
		if self:GetTileType(tempX, tempZ) == Type then
			return tempX, tempZ
		else
			nTries	=	nTries+1
			
			if nTries >= nMaxTries then
				break
			end
		end
	end
	
	--	Loop through the whole list and see if there are 
	--	any tiles of the given type, and return first found
	for Z = 0, self.MapSizeZ-1 do
		for X = 0, self.MapSizeX-1 do
			if self:GetTileType(X, Z) == Type then
				return X, Z
			end
		end
	end
	
	
	if self.DebugInfo then
		print("!!! ERROR OCCURED IN GetRandomTileOfType(".. Type .. ") !!!")
		print("!!! Max number of tries reached and no position found !!!")
	end
	return	-1, -1
end

--	Returns true if there is a path from (X1, Z1) to (X2, Z2)
MapGenerator.CanWalkBetween = function(self, X1, Z1, X2, Z2)
	return	PathfinderHandler.GeneratePath(X1, Z1, X2, Z2) ~= 2147483647
end

--	Get number of neighbours
MapGenerator.NumberOfAdjacentTiles = function(self, X, Z)
	
	local	nAdjacentTiles	=	0
	local	tempTileType	=	self.UNDEFINED

	--	Above
	tempTileType	=	self:GetTileType(X, Z-1)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		nAdjacentTiles	=	nAdjacentTiles+1
	end
	
	--	Right
	tempTileType	=	self:GetTileType(X+1, Z)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		nAdjacentTiles	=	nAdjacentTiles+1
	end
	
	--	Below
	tempTileType	=	self:GetTileType(X, Z+1)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		nAdjacentTiles	=	nAdjacentTiles+1
	end
	
	--	Left
	tempTileType	=	self:GetTileType(X-1, Z)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		nAdjacentTiles	=	nAdjacentTiles+1
	end
	
	return	nAdjacentTiles
end

--	Returns (bool, bool, bool, bool) of which sides are
--	empty/has no tile on them (UP, RIGHT, DOWN, LEFT)
MapGenerator.GetEmptyAdjacentTiles = function(self, X , Z)
	local	tUP, tDOWN, tLEFT, tRIGHT	=	true,true,true,true
	local	tempTileType	=	0
	
	--	Above
	tempTileType	=	self:GetTileType(X, Z-1)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		tUP	=	false
	end
	
	--	Right
	tempTileType	=	self:GetTileType(X+1, Z)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		tRIGHT	=	false
	end
	
	--	Below
	tempTileType	=	self:GetTileType(X, Z+1)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		tDOWN	=	false
	end
	
	--	Left
	tempTileType	=	self:GetTileType(X-1, Z)
	if tempTileType ~= self.Void and tempTileType ~= self.UNDEFINED then
		tLEFT	=	false
	end
	
	return	tUP, tRIGHT, tDOWN, tLEFT
end

MapGenerator.HasAdjacentTileOfType = function(self, X, Z, TileType)
	
	if self:GetTileType(X, Z-1) == TileType then
		return true
	end
	if self:GetTileType(X+1, Z) == TileType then
		return true
	end
	if self:GetTileType(X, Z+1) == TileType then
		return true
	end
	if self:GetTileType(X-1, Z) == TileType then
		return true
	end

	return false
end

MapGenerator.GetCenterOfMap = function(self)
	return	math.ceil((self.MapSizeX-1)/2), math.ceil((self.MapSizeZ-1)/2)
end

MapGenerator.GetPlayableTiles = function(self)
	local	numberOfTiles	=	0
	for Z = 0, self.MapSizeZ-1 do
		for X = 0, self.MapSizeX-1 do
			if self:GetTileType(X, Z) ~= self.Void then
				numberOfTiles	=	numberOfTiles+1
			end
		end
	end
	return	numberOfTiles
end

MapGenerator.CanWalkBetween = function(self, X1, Z1, X2, Z2)
	local	tCost	=	PathfinderHandler.GeneratePath(X1, Z1, X2, Z2)
	
	if tCost == 2147483647 then
		return false
	else
		return true
	end
end



MapGenerator.PrintDebugMessage = function(self, Message)
	if self.DebugInfo then
		print(Message)
	end
end




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

MapGenerator.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	--self:GenerateMap(os.time()%29181249, 4, 4)
	--self:GenerateMap(23246299, 4, 4)
	self:GenerateMap(23244210, 4, 4)
	--self:GenerateMap(23239474, 4, 4)
	--self:GenerateMap(5747, 4, 4)
	--self:GenerateMap(1338, 6, 4)
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
	
	--	Randomize initial values
	self.MapSizeX		=	math.random(8+NumberOfPlayers, 10+NumberOfPlayers) + 2*self.VoidMargin
	self.MapSizeZ		=	math.random(8+NumberOfPlayers, 10+NumberOfPlayers) + 2*self.VoidMargin
	self.Players		=	NumberOfPlayers
	self.Checkpoints	=	NumberOfCheckpoints
	self.Rivers			=	math.random(NumberOfPlayers, 2*NumberOfPlayers-NumberOfPlayers)
	
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
		print("Rivers: " .. self.Rivers)
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
	self:CreateGrassPlane()
	self:CarveVoidMargin()
	self:CarveVoidCorners()
	self:PlaceStones(math.random(self.Players, 2*self.Players-self.Players))
	
	--	Create all rivers
	self:CreateRivers()
	
	--	Place spawnpoints
	self:PlaceCheckpoints()
	self:PlaceSpawnpoints()
	
	self:FixEmptyTiles()
	--	Create the actual map
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
	local	centerDistance		=	math.ceil(self:GetDistanceBetween(centerX, centerZ, self.VoidMargin, self.VoidMargin)*0.1)
	local	riversCarved		=	0
	
	print("Distance : " .. centerDistance)
	while true do
		
		tempX, tempZ	=	self:GetPositionXDistanceAwayFrom(centerX, centerZ, centerDistance)
		if self:CarveRiverAt(tempX, tempZ) then
			riversCarved	=	riversCarved+1
			
			centerDistance	=	centerDistance+1
			
			if riversCarved == self.Rivers then
				break
			end
		end
	end
end

--	Carves a river at the given location
MapGenerator.CarveRiverAt = function(self, X, Z)
	
	--	Checks so the tile is OK
	if self:IsRiver(X, Z) then
		return	false
	end
	
	--	Create a random direction
	local	tempDirection	=	math.random(self.RiverUp, self.RiverLeft)
	local	backupDirection	=	tempDirection
	local	dirX,	dirZ	=	self:GetRiverDirection(tempDirection)
	local	tempX,	tempZ	=	X, Z
	
	if self:IsRiver(X+dirX, Z+dirZ) then
		return	false
	elseif self:IsRiver(X-dirX, Z-dirZ) then
		return	false
	end
	
	--	Create the initial river
	self:SetTileType(tempX, tempZ, tempDirection)
	
	--	Begin carving forward
	self:CarveRiverForward(tempX, tempZ, dirX, dirZ, tempDirection)

	--	Reset variables
	tempDirection	=	backupDirection
	dirX,	dirZ	=	self:GetRiverDirection(tempDirection)
	tempX,	tempZ	=	X, Z
	
	--	Begin carving backwards
	self:CarveRiverBackwards(tempX, tempZ, dirX, dirZ, tempDirection)
	
	return	true
end

--	Carves the river in its direction forward
MapGenerator.CarveRiverForward = function(self, tempX, tempZ, dirX, dirZ, tempDirection)
	
	local	carveStatus		=	0
	
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
	end

end

--	Carves the river in its direction backwards
MapGenerator.CarveRiverBackwards = function(self, tempX, tempZ, dirX, dirZ, tempDirection)
	
	local	carveStatus		=	0
	
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
			
			if tileType ~= self.Void and tileType ~= self.Stone then
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

	local	tX, tZ			=	X, Z
	local	tileType		=	self.UNDEFINED
	local	directionX		=	0
    local	directionZ		=	0
	local	tempDistance	=	0
	while true do
	
		directionX		=	(-1)^math.random(1, 10) * math.random(0, 1)
		directionZ		=	(-1)^math.random(1, 10) * math.random(0, 1)
		tempDistance	=	math.random(0, Distance)
		
		tX	=	X + tempDistance*directionX
		tZ	=	Z + tempDistance*directionZ
		
		tileType	=	self:GetTileType(tX, tZ)
		
		if tileType ~= self.Void and tileType ~= self.Stone and tileType < self.Checkpoint then
			self:SetTileType(tX, tZ, self.Stone)
			return tX, tZ
		end
	
	end
end


--	 ______ _____   _____ ______ 	  _____          _______      _______ _   _  _____ 
--	|  ____|  __ \ / ____|  ____|	 / ____|   /\   |  __ \ \    / /_   _| \ | |/ ____|
--	| |__  | |  | | |  __| |__   	| |       /  \  | |__) \ \  / /  | | |  \| | |  __ 
--	|  __| | |  | | | |_ |  __|  	| |      / /\ \ |  _  / \ \/ /   | | | . ` | | |_ |
--	| |____| |__| | |__| | |____ 	| |____ / ____ \| | \ \  \  /   _| |_| |\  | |__| |
--	|______|_____/ \_____|______|	 \_____/_/    \_\_|  \_\  \/   |_____|_| \_|\_____|
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
		local	maxDepth		=	math.floor((self.MapSizeX*self.MapSizeZ)/(4*math.sqrt(self.MapSizeX*self.MapSizeZ)))
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
					self:SetTileType(X, Z, self.Stone)
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

MapGenerator.PlaceSpawnpoints = function(self)

	local	centerX, centerZ	=	self:GetCenterOfMap()
	local	tSpawns	=	math.ceil(self.Players/2)
	local	tX,	tZ	=	0, 0 --self:GetPositionXDistanceAwayFrom(centerX, centerZ, 2*tSpawns)
	
	while true do
		tX,	tZ	=	self:GetPositionXDistanceAwayFrom(centerX, centerZ, 2*tSpawns)
		local	canSpawn	=	true
		for X = -tSpawns, tSpawns-1 do
			if self:GetTileType(tX+X, tZ) ~= self.Grass then
				canSpawn	=	false
				break
			end
		end
		
		if canSpawn then
			for X = -tSpawns, tSpawns-1 do
				self:SetTileType(tX+X, tZ, self.Spawnpoint)
			end
			break
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
	local	lastX, lastZ		=	self:GetPositionXDistanceAwayFrom(centerX, centerZ, 0)--self:GetRandomPositionWithinMargin(self.VoidMargin, self.VoidMargin)
	local	tempDistance		=	self:GetDistanceBetween(self.Void, self.Void, centerX, centerZ)--math.ceil(self:GetDistanceBetween(self.VoidMargin, self.VoidMargin, self.MapSizeX-self.VoidMargin-1, self.MapSizeZ-self.VoidMargin-1)/3)
	local	nTries				=	0
	local	nMaxTries			=	50
	for n = 0, self.Checkpoints-1 do
	
		while true do
			
			local	tX, tZ	=	self:GetPositionXDistanceAwayFrom(lastX, lastZ, tempDistance)
			if self:GetTileType(tX, tZ) == self.Grass then
				self:SetTileType(tX, tZ, self.Checkpoint+n)
				
				lastX	=	tX
				lastZ	=	tZ
				nTries	=	0
				
				self:PlaceStonesNear(tX, tZ, 2, 2)
				
				break
			else
				nTries = nTries + 1
				if nTries >= nMaxTries then
					tempDistance	=	math.floor(tempDistance - 1)
					nTries			=	0
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
	PathfinderHandler.SetTileWalkable(X, Z, self:IsWalkable(X, Z))
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
				self:CreateHoleEntity(X, Z)
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
	
	--	Spawn info entity
	local	dataEntity	=	world:CreateNewEntity()
	world:CreateComponentAndAddTo("MapSpecs", dataEntity)
	world:CreateComponentAndAddTo("SyncNetwork", dataEntity)
	world:GetComponent(dataEntity, "MapSpecs", "NoOfSpawnpoints"):SetInt(self.Players)
	world:GetComponent(dataEntity, "MapSpecs", "NoOfCheckpoints"):SetInt(self.Checkpoints)
	world:GetComponent(dataEntity, "MapSpecs", "SizeX"):SetInt2(self.MapSizeX, self.MapSizeZ)
	
	
end

--	Creates a new entity with the Tile template
--	and returns the id
MapGenerator.CreateTileEntity = function(self, X, Z)

	local	newTile	=	world:CreateNewEntity("Tile")
	world:GetComponent(newTile, "Position", "X"):SetFloat3(X, 0, Z)
	world:GetComponent(newTile, "MapPosition", "X"):SetInt2(X, Z)
	
	return	newTile
end

MapGenerator.CreateGrassEntity = function(self, X, Z)
	
	local	newGrass	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newGrass)
	world:GetComponent(newGrass, "Model", 0):SetModel("grass", "grass", 0, 0)
	world:GetComponent(newGrass, "Rotation", 0):SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
	
	return	newGrass
end

MapGenerator.CreateHoleEntity = function(self, X, Z)
	
	local	newHole	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newHole)
	world:GetComponent(newHole, "Model", 0):SetModel("hole", "hole", 0, 0)
	
	world:CreateComponentAndAddTo("Void", newHole)
	world:GetComponent(newHole, "Rotation", 0):SetFloat3(0, math.pi * 0.5 * math.random(0, 4), 0)
	
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

	local	newStone	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("NotWalkable", newStone)
	world:CreateComponentAndAddTo("Model", newStone)
	world:GetComponent(newStone, "Model", 0):SetModel("stone", "stone", 0)
	world:GetComponent(newStone, "Position", 0):SetFloat3(X, 0.5 + 0.1* math.random(-1, 1), Z)
	world:GetComponent(newStone, "Rotation", 0):SetFloat3
	(
		math.pi * 0.01 * math.random(0, 25), 
		math.pi * 0.01 * math.random(0, 100), 
		math.pi * 0.01 * math.random(0, 25)
	)
	
	local groundEntity = world:CreateNewEntity()
	
	world:CreateComponentAndAddTo("Position", groundEntity)
	world:CreateComponentAndAddTo("Rotation", groundEntity)
	world:CreateComponentAndAddTo("Scale", groundEntity)
	world:CreateComponentAndAddTo("MapPosition", groundEntity)
	world:CreateComponentAndAddTo("SyncNetwork", groundEntity)
	world:CreateComponentAndAddTo("Model", groundEntity)
	
	world:GetComponent(groundEntity, "Position", 0):SetFloat3(X, 0.0, Z)
	world:GetComponent(groundEntity, "MapPosition", 0):SetInt2(X, Z)
	world:GetComponent(groundEntity, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(groundEntity, "Scale", 0):SetFloat3(1.0, 1.0, 1.0)
	world:GetComponent(groundEntity, "Model", 0):SetModel("grass", "grass", 0)
	
	return	newStone
end

MapGenerator.CreateSpawnpointEntity = function(self, X, Z)
	
	local	newSpawnpoint	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Model", newSpawnpoint)
	world:GetComponent(newSpawnpoint, "Model", 0):SetModel("grass", "grass", 0, 0)
	
	local newSpawnId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("AvailableSpawnpoint", newSpawnId)
	world:GetComponent(newSpawnId, "AvailableSpawnpoint", 0):SetInt2(X, Z)
	
	return	newSpawnpoint
end

MapGenerator.CreateCheckpointEntity = function(self, X, Z, Number)
	
	local	newCheckpoint	=	self:CreateTileEntity(X, Z)
	world:CreateComponentAndAddTo("Checkpoint", newCheckpoint)
	world:GetComponent(newCheckpoint, "Checkpoint", 0):SetInt(Number)
	world:CreateComponentAndAddTo("Model", newCheckpoint)
	world:GetComponent(newCheckpoint, "Model", 0):SetModel("checkpoint" .. Number, "checkpoint", 0)
	
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
			--local	newParticle	=	world:CreateNewEntity()
			--world:CreateComponentAndAddTo("Position", newParticle)
			--world:CreateComponentAndAddTo("Color", newParticle)
			--world:CreateComponentAndAddTo("Particle", newParticle)
			--
			--world:GetComponent(newParticle, "Position", "X"):SetFloat3(posAX, 0.33, posAY)
			--world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.6)
			--
			--world:GetComponent(newParticle, "Particle", "Name"):SetText("smoke")
			--world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/firewhite.png")
			--world:GetComponent(newParticle, "Particle", "Particles"):SetInt(1000)
			--world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(400)
			--world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.15)
			--world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.6)
			--world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
			
		elseif isLastTile then
		
			--local	newParticle	=	world:CreateNewEntity()
			--world:CreateComponentAndAddTo("Position", newParticle)
			--world:CreateComponentAndAddTo("Color", newParticle)
			--world:CreateComponentAndAddTo("Particle", newParticle)
			--
			--world:GetComponent(newParticle, "Position", "X"):SetFloat3(posAX + 0.6*dirAX, 0.23, posAY + 0.6*dirAY)
			--
			--world:GetComponent(newParticle, "Color", "X"):SetFloat3(0.0, 0.0, 0.6)
			--
			--world:GetComponent(newParticle, "Particle", "Name"):SetText("fire")
			--world:GetComponent(newParticle, "Particle", "Texture"):SetText("content/textures/firewhite.png")
			--world:GetComponent(newParticle, "Particle", "Particles"):SetInt(1000)
			--world:GetComponent(newParticle, "Particle", "Lifetime"):SetFloat(400)
			--world:GetComponent(newParticle, "Particle", "Scale"):SetFloat(0.05)
			--world:GetComponent(newParticle, "Particle", "SpriteSize"):SetFloat(0.6)
			--world:GetComponent(newParticle, "Particle", "Id"):SetInt(-1)
		end
		
	end
	
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
		
		newX, newZ	=	self:GetRandomPositionWithinMargin(self.VoidMargin, self.VoidMargin)
		
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

MapGenerator.GetCenterOfMap = function(self)
	return	math.ceil((self.MapSizeX-1)/2), math.ceil((self.MapSizeZ-1)/2)
end











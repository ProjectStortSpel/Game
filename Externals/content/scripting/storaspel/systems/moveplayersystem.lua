
---------------------------- PlayerMovementSystem

PlayerMovementSystem = System()
PlayerMovementSystem.currentPlayer = 1

PlayerMovementSystem.Initialize = function(self)
	self:SetName("Test Movement System")
	
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Model",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
		
	print("PlayerMovementSystem initialized!")
end

PlayerMovementSystem.Update = function(self, dt)
	
	local entities = self:GetEntities()
	--Console.Print(#entities)
	
	--Console.Print(MapCreationSystem.testnumber)
	
	local cards = {}
	cards = self:GetCardsFromPlayers()
	
	if self.currentPlayer <= #entities then
	
		local switchplayer = false
		if Input.GetKeyState(Key.Up) == InputState.Pressed then
			world:CreateComponentAndAddTo("Forward", entities[self.currentPlayer])
			switchplayer = true
		elseif Input.GetKeyState(Key.Down) == InputState.Pressed then
			world:CreateComponentAndAddTo("Backward", entities[self.currentPlayer])
			switchplayer = true
		elseif Input.GetKeyState(Key.Left) == InputState.Pressed then
			world:CreateComponentAndAddTo("TurnLeft", entities[self.currentPlayer])
			switchplayer = true
		elseif Input.GetKeyState(Key.Right) == InputState.Pressed then
			world:CreateComponentAndAddTo("TurnRight", entities[self.currentPlayer])
			switchplayer = true
		elseif Input.GetKeyState(Key.T) == InputState.Pressed then
			world:CreateComponentAndAddTo("TurnAround", entities[self.currentPlayer])
			switchplayer = true
		
			
		end
		
		if switchplayer == true then
			--Console.Print(self.currentPlayer)
			self.currentPlayer = self.currentPlayer + 1
			--self.currentPlayer = 1
		end
	else
		self.currentPlayer = 1
	end
	 
 	-- Tillfällig representation av en rundas slut.     
	if Input.GetKeyState(Key.Space) == InputState.Pressed then
			 
		local players = RespawnSystem:GetEntities()
		for i = 1, #players do
			world:CreateComponentAndAddTo("NewRound", players[i])
		end
		
		switchplayer = true
	end
	
	-- Tillfällig representation av ett step.     
	if Input.GetKeyState(Key.P) == InputState.Pressed then
		
		for i = 1, #entities do
			--print("p-key pressed, adding newstep to player ", i)
			world:CreateComponentAndAddTo("NewStep", entities[i])
		end
		
		switchplayer = true
	end
end

PlayerMovementSystem.GetCardsFromPlayers = function(self)
	
end

-- If the tile we are trying to reach is walkable, we go there.
PlayerMovementSystem.MoveTo = function(self, entity, posX, posY, dirX, dirY)
	
	if MapCreationSystem:TileIsWalkable(posX, posY) then
	
		-- Check if another player is on that tile, if so, recursively call this function to check whether the tile we are trying to push this player to is walkable or occupied.
		local playerId = self:PlayerOnTile(posX, posY)
		
		if -1 ~= playerId then
			local newPosX = posX + dirX
			local newPosY = posY + dirY
			if self:MoveTo(playerId, newPosX, newPosY, dirX, dirY) then
				self:SetPosition(entity, posX, 1.0, posY)
				return true
			end
		-- No player on the tile, move there.
		else
			self:SetPosition(entity, posX, 1.0, posY)
			
			return true
		end
	end
	
	return false
end

PlayerMovementSystem.PostInitialize = function(self, posX, posY)
	
	
	return 
end

-- Checks if there is a player on the tile. If so, return the id of the entity, otherwise return -1.
PlayerMovementSystem.PlayerOnTile = function(self, posX, posY)
	
	local entities = self:GetEntities()
	
	for i = 1, #entities do
		local entity = entities[i]
		local posComp = self:GetComponent(entity, "MapPosition", 0)
		local playerPosX, playerPosY = posComp:GetInt2()
		--print(posX, posY, playerPosX, playerPosY)
		
		if posX == playerPosX and posY == playerPosY then
			
			--PlayerMovementSystem:SetPosition(entity, posX, 1.0, posY)
			return entity
			--position:SetFloat3(px, py, pz)
		end
	end
	
	return -1
end

PlayerMovementSystem.SetPosition = function(self, entity, posX, posY, posZ)
	local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
    local posComp = self:GetComponent(entity, "Position", 0)
    mapPosComp:SetInt2(posX, posZ)
    posComp:SetFloat3(posX, posY, posZ)
	
	local checkpointID = MapCreationSystem:GetCheckPointId(posX, posZ)
	
	if -1 ~= checkpointID then
		local targetComp = self:GetComponent(entity, "TargetCheckpoint", 0)
		local targetCheckPointID = targetComp:GetInt()
		
		if targetCheckPointID == checkpointID then
			targetComp:SetInt(checkpointID + 1)
			local spawnComp = self:GetComponent(entity, "Spawn", 0)
			spawnComp:SetInt2(posX, posZ)
		end	
	elseif MapCreationSystem:TileIsVoid(posX, posZ) then
		--print("Tile Is Void", posX, posY)
		world:CreateComponentAndAddTo("InactivePlayer", entity)
		
	end
	
end



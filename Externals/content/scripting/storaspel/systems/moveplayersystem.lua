
---------------------------- TestMovementSystem

TestMovementSystem = System()
TestMovementSystem.currentPlayer = 1

TestMovementSystem.Initialize = function(self)
	self:SetName("Test Movement System")
	
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Model",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
		
	print("TestMovementSystem initialized!")
end

TestMovementSystem.Update = function(self, dt)
	
	local entities = self:GetEntities()
	--Console.Print(#entities)
	
	--Console.Print(MapCreationSystem.testnumber)
	
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
	
	if Input.GetKeyState(Key.Space) == InputState.Pressed then
			
			local players = RespawnSystem:GetEntities()
			for i = 1, #players do
				world:CreateComponentAndAddTo("NewRound", players[i])
			end
			
			switchplayer = true
	end
end

-- If the tile we are trying to reach is walkable, we go there.
TestMovementSystem.MoveTo = function(self, entity, posX, posY, dirX, dirY)
	
	if MapCreationSystem:TileIsWalkable(posX, posY) then
	
		-- Check if another player is on that tile, if so, recursively call this function to check whether the tile we are trying to push this player to is walkable or occupied.
		local playerId = self:PlayerOnTile(posX, posY)
		
		if -1 ~= playerId then
			local newPosX = posX + dirX
			local newPosY = posY + dirY
			if self:MoveTo(playerId, newPosX, newPosY, dirX, dirY) then
				MapCreationSystem:SetPosition(entity, posX, 1.0, posY)
				return true
			end
		-- No player on the tile, move there.
		else
			if MapCreationSystem:TileIsVoid(posX, posY) then
				--print("Tile Is Void", posX, posY)
				world:CreateComponentAndAddTo("Respawn", entity)
			else
				--print("Tile Is Not Void", posX, posY)
				MapCreationSystem:SetPosition(entity, posX, 1.0, posY)
			end
			return true
		end
	end
	
	return false
end

TestMovementSystem.PostInitialize = function(self, posX, posY)
	
	
	return 
end

-- Checks if there is a player on the tile. If so, return the id of the entity, otherwise return -1.
TestMovementSystem.PlayerOnTile = function(self, posX, posY)
	
	local entities = self:GetEntities()
	
	for i = 1, #entities do
		local entity = entities[i]
		local posComp = self:GetComponent(entity, "MapPosition", 0)
		local playerPosX, playerPosY = posComp:GetInt2()
		--print(posX, posY, playerPosX, playerPosY)
		
		if posX == playerPosX and posY == playerPosY then
			
			--MapCreationSystem:SetPosition(entity, posX, 1.0, posY)
			return entity
			--position:SetFloat3(px, py, pz)
		end
	end
	
	return -1
end

---------------------------- RespawnSystem

RespawnSystem = System()

RespawnSystem.Initialize = function(self)
	self:SetName("Respawn System")
	self:AddComponentTypeToFilter("Spawn", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Respawn", FilterType.Mandatory)
	self:AddComponentTypeToFilter("MapPosition", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("NewRound", FilterType.Excluded)
	--self:AddComponentTypeToFilter("Position", FilterType.Excluded)
	print("Respawn System initialized!")
end

RespawnSystem.OnEntityAdded = function(self, entity)

	--print("Respawn add")
	if self:EntityHasComponent(entity, "Position" ) then
		world:RemoveComponentFrom("Position", entity);
	end
	--print("Respawn added")
end

RespawnSystem.OnEntityRemoved = function(self, entity)
	
	--print("Respawn remove")
	local spawnComp = self:GetComponent(entity, "Spawn", 0)
	local spawnX, spawnY = spawnComp:GetInt2()
	
	local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
	mapPosComp:SetInt2(spawnX, spawnY)
	
	world:CreateComponentAndAddTo("Position", entity)
	local posComp = self:GetComponent(entity, "Position", 0)
	posComp:SetFloat3(spawnX, 1.0, spawnY)
	
	world:RemoveComponentFrom("Respawn", entity);
	--print("Respawn removed")
end


---------------------------- New Round System

NewRoundSystem = System()

NewRoundSystem.Initialize = function(self)
	self:SetName("New Round System")
	self:AddComponentTypeToFilter("NewRound", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("Respawn",FilterType.Excluded)
	print("New Round System initialized!")
end

NewRoundSystem.OnEntityAdded = function(self, entity)
	
	--print("New Round System entity added")
	world:RemoveComponentFrom("NewRound", entity);
end

---------------------------- ForwardSystem

ForwardSystem = System()

ForwardSystem.Initialize = function(self)
	self:SetName("Forward System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Forward",FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	print("ForwardSystem initialized!")
end

ForwardSystem.OnEntityAdded = function(self, entity)
	local dir = self:GetComponent(entity, "Direction", 0)
	local mapPos = self:GetComponent(entity, "MapPosition", 0)
	
	local x, y = mapPos:GetInt2()
	local dirX, dirY = dir:GetInt2()
	
	local targetX = x + dirX
	local targetY = y + dirY
	
	local moved = TestMovementSystem:MoveTo(entity, targetX, targetY, dirX, dirY)
	
	world:RemoveComponentFrom("Forward", entity);
end


---------------------------- BackwardSystem

BackwardSystem = System()

BackwardSystem.Initialize = function(self)
	self:SetName("Backward System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Backward",FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	print("BackwardSystem initialized!")
end

BackwardSystem.OnEntityAdded = function(self, entity)
	local dir = self:GetComponent(entity, "Direction", 0)
	local mapPos = self:GetComponent(entity, "MapPosition", 0)
	
	local x, y = mapPos:GetInt2()
	local dirX, dirY = dir:GetInt2()
	
	local targetX = x - dirX
	local targetY = y - dirY
	
	local moved = TestMovementSystem:MoveTo(entity, targetX, targetY, -dirX, -dirY)
	
	world:RemoveComponentFrom("Backward", entity);
end

---------------------------- RightTurnSystem
RightTurnSystem = System()

RightTurnSystem.Initialize = function(self)
	self:SetName("Right Turn System")
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnRight",FilterType.Mandatory)
	
	print("RightTurnSystem initialized!")
end

RightTurnSystem.OnEntityAdded = function(self, entity)
	local dir = self:GetComponent(entity, "Direction", 0)
	local rot = self:GetComponent(entity, "Rotation", 4)
	local dx, dy = dir:GetInt2()
	local roty = rot:GetFloat()
	
	local tempdy = dx
	dx = -dy
	dy = tempdy
	
	dir:SetInt2(dx, dy)
	rot:SetFloat(roty - math.pi/2)
	
	world:RemoveComponentFrom("TurnRight", entity);
end

---------------------------- LeftTurnSystem

LeftTurnSystem = System()

LeftTurnSystem.Initialize = function(self)
	self:SetName("Left Turn System")
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnLeft",FilterType.Mandatory)
	
	print("LeftTurnSystem initialized!")
end

LeftTurnSystem.OnEntityAdded = function(self, entity)
	local dir = self:GetComponent(entity, "Direction", 0)
	local rot = self:GetComponent(entity, "Rotation", 4)
	local dx, dy = dir:GetInt2()
	local roty = rot:GetFloat()
	local tempdy = dx
	dx = dy
	dy = -tempdy
	
	dir:SetInt2(dx, dy)
	rot:SetFloat(roty + math.pi/2)
	
	world:RemoveComponentFrom("TurnLeft", entity);
end

---------------------------- TurnAroundSystem

TurnAroundSystem = System()

TurnAroundSystem.Initialize = function(self)
	self:SetName("Turn Around System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnAround",FilterType.Mandatory)
	
	print("TurnAroundSystem initialized!")
end

TurnAroundSystem.OnEntityAdded = function(self, entity)
	local dir = self:GetComponent(entity, "Direction", 0)
	local rot = self:GetComponent(entity, "Rotation", 4)
	local dx, dy = dir:GetInt2()
	local roty = rot:GetFloat()
	
	dx = -dx
	dy = -dy
	
	dir:SetInt2(dx, dy)
	rot:SetFloat(roty + math.pi)
	
	world:RemoveComponentFrom("TurnAround", entity);
end

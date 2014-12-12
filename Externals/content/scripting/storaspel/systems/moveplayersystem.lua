
---------------------------- TestMovementSystem

TestMovementSystem = System()
TestMovementSystem.currentPlayer = 1

TestMovementSystem.Initialize = function(self)
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
		elseif Input.GetKeyState(Key.Space) == InputState.Pressed then
			local comp = self:GetComponent(entities[self.currentPlayer], "Spawn", 0)
			local newPosX, newPosY = comp:GetInt2()
			MapCreationSystem:SetPosition(entities[self.currentPlayer], newPosX, 1.0, newPosY)
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
	
	
end

TestMovementSystem.MoveTo = function(self, entity, posX, posY, dirX, dirY)
	-- If the tile we are trying to reach is walkable, then we go there.
	-- TODO: En metod som får in position vi vill gå till och riktning som spelaren vill gå. Kolla om rutan är walkable och returnera sant eller falskt, om det är en spelare (spelare2) så anropar den rekursivt samma metod med en ny position som gäller, dvs spelare2s position men med samma riktning.
		
	if self:TileIsWalkable(posX, posY) then
		--if self:PlayerOnTile(posX, posY) then
			MapCreationSystem:SetPosition(entity, posX, 1.0, posY)
			
			--local pos = self:GetComponent(entity, "MapPosition", 0)
			--local mapPos = self:GetComponent(entity, "Position", 0)
			
			--pos:SetFloat3(posX, 1.0, posZ)
			--mapPos:SetInt2(posX, posZ)
			--targetposition.SetFloat3(newtargetx, newtargety, newtargetz)
			
			--print("set variables done")
		--end
	end
end

TestMovementSystem.PostInitialize = function(self, posX, posY)
	
	
	return 
end

TestMovementSystem.TileIsWalkable = function(self, posX, posY)
	
	local index = MapCreationSystem.mapX * posY + posX + 1
	entity = MapCreationSystem.entities[index]
	
	local returnValue = not self:EntityHasComponent(entity, "NotWalkable")
	return returnValue 
end

TestMovementSystem.PlayerOnTile = function(self, posX, posY)
	
	local entities = self:GetEntities()
	
	--for i = 1, #entities do
	--	local entity = entities[i]
	--	local posComp = self:GetComponent(entity, "MapPosition", 0)
	--	local posX, posY = posComp:GetInt2()
	--	
	--	
	--	--position:SetFloat3(px, py, pz)
	--end
	
	return false
end

---------------------------- ForwardSystem

ForwardSystem = System()

ForwardSystem.Initialize = function(self)
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
	
	TestMovementSystem:MoveTo(entity, targetX, targetY, dirX, dirY)
	
	world:RemoveComponentFrom("Forward", entity);
end

---------------------------- BackwardSystem

BackwardSystem = System()

BackwardSystem.Initialize = function(self)
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
	
	TestMovementSystem:MoveTo(entity, targetX, targetY, dirX, dirY)
	
	world:RemoveComponentFrom("Backward", entity);
end

---------------------------- RightTurnSystem
RightTurnSystem = System()

RightTurnSystem.Initialize = function(self)
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

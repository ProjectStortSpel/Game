
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
			--print("pre-forward")
			world:CreateComponentAndAddTo("Forward", entities[self.currentPlayer])
			--print("added forward-component")
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
			local posComp = self:GetComponent(entities[self.currentPlayer], "Position", 0)
			posComp:SetInt2(newPosX, newPosY)
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

TestMovementSystem.MoveTo = function(self)
	
end

TestMovementSystem.PostInitialize = function(self)
	
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
	local position = self:GetComponent(entity, "Position", 0)
	local dir = self:GetComponent(entity, "Direction", 0)
	local mapPos = self:GetComponent(entity, "MapPosition", 0)
	
	--world:CreateComponentAndAddTo("TargetPosition", entity)
	--local targetposition = self:GetComponent(entity, "TargetPosition", 0)
	
	local x, y, z = position:GetFloat3()
	local dx, dy = dir:GetInt2()
	local mapX, mapY = mapPos:GetInt2()
	
	-- TODO: Gör resterande av metoden till en ny metod som både forward och backward anropar med dx- och dy-värden som skiljer sig bara.
			
	local newtargetx = x + dx
	local newtargety = y 
	local newtargetz = z + dy
	
	local mapTargetX = mapX + dx
	local mapTargetY = mapY + dy
	
	
	--print("created all variables in forward")
	
	-- If the tile we are trying to reach is walkable, then we go there.
	-- TODO: En metod som får in position vi vill gå till och riktning som spelaren vill gå. Kolla om rutan är walkable och returnera sant eller falskt, om det är en spelare (spelare2) så anropar den rekursivt samma metod med en ny position som gäller, dvs spelare2s position men med samma riktning.
	--if walkable(mapTargetX, mapTargetY) then
		
		
		
		position:SetFloat3(newtargetx, newtargety, newtargetz)
		mapPos:SetInt2(mapTargetX, mapTargetY)
		--targetposition.SetFloat3(newtargetx, newtargety, newtargetz)
		
		--print("set variables done")
		
	--end
	world:RemoveComponentFrom("Forward", entity);
	--print("removed forward-component")
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
	local position = self:GetComponent(entity, "Position", 0)
	local dir = self:GetComponent(entity, "Direction", 0)
	
	--world:CreateComponentAndAddTo("TargetPosition", entity)
	--local targetposition = self:GetComponent(entity, "TargetPosition", 0)
	
	local x, y, z = position:GetFloat3()
	local dx, dy = dir:GetInt2()
			
	local newtargetx = x - dx
	local newtargety = y 
	local newtargetz = z - dy
	
	position:SetFloat3(newtargetx, newtargety, newtargetz)
	
	--targetposition.SetFloat3(newtargetx, newtargety, newtargetz)
	
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

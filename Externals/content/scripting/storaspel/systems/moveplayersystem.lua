
---------------------------- TestMovementSystem

TestMovementSystem = System()
TestMovementSystem.currentPlayer = 1

TestMovementSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Model",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	
	self:InitializeNetworkEvents()
	
	print("TestMovementSystem initialized!")
end

TestMovementSystem.Update = function(self, dt)
	

	local entities = self:GetEntities()
	--Console.Print(#entities)
	
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
			local newPosX, newPosY, newPosZ = comp:GetFloat3()
			local posComp = self:GetComponent(entities[self.currentPlayer], "Position", 0)
			posComp:SetFloat3(newPosX, newPosY, newPosZ)
			switchplayer = true
			
		end
		
		if switchplayer == true then
			Console.Print(self.currentPlayer)
			self.currentPlayer = self.currentPlayer + 1
			--self.currentPlayer = 1
		end
	else
		self.currentPlayer = 1
	end
	
	
end


TestMovementSystem.OnConnectedToServer = function(self, _ip, _port)
	local id = Client.StartPack("Username")
	Client.WriteString(id, "Username_Lua")
	Client.Send(id)
	local s = "[Client] 2344564575662345fghftgConnected to server " .. _ip .. ":" .. _port
	Console.Print(s)
end

TestMovementSystem.PostInitialize = function(self)
	local entity = world:CreateNewEntity("Player")
	world:CreateComponentAndAddTo("Spawn", entity)
	local pos = {-3.0, 1.0, 5.0}
    local comp = self:GetComponent(entity, "Spawn", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
    local comp = self:GetComponent(entity, "Position", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
	world:CreateComponentAndAddTo("SyncNetwork", entity)
	
	local entity = world:CreateNewEntity("Player")
	world:CreateComponentAndAddTo("Spawn", entity)
	local pos = {-1.0, 1.0, 5.0}
    local comp = self:GetComponent(entity, "Spawn", 0)
	comp:SetFloat3(pos[1], pos[2], pos[3])
    local comp = self:GetComponent(entity, "Position", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
	world:CreateComponentAndAddTo("SyncNetwork", entity)
	
	local entity = world:CreateNewEntity("Player")
	world:CreateComponentAndAddTo("Spawn", entity)
	local pos = {1.0, 1.0, 5.0}
    local comp = self:GetComponent(entity, "Spawn", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
    local comp = self:GetComponent(entity, "Position", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
	world:CreateComponentAndAddTo("SyncNetwork", entity)
	
	local entity = world:CreateNewEntity("Player")
	world:CreateComponentAndAddTo("Spawn", entity)
	local pos = {3.0, 1.0, 5.0}
    local comp = self:GetComponent(entity, "Spawn", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
    local comp = self:GetComponent(entity, "Position", 0)
    comp:SetFloat3(pos[1], pos[2], pos[3])
	world:CreateComponentAndAddTo("SyncNetwork", entity)
	
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
	
	--world:CreateComponentAndAddTo("TargetPosition", entity)
	--local targetposition = self:GetComponent(entity, "TargetPosition", 0)
	
	local x, y, z = position:GetFloat3()
	local dx, dy = dir:GetInt2()
			
	local newtargetx = x + dx
	local newtargety = y 
	local newtargetz = z + dy
	
	position:SetFloat3(newtargetx, newtargety, newtargetz)
	
	--targetposition.SetFloat3(newtargetx, newtargety, newtargetz)
	
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

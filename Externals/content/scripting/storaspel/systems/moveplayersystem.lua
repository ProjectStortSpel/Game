
---------------------------- TestMovementSystem

TestMovementSystem = System()
TestMovementSystem.entities = { }

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
	
	if Input.GetKeyState(Key.Up) == InputState.Pressed then
		world:CreateComponentAndAddTo("Forward", entities[1])
	elseif Input.GetKeyState(Key.Down) == InputState.Pressed then
		world:CreateComponentAndAddTo("Backward", entities[1])
	elseif Input.GetKeyState(Key.Left) == InputState.Pressed then
		world:CreateComponentAndAddTo("TurnLeft", entities[1])
	elseif Input.GetKeyState(Key.Right) == InputState.Pressed then
		world:CreateComponentAndAddTo("TurnRight", entities[1])
	elseif Input.GetKeyState(Key.T) == InputState.Pressed then
		world:CreateComponentAndAddTo("TurnAround", entities[1])
		
	end
end

TestMovementSystem.PostInitialize = function(self)
	local entity = world:CreateNewEntity("Player")
	table.insert(self.entities, entity)
end

---------------------------- ForwardSystem

ForwardSystem = System()

ForwardSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Forward",FilterType.Mandatory)
	
	--self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
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


---------------------------- ForwardSystem

ForwardSystem = System()

ForwardSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]

		local position = self:GetComponent(entity, "Position", 0)
		local dir = self:GetComponent(entity, "Direction", 0)
		
		world:CreateComponentAndAddTo("TargetPosition", entity)
		local targetposition = self:GetComponent(entity, "TargetPosition", 0)
		
		local x, y, z = position.GetFloat3()
		local dx, dy = dir.GetInt2()
		
		
		local newtargetx = x + dx
		local newtargety = y
		local newtargetz = z + dy
		
		targetposition.SetFloat3(newtargetx, newtargety, newtargetz)
		
		world:RemoveComponentFrom("Forward", entity);
		
	end
end

ForwardSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Forward",FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	print("ForwardSystem initialized!")
end

ForwardSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end

ForwardSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

---------------------------- RightTurnSystem
RightTurnSystem = System()

RightTurnSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]

		local dir = self:GetComponent(entity, "Direction", 0)
		
		local dx, dy = dir.GetInt2()
		
		
		local tempdy = dx
		dx = -dy
		dy = tempdy
		
		dir:SetInt2(dx, dy)
		
		world:RemoveComponentFrom("TurnRight", entity);
		
	end
end

RightTurnSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnRight",FilterType.Mandatory)
	
	print("RightTurnSystem initialized!")
end

RightTurnSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end

RightTurnSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

---------------------------- LeftTurnSystem

LeftTurnSystem = System()

LeftTurnSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]

		local dir = self:GetComponent(entity, "Direction", 0)
		
		local dx, dy = dir.GetFloat2()
		
		
		local tempdy = dx
		dx = dy
		dy = -tempdy
		
		dir:SetFloat2(dx, dy)
		
		world:RemoveComponentFrom("TurnLeft", entity);
		
	end
end

LeftTurnSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnLeft",FilterType.Mandatory)
	
	print("LeftTurnSystem initialized!")
end

LeftTurnSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end

LeftTurnSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

---------------------------- BackwardSystem

BackwardSystem = System()

BackwardSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]

		local position = self:GetComponent(entity, "Position", 0)
		local dir = self:GetComponent(entity, "Direction", 0)
		
		world:CreateComponentAndAddTo("TargetPosition", entity)
		local targetposition = self:GetComponent(entity, "TargetPosition", 0)
		
		local x, y, z = position.GetFloat3()
		local dx, dy = dir.GetInt2()
		
		
		local newtargetx = x - dx
		local newtargety = y
		local newtargetz = z - dy
		
		targetposition.SetFloat3(newtargetx, newtargety, newtargetz)
		
		world:RemoveComponentFrom("Backward", entity);
		
	end
end

BackwardSystem.Initialize = function(self)
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Backward",FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	print("BackwardSystem initialized!")
end

BackwardSystem.OnEntityAdded = function(self, entityId)
	print("OnEntityAdded (LUA)")
end

BackwardSystem.OnEntityRemoved = function(self, entityId)
	print("OnEntityRemoved")
end

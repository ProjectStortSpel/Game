TurnAroundSystem = System()

TurnAroundSystem.Initialize = function(self)
	self:SetName("TurnAroundSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnAround",FilterType.Mandatory)
		
	print("TurnAroundSystem initialized!")
end

TurnAroundSystem.OnEntityAdded = function(self, entity)
	
	local dir = world:GetComponent(entity, "Direction", 0)
	local x, z = dir:GetInt2()
	x = x * -1
	z = z * -1
	dir:SetInt2(x, z)

	

	--rotate model
	local rotY = world:GetComponent(entity, "Rotation", "Y"):GetFloat()

	if	x == 0 and z == 1 then
		rotY = 0
	elseif  x == 0 and z == -1 then
		rotY = math.pi
	elseif  x == 1 then		
		rotY = math.pi / 2
	elseif  x == -1 then
		rotY = -math.pi / 2 
	end

	world:GetComponent(entity, "Rotation", "Y"):SetFloat(rotY)


	world:RemoveComponentFrom("UnitTurnAround", entity)
end


TurnLeftSystem = System()

TurnLeftSystem.Initialize = function(self)
	self:SetName("TurnLeftSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnLeft",FilterType.Mandatory)
		
	print("TurnLeftSystem initialized!")
end

TurnLeftSystem.OnEntityAdded = function(self, entity)
	
	local dir = world:GetComponent(entity, "Direction", 0)
	local rot = world:GetComponent(entity, "Rotation", 0)
	local x, z = dir:GetInt2()

	if	x == 0 and z == 1 then
		x = 1
		z = 0
		rotY = math.pi / 2
	elseif  x == 0 and z == -1 then
		x = -1
		z = 0
		rotY = -math.pi / 2
	elseif  x == 1 then
		x = 0
		z = -1
		rotY = math.pi
	elseif  x == -1 then
		x = 0
		z = 1
		rotY = 0
	end


	dir:SetInt2(x, z)
	world:GetComponent(entity, "Rotation", "Y"):SetFloat(rotY)

	world:RemoveComponentFrom("UnitTurnLeft", entity)

end

TurnRightSystem = System()

TurnRightSystem.Initialize = function(self)
	self:SetName("TurnRightSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnRight",FilterType.Mandatory)
		
	print("TurnRightSystem initialized!")
end

TurnRightSystem.OnEntityAdded = function(self, entity)

	local dir = world:GetComponent(entity, "Direction", 0)
	local x, z = dir:GetInt2()

	if	x == 0 and z == 1 then
		x = -1
		z = 0
		rotY = -math.pi / 2
	elseif  x == 0 and z == -1 then
		x = 1
		z = 0
		rotY = math.pi / 2
	elseif  x == 1 then
		x = 0
		z = 1
		rotY = 0
	elseif  x == -1 then
		x = 0
		z = -1
		rotY = math.pi
	end


	dir:SetInt2(x, z)
	world:GetComponent(entity, "Rotation", "Y"):SetFloat(rotY)

	world:RemoveComponentFrom("UnitTurnRight", entity)
end

MoveForwardSystem = System()

MoveForwardSystem.Initialize = function(self)
	self:SetName("MoveForwardSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitForward",FilterType.Mandatory)
		
	print("MoveForwardSystem initialized!")
end

MoveForwardSystem.OnEntityAdded = function(self, entity)
	
	local dir = world:GetComponent(entity, "Direction", 0)
	local mapPos = world:GetComponent(entity, "MapPosition", 0)
	local pos = world:GetComponent(entity, "Position", 0)

	local dirX, dirZ = dir:GetInt2()
	local mapPosX, mapPosZ = mapPos:GetInt2()
	local posX, posY, posZ = pos:GetFloat3()

	mapPos:SetInt2(mapPosX + dirX, mapPosZ + dirZ)
	pos:SetFloat3(posX + dirX, posY, posZ + dirZ)

	world:RemoveComponentFrom("UnitForward", entity)
end

MoveBackwardSystem = System()

MoveBackwardSystem.Initialize = function(self)
	self:SetName("MoveBackwardSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitBackward",FilterType.Mandatory)
		
	print("MoveBackwardSystem initialized!")
end

MoveBackwardSystem.OnEntityAdded = function(self, entity)
	
	local dir = world:GetComponent(entity, "Direction", 0)
	local mapPos = world:GetComponent(entity, "MapPosition", 0)
	local pos = world:GetComponent(entity, "Position", 0)

	local dirX, dirZ = dir:GetInt2()
	local mapPosX, mapPosZ = mapPos:GetInt2()
	local posX, posY, posZ = pos:GetFloat3()

	mapPos:SetInt2(mapPosX - dirX, mapPosZ - dirZ)
	pos:SetFloat3(posX - dirX, posY, posZ - dirZ)

	world:RemoveComponentFrom("UnitBackward", entity)
end

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

TestMoveSystem = System()

TestMoveSystem.Initialize = function(self)
	
	self:SetName("TestMoveSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TestMove",FilterType.RequiresOneOf)
		
	print("TestMoveSystem initialized!")

end

TestMoveSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "TestMove") then
		
		local units = self:GetEntities("Unit")
		local notWalkable = self:GetEntities("NotWalkable")

		local unit = world:GetComponent(entity, "TestMove", "Unit"):GetInt()
		local posX = world:GetComponent(entity, "TestMove", "PosX"):GetInt()
		local posZ = world:GetComponent(entity, "TestMove", "PosZ"):GetInt()
		local dirX = world:GetComponent(entity, "TestMove", "DirX"):GetInt()
		local dirZ = world:GetComponent(entity, "TestMove", "DirZ"):GetInt()

		local moveUnits = { }
		
		local X1, Z1 = posX, posZ

		local bla = true

		while bla do
			
			bla = false
			for i = 1, #units do
			
				local X2, Z2 = world:GetComponent(units[i], "MapPosition", 0):GetInt2()

				if X1 == X2 and Z1 == Z2 then
				
					moveUnits[#moveUnits + 1] = units[i]
					X1 = X1 + dirX
					Z1 = Z1 + dirZ
					bla = true
				end
			end

		end

		local isWalkable = true
		for i = 1, #notWalkable do
			
			local X2, Z2 = world:GetComponent(notWalkable[i], "MapPosition", 0):GetInt2()

			if X1 == X2 and Z1 == Z2 then
				
				isWalkable = false
				break

			end
		end

		if isWalkable then
			
			print("Push units: " .. #moveUnits)

			for i = 1, #moveUnits do
				
				local mapPos = world:GetComponent(moveUnits[i], "MapPosition", 0)
				local pos = world:GetComponent(moveUnits[i], "Position", 0)

				local mapPosX, mapPosZ = mapPos:GetInt2()
				local newPosX, newPosY, newPosZ = pos:GetFloat3()

				mapPos:SetInt2(mapPosX + dirX, mapPosZ + dirZ)
				--pos:SetFloat3(newPosX + dirX, newPosY, newPosZ + dirZ)

			end

			local posY = world:GetComponent(unit, "Position", "Y"):GetFloat()
			world:GetComponent(unit, "MapPosition", 0):SetInt2(posX, posZ)			
			--world:GetComponent(unit, "Position", 0):SetFloat3(posX, posY, posZ)


			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PostMove", id)

		end


		world:KillEntity(entity)

	end
	
end






MoveForwardSystem = System()

MoveForwardSystem.Initialize = function(self)
	self:SetName("MoveForwardSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitForward",FilterType.Mandatory)
		
	print("MoveForwardSystem initialized!")
end

MoveForwardSystem.OnEntityAdded = function(self, entity)
	
		local dirX, dirZ = world:GetComponent(entity, "Direction", 0):GetInt2()
		local mapPosX, mapPosZ = world:GetComponent(entity, "MapPosition", 0):GetInt2()

	
		if dirX ~= 0 then
			dirX = dirX / math.abs(dirX)
		end

		if dirZ ~= 0 then
			dirZ = dirZ / math.abs(dirZ)
		end

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("TestMove", id)
		world:SetComponent(id, "TestMove", "Unit", entity)
		world:SetComponent(id, "TestMove", "PosX", mapPosX + dirX)
		world:SetComponent(id, "TestMove", "PosZ", mapPosZ + dirZ)
		world:SetComponent(id, "TestMove", "DirX", dirX)
		world:SetComponent(id, "TestMove", "DirZ", dirZ)
	
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
	
	local dirX, dirZ = world:GetComponent(entity, "Direction", 0):GetInt2()
	local mapPosX, mapPosZ = world:GetComponent(entity, "MapPosition", 0):GetInt2()

	if dirX ~= 0 then
		dirX = dirX / math.abs(dirX)
	end

	if dirZ ~= 0 then
		dirZ = dirZ / math.abs(dirZ)
	end

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("TestMove", id)
	world:SetComponent(id, "TestMove", "Unit", entity)
	world:SetComponent(id, "TestMove", "PosX", mapPosX - dirX)
	world:SetComponent(id, "TestMove", "PosZ", mapPosZ - dirZ)
	world:SetComponent(id, "TestMove", "DirX", -dirX)
	world:SetComponent(id, "TestMove", "DirZ", -dirZ)
	
	world:RemoveComponentFrom("UnitBackward", entity)

end
TurnAroundSystem = System()

TurnAroundSystem.Initialize = function(self)
	self:SetName("TurnAroundSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnAround",FilterType.Mandatory)
end

TurnAroundSystem.OnEntityAdded = function(self, entity)
	if world:EntityHasComponent(entity, "Stunned") then
		world:SetComponent(entity, "NoSubSteps", "Counter", 1)
		world:RemoveComponentFrom("UnitTurnAround", entity)
		return
	end
	
	local dir = world:GetComponent(entity, "Direction", 0)
	local x, z = dir:GetInt2()
	x = x * -1
	z = z * -1
	dir:SetInt2(x, z)
	local rot = world:GetComponent(entity, "Rotation", 0)
	
	local rot_x, rot_y, rot_z = rot:GetFloat3()
	
	if not world:EntityHasComponent(entity, "SlerpRotation") then
		world:CreateComponentAndAddTo("SlerpRotation", entity)
	end 

	local rotY = world:GetComponent(entity, "Rotation", "Y"):GetFloat()

	world:GetComponent(entity, "SlerpRotation", "fromX"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "fromY"):SetFloat( 1 )
	world:GetComponent(entity, "SlerpRotation", "fromZ"):SetFloat( 0 )

	if	x == 0 and z == 1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat( math.pi )
		rotY = 0
	elseif  x == 0 and z == -1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat( 0 )
		rotY = math.pi
	elseif  x == 1 then		
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat( -math.pi / 2 )
		rotY = math.pi / 2
	elseif  x == -1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat( math.pi / 2 )
		rotY = -math.pi / 2 
	end

	world:GetComponent(entity, "SlerpRotation", "toX"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "toY"):SetFloat( 1 )
	world:GetComponent(entity, "SlerpRotation", "toZ"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "toW"):SetFloat( math.pi )

	world:SetComponent(entity, "NoSubSteps", "Counter", 1)
	world:RemoveComponentFrom("UnitTurnAround", entity)
end


TurnLeftSystem = System()

TurnLeftSystem.Initialize = function(self)
	self:SetName("TurnLeftSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnLeft",FilterType.Mandatory)
end

TurnLeftSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent(entity, "Stunned") then
		world:SetComponent(entity, "NoSubSteps", "Counter", 1)
		world:RemoveComponentFrom("UnitTurnLeft", entity)
		return
	end
	
	local dir = world:GetComponent(entity, "Direction", 0)
	local rot = world:GetComponent(entity, "Rotation", 0)

	local rot_x, rot_y, rot_z = rot:GetFloat3()

	if not world:EntityHasComponent(entity, "SlerpRotation") then
		world:CreateComponentAndAddTo("SlerpRotation", entity)
	end 

	local x, z = dir:GetInt2()
	world:GetComponent(entity, "SlerpRotation", "fromX"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "fromY"):SetFloat( 1 )
	world:GetComponent(entity, "SlerpRotation", "fromZ"):SetFloat( 0 )
	
	if	x == 0 and z == 1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(0)
		x = 1
		z = 0
		rotY = math.pi / 2
	elseif  x == 0 and z == -1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(math.pi)
		x = -1
		z = 0
		rotY = -math.pi / 2
	elseif  x == 1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(math.pi / 2)
		x = 0
		z = -1
		rotY = math.pi
	elseif  x == -1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(-math.pi / 2)
		x = 0
		z = 1
		rotY = 0
	end

	world:GetComponent(entity, "SlerpRotation", "toX"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "toY"):SetFloat( 1 )
	world:GetComponent(entity, "SlerpRotation", "toZ"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "toW"):SetFloat(math.pi/2)
	
	dir:SetInt2(x, z)
	world:SetComponent(entity, "NoSubSteps", "Counter", 1)
	world:RemoveComponentFrom("UnitTurnLeft", entity)

end

TurnRightSystem = System()

TurnRightSystem.Initialize = function(self)
	self:SetName("TurnRightSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnRight",FilterType.Mandatory)
end

TurnRightSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent(entity, "Stunned") then
		world:SetComponent(entity, "NoSubSteps", "Counter", 1)
		world:RemoveComponentFrom("UnitTurnRight", entity)
		return
	end


	local dir = world:GetComponent(entity, "Direction", 0)
	local x, z = dir:GetInt2()

	if not world:EntityHasComponent(entity, "SlerpRotation") then
		world:CreateComponentAndAddTo("SlerpRotation", entity)
	end 

	world:GetComponent(entity, "SlerpRotation", "fromX"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "fromY"):SetFloat( 1 )
	world:GetComponent(entity, "SlerpRotation", "fromZ"):SetFloat( 0 )

	if	x == 0 and z == 1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(0)
		x = -1
		z = 0
		rotY = -math.pi / 2
	elseif  x == 0 and z == -1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(math.pi)
		x = 1
		z = 0
		rotY = math.pi / 2
	elseif  x == 1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(math.pi / 2)
		x = 0
		z = 1
		rotY = 0
	elseif  x == -1 then
		world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(-math.pi / 2)
		x = 0
		z = -1
		rotY = math.pi
	end

	world:GetComponent(entity, "SlerpRotation", "toX"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "toY"):SetFloat( 1 )
	world:GetComponent(entity, "SlerpRotation", "toZ"):SetFloat( 0 )
	world:GetComponent(entity, "SlerpRotation", "toW"):SetFloat(-math.pi/2)
	dir:SetInt2(x, z)

	world:SetComponent(entity, "NoSubSteps", "Counter", 1)
	world:RemoveComponentFrom("UnitTurnRight", entity)
end

TestMoveSystem = System()

TestMoveSystem.Initialize = function(self)
	
	self:SetName("TestMoveSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TestMove",FilterType.RequiresOneOf)

end

TestMoveSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent(entity, "TestMove") then
	
		local units 		= self:GetEntities("Unit")
		local notWalkable 	= self:GetEntities("NotWalkable")
		
		local unit 	= world:GetComponent(entity, "TestMove", "Unit"):GetInt()
		local posX 	= world:GetComponent(entity, "TestMove", "PosX"):GetInt()
		local posZ 	= world:GetComponent(entity, "TestMove", "PosZ"):GetInt()
		local dirX 	= world:GetComponent(entity, "TestMove", "DirX"):GetInt()
		local dirZ 	= world:GetComponent(entity, "TestMove", "DirZ"):GetInt()
		local steps = world:GetComponent(entity, "TestMove", "Steps"):GetInt()
		
		world:SetComponent(unit, "NoSubSteps", "Counter", steps)
		local isWalkable = true
		local moveUnits = { }
		
		for hest = 1, steps do
			
			local hestPosX = posX + (dirX * hest)
			local hestPosZ = posZ + (dirZ * hest)
			local X1, Z1 = hestPosX, hestPosZ
			local tmp = true
			
			while tmp do
			
				tmp = false
				for i = 1, #units do
				
					--print("UnitId2: " .. units[i])
					if world:EntityHasComponent(units[i], "Stunned") then
						local id = world:CreateNewEntity()
						world:CreateComponentAndAddTo("PostMove", id)
						world:KillEntity(entity)
						return
					end
				
					local X2, Z2 = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
					
					if X1 == X2 and Z1 == Z2 then
						moveUnits[#moveUnits + 1] = units[i]
						X1 = X1 + dirX
						Z1 = Z1 + dirZ
						tmp = true
					end
					
				end
			
			end
			
			isWalkable = true
			for i = 1, #notWalkable do
			
				local X2, Z2 = world:GetComponent(notWalkable[i], "MapPosition", 0):GetInt2()
				
				if X1 == X2 and Z1 == Z2 then
					--world:SetComponent(unit, "NoSubSteps", "Counter", hest)
					local id = world:CreateNewEntity()
					world:CreateComponentAndAddTo("PostMove", id)
					world:KillEntity(entity)
					return
				end
			
			end
			
			if isWalkable then
			--	print("Push units: " .. #moveUnits)
				for i = 1, #moveUnits do
					local mapPos 	= world:GetComponent(moveUnits[i], "MapPosition", 0)
					local pos		= world:GetComponent(moveUnits[i], "Position", 0)
					
					local mapPosX, mapPosZ = mapPos:GetInt2()
					local newPosX, newPosY, newPosZ = pos:GetFloat3();
					
					mapPos:SetInt2(mapPosX + dirX, mapPosZ + dirZ)
				end
				
				world:GetComponent(unit, "MapPosition", 0):SetInt2(hestPosX, hestPosZ)
				
				-- CAMERA INTEREST POINT
				local cipID = Net.StartPack("Client.SendCIP")
				Net.WriteFloat(cipID, hestPosX)
				Net.WriteFloat(cipID, hestPosZ)
				Net.WriteFloat(cipID, dirX)
				Net.WriteFloat(cipID, dirZ)
				Net.WriteFloat(cipID, 1)
				Net.Broadcast(cipID)

			end
		end
		
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PostMove", id)
		
		world:KillEntity(entity)
		
	end
	
	
end

TestMoveSystem.OnEntityRemoved = function(self, entity)
end


MoveForwardSystem = System()

MoveForwardSystem.Initialize = function(self)
	self:SetName("MoveForwardSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitForward",FilterType.Mandatory)
end

MoveForwardSystem.OnEntityAdded = function(self, entity)
	
	--print("MoveForwardSystem.OnEntityAdded")
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
	world:SetComponent(id, "TestMove", "PosX", mapPosX)
	world:SetComponent(id, "TestMove", "PosZ", mapPosZ)
	world:SetComponent(id, "TestMove", "DirX", dirX)
	world:SetComponent(id, "TestMove", "DirZ", dirZ)
	world:SetComponent(id, "TestMove", "Steps", 1)
    
	world:RemoveComponentFrom("UnitForward", entity)
end

MoveBackwardSystem = System()

MoveBackwardSystem.Initialize = function(self)
	self:SetName("MoveBackwardSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitBackward",FilterType.Mandatory)
end

MoveBackwardSystem.OnEntityAdded = function(self, entity)
	
	--print("MoveBackwardSystem.OnEntityAdded")
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
	world:SetComponent(id, "TestMove", "PosX", mapPosX)
	world:SetComponent(id, "TestMove", "PosZ", mapPosZ)
	world:SetComponent(id, "TestMove", "DirX", -dirX)
	world:SetComponent(id, "TestMove", "DirZ", -dirZ)
	world:SetComponent(id, "TestMove", "Steps", 1)
	
	world:RemoveComponentFrom("UnitBackward", entity)

end



AbilitySprintSystem = System()

AbilitySprintSystem.Initialize = function(self)
	self:SetName("AbilitySprintSystem")
	
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitSprint",FilterType.Mandatory)
end

AbilitySprintSystem.OnEntityAdded = function(self, entity)
	
	--print("AbilitySprintSystem.OnEntityAdded")
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
	world:SetComponent(id, "TestMove", "PosX", mapPosX)
	world:SetComponent(id, "TestMove", "PosZ", mapPosZ)
	world:SetComponent(id, "TestMove", "DirX", dirX)
	world:SetComponent(id, "TestMove", "DirZ", dirZ)
	world:SetComponent(id, "TestMove", "Steps", 2)

	world:RemoveComponentFrom("UnitSprint", entity)

end
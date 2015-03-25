
TestMoveSystem = System()

TestMoveSystem.Initialize = function(self)
	--	Set Name
	self:SetName("TestMoveSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TestMove",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Void",FilterType.RequiresOneOf)

end

TestMoveSystem.RecursiveMove = function(self, unitToMove, allUnits, allNonWalkables, allVoids, posX, posZ, dirX, dirZ)

	--	Check all obstacles
	local	obstacleFound	=	false
	for n = 1, #allNonWalkables do
		local X, Z = world:GetComponent(allNonWalkables[n], "MapPosition", 0):GetInt2()
		if X == posX and Z == posZ then
			obstacleFound = true
			break
		end
	end
	
	--	Check if collision occured
	if obstacleFound then
		return false
	end
	
	local voidFound = false
	for n = 1, #allVoids do
		local X, Z = world:GetComponent(allVoids[n], "MapPosition", 0):GetInt2()
		if X == posX and Z == posZ then
			voidFound = true
			break
		end
	end
	
	for n = 1, #allUnits do
	end
	
	
	if not voidFound then
		--	Check all units
		for n = 1, #allUnits do
			local X, Z = world:GetComponent(allUnits[n], "MapPosition", 0):GetInt2()
			if X == posX and Z == posZ then
				
				if world:EntityHasComponent(allUnits[n], "ActionGuard") then
					-- SOUND
					local audioId = Net.StartPack("Client.PlaySoundC")
					Net.WriteString(audioId, "BlockVoice" .. math.random(1, 3))
					Net.WriteString(audioId, "BlockVoice" .. allUnits[n])
					Net.WriteBool(audioId, false)
					Net.Broadcast(audioId)
					
					audioId = Net.StartPack("Client.SetSoundVolume")
					Net.WriteString(audioId, "BlockVoice" .. allUnits[n])
					Net.WriteInt(audioId, 12)
					Net.Broadcast(audioId)
					return false
				end
				
				if not self:RecursiveMove(allUnits[n], allUnits, allNonWalkables, allVoids, posX+dirX, posZ+dirZ, dirX, dirZ) then
					return false
				end

				local newCheck = world:CreateNewEntity()
				world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(allUnits[n])
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(posX+dirX)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(posZ+dirZ)
				
				local audioId = Net.StartPack("Client.PlaySoundC")
				Net.WriteString(audioId, "Laugh" .. math.random(1, 4))
				Net.WriteString(audioId, "Laugh" .. allUnits[n])
				Net.WriteBool(audioId, false)
				Net.Broadcast(audioId)
				
				audioId = Net.StartPack("Client.SetSoundVolume")
				Net.WriteString(audioId, "Laugh" .. allUnits[n])
				Net.WriteInt(audioId, 4)
				Net.Broadcast(audioId)
				
				break
				
			end
		end
	end
	
	--	Check if recursive method has found obstacle (LUA <3)
	if obstacleFound then
		return false
	end
	
	--	Move the unit
	world:GetComponent(unitToMove, "MapPosition", 0):SetInt2(posX, posZ)
	if not world:EntityHasComponent(unitToMove, "LerpPosition") then
		world:CreateComponentAndAddTo("LerpPosition", unitToMove)
	end
	world:GetComponent(unitToMove, "LerpPosition", "X"):SetFloat(posX)
	world:GetComponent(unitToMove, "LerpPosition", "Y"):SetFloat(0.5)
	world:GetComponent(unitToMove, "LerpPosition", "Z"):SetFloat(posZ)
	world:GetComponent(unitToMove, "LerpPosition", "Time"):SetFloat(1)
	world:GetComponent(unitToMove, "LerpPosition", "Algorithm"):SetText("PlayerMove")
	
	local unitDirX, unitDirZ = world:GetComponent(unitToMove, "Direction", "X"):GetInt2(0)
	if not world:EntityHasComponent(unitToMove, "Animation") then
		world:CreateComponentAndAddTo("Animation", unitToMove)
	end
	
	if unitDirX == dirX and unitDirZ == dirZ then
		world:GetComponent(unitToMove, "Animation", "Id"):SetInt(1)
	elseif unitDirX == dirX or unitDirZ == dirZ then
		world:GetComponent(unitToMove, "Animation", "Id"):SetInt(2)
	elseif unitDirX == dirZ then
		world:GetComponent(unitToMove, "Animation", "Id"):SetInt(3)
	elseif unitDirZ == dirX then
		world:GetComponent(unitToMove, "Animation", "Id"):SetInt(4)
	else
		world:GetComponent(unitToMove, "Animation", "Id"):SetInt(6)
	end
	world:GetComponent(unitToMove, "Animation", "FrameTime"):SetFloat(0.01)
	world:GetComponent(unitToMove, "Animation", "Time"):SetFloat(1)
	
	if not world:EntityHasComponent(unitToMove, "UnitWantTileOffset") then
		world:CreateComponentAndAddTo("UnitWantTileOffset", unitToMove)
	end
	
	return true
end

TestMoveSystem.EntitiesAdded = function(self, dt, entities)
	
	for newEntity = 1, #entities do
		local entity = entities[newEntity]
		if world:EntityHasComponent(entity, "TestMove") then
		
			local 	tUnits 			= 	self:GetEntities("Unit")
			local 	tNonWalkables 	=	self:GetEntities("NotWalkable")
			local tVoids = self:GetEntities("Void")
			
			local 	tUnit 	= 	world:GetComponent(entity, "TestMove", "Unit"):GetInt()
			local 	tPosX 	= 	world:GetComponent(entity, "TestMove", "PosX"):GetInt()
			local 	tPosZ 	= 	world:GetComponent(entity, "TestMove", "PosZ"):GetInt()
			local 	tDirX 	= 	world:GetComponent(entity, "TestMove", "DirX"):GetInt()
			local	tDirZ 	= 	world:GetComponent(entity, "TestMove", "DirZ"):GetInt()
			local 	tSteps	= 	world:GetComponent(entity, "TestMove", "Steps"):GetInt()
			
			local	stepsTaken = 0
			for nStep = 0, tSteps-1 do
			
				if self:RecursiveMove(tUnit, tUnits, tNonWalkables, tVoids, tPosX+tDirX*nStep, tPosZ+tDirZ*nStep, tDirX, tDirZ) then
					stepsTaken = stepsTaken + 1
					
					local newCheck = world:CreateNewEntity()
					world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
					world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(tUnit)
					world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(tPosX+tDirX*nStep)
					world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(tPosZ+tDirZ*nStep)
					
					local testMoveSuccess = world:CreateNewEntity()
					world:CreateComponentAndAddTo("TestMoveSuccess", testMoveSuccess)
					world:SetComponent(testMoveSuccess, "TestMoveSuccess", "Unit", tUnit)
					world:SetComponent(testMoveSuccess, "TestMoveSuccess", "Steps", tSteps)
					
					
				else
					break
				end
				
			end
			
			world:SetComponent(tUnit, "NoSubSteps", "Counter", stepsTaken)
			
			--local cipID = Net.StartPack("Client.SendCIP")
			--Net.WriteFloat(cipID, hestPosX)
			--Net.WriteFloat(cipID, hestPosZ)
			--Net.WriteFloat(cipID, dirX)
			--Net.WriteFloat(cipID, dirZ)
			--Net.WriteFloat(cipID, 0.5)
			--Net.Broadcast(cipID)
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PostMove", id)
			
			world:KillEntity(entity)
			
		end
	end
end
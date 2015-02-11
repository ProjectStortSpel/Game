
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

end

TestMoveSystem.RecursiveMove = function(self, unitToMove, allUnits, allNonWalkables, posX, posZ, dirX, dirZ)

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
	
	--	Check all units
	for n = 1, #allUnits do
		local X, Z = world:GetComponent(allUnits[n], "MapPosition", 0):GetInt2()
		if X == posX and Z == posZ then
			if not self:RecursiveMove(allUnits[n], allUnits, allNonWalkables, posX+dirX, posZ+dirZ, dirX, dirZ) then
				return false
			end
				local newCheck = world:CreateNewEntity()
				world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(allUnits[n])
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(posX+dirX)
				world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(posZ+dirZ)
			break
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
	world:GetComponent(unitToMove, "LerpPosition", "Algorithm"):SetText("SmoothLerp")
	return true
end

TestMoveSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
	for newEntity = 1, #entities do
		local entity = entities[newEntity]
		if world:EntityHasComponent(entity, "TestMove") then
		
			local 	tUnits 			= 	self:GetEntities("Unit")
			local 	tNonWalkables 	=	self:GetEntities("NotWalkable")
			
			local 	tUnit 	= 	world:GetComponent(entity, "TestMove", "Unit"):GetInt()
			local 	tPosX 	= 	world:GetComponent(entity, "TestMove", "PosX"):GetInt()
			local 	tPosZ 	= 	world:GetComponent(entity, "TestMove", "PosZ"):GetInt()
			local 	tDirX 	= 	world:GetComponent(entity, "TestMove", "DirX"):GetInt()
			local	tDirZ 	= 	world:GetComponent(entity, "TestMove", "DirZ"):GetInt()
			local 	tSteps	= 	world:GetComponent(entity, "TestMove", "Steps"):GetInt()
			
			local	stepsTaken = 0;
			for nStep = 0, tSteps-1 do
			
				if self:RecursiveMove(tUnit, tUnits, tNonWalkables, tPosX+tDirX*nStep, tPosZ+tDirZ*nStep, tDirX, tDirZ) then
					stepsTaken = stepsTaken + 1
					local newCheck = world:CreateNewEntity()
					world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
					world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(tUnit)
					world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(tPosX+tDirX*nStep)
					world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(tPosZ+tDirZ*nStep)
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
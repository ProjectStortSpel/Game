SimultaneousMove	=	System()

SimultaneousMove.Initialize = function(self)
	--	Set Name
	self:SetName("SimultaneousMove")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit",				FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("SimultaneousMove",	FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Void",				FilterType.RequiresOneOf)

end

SimultaneousMove.EntitiesAdded = function(self, dt, entities)

	for newEntity = 1, #entities do
		local entity = entities[newEntity]
		if world:EntityHasComponent(entity, "SimultaneousMove") then

		end
	end
	
	local	movesToCheck	=	self:GetEntities("SimultaneousMove")
	
	if #movesToCheck ~= 0 then
		--	Kill all sim moves
		for moveToDo = 1, #movesToCheck do
			world:KillEntity(movesToCheck[moveToDo])
		end
		self:StartMoving(movesToCheck)
		
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PostMove", id)
		

	end
	
end

SimultaneousMove.StartMoving = function(self, allMovesToDo)
	
	local	listOfMoves			=	{}
			listOfMoves.__mode	=	"k"
	local	failedMoves			=	{}
			failedMoves.__mode	=	"k"
			
	--	
	for n = 1, #allMovesToDo do
		local	tPosX	=	world:GetComponent(allMovesToDo[n], "SimultaneousMove", "PosX"):GetInt()
		local	tPosZ	=	world:GetComponent(allMovesToDo[n], "SimultaneousMove", "PosZ"):GetInt()
		listOfMoves[#listOfMoves+1] = allMovesToDo[n]
	end
	
	while true do 
		
		local	sizeBefore	=	#listOfMoves
		
		for n = #listOfMoves, 1, -1 do
		
			local	tPosX	=	world:GetComponent(listOfMoves[n], "SimultaneousMove", "PosX"):GetInt()
			local	tPosZ	=	world:GetComponent(listOfMoves[n], "SimultaneousMove", "PosZ"):GetInt()
			
			if self:HasObstacleAt(tPosX, tPosZ) then
				failedMoves[#failedMoves+1]	=	listOfMoves[n]
				table.remove(listOfMoves, n)
				break
			elseif self:HasNonMovingUnitAt(tPosX, tPosZ, listOfMoves) then
				failedMoves[#failedMoves+1]	=	listOfMoves[n]
				table.remove(listOfMoves, n)
				break
			else
				local	shouldStop	=	false
				for i = 1, #failedMoves do
					local	tUnit	=	world:GetComponent(failedMoves[i], "SimultaneousMove", "Unit"):GetInt()
					local	posX	=	world:GetComponent(tUnit, "MapPosition", "X"):GetInt()
					local	posZ	=	world:GetComponent(tUnit, "MapPosition", "Z"):GetInt()
					
					if tPosX == posX and tPosZ == posZ then
						failedMoves[#failedMoves+1]	=	listOfMoves[n]
						--listOfMoves[n]	=	nil
						table.remove(listOfMoves, n)
						shouldStop	=	true
						break
					end
				end
				if shouldStop then
					break
				end
			end

		end
		
		if sizeBefore == #listOfMoves then
			break
		end
	end
	
	for n = 1, #listOfMoves do
		
		local	tempMove	=	listOfMoves[n]
		
		local 	tUnit 	= 	world:GetComponent(tempMove, "SimultaneousMove", "Unit"):GetInt()
		local 	tPosX 	= 	world:GetComponent(tempMove, "SimultaneousMove", "PosX"):GetInt()
		local 	tPosZ 	= 	world:GetComponent(tempMove, "SimultaneousMove", "PosZ"):GetInt()
		
		
		world:GetComponent(tUnit, "NoSubSteps", "Counter"):SetInt(1)
		world:GetComponent(tUnit, "MapPosition", "X"):SetInt2(tPosX, tPosZ)
		self:SetLerpFor(tempMove)
		
		local newCheck = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CheckCheckpointForEntity", newCheck)
		world:GetComponent(newCheck, "CheckCheckpointForEntity", "EntityId"):SetInt(tUnit)
		world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosX"):SetInt(tPosX)
		world:GetComponent(newCheck, "CheckCheckpointForEntity", "PosZ"):SetInt(tPosZ)
		
		
	end
	
	

end

SimultaneousMove.HasObstacleAt = function(self, X, Z)

	local	allNonwalkables	=	self:GetEntities("NotWalkable")
	for tempTile = 1, #allNonwalkables do
		
		local	tX, tZ	=	world:GetComponent(allNonwalkables[tempTile], "MapPosition", "X"):GetInt2()
		
		if tX == X and tZ == Z then
			return true
		end
	end
	
	return false
end

SimultaneousMove.HasNonMovingUnitAt = function(self, X, Z, unitMoves)

	local	allUnits	=	self:GetEntities("Unit")
	for tempUnit = 1, #allUnits do
		local	checkThisUnit	=	true
		for tMove = 1, #unitMoves do
			local	tUnit	=	world:GetComponent(unitMoves[tMove], "SimultaneousMove", "Unit"):GetInt()
			
			local	tX, tZ	=	world:GetComponent(tUnit, "MapPosition", "X"):GetInt2()
				
			if tX == X and tZ == Z then
				checkThisUnit	=	false
				break
			end
		end
		
		if checkThisUnit then
			
			if not world:EntityHasComponent(allUnits[tempUnit], "UnitDead") then
				local	tX, tZ	=	world:GetComponent(allUnits[tempUnit], "MapPosition", "X"):GetInt2()
				if tX == X and tZ == Z then
					return true
				end
			end
		end
		
	end

	return false
end

SimultaneousMove.SetLerpFor = function(self, moveToLerp)

	local 	tUnit 	= 	world:GetComponent(moveToLerp, "SimultaneousMove", "Unit"):GetInt()
	local 	tPosX 	= 	world:GetComponent(moveToLerp, "SimultaneousMove", "PosX"):GetInt()
	local 	tPosZ 	= 	world:GetComponent(moveToLerp, "SimultaneousMove", "PosZ"):GetInt()
	local 	tTime 	= 	world:GetComponent(moveToLerp, "SimultaneousMove", "SlerpTime"):GetFloat()
	
	--	Move the unit
	world:GetComponent(tUnit, "MapPosition", 0):SetInt2(tPosX, tPosZ)
	if not world:EntityHasComponent(tUnit, "LerpPosition") then
		world:CreateComponentAndAddTo("LerpPosition", tUnit)
	end
	world:GetComponent(tUnit, "LerpPosition", "X"):SetFloat(tPosX)
	world:GetComponent(tUnit, "LerpPosition", "Y"):SetFloat(0.5)
	world:GetComponent(tUnit, "LerpPosition", "Z"):SetFloat(tPosZ)
	world:GetComponent(tUnit, "LerpPosition", "Time"):SetFloat(tTime)
	world:GetComponent(tUnit, "LerpPosition", "Algorithm"):SetText("PlayerMove")
	
	if not world:EntityHasComponent(tUnit, "UnitWantTileOffset") then
		world:CreateComponentAndAddTo("UnitWantTileOffset", tUnit)
	end
end









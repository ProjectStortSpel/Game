FindSpawnpointSystem = System()
FindSpawnpointSystem.MapSizeX	=	0
FindSpawnpointSystem.MapSizeZ	=	0

FindSpawnpointSystem.Initialize = function(self)
	--	Set Name
	self:SetName("FindSpawnpointSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Spawn", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Void", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("River", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Hole", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
end

FindSpawnpointSystem.IsEmpty = function(self, X, Z)
	
	local	nonWalkables	=	self:GetEntities("NotWalkable")
	for n = 1, #nonWalkables do
		local	tX, tZ	=	world:GetComponent(nonWalkables[n], "MapPosition", "X"):GetInt2()
		
		if X == tX and tZ == Z then
			return false
		end
	end
	
	local	voidTiles	=	self:GetEntities("Void")
	for n = 1, #voidTiles do
		local	tX, tZ	=	world:GetComponent(voidTiles[n], "MapPosition", "X"):GetInt2()
		
		if X == tX and tZ == Z then
			return false
		end
	end
	
	local	unitPos	=	self:GetEntities("Unit")
	for n = 1, #unitPos do
		local	tX, tZ	=	world:GetComponent(unitPos[n], "MapPosition", "X"):GetInt2()
		
		if X == tX and tZ == Z then
			return false
		end
	end
	
	local	riverTiles	=	self:GetEntities("River")
	for n = 1, #riverTiles do
		local	tX, tZ	=	world:GetComponent(riverTiles[n], "MapPosition", "X"):GetInt2()
		
		if X == tX and tZ == Z then
			return false
		end
	end
	
	local	holeTiles	=	self:GetEntities("Hole")
	for n = 1, #holeTiles do
		local	tX, tZ	=	world:GetComponent(holeTiles[n], "MapPosition", "X"):GetInt2()
		
		if X == tX and tZ == Z then
			return false
		end
	end

	return true
end

FindSpawnpointSystem.FindValidSpawnpointNear = function(self, X, Z, unitToSpawn)
	
	if self:IsEmpty(X, Z) then
		world:GetComponent(unitToSpawn, "Spawnpoint", "X"):SetInt2(X, Z)
		return
	end
	

	
end

FindSpawnpointSystem.SpawnUnitAt = function(self, X, Z, unitToSpawn)

	if not world:EntityHasComponent(unitToSpawn, "LerpPosition") then
		world:CreateComponentAndAddTo("LerpPosition", unitToSpawn)
	end
	world:GetComponent(unitToSpawn, "LerpPosition", "X"):SetFloat(X)
	world:GetComponent(unitToSpawn, "LerpPosition", "Y"):SetFloat(0.5)
	world:GetComponent(unitToSpawn, "LerpPosition", "Z"):SetFloat(Z)
	world:GetComponent(unitToSpawn, "LerpPosition", "Time"):SetFloat(0)
	world:GetComponent(unitToSpawn, "LerpPosition", "Algorithm"):SetText("PlayerMove")
	
	if not world:EntityHasComponent(unitToSpawn, "UnitWantTileOffset") then
		world:CreateComponentAndAddTo("UnitWantTileOffset", unitToSpawn)
	end
	world:GetComponent(unitToSpawn, "MapPosition", "X"):SetInt2(X, Z)
	
	if world:EntityHasComponent(unitToSpawn, "Hide") then
		world:RemoveComponentFrom("Hide", unitToSpawn)
	end
	
end

FindSpawnpointSystem.GetSpawnDirection = function(self, nIteration)
	
	if nIteration % 4 == 0 then
		return -1*(nIteration%4 + 1), 0
	elseif nIteration % 3 == 0 then
		return 0, 1
	elseif nIteration % 2 == 0 then
		return 1, 0
	elseif nIteration % 1 == 0 then
		return 0, -1
	end
	
	return	0, 0
end

FindSpawnpointSystem.GetListIndex = function(self, X, Z, listWidth)
	return (Z * listWidth + X + 1)
end

FindSpawnpointSystem.GetCheckpointId = function(self, checkpointNumber)
	
	local	nCheckpoints	=	self:GetEntities("Checkpoint")
	
	for tempN = 1, #nCheckpoints do
		
		if world:GetComponent(nCheckpoints[tempN], "Checkpoint", "Number"):GetInt() == checkpointNumber then
			return nCheckpoints[tempN]
		end
		
	end
	
	return -1
end

FindSpawnpointSystem.IsInsideWorld = function(self, X, Z)

	if X >= 0 and X < self.MapSizeX then
		if X >= 0 and X < self.MapSizeZ then
			return true
		end
	end
	return false
end

FindSpawnpointSystem.EntitiesAdded = function(self, dt, newEntities)

	for n = 1, #newEntities do
		
		local	newEntity	=	newEntities[n]
		if world:EntityHasComponent(newEntity, "Spawn") then
			
			local	unitId	=	world:GetComponent(newEntity, "Spawn", "UnitEntityId"):GetInt()
			local	tempX, tempZ	=	world:GetComponent(unitId, "Spawnpoint", "X"):GetInt2()
			
			local	originalX		=	tempX
			local	originalZ		=	tempZ
			
			
			if not self:IsEmpty(tempX, tempZ) then
				
				local	distanceAround	=	1
				local	tilesDistance			=	{}
						tilesDistance.__mode 	= 	"k"
						
				local	tempId = world:GetComponent(unitId, "TargetCheckpoint", "Id"):GetInt()
				local	targetId		=	self:GetCheckpointId(tempId)
				local	goalX, goalZ	=	world:GetComponent(targetId, "MapPosition", "X"):GetInt2()
				
				local	goalValue		=	PathfinderHandler.GeneratePath(tempX, tempZ, goalX, goalZ)
				local	closestValue	=	2147483647
				local	closestIndex	=	-1
				local	closestX		=	tempX
				local	closestZ		=	tempZ
				
				--	Give it values
				for xOffset = -distanceAround, distanceAround do
					for zOffset = -distanceAround, distanceAround do
						tilesDistance[#tilesDistance+1]	=	-1
						local	tX	=	tempX+xOffset
						local	tZ	=	tempZ+zOffset
						
						if self:IsInsideWorld(tX, tZ) then
						
							if self:IsEmpty(tX, tZ) then
							
								local	tempIndex			=	self:GetListIndex(xOffset+distanceAround, zOffset+distanceAround, 2*distanceAround)
								local	tempDistance		=	PathfinderHandler.GeneratePath(tX, tZ, goalX, goalZ)
								tilesDistance[tempIndex]	=	tempDistance
								
								if math.abs(tempDistance - goalValue) < closestValue then
									closestValue	=	math.abs(tempDistance - goalValue)
									closestIndex	=	tempIndex
									
									closestX	=	tX
									closestZ	=	tZ
								end
							end
						end
						
					end
				end
				
				if closestIndex ~= -1 then
				
					tempX	=	closestX
					tempZ	=	closestZ
				end
				
			end
			
			self:SpawnUnitAt(tempX, tempZ, unitId)
		
			world:KillEntity(newEntity)
			
		end
		
		if world:EntityHasComponent(newEntity, "MapSpecs") then
			local tX, tZ = world:GetComponent(newEntity, "MapSpecs", "SizeX"):GetInt2()
			
			self.MapSizeX = tX
			self.MapSizeZ = tZ	
		end
		
		
	end
	
end
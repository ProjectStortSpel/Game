AbilityStoneSystem = System()

AbilityStoneSystem.Initialize = function(self)

	self:SetName("AbilityStoneSystem")
		
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("UnitStone",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AbilityStone",FilterType.RequiresOneOf)

	--self:AddComponentTypeToFilter("NotWalkable",FilterType.RequiresOneOf)
	
	--self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewStep", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("TileComp",FilterType.RequiresOneOf)
	
	--self:AddComponentTypeToFilter("River",FilterType.Excluded)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.Excluded)
	self:AddComponentTypeToFilter("Void",FilterType.Excluded)
	self:AddComponentTypeToFilter("AvailableSpawnpoint",FilterType.Excluded)
	self:AddComponentTypeToFilter("Finishpoint",FilterType.Excluded)
	self:AddComponentTypeToFilter("Checkpoint",FilterType.Excluded)
	
	
	
end

AbilityStoneSystem.UpdateLifeTime = function(self, entity)

	local stones = self:GetEntities("AbilityStone")
	
	for i = 1, #stones do
	
		local lifeSpan = world:GetComponent(stones[i], "AbilityStone", "LifeSpan")
		local count = lifeSpan:GetInt()
		print("LifeSpan: " .. count)
		lifeSpan:SetInt(count - 1)
		
		if lifeSpan:GetInt() <= 0 then
			local parent = world:GetComponent(stones[i], "Parent", "EntityId"):GetInt()
			world:RemoveComponentFrom("NotWalkable", parent)
			world:KillEntity(stones[i])
		end

	end

end

AbilityStoneSystem.CheckAddStone = function(self, entity)

	local tiles = self:GetEntities("TileComp")
	local posX, posZ = world:GetComponent(entity, "MapPosition", 0):GetInt2()
	local dirX, dirZ = world:GetComponent(entity, "Direction", 0):GetInt2()
	local X = posX - dirX
	local Z = posZ - dirZ
	
	for i = 1, #tiles do
	
		local mapX, mapZ = world:GetComponent(tiles[i], "MapPosition", 0):GetInt2()
		
		if mapX == X and mapZ == Z then
		
			world:CreateComponentAndAddTo("NotWalkable", tiles[i])
			
			local stone = world:CreateNewEntity("Stone")
			world:CreateComponentAndAddTo("SyncNetwork", stone)
			world:CreateComponentAndAddTo("Parent", stone)
			world:CreateComponentAndAddTo("AbilityStone", stone)
			
			world:SetComponent(stone, "AbilityStone", "LifeSpan", 5)
			world:SetComponent(stone, "Parent", "EntityId", tiles[i])
			world:GetComponent(stone, "Position", 0):SetFloat3(mapX, 0.5, mapZ)
			world:GetComponent(stone, "Rotation", 0):SetFloat3
			(
				math.pi * 0.01 * math.random(0, 25), 
				math.pi * 0.01 * math.random(0, 100), 
				math.pi * 0.01 * math.random(0, 25)
			)
		
			break
		
		end
		
	end
	
	world:RemoveComponentFrom("UnitStone", entity)
	
end

AbilityStoneSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	
	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "NewStep") then
			self:UpdateLifeTime(entity)
		elseif world:EntityHasComponent(entity, "UnitStone") then
			self:CheckAddStone(entity)
		end	
	end
end
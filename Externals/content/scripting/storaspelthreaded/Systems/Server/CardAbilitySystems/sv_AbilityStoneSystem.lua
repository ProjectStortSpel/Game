AbilityStoneSystem = System()
AbilityStoneSystem.LifeSpan = 5

AbilityStoneSystem.Initialize = function(self)

	self:SetName("AbilityStoneSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("UnitStone",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AbilityStone",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("StoneTimer",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MoveRiver", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TestMoveSuccess",FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("NotWalkable",FilterType.Excluded)
	self:AddComponentTypeToFilter("Void",FilterType.Excluded)
	
	
	
end

AbilityStoneSystem.UpdateLifeTime = function(self, entity)

	local stones = self:GetEntities("AbilityStone")
	local timers = self:GetEntities("StoneTimer")
	
	for j =1 , #timers do
	
		local lifeSpan = world:GetComponent(timers[j], "StoneTimer", "LifeSpan")
		local count = lifeSpan:GetInt()
		lifeSpan:SetInt(count - 1)
		
		if lifeSpan:GetInt() <= 0 then
			world:KillEntity(timers[j])
		end
		
	
	end
	
	for i = 1, #stones do
	
		local lifeSpan = world:GetComponent(stones[i], "AbilityStone", "LifeSpan")
		local count = lifeSpan:GetInt()
		lifeSpan:SetInt(count - 1)
		
		if lifeSpan:GetInt() <= 0 then
		
			local parent = world:GetComponent(stones[i], "Parent", "EntityId"):GetInt()
			local x, z = world:GetComponent(parent, "MapPosition", 0):GetInt2()
			
			PathfinderHandler.SetTileWalkable(x,z, true)
			world:RemoveComponentFrom("NotWalkable", parent)
			world:KillEntity(stones[i])
			
		end
	
	
	end

	local uStone = self:GetEntities("UnitStone")
	for i = 1, #uStone do
		world:RemoveComponentFrom("UnitStone", uStone[i])
	end
	
end

AbilityStoneSystem.CheckAddStone = function(self, entity)

	local posX, posZ = world:GetComponent(entity, "MapPosition", 0):GetInt2()
	local dirX, dirZ = world:GetComponent(entity, "Direction", 0):GetInt2()

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("TestMove", id)
	world:SetComponent(id, "TestMove", "Unit", entity)
	world:SetComponent(id, "TestMove", "PosX", posX+dirX)
	world:SetComponent(id, "TestMove", "PosZ", posZ+dirZ)
	world:SetComponent(id, "TestMove", "DirX", dirX)
	world:SetComponent(id, "TestMove", "DirZ", dirZ)
	world:SetComponent(id, "TestMove", "Steps", 1)
	
end

AbilityStoneSystem.AddTimer = function(self, entity)
	
	local timerAbove = world:CreateNewEntity("StoneTimer")
	local R, G, B = world:GetComponent(entity, "Color", "X"):GetFloat3()
	
	world:GetComponent(timerAbove, "Position", 0):SetFloat3(0.0, 1 + 0.001, 0.0)
	world:GetComponent(timerAbove, "Rotation", 0):SetFloat3(1.5 * math.pi, math.pi * 0.5, 0.0)
	world:GetComponent(timerAbove, "Scale", 0):SetFloat3(0.7,0.7,0.7)
	world:GetComponent(timerAbove, "Model", "ModelName"):SetText("timer")
	world:GetComponent(timerAbove, "Model", "ModelPath"):SetText("quad")
	world:GetComponent(timerAbove, "Model", "RenderType"):SetInt(1)
	world:GetComponent(timerAbove, "Parent", 0):SetInt(entity)
	world:GetComponent(timerAbove, "StoneTimer", "LifeSpan"):SetInt(self.LifeSpan)
	world:GetComponent(timerAbove, "Color", "X"):SetFloat3(R,G,B)	
	
end

AbilityStoneSystem.PlaceStone = function(self, entity)
	
	local unit = world:GetComponent(entity, "TestMoveSuccess", "Unit"):GetInt()
	
	if world:EntityHasComponent(unit, "UnitStone") then
		
		local tiles = self:GetEntities("TileComp")
		local posX, posZ = world:GetComponent(unit, "MapPosition", 0):GetInt2()
		local dirX, dirZ = world:GetComponent(unit, "Direction", 0):GetInt2()
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
				world:CreateComponentAndAddTo("LerpPosition", stone)
				
				PathfinderHandler.SetTileWalkable(X,Z, false)
				
				math.randomseed( os.time() )
				math.random(); math.random(); math.random(); -- pop the not randomized values (blame lua)
				world:SetComponent(stone, "AbilityStone", "LifeSpan", self.LifeSpan)
				world:SetComponent(stone, "Parent", "EntityId", tiles[i])
				world:GetComponent(stone, "Position", 0):SetFloat3( math.random(1, 5), 10.5, math.random(1, 5) )
				
				world:GetComponent(stone, "LerpPosition", "X"):SetFloat(0)
				world:GetComponent(stone, "LerpPosition", "Y"):SetFloat(0.5)
				world:GetComponent(stone, "LerpPosition", "Z"):SetFloat(0)
				world:GetComponent(stone, "LerpPosition", "Time"):SetFloat(0.7)
				world:GetComponent(stone, "LerpPosition", "Algorithm"):SetText("NormalLerp")
				
				self:AddTimer(stone)
				
				break				
			
			end
			
		end
		world:RemoveComponentFrom("UnitStone", unit)
	end
	
	world:KillEntity(entity)

end

AbilityStoneSystem.EntitiesAdded = function(self, dt, entities)

	
	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent(entity, "MoveRiver") then
			self:UpdateLifeTime(entity)
		elseif world:EntityHasComponent(entity, "UnitStone") then
			self:CheckAddStone(entity)
		elseif world:EntityHasComponent(entity, "TestMoveSuccess") then
			self:PlaceStone(entity)
		end	
	end
end
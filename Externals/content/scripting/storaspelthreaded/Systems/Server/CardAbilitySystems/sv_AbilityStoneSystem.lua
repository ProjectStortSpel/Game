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
	
	self:AddComponentTypeToFilter("StoneTimerText",FilterType.RequiresOneOf)
	
end

AbilityStoneSystem.UpdateLifeTime = function(self, entity)

	local stones = self:GetEntities("AbilityStone")
	local timers = self:GetEntities("StoneTimer")
	
	local timertext = self:GetEntities("StoneTimerText")
	for i = 1, #timertext do
		world:KillEntity(timertext[i])
	end
	
	for j = 1 , #timers do
	
		local lifeSpan = world:GetComponent(timers[j], "StoneTimer", "LifeSpan")
		local count = lifeSpan:GetInt() - 1
		lifeSpan:SetInt(count)
		
		if count <= 0 then
			world:KillEntity(timers[j])
		end
	end
	
	for i = 1, #stones do
	
		local lifeSpan = world:GetComponent(stones[i], "AbilityStone", "LifeSpan")
		local count = lifeSpan:GetInt() - 1
		lifeSpan:SetInt(count)
		
		local parent = world:GetComponent(stones[i], "Parent", "EntityId"):GetInt()
		local x, z = world:GetComponent(parent, "MapPosition", 0):GetInt2()
		
		local newEntity = world:CreateNewEntity()
		world:CreateComponentAndAddTo("TileWalkabilityHasChanged", newEntity)
		world:GetComponent(newEntity, "TileWalkabilityHasChanged", 0):SetInt3(count, x, z)
		--world:GetComponent(newEntity, "TileWalkabilityHasChanged", "Walkable"):SetBool(true)
		
		if count <= 0 then
			world:RemoveComponentFrom("NotWalkable", parent)
			world:KillEntity(stones[i])
		else
			local text = self:CreateElement("quad", "text", x, 1.22, z, 0.46, 0.46)
			world:CreateComponentAndAddTo("StoneTimerText", text)
			self:AddTextToTexture("StoneText"..i, lifeSpan:GetInt(), 0, 0.5, 0.5, 0.2, text)
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

AbilityStoneSystem.AddTimer = function(self, entity, X, Z)
	
	local timerAbove = world:CreateNewEntity("StoneTimer")

	world:GetComponent(timerAbove, "Position", 0):SetFloat3(X, 1.2, Z)
	world:GetComponent(timerAbove, "Rotation", 0):SetFloat3(1.5 * math.pi, math.pi * 0.25, 0.0)
	world:GetComponent(timerAbove, "Scale", 0):SetFloat3(0.7,0.7,0.7)
	world:GetComponent(timerAbove, "Model", "ModelName"):SetText("timer")
	world:GetComponent(timerAbove, "Model", "ModelPath"):SetText("quad")
	world:GetComponent(timerAbove, "Model", "RenderType"):SetInt(1)
	world:GetComponent(timerAbove, "StoneTimer", "LifeSpan"):SetInt(self.LifeSpan)
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
				world:CreateComponentAndAddTo("AddEntityAfterLerp", stone)
				
				local newEntity = world:CreateNewEntity()
				world:CreateComponentAndAddTo("TileWalkabilityHasChanged", newEntity)
				world:GetComponent(newEntity, "TileWalkabilityHasChanged", 0):SetInt3(self.LifeSpan, X, Z)
				--world:GetComponent(newEntity, "TileWalkabilityHasChanged", "Walkable"):SetBool(false)
				
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
				world:GetComponent(stone, "AddEntityAfterLerp", "ComponentName"):SetText("AddStoneImpact")

				local audioId = Net.StartPack("Client.PlaySoundC")
				Net.WriteString(audioId, "FallingWhistleShort")
				Net.WriteString(audioId, "FallingWhistleShort" .. stone)
				Net.WriteBool(audioId, false)
				Net.Broadcast(audioId)
				
				audioId = Net.StartPack("Client.SetSoundVolume")
				Net.WriteString(audioId, "FallingWhistleShort" .. stone)
				Net.WriteInt(audioId, 10)
				Net.Broadcast(audioId)
				
				self:AddTimer(stone,X,Z)
				
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


AbilityStoneSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SyncNetwork", id)
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 1)
	local position = world:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = world:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1, false)
	local rotation = world:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(1.5 * math.pi, 0.25 * math.pi, 0)
	return id	
end

AbilityStoneSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(1)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(1)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(1)
	world:CreateComponentAndAddTo("Color", button)
	world:GetComponent(button, "Color", "X"):SetFloat(r)
	world:GetComponent(button, "Color", "Y"):SetFloat(g)
	world:GetComponent(button, "Color", "Z"):SetFloat(b)
end
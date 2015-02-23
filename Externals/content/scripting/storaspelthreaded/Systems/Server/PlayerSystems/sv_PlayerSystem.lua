PlayerSystem = System()

PlayerSystem.Initialize = function(self)
	self:SetName("Players Connected System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	self:AddComponentTypeToFilter("Player", FilterType.Mandatory)
	self:AddComponentTypeToFilter("ActiveNetConnection", FilterType.Mandatory)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

--PlayerSystem.FindEmptyId = function(self)
--
--	local currentPlayers = self:GetEntities()
--	local newPlayerId = 5
--	
--	local availableIds = {}
--	availableIds.__mode = "k"
--	for n = 1, 5 do
--		availableIds[n] = false
--	end
--	
--	for i = 1, #currentPlayers do
--		local playerId = world:GetComponent(currentPlayers[i], "PlayerNumber", "Number"):GetInt()
--		
--		if playerId ~= -1 then
--			availableIds[playerId] = true
--		end
--	end
--	
--	local newIndex = 1
--	while availableIds[newIndex] do
--		newIndex = newIndex + 1
--	end
--	
--	return newIndex
--end

PlayerSystem.EntitiesAdded = function(self, dt, entities)
	
	for i = 1, #entities do
		
		world:CreateComponentAndAddTo("NeedUnit", entities[i])
	end
	
	--for n = 1, #entities do
	--	local entityId = entities[n]
	--	world:SetComponent(entityId, "PlayerNumber", "Number", -1)
	--end
    --
	--for n = 1, #entities do
	--	local entityId = entities[n]
	--	
	--	local playerNumber = self:FindEmptyId()
	--	local newEntityId = world:CreateNewEntity("Unit")
    --
	--	world:SetComponent(newEntityId, "Model", "ModelName", "ply" .. playerNumber);
	--	world:SetComponent(newEntityId, "Model", "ModelPath", "caveman");
	--	world:SetComponent(newEntityId, "Model", "RenderType", 0);
    --
	--	world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
	--	world:SetComponent(newEntityId, "PlayerEntityId", "Id", entityId)
	--	world:SetComponent(newEntityId, "TargetCheckpoint", "Id", 1)
	--	world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)
	--	world:CreateComponentAndAddTo("NeedSpawnLocation", newEntityId)
    --
    --
	--	world:SetComponent(entityId, "PlayerNumber", "Number", playerNumber)
	--	world:SetComponent(entityId, "UnitEntityId", "Id", newEntityId)
    --
	--	local ip = world:GetComponent(entityId, "NetConnection", "IpAddress"):GetText()
	--	local port = world:GetComponent(entityId, "NetConnection", "Port"):GetInt()
    --
    --
	--	print("Unit ", newEntityId)
    --
	--	local id = Net.StartPack("Client.SendPlayerUnitId")
	--	Net.WriteInt(id, playerNumber)
	--	Net.Send(id, ip, port)
	--end
end

PlayerSystem.EntitiesRemoved = function(self, dt, entities)

	for i = 1, #entities do
		
		local entity = world:CreateNewEntity()
		local plyNum = world:GetComponent(entities[i], "PlayerNumber", "Number"):GetInt()
		local unitId = world:GetComponent(entities[i], "UnitEntityId", "Id"):GetInt()
		world:CreateComponentAndAddTo("RemoveUnit", entity)
		world:GetComponent(entity, "RemoveUnit", "PlayerNo"):SetInt(plyNum)
		world:GetComponent(entity, "RemoveUnit", "UnitEntityId"):SetInt(unitId)
	end
	
	--for n = 1, #entities do
		--local entityId = entities[n]
        --
		--local plyNum = world:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
        --
		--local unitId = world:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()
        --
		--world:KillEntity(unitId)
		
	--end
end
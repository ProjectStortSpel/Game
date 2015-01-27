GiveSpawnLocationSystem = System()

GiveSpawnLocationSystem.Initialize = function(self)

	--	Set Name
	self:SetName("GiveSpawnLocationSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("NeedSpawnLocation", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AvailableSpawnpoint", FilterType.RequiresOneOf)
end

GiveSpawnLocationSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "NeedSpawnLocation") then
			print("LAOSDAKOFAOSFKAOFKOFK")
			local availableSpawns = self:GetEntities("AvailableSpawnpoint")
			local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number").GetInt()
			
			local unitSpawnpoint = self:GetComponent(entityId, "Spawnpoint", 0)
			local mapSpawnpoint = self:GetComponent(availableSpawns[plyNum], "AvailableSpawnpoint", 0)
			local posX, posZ = mapSpawnpoint:GetInt2()
			unitSpawnpoint:SetInt2(posX, posZ)
			local a, b = unitSpawnpoint:GetInt2()
			print("Player " .. plyNum .. " wants to spawn at (".. a .. ", " .. b .. ")")
			
			world:CreateComponentAndAddTo("Spawn", entityId)
		end
	end
end
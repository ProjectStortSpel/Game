GiveSpawnLocation = System()



GiveSpawnLocation.Update = function(self, dt)

end

GiveSpawnLocation.Initialize = function(self)
	self:SetName("GiveSpawnLocation")
	self:AddComponentTypeToFilter("NeedSpawnLocation", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AvailableSpawnpoint", FilterType.RequiresOneOf)
end

GiveSpawnLocation.PostInitialize = function(self)

end

GiveSpawnLocation.OnEntityAdded = function(self, entityId)



	if world:EntityHasComponent(entityId, "NeedSpawnLocation") then

		local availableSpawns = self:GetEntities("AvailableSpawnpoint")
		local plyNum = world:GetComponent(entityId, "PlayerNumber", "Number").GetInt()
		
		local unitSpawnpoint = world:GetComponent(entityId, "Spawnpoint", 0)
		print("GiveSpawnLocation.OnEntityAdded")
		print("GiveSpawnLocation.OnEntityAdded")
		print("GiveSpawnLocation.OnEntityAdded")
		local mapSpawnpoint = world:GetComponent(availableSpawns[plyNum], "AvailableSpawnpoint", 0)
		local posX, posZ = mapSpawnpoint:GetInt2()
		unitSpawnpoint:SetInt2(posX, posZ)
		local a, b = unitSpawnpoint:GetInt2()
		print("Player " .. plyNum .. " wants to spawn at (".. a .. ", " .. b .. ")")
		
		world:CreateComponentAndAddTo("Spawn", entityId)
	end
	

end

GiveSpawnLocation.OnEntityRemoved = function(self, entityId)
	
end
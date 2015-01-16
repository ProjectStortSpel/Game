GiveSpawnLocation = System()



GiveSpawnLocation.Update = function(self, dt)

end

GiveSpawnLocation.Initialize = function(self)
	self:SetName("GiveSpawnLocation")
	self:AddComponentTypeToFilter("NeedSpawnLocation", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AvailableSpawnpoint", FilterType.RequiresOneOf)

	print("GiveSpawnLocation initialized!")
end

GiveSpawnLocation.PostInitialize = function(self)

end

GiveSpawnLocation.OnEntityAdded = function(self, entityId)



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

GiveSpawnLocation.OnEntityRemoved = function(self, entityId)
	
end
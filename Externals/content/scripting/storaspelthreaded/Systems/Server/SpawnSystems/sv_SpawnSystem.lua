SpawnSystem = System()

SpawnSystem.Initialize = function(self)
	--	Set Name
	self:SetName("SpawnSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Spawn", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Spawnpoint", FilterType.Mandatory)
end

SpawnSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		local mapPosComp = world:GetComponent(entityId, "MapPosition", 0)
		local spawnpointComp = world:GetComponent(entityId, "Spawnpoint", 0)

		local X, Z = spawnpointComp:GetInt2()

		if not world:EntityHasComponent(entityId, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", entityId)
		end
		world:GetComponent(entityId, "LerpPosition", "X"):SetFloat(X)
		world:GetComponent(entityId, "LerpPosition", "Y"):SetFloat(0.5)
		world:GetComponent(entityId, "LerpPosition", "Z"):SetFloat(Z)
		world:GetComponent(entityId, "LerpPosition", "Time"):SetFloat(0)
		world:GetComponent(entityId, "LerpPosition", "Algorithm"):SetText("PlayerMove")

		if not world:EntityHasComponent(entityId, "UnitWantTileOffset") then
			world:CreateComponentAndAddTo("UnitWantTileOffset", entityId)
		end
	
		mapPosComp:SetInt2(X, Z)

		world:RemoveComponentFrom("Spawn", entityId)
		print(entityId.." spawned at " .. X .. " " .. Z)
	end
end
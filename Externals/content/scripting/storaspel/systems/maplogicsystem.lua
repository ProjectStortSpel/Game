---------------------------- New Step System

NewStepSystem = System()

NewStepSystem.Initialize = function(self)
	self:SetName("New Step System")
	self:AddComponentTypeToFilter("NewStep", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("InactivePlayer",FilterType.Excluded)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
	print("New Step System initialized!")
end

NewStepSystem.OnEntityAdded = function(self, entity)
	
	--print("New Step System entity added")
	--world:RemoveComponentFrom("NewStep", entity);
end

---------------------------- New Round System

NewRoundSystem = System()

NewRoundSystem.Initialize = function(self)
	self:SetName("New Round System")
	self:AddComponentTypeToFilter("NewRound", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("InactivePlayer",FilterType.Excluded)
	print("New Round System initialized!")
end

NewRoundSystem.OnEntityAdded = function(self, entity)
	
	print("New Round System entity added")
	world:RemoveComponentFrom("NewRound", entity);
end

---------------------------- RespawnSystem

RespawnSystem = System()

RespawnSystem.Initialize = function(self)
	self:SetName("Respawn System")
	self:AddComponentTypeToFilter("Spawn", FilterType.Mandatory)
	self:AddComponentTypeToFilter("InactivePlayer", FilterType.Mandatory)
	self:AddComponentTypeToFilter("MapPosition", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("NewRound", FilterType.Excluded)
	--self:AddComponentTypeToFilter("Position", FilterType.Excluded)
	print("Respawn System initialized!")
end

RespawnSystem.OnEntityAdded = function(self, entity)

	--print("Respawn add")
	if self:EntityHasComponent(entity, "Position" ) then
		--print("remove position")
		world:RemoveComponentFrom("Position", entity);
	end
	--print("Respawn added")
end

RespawnSystem.OnEntityRemoved = function(self, entity)
	
	--print("Respawn remove")
	local spawnComp = self:GetComponent(entity, "Spawn", 0)
	local spawnX, spawnY = spawnComp:GetInt2()
	
	local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
	mapPosComp:SetInt2(spawnX, spawnY)
	
	world:CreateComponentAndAddTo("Position", entity)
	local posComp = self:GetComponent(entity, "Position", 0)
	posComp:SetFloat3(spawnX, 1.0, spawnY)
	
	world:RemoveComponentFrom("InactivePlayer", entity);
	--print("Respawn removed")
end

---------------------------- WaterMovementSystem

WaterMovementSystem = System()

WaterMovementSystem.Initialize = function(self)
	self:SetName("Water Movement System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("MapPosition", FilterType.Mandatory)
	--self:AddComponentTypeToFilter("Direction", FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("NewStep", FilterType.Mandatory)
	print("Water Movement System initialized!")
end

WaterMovementSystem.OnEntityAdded = function(self, entity)
	
	local mapPosComp = self:GetComponent(entity, "MapPosition", 0)
	local mapPosX, mapPosZ = mapPosComp:GetInt2()
	
	--print("Water entity added", mapPosX, mapPosZ)
	
	--MapSystem:TileHasComponent("WaterDirection", mapPosX, mapPosZ)
	
	local index = MapSystem.mapX * mapPosZ + mapPosX + 1
	local mapEntity = MapSystem.entities[index]
	
	if MapSystem:EntityHasComponent(mapEntity, "River") then
		
		local waterDirComp = MapSystem:GetComponent(mapEntity, "River", 0)
		local waterDirX, waterDirZ = waterDirComp:GetInt2()
		
		PlayerMovementSystem:SetPosition(entity, mapPosX + waterDirX, 1.0, mapPosZ + waterDirZ)
		
	end
	
	--print("Water entity added done")
	
	world:RemoveComponentFrom("NewStep", entity);
end
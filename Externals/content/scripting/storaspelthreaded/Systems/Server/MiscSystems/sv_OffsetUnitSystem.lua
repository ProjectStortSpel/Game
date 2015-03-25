OffsetUnitSystem = System()
OffsetUnitSystem.MapSizeX	=	0

OffsetUnitSystem.Initialize = function(self)
	--	Set Name
	self:SetName("OffsetUnitSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("TileOffset", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("UnitWantTileOffset", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
end

OffsetUnitSystem.IsTileRiver = function(self, X, Z)
	
	local	tileList	=	self:GetEntities("TileOffset")
	local 	tileIndex 	= 	self:GetListIndex(X, Z)
	
	if tileIndex <= 0 or tileIndex > #tileList then
		return false
	end
	
	return world:EntityHasComponent(tileList[tileIndex], "River")
end

OffsetUnitSystem.UpdateUnitLerp = function(self, unitId)

	if not world:EntityHasComponent(unitId, "LerpPosition") then
		return
	end
	
	local	pX, pY, pZ	=	world:GetComponent(unitId, "Position", "X"):GetFloat3()
	local	tX, tZ	=	world:GetComponent(unitId, "MapPosition", "X"):GetInt2()
	local	oldX, oldZ	=	math.floor(pX), math.floor(pZ)
	local	tIndex		=	self:GetListIndex(tX, tZ)
	local	allTiles	=	self:GetEntities("TileOffset")
	local	yOffset		=	-50
	if tIndex >= 1 and tIndex <= #allTiles then
		yOffset	=	world:GetComponent(allTiles[tIndex], "TileOffset", "Offset"):GetFloat()
	end
	
	if yOffset - pY > 0 then
		world:GetComponent(unitId, "LerpPosition", "Algorithm"):SetText("ExitRiver")
		
		if self:IsTileRiver(tX, tZ) or self:IsTileRiver(oldX, oldZ) then
			local audioId = Net.StartPack("Client.PlaySoundC")
			Net.WriteString(audioId, "LeavingWater")
			Net.WriteString(audioId, "LeavingWater" .. unitId)
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundPosition")
			Net.WriteString(audioId, "LeavingWater" .. unitId)
			Net.WriteFloat(audioId, pX)
			Net.WriteFloat(audioId, pY)
			Net.WriteFloat(audioId, pZ)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundVolume")
			Net.WriteString(audioId, "LeavingWater" .. unitId)
			Net.WriteInt(audioId, 94)
			Net.Broadcast(audioId)
		end
	elseif yOffset - pY < 0 then
		world:GetComponent(unitId, "LerpPosition", "Algorithm"):SetText("EnterRiver")
		
		if self:IsTileRiver(tX, tZ) or self:IsTileRiver(oldX, oldZ) then
			local audioId = Net.StartPack("Client.PlaySoundC")
			Net.WriteString(audioId, "WaterSplash")
			Net.WriteString(audioId, "WaterSplash" .. unitId)
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundPosition")
			Net.WriteString(audioId, "WaterSplash" .. unitId)
			Net.WriteFloat(audioId, pX)
			Net.WriteFloat(audioId, pY)
			Net.WriteFloat(audioId, pZ)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundVolume")
			Net.WriteString(audioId, "WaterSplash" .. unitId)
			Net.WriteInt(audioId, 34)
			Net.Broadcast(audioId)
		end
	end
	
	world:GetComponent(unitId, "LerpPosition", "Y"):SetFloat(yOffset)
	
	world:RemoveComponentFrom("UnitWantTileOffset", unitId)
end

OffsetUnitSystem.EntitiesAdded = function(self, dt, addedEntities)

	for n = 1, #addedEntities do
		local	newEntity	=	addedEntities[n]
		
		if world:EntityHasComponent(newEntity, "UnitWantTileOffset") then
			self:UpdateUnitLerp(newEntity)
		elseif world:EntityHasComponent(newEntity, "MapSpecs") then
			local tX, tZ = world:GetComponent(newEntity, "MapSpecs", "SizeX"):GetInt2()
			self.MapSizeX	=	tX
		end
	end 
end

OffsetUnitSystem.GetListIndex = function(self, X, Z)
	return Z * self.MapSizeX + X + 1
end
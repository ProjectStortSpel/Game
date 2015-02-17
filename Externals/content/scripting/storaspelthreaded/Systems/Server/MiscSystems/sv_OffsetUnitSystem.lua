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

OffsetUnitSystem.UpdateUnitLerp = function(self, unitId)

	print("AOSKDPOASKDPOASKDPOASKDPOK")
	if not world:EntityHasComponent(unitId, "LerpPosition") then
		return
	end
	
	local	tX, tZ	=	world:GetComponent(unitId, "MapPosition", "X"):GetInt2()
	
	local	yOffset	=	world:GetComponent(self:GetEntities("TileOffset")[self:GetListIndex(tX, tZ)], "TileOffset", "Offset"):GetFloat()
	
	world:GetComponent(unitId, "LerpPosition", "Y"):SetFloat(yOffset)
	
	world:RemoveComponentFrom("UnitWantTileOffset", unitId)
end

OffsetUnitSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, addedEntities)

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
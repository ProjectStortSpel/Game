PlayersSystem = System()

PlayersSystem.Update = function(self, dt)

end

PlayersSystem.Initialize = function(self)
	self:SetName("Players Connected System")
	
	self:AddComponentTypeToFilter("Player", FilterType.Mandatory)
	self:AddComponentTypeToFilter("ActiveNetConnection", FilterType.Mandatory)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

PlayersSystem.OnEntityAdded = function(self, entityId)

	world:CreateComponentAndAddTo("NeedUnit", entityId)
end

PlayersSystem.OnEntityRemoved = function(self, entityId)

	local entity = world:CreateNewEntity()
	local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
	local unitId = self:GetComponent(entityId, "UnitEntityId", "Id"):GetInt()
	
	world:CreateComponentAndAddTo("RemoveUnit", entity)
	world:GetComponent(entity, "RemoveUnit", "PlayerNo"):SetInt(plyNum)
	world:GetComponent(entity, "RemoveUnit", "UnitEntityId"):SetInt(unitId)

end
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

	local plyNum = self:GetComponent(entityId, "PlayerNumber", "Number"):GetInt()
	world:CreateComponentAndAddTo("RemoveUnit", entityId)
	world:GetComponent(entityId, "RemoveUnit", 0):SetInt(plyNum)

end
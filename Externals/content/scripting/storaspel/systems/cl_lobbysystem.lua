ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")
	
	self:AddComponentTypeToFilter("Null", FilterType.Mandatory);
end

ClientLobbySystem.Update = function(self, dt)

end

ClientLobbySystem.OnEntityAdded = function(self, entityId)

end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)

end

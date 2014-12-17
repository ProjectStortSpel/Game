ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")

	print("ClientLobbySystem initialized!")
end

ClientLobbySystem.Update = function(self, dt)
	
	if Net.IsConnected() then
	
		local text = "Connected to server.";	
		graphics:RenderSimpleText(text, 55, 5);
	
	end

end

ClientLobbySystem.OnEntityAdded = function(self, entityId)
end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)
end
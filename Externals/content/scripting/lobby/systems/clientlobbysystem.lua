ClientLobbySystem = System()

noConnections = 0;
maxConnections = 3;

ClientLobbySystem.Update = function(self, dt)
	
	if Net.IsConnected() then
	
		local text = "Connected to server.";	
		graphics:RenderSimpleText(text, 55, 5);
	
	end

end

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory)
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory)
	
	Net.Receive("StartGame", ClientLobbySystem.OnStartGame);
	maxConnections = Net.MaxConnections();
	
	print("ClientLobbySystem initialized!")
end

ClientLobbySystem.OnEntityAdded = function(self, entityId)
end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)
end


ClientLobbySystem.OnStartGame = function(self, _ip, _port)
end
ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")

	Net.Receive("NewGame", self.NewGame);
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
end

ClientLobbySystem.Update = function(self, dt)
	
	if GameRunning then
		return
	end
	
	
	if Net.IsConnected() then
	
		local text = "Connected to server.";	
		graphics:RenderSimpleText(text, 55, 5);
	
	end

end

ClientLobbySystem.OnEntityAdded = function(self, entityId)

	if GameRunning then
		return
	end

end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

end


ClientLobbySystem.NewGame = function(self, _ip, _port)
	Console.Print("ClientLobbySystem.NewGame");
end
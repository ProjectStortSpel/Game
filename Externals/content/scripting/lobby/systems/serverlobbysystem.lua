ServerLobbySystem = System()

ServerLobbySystem.m_noConnections = 0;
ServerLobbySystem.m_maxConnections = 3;

ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System")

	self:AddComponentTypeToFilter("Username", FilterType.Mandatory)
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory)
	
	ServerLobbySystem.m_maxConnections = Net.MaxConnections();
	
	print("ServerLobbySystem initialized!")
end

ServerLobbySystem.Update = function(self, dt)
	
	if Net.IsRunning() then
		ServerLobbySystem:UpdateServerOnline();
	else
		--ServerLobbySystem:UpdateServerOffline();
	end

end

ServerLobbySystem.UpdateServerOnline = function(self)

	local text = "Server started. " .. tostring(ServerLobbySystem.m_noConnections) .. "/" .. tostring(ServerLobbySystem.m_maxConnections) .. " connected. Connected players are: ";	
	graphics:RenderSimpleText(text, 55, 5);
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local username = self:GetComponent(entities[i], "Username", "Name");
		graphics:RenderSimpleText("Player" .. tostring(i) .. ": " .. username:GetString(), 55, 6 + (i * 2));
	end
		
end

ServerLobbySystem.UpdateServerOffline = function(self)
	local text = "Server not running.";
	graphics:RenderSimpleText(text, 55, 5);
end

ServerLobbySystem.RenderPlayers = function(self)



end

ServerLobbySystem.OnEntityAdded = function(self, entityId)
	ServerLobbySystem.m_noConnections = ServerLobbySystem.m_noConnections + 1;
end
ServerLobbySystem.OnEntityRemoved = function(self, entityId)
	ServerLobbySystem.m_noConnections = ServerLobbySystem.m_noConnections - 1;
end
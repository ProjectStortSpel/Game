ServerLobbySystem = System()

ServerLobbySystem.m_noConnections = 0;
ServerLobbySystem.m_maxConnections = 3;

ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System");

	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	self.m_maxConnections = Net.MaxConnections();
end

ServerLobbySystem.Update = function(self, dt)
	
	if GameRunning then
		return
	end
	
	if Net.IsRunning() then
		self:UpdateServerOnline();
	else
		--self:UpdateServerOffline();
	end

end

ServerLobbySystem.UpdateServerOnline = function(self)

	local text = "Server started. " .. tostring(self.m_noConnections) .. "/" .. tostring(self.m_maxConnections) .. " connected. Connected players are: ";	
	graphics:RenderSimpleText(text, 55, 5);
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local username = self:GetComponent(entities[i], "Username", "Name");
		graphics:RenderSimpleText("Player" .. tostring(i) .. ": " .. username:GetString(), 55, 6 + (i * 2));
	end
	
	if Input.GetKeyState(Key.Return) == InputState.Pressed and Console.IsOpen() == false then
		Console.Print("Game started");
		GameRunning = true;
		local id = Net.StartPack("NewGame");
		Net.Broadcast(id);
		Console.Print("NewGame");
	end
	
	
	
end

ServerLobbySystem.OnEntityAdded = function(self, entityId)
	
	if GameRunning then
		return
	end

	self.m_noConnections = self.m_noConnections + 1;
end
ServerLobbySystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

	self.m_noConnections = self.m_noConnections - 1;
end
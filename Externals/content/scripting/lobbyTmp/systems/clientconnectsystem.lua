

ClientConnectSystem = System()

ClientConnectSystem.Update = function(self, dt)

	if GameRunning == true or Net.IsRunning() == true then
		return;
	end

end

ClientConnectSystem.Initialize = function(self)
	self:SetName("ClientConnectSystem System");
	
	self:InitializeNetworkEvents();
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	print("ClientConnectSystem initialized!");
end

ClientConnectSystem.OnEntityAdded = function(self, entityId)

	if GameRunning == true or Net.IsRunning() == true then
		Console.Print("RETURN TRUE");
		return;
	end

	Console.Print("ClientConnectSystem.OnEntityAdded");
end

ClientConnectSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning == true or Net.IsRunning() == true then
		return;
	end

	Console.Print("ClientConnectSystem.OnEntityRemoved");
end

ClientConnectSystem.OnConnectedToServer = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return;
	end

	Console.Print("ClientConnectSystem.OnConnectedToServer");
	
	local id = Net.StartPack("UsernameLobby");
	Net.WriteString(id, "USERNAME_ClientConnectSystem");
	Net.SendToServer(id);
end

ClientConnectSystem.OnDisconnectedFromServer = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return;
	end

	Console.Print("ClientConnectSystem.OnDisconnectedFromServer");
end

ClientConnectSystem.OnTimedOutFromServer = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return;
	end

	Console.Print("ClientConnectSystem.OnTimedOutFromServer");
end

ClientConnectSystem.OnRemotePlayerConnected = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return
	end

	Console.Print("ClientConnectSystem.OnRemotePlayerConnected");
	
	
end

ClientConnectSystem.OnRemotePlayerDisconnected = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return
	end

	Console.Print("ClientConnectSystem.OnRemotePlayerDisconnected");
end

ClientConnectSystem.OnRemotePlayerKicked = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return
	end

	Console.Print("ClientConnectSystem.OnRemotePlayerKicked");
end



ServerConnectSystem.RemovePlayer = function(self, _ip, _port)

	if GameRunning == true or Net.IsRunning() == true then
		return
	end

	Console.Print("ServerConnectSystem.RemovePlayer");
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local eIp 	= self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString();
		local ePort = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt();

		if _ip == eIp and _port == ePort then
			world:KillEntity(entities[i]);
		end
		
	end

end
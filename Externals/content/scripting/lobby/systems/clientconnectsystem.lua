ClientConnectSystem = System()

ClientConnectSystem.Update = function(self, dt)
end

ClientConnectSystem.Initialize = function(self)
	self:SetName("ClientConnectSystem System")
	
	self:InitializeNetworkEvents()
	
	print("ClientConnectSystem initialized!")
end

ClientConnectSystem.OnEntityAdded = function(self, entityId)
	Console.Print("ClientConnectSystem.OnConnectedToServer");
end

ClientConnectSystem.OnEntityRemoved = function(self, entityId)
	Console.Print("ClientConnectSystem.OnConnectedToServer");
end

ClientConnectSystem.OnConnectedToServer = function(self, _ip, _port)
	Console.Print("ClientConnectSystem.OnConnectedToServer");
	
	local id = Net.StartPack("Username");
	Net.WriteString(id, "USERNAME_ClientConnectSystem");
	Net.SendToServer(id);
end

ClientConnectSystem.OnDisconnectedFromServer = function(self, _ip, _port)
	Console.Print("ClientConnectSystem.OnDisconnectedFromServer");
end

ClientConnectSystem.OnTimedOutFromServer = function(self, _ip, _port)
	Console.Print("ClientConnectSystem.OnTimedOutFromServer");
end

ClientConnectSystem.OnRemotePlayerConnected = function(self, _ip, _port)
	Console.Print("ClientConnectSystem.OnRemotePlayerConnected");
end

ClientConnectSystem.OnRemotePlayerDisconnected = function(self, _ip, _port)
	Console.Print("ClientConnectSystem.OnRemotePlayerDisconnected");
end

ClientConnectSystem.OnRemotePlayerKicked = function(self, _ip, _port)
	Console.Print("ClientConnectSystem.OnRemotePlayerKicked");
end


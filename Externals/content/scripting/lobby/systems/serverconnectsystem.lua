ServerConnectSystem = System()

ServerConnectSystem.Initialize = function(self)
	self:SetName("ServerConnectSystem System");
	
	self:InitializeNetworkEvents();
	
	Net.Receive("Username", ServerConnectSystem.OnUsername);
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	print("ServerConnectSystem initialized!");
end

ServerConnectSystem.Update = function(self, dt)

	if GameRunning then
		return
	end

end

ServerConnectSystem.OnEntityAdded = function(self, entityId)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnEntityAdded");
	local match = false;
	local matchId = 0;
	
	local username 	= self:GetComponent(entityId, "Username", "Name"):GetString();
	local ipAddress = self:GetComponent(entityId, "NetConnection", "IpAddress"):GetString();
	local port 		= self:GetComponent(entityId, "NetConnection", "Port"):GetInt();

	local entities = self:GetEntities();
	for i = 1, #entities do
	
		if entityId ~= entities[i] then
			local uname = self:GetComponent(entities[i], "Username", "Name"):GetString();
			
			if username == uname then
				match = true;
				matchId = entities[i];
				break;
			end
			
			
		end
	
	end
	
	if match then
		
		local setActive = true;
		local isActive = self:GetComponent(matchId, "NetConnection", "Active"):GetBool();
		
		if isActive then
			local oldIp 	= self:GetComponent(matchId, "NetConnection", "IpAddress"):GetString();
			local oldPort	= self:GetComponent(matchId, "NetConnection", "Port"):GetInt();
			local reason	= "ServerConnectSystem kicked you.";
			
			Net.Kick(oldIp, oldPort, reason);
		end
		
		world:SetComponent(matchId, "NetConnection", "IpAddress", ipAddress);
		world:SetComponent(matchId, "NetConnection", "Port", port);
		world:SetComponent(matchId, "NetConnection", "Active", setActive);
		world:KillEntity(entityId);
		
	else
	
		local setActive = true;
		world:SetComponent(entityId, "NetConnection", "Active", setActive);

	end
	
	
	
end

ServerConnectSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnEntityRemoved");
end

ServerConnectSystem.OnPlayerTimedOut = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnPlayerTimedOut");
	ServerConnectSystem:RemovePlayer(_ip, _port);

end

ServerConnectSystem.OnPlayerDisconnected = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnPlayerDisconnected");
	ServerConnectSystem:RemovePlayer(_ip, _port);

end

ServerConnectSystem.OnUsername = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnUsername");
	
	local name = _ip .. _port;
	local active = false;
	
	local eId = world:CreateNewEntity("User");
	world:SetComponent(eId, "Username", "Name", name);
	world:SetComponent(eId, "NetConnection", "IpAddress", _ip);
	world:SetComponent(eId, "NetConnection", "Port", _port);
	world:SetComponent(eId, "NetConnection", "Active", active);
	
end



ServerConnectSystem.RemovePlayer = function(self, _ip, _port)

	if GameRunning then
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
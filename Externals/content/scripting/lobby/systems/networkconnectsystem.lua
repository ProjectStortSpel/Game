NetworkConnectSystem = System()

noConnections = 0;
maxConnections = 3;

NetworkConnectSystem.Update = function(self, dt)

	if Net.IsRunning() then 
	
		local text = "Server started. " .. tostring(noConnections) .. "/" .. tostring(maxConnections) .. " connected. Connected players are: ";	
		graphics:RenderSimpleText(text, 55, 5);
		
		local entities = self:GetEntities();
		for i = 1, #entities do
			local username 	= self:GetComponent(entities[i], "Username", "Name");
			graphics:RenderSimpleText("Player" .. tostring(i) .. ": " .. username:GetString(), 55, 6 + (i*2));
		end
	end
	
end

NetworkConnectSystem.OnUsername = function(id, ipAddress, port)
	
	local name = ipAddress .. port;
	local active = false;

	local eId = world:CreateNewEntity("User");
	world:SetComponent(eId, "Username", "Name", name);
	world:SetComponent(eId, "NetConnection", "IpAddress", ipAddress);
	world:SetComponent(eId, "NetConnection", "Port", port);
	world:SetComponent(eId, "NetConnection", "Active", active);

end

NetworkConnectSystem.Initialize = function(self)
	self:SetName("Reconnect System")
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory)
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory)
	
	Net.Receive("Username", NetworkConnectSystem.OnUsername);
	maxConnections = Net.MaxConnections();
	
	print("NetworkConnectSystem initialized!")
end

NetworkConnectSystem.OnEntityAdded = function(self, entityId)

	local match = false;
	local matchId = 0;
	
	local username 	= self:GetComponent(entityId, "Username", "Name");
	local ipAddress = self:GetComponent(entityId, "NetConnection", "IpAddress");
	local port 		= self:GetComponent(entityId, "NetConnection", "Port");
	
	
	local entites = self:GetEntities();
	for i = 1, #entites do
	
		if entityId == math.floor(entites[i]) then
		else
		
			local uname = self:GetComponent(entites[i], "Username", "Name");

			if username:GetString() == uname:GetString() then
				match = true;
				matchId = math.floor(entites[i]);
				break;
			end
			
		end
		
	end

	if match then
		Console.Print("Match!")
		
		local setActive = true;
		local isActive = self:GetComponent(matchId, "NetConnection", "Active");
		
		if isActive:GetBool() then
		
			local oldIp 	= self:GetComponent(matchId, "NetConnection", "IpAddress");
			local oldPort	= self:GetComponent(matchId, "NetConnection", "Port");
			local reason	= "NetworkConnectSystem kicked you.";
			
			Net.Kick(oldIp:GetString(), oldPort:GetInt(), reason);
			
		end
		
		world:SetComponent(matchId, "NetConnection", "IpAddress", ipAddress:GetString());
		world:SetComponent(matchId, "NetConnection", "Port", port:GetInt());
		world:SetComponent(matchId, "NetConnection", "Active", setActive);
		
		world:KillEntity(entityId);
	else
		local setActive = true;
		world:SetComponent(entityId, "NetConnection", "Active", setActive);
	end
	
	noConnections = noConnections + 1;
	
	
	
	
	if noConnections == maxConnections then
		local entities = self:GetEntities();
		for i = 1, #entities do
			local username 	= self:GetComponent(entities[i], "Username", "Name");
			--GraphicDevice.RenderSimpleText("Server is full. Connected players are: ", 10 5);
			Console.Print("Server is full. Connected players is:");
			Console.Print(username:GetString());
		end
	end
	
	
	
	
	
	
end
NetworkConnectSystem.OnEntityRemoved = function(self, entityId)
	noConnections = noConnections - 1;
end


NetworkConnectSystem.OnConnectedToServer = function(self, _ip, _port)
	local id = Net.StartPack("Username");
	Net.WriteString(id, "Username_Lua");
	Net.SendToServer(id);
end



NetworkConnectSystem.OnPlayerTimedOut = function(self, _ip, _port)
	
	local entities = self:GetEntities();
	for i = 1, #entities do
	
		local oIp 	= self:GetComponent(entities[i], "NetConnection", "IpAddress");
		local oPort = self:GetComponent(entities[i], "NetConnection", "Port");
		
		if _ip == oIp:GetString() and _port == oPort:GetInt() then
			local username 	= self:GetComponent(entities[i], "Username", "Name");
			Console.Print(username:GetString());
			world:KillEntity(entities[i]);
			break;
		end
	
	end
	
end

NetworkConnectSystem.OnPlayerDisconnected = function(self, _ip, _port)
		
	Console.Print("NetworkConnectSystem.OnPlayerDisconnected");
	
	local entities = self:GetEntities();
	for i = 1, #entities do
	
		local oIp 	= self:GetComponent(entities[i], "NetConnection", "IpAddress");
		local oPort = self:GetComponent(entities[i], "NetConnection", "Port");
		
		if _ip == oIp:GetString() and _port == oPort:GetInt() then
			local username 	= self:GetComponent(entities[i], "Username", "Name");
			Console.Print(username:GetString());
			world:KillEntity(entities[i]);
			break;
		end
	
	end
	
end
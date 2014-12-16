NetworkConnectSystem = System()

NetworkConnectSystem.Update = function(self, dt)

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
	
	print("NetworkConnectSystem initialized!")
end
NetworkConnectSystem.OnEntityAdded = function(self, entityId)

	local match = false;
	local matchId = 0;
	
	local username 	= self:GetComponent(entityId, "Username", "Name");
	local ipAddress = self:GetComponent(entityId, "NetConnection", "IpAddress");
	local port 		= self:GetComponent(entityId, "NetConnection", "Port");
	
	
	local entities = self:GetEntities();
	for i = 1, #entities do
	
		if entityId == math.floor(entities[i]) then
		else
		
			local uname = self:GetComponent(entities[i], "Username", "Name");

			if username:GetString() == uname:GetString() then
				match = true;
				matchId = math.floor(entities[i]);
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
	
		--world:KillEntity(entityId);
		--local reason	= "No new players allowed while a game is already active.";
		--Net.Kick(ipAddress:GetString(), port:GetInt(), reason);
		
		
	end
	
end
NetworkConnectSystem.OnEntityRemoved = function(self, entityId)

end


NetworkConnectSystem.OnConnectedToServer = function(self, _ip, _port)
	local id = Net.StartPack("Username");
	Net.WriteString(id, "Username_Lua");
	Net.SendToServer(id);
end



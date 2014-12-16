ReconnectSystem = System()

ReconnectSystem.Update = function(self, dt)

end

ReconnectSystem.OnUsername = function(id, ipAddress, port)
	Console.Print("ReconnectSystem.OnUsername");
	
	local name = "user" -- ipAddress .. port;
	local active = false;
	
	Console.Print("Name: " .. name);
	Console.Print("Id: " .. id);
	Console.Print("IpAddress: " .. ipAddress);
	Console.Print("Port: " .. tostring(port));
	
	local eId = world:CreateNewEntity("User");
	world:SetComponent(eId, "Username", "Name", name);
	world:SetComponent(eId, "NetConnection", "IpAddress", ipAddress);
	world:SetComponent(eId, "NetConnection", "Port", port);
	world:SetComponent(eId, "NetConnection", "Active", active);

end

ReconnectSystem.Initialize = function(self)
	self:SetName("Reconnect System")
	self:InitializeNetworkEvents()
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory)
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory)
	
	Net.Receive("Username", ReconnectSystem.OnUsername);
	
	print("ReconnectSystem initialized!")
	
end
ReconnectSystem.OnEntityAdded = function(self, entityId)
	Console.Print("ReconnectSystem.OnEntityAdded");

	local match = false;
	local matchId = 0;
	
	Console.Print("Entityid: " .. entityId);
	local username 	= self:GetComponent(entityId, "Username", "Name");
	local ipAddress = self:GetComponent(entityId, "NetConnection", "IpAddress");
	local port 		= self:GetComponent(entityId, "NetConnection", "Port");
	
	
	local entites = self:GetEntities();
	for i = 1, #entites do
	
		Console.Print("Entityid2: " .. entites[i]);
	
		if entityId == math.floor(entites[i]) then
			Console.Print("id1: " .. entityId);
			Console.Print("id2: " .. entites[i]);
		else
			local uname = self:GetComponent(entites[i], "Username", "Name");
			Console.Print("Uname: " .. uname:GetString());
			
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
			Console.Print("isActive");
			local oldIp 	= self:GetComponent(matchId, "NetConnection", "IpAddress");
			local oldPort	= self:GetComponent(matchId, "NetConnection", "Port");
			local reason	= "ReconnectSystem kicked you.";
			
			Server.Kick(oldIp:GetString(), oldPort:GetInt(), reason);
			
		end
		
		world:SetComponent(matchId, "NetConnection", "IpAddress", ipAddress:GetString());
		world:SetComponent(matchId, "NetConnection", "Port", port:GetInt());
		world:SetComponent(matchId, "NetConnection", "Active", setActive);
		
		world:KillEntity(entityId);
	else
		local setActive = true;
		world:SetComponent(entityId, "NetConnection", "Active", setActive);
	end
	
end
ReconnectSystem.OnEntityRemoved = function(self, entityId)

end
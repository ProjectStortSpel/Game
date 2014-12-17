ServerLobbySystem = System()

noConnections = 0;
maxConnections = 3;

ServerLobbySystem.Update = function(self, dt)

	if Net.IsRunning() then 
	
		local text = "Server started. " .. tostring(noConnections) .. "/" .. tostring(maxConnections) .. " connected. Connected players are: ";	
		graphics:RenderSimpleText(text, 55, 5);
		
		local entities = self:GetEntities();
		for i = 1, #entities do
			local username 	= self:GetComponent(entities[i], "Username", "Name");
			graphics:RenderSimpleText("Player" .. tostring(i) .. ": " .. username:GetString(), 55, 6 + (i*2));
		end
		
		if Input.GetKeyState(Key.Return) == InputState.Pressed then
			
			for i = 1, #entities do
			end
			
		end
		
	end
	
end

ServerLobbySystem.OnUsername = function(id, ipAddress, port)
	
	local name = ipAddress .. port;
	local active = false;

	local eId = world:CreateNewEntity("User");
	world:SetComponent(eId, "Username", "Name", name);
	world:SetComponent(eId, "NetConnection", "IpAddress", ipAddress);
	world:SetComponent(eId, "NetConnection", "Port", port);
	world:SetComponent(eId, "NetConnection", "Active", active);

end

ServerLobbySystem.OnStartGame = function(id, ipAddress, port)


end

ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System")
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory)
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory)
	
	Net.Receive("Username", ServerLobbySystem.OnUsername);
	Net.Receive("StartGame", ServerLobbySystem.OnStartGame);
	maxConnections = Net.MaxConnections();
	
	print("ServerLobbySystem initialized!")
end

ServerLobbySystem.OnEntityAdded = function(self, entityId)
end
ServerLobbySystem.OnEntityRemoved = function(self, entityId)
end
ServerGameSystem = System()

ServerGameSystem.Initialize = function(self)
	self:SetName("ServerGameSystem System");

	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	
	ServerGameSystem.m_maxConnections = Net.MaxConnections();
	
	print("ServerGameSystem initialized!");
end

ServerGameSystem.Update = function(self, dt)
	
	if GameRunning == false or Net.IsConnected() == true then
		return
	end
	
	if Net.IsRunning() then
		ServerGameSystem:UpdateServerOnline();
	end
	if Net.IsConnected() == false then
		graphics:RenderSimpleText("This is a game state!", 70, 4);
	end

end

ServerGameSystem.UpdateServerOnline = function(self)

	local text = "Server started. " .. tostring(ServerGameSystem.m_noConnections) .. "/" .. tostring(ServerGameSystem.m_maxConnections) .. " connected. Connected players are: ";	
	graphics:RenderSimpleText(text, 55, 5);
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local username = self:GetComponent(entities[i], "Username", "Name");
		graphics:RenderSimpleText("Player" .. tostring(i) .. ": " .. username:GetString(), 70, 6 + (i * 2));
	end
	
end

ServerGameSystem.OnEntityAdded = function(self, entityId)
	
	if GameRunning == false or Net.IsConnected() == true  then
		return
	end
	
end
ServerGameSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning == false or Net.IsConnected() == true then
		return
	end
	
end
ServerLobbySystem = System()


ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System");

	self:AddComponentTypeToFilter("GameRunning", FilterType.Mandatory);
	
	Console.AddCommand("Start", 
		function (command, ...)
			
			Console.Print("Game started");

			
			
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("GameRunning", id)
			world:CreateComponentAndAddTo("SyncNetwork", id)
			
			id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("CreateDeck", id)
			
			Console.Print("NewGame");

		end 
	)

	print("ServerLobbySystem initialized!");
end

ServerLobbySystem.Update = function(self, dt)
	
	if #self:GetEntities() > 0 then
		ServerLobbySystem:UpdateServerOnline();
	end
end

ServerLobbySystem.UpdateServerOnline = function(self)
	
	--if Input.GetKeyState(Key.Return) == InputState.Pressed and not Console.IsOpen() then
		--Console.Print("Game started");
		--GameRunning = true;
		--local id = Net.StartPack("NewGame");
		--Net.Broadcast(id);
		--Console.Print("NewGame");
	--end
end


LogStartSystem = System()


LogStartSystem.Initialize = function(self)
	self:SetName("LogStartSystem System");

	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf);
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf);
	

	print("LogStartSystem initialized!");
end

LogStartSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent(entity, "GameRunning") then
		
		File.Create("gamelog.txt")

		local units = self:GetEntities("Unit")

		local file = File.Append("gamelog.txt")

		for i = 1, #units do
			
			local plyNum = world:GetComponent(units[i], "PlayerNumber", "Number"):GetInt()
			local mapPosX, mapPosZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
			File.WriteLine(file, "Unit " .. plyNum .. " " .. mapPosX .. " " .. mapPosZ)

		end

		File.Close(file)

	end
end

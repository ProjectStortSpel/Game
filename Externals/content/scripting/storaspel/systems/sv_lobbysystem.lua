ServerLobbySystem = System()


ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System");

	self:AddComponentTypeToFilter("GameRunning", FilterType.Mandatory);
	
	Console.AddCommand("Start", 
		function (command, ...)
			
			Console.Print("Game started");

			File.Create("gamelog.txt")
			
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

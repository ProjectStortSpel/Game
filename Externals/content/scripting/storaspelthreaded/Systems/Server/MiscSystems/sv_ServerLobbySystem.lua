ServerLobbySystem = System()

ServerLobbySystem.Initialize = function(self)
	--	Set Name
	self:SetName("ServerLobbySystem")
	
	--	Toggle EntitiesAdded
	--self:UsingUpdate()
	
	--	Set Filter
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
end




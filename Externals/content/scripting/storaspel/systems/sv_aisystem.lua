AISystem = System()

AISystem.PostInitialize = function(self)
	
	
end

AISystem.Initialize = function(self)
	
	Console.AddCommand("AddAI", self.AddAI)
	
end

AISystem.AddAI = function(command, ...)
	
	local entity = world:CreateNewEntity("AI")
	
	local newName = "Player_" .. tostring(self.PlayerId)
		world:SetComponent(newPlayer, "PlayerName", "Name", newName);
	
		self.NumPlayers = self.NumPlayers + 1
		self.PlayerId = self.PlayerId + 1
		
		print("Player_: " .. newPlayer .. " connected")
	
	print("AI Added")
end
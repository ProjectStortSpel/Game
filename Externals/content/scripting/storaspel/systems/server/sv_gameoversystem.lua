GameOverSystem = System()
GameOverSystem.PlayerCount = 0

GameOverSystem.Initialize = function(self)
	self:SetName("GameOverSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)
end

GameOverSystem.Update = function(self, dt)
end

GameOverSystem.OnEntityAdded = function(self, entity)
	GameOverSystem.PlayerCount = GameOverSystem.PlayerCount + 1
end

GameOverSystem.OnEntityRemoved = function(self, entity)
	GameOverSystem.PlayerCount = GameOverSystem.PlayerCount - 1
	
	if GameOverSystem.PlayerCount <= 0 then
		print("Game is over, restart game")
		Console.AddToCommandQueue("reload")
	end
	
end

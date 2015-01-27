GameOverSystem = System()
GameOverSystem.PlayerCount = 0

GameOverSystem.Initialize = function(self)
	--	Set Name
	self:SetName("GameOverSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)
end

GameOverSystem.Update = function(self, dt)
end

GameOverSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		GameOverSystem.PlayerCount = GameOverSystem.PlayerCount + 1
	end
end

GameOverSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		GameOverSystem.PlayerCount = GameOverSystem.PlayerCount - 1
		
		if GameOverSystem.PlayerCount <= 0 then
			Console.AddToCommandQueue("reload")
		end
	end
end

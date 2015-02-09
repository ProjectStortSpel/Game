AddAISystem = System()

AddAISystem.Initialize = function(self)
	self:SetName("AI System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	
	Console.AddCommand("AddAI", self.AddAI)
	Console.AddCommand("AI", self.FillWithAIs)
end

AddAISystem.AddAI = function(_command, ...)
	
	local noToAdd = 1
	local args = { ... }
	
	if #args == 1 then
		if type(args[1]) == "number" then
			noToAdd = args[1]
		end
	end
	
	for i = 1, noToAdd do
		local newAI = world:CreateNewEntity("AI")
	end
end

AddAISystem.FillWithAIs = function(_command, ...)
	
	for i = 1, 10 do
		local newAI = world:CreateNewEntity("AI")
	end
end

AddAISystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
	for	i = 1, #entities do 
			
		if world:EntityHasComponent(entities[i], "AI") then
			
			local counterEntities = self:GetEntities("PlayerCounter")			
			local mapSpecsEntities = self:GetEntities("MapSpecs")
			local noOfPlayers = self:GetComponent(counterEntities[1], "PlayerCounter", "Players"):GetInt()
			local noOfSpawnpoints = self:GetComponent(mapSpecsEntities[1], "MapSpecs", "NoOfSpawnpoints"):GetInt()
			local availableSpawnsLeft = noOfSpawnpoints - noOfPlayers
			
			if availableSpawnsLeft > 0 then
			
				local newName = "Player_" .. tostring(noOfPlayers + 1)
				world:SetComponent(entities[i], "PlayerName", "Name", newName)
				world:SetComponent(entities[i], "PlayerNumber", "Number", noOfPlayers + 1)
				
				self:CounterComponentChanged(1, "Players")
				availableSpawnsLeft = availableSpawnsLeft - 1
				
				world:CreateComponentAndAddTo("NeedUnit", entities[i])
				
				print("AI Added")
			else
				world:KillEntity(entities[i])
				print("Could not add AI, no spawnpoints left")
			end
		end
	end
end

AddAISystem.CounterComponentChanged = function(self, _change, _component)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", _component)
	local number = counterComp:GetInt()
	number = number + _change
	world:SetComponent(counterEntities[1], "PlayerCounter", _component, number)
end
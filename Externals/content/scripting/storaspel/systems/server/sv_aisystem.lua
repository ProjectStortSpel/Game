AISystem = System()

AISystem.PostInitialize = function(self)
	
	
end

AISystem.Initialize = function(self)
	self:SetName("AI System")
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	
	Console.AddCommand("AddAI", self.AddAI)
	
end

AISystem.AddAI = function(_command, ...)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", 0)
	local maxPlayers, noOfPlayers = counterComp:GetInt2()
	local newAI = world:CreateNewEntity("AI")
	
	local newName = "Player_" .. tostring(noOfPlayers + 1)
	world:SetComponent(newAI, "PlayerName", "Name", newName);
	
	self:CounterComponentChanged(1, "Players")
	
	
	print("AI Added")
end

AISystem.CounterComponentChanged = function(self, _change, _component)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", _component)
	local number = counterComp:GetInt()
	number = number + _change
	world:SetComponent(counterEntities[1], "PlayerCounter", _component, number)
end
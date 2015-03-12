AddAISystem = System()

AddAISystem.Initialize = function(self)
	self:SetName("AI System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	
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

AddAISystem.EntitiesAdded = function(self, dt, entities)
	
	local ais = self:GetEntities("AI")
	local voids = self:GetEntities("Void")
	local nonWalkable = self:GetEntities("NotWalkable")
	
	for	i = 1, #ais do 
		
		--print(world:EntityHasComponent(ais[i], "UnitEntityId"))
		
		if world:EntityHasComponent(ais[i], "AI") and not world:EntityHasComponent(ais[i], "UnitEntityId") then
			
			local GameRunning = self:GetEntities("GameRunning")
			if #GameRunning > 0 then
				print("Trying to add AI when the game has already started.")
				world:KillEntity(ais[i])
				return
			end
			
			local counterEntities = self:GetEntities("PlayerCounter")			
			local mapSpecsEntities = self:GetEntities("MapSpecs")
			local noOfAIs = world:GetComponent(counterEntities[1], "PlayerCounter", "AIs"):GetInt(0)
			local noOfPlayers = world:GetComponent(counterEntities[1], "PlayerCounter", "Players"):GetInt(0)
			local noOfSpawnpoints = world:GetComponent(mapSpecsEntities[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
			local availableSpawnsLeft = noOfSpawnpoints - noOfPlayers
			
			if availableSpawnsLeft > 0 then
			
				local newName = "Player_" .. tostring(noOfPlayers + 1)
				local playerNumber = noOfPlayers + 1
				world:SetComponent(ais[i], "PlayerName", "Name", newName)
				world:SetComponent(ais[i], "PlayerNumber", "Number", playerNumber)
				
				self:CounterComponentChanged(1, "Players")
				self:CounterComponentChanged(1, "AIs")
				availableSpawnsLeft = availableSpawnsLeft - 1
				
				world:CreateComponentAndAddTo("NeedUnit", ais[i])
				
				local onTheSpotValue = 0.0
				local weight = 1
				local length = 2
				local power = 2
				
				local bookIndex = DynamicScripting.LoadRuleBook("content/dynamicscripting/map.txt")
				
				DynamicScripting.SetRuleBook( bookIndex );
				
				local fail = DynamicScripting.GenerateScript(playerNumber)
				
				--DynamicScripting.UpdateWeight(math.random())
				
				local found, weight, length, onTheSpotValue = DynamicScripting.GetWeightFrom("Void", playerNumber)
				self:PFstuff( found, voids, playerNumber, "Void", onTheSpotValue, weight, length, power )
				
				local found, weight, length, onTheSpotValue = DynamicScripting.GetWeightFrom("NotWalkable", playerNumber)
				self:PFstuff( found, nonWalkable, playerNumber, "NotWalkable", onTheSpotValue, weight, length, power )
				
				PotentialFieldHandler.SumPFs(playerNumber)
				
				io.write("AI ", i, " Added. Player Nr: ", playerNumber, "\n")
			else
				world:KillEntity(ais[i])
				print("Could not add AI, no spawnpoints left")
			end
		end
	end
end

AddAISystem.PFstuff = function(self, found, superstuff, playerNumber, object, onTheSpotValue, weight, length, power)
	if found then
		local param = PFParam()
		for j = 1, #superstuff do
			
			local x, y = world:GetComponent(superstuff[j], "MapPosition", 0):GetInt2(0)
			
			param:AddPosition(x, y)
		end
		
		PotentialFieldHandler.InitPF(param, playerNumber, object, onTheSpotValue, weight, length, power)
		
	end
end

AddAISystem.CounterComponentChanged = function(self, _change, _component)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", _component)
	local number = counterComp:GetInt(0)
	number = number + _change
	world:SetComponent(counterEntities[1], "PlayerCounter", _component, number)
end
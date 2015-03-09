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
	local nonWalkable = self:GetEntities("NonWalkable")
	
	for	i = 1, #ais do 
		
		--print(world:EntityHasComponent(ais[i], "UnitEntityId"))
		
		if world:EntityHasComponent(ais[i], "AI") and not world:EntityHasComponent(ais[i], "UnitEntityId") then
			
			local GameRunning = self:GetEntities("GameRunning")
			if #GameRunning > 0 then
				print("Trying to add AI when the game has already started.")
				world:KillEntity(ais[i])
				return
			end
			
			
			--print("nu blir det ai")
			local counterEntities = self:GetEntities("PlayerCounter")			
			local mapSpecsEntities = self:GetEntities("MapSpecs")
			local noOfPlayers = world:GetComponent(counterEntities[1], "PlayerCounter", "Players"):GetInt()
			local noOfSpawnpoints = world:GetComponent(mapSpecsEntities[1], "MapSpecs", "NoOfSpawnpoints"):GetInt()
			local availableSpawnsLeft = noOfSpawnpoints - noOfPlayers
			
			--print(noOfSpawnpoints, noOfPlayers, availableSpawnsLeft)
			
			if availableSpawnsLeft > 0 then
			
				local newName = "Player_" .. tostring(noOfPlayers + 1)
				local playerNumber = noOfPlayers + 1
				world:SetComponent(ais[i], "PlayerName", "Name", newName)
				world:SetComponent(ais[i], "PlayerNumber", "Number", playerNumber)
				
				self:CounterComponentChanged(1, "Players")
				availableSpawnsLeft = availableSpawnsLeft - 1
				
				world:CreateComponentAndAddTo("NeedUnit", ais[i])
				
				local param = PFParam()
				local object = "Void"
				local onTheSpotValue = 0.0
				local weight = 1
				local length = 2
				local power = 2
				
				local bookIndex = DynamicScripting.LoadRuleBook("content/dynamicscripting/map.txt")
				
				DynamicScripting.SetRuleBook( bookIndex );
				
				local fail = DynamicScripting.GenerateScript(playerNumber)
				
				local found, weight = DynamicScripting.GetWeightFrom("Void", playerNumber)
				
				--DynamicScripting.UpdateWeight(math.random())
				if found then
					for j = 1, #voids do
						
						local x, y = world:GetComponent(voids[j], "MapPosition", 0):GetInt2(0)
						
						param:AddPosition(x, y)
					end
					
					PotentialFieldHandler.InitPF(param, playerNumber, object, onTheSpotValue, weight, length, power)
					
				end
				
				found, weight = DynamicScripting.GetWeightFrom("NonWalkable", playerNumber)
				
				if found then
					for j = 1, #nonWalkable do
						
						local x, y = world:GetComponent(nonWalkable[j], "MapPosition", 0):GetInt2(0)
						
						param:AddPosition(x, y)
					end
					
					PotentialFieldHandler.InitPF(param, playerNumber, object, onTheSpotValue, weight, length, power)
					
					
				end
				
				-- Sum all the pfs.
				PotentialFieldHandler.SumPFs(playerNumber)
				
				print("AI Added", i)
			else
				world:KillEntity(ais[i])
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
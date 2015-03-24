AddAISystem = System()
AddAISystem.Names = {}
AddAISystem.Names.__mode = "k"
AddAISystem.AutoStart = 0

AddAISystem.Initialize = function(self)
	self:SetName("AI System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("HostSettings", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Spawn", FilterType.RequiresOneOf)
	
	self:AddNames()
	
	Console.AddCommand("AddAI", self.AddAI)
	Console.AddCommand("AI", self.FillWithAIs)
end


AddAISystem.AddNames = function(self)

	self.Names[#self.Names+1] = "Albert"
	self.Names[#self.Names+1] = "Allen"
	self.Names[#self.Names+1] = "Bert"
	self.Names[#self.Names+1] = "Bob"
	self.Names[#self.Names+1] = "Cecil"
	self.Names[#self.Names+1] = "Clarence"
	self.Names[#self.Names+1] = "Elliot"
	self.Names[#self.Names+1] = "Elmer"
	self.Names[#self.Names+1] = "Ernie"
	self.Names[#self.Names+1] = "Eugene"
	self.Names[#self.Names+1] = "Fergus"
	self.Names[#self.Names+1] = "Ferris"
	self.Names[#self.Names+1] = "Frasier"
	self.Names[#self.Names+1] = "Fred"
	self.Names[#self.Names+1] = "George"
	self.Names[#self.Names+1] = "Graham"
	self.Names[#self.Names+1] = "Harvey"
	self.Names[#self.Names+1] = "Irwin"
	self.Names[#self.Names+1] = "Lester"
	self.Names[#self.Names+1] = "Marvin"
	self.Names[#self.Names+1] = "Neil"
	self.Names[#self.Names+1] = "Niles"
	self.Names[#self.Names+1] = "Oliver"
	self.Names[#self.Names+1] = "Opie"
	self.Names[#self.Names+1] = "Toby"
	self.Names[#self.Names+1] = "Ulric"
	self.Names[#self.Names+1] = "Ulysses"
	self.Names[#self.Names+1] = "Uri"
	self.Names[#self.Names+1] = "Waldo"
	self.Names[#self.Names+1] = "Wally"
	self.Names[#self.Names+1] = "Niklas"
	self.Names[#self.Names+1] = "Carl"
	self.Names[#self.Names+1] = "Johannes"
	self.Names[#self.Names+1] = "Erik"
	self.Names[#self.Names+1] = "Anders"
	self.Names[#self.Names+1] = "Marcus"
	self.Names[#self.Names+1] = "Christian"
	self.Names[#self.Names+1] = "Pontus"

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
		--local newAI = world:CreateNewEntity("AI")
		world:CreateNewEntity("AI")
	end
end

AddAISystem.FillWithAIs = function(_command, ...)
	
	for i = 1, 10 do
		--local newAI = world:CreateNewEntity("AI")
		world:CreateNewEntity("AI")
	end
end

AddAISystem.EntitiesAdded = function(self, dt, entities)
	
	--local ais = self:GetEntities("AI")
	local voids = self:GetEntities("Void")
	local nonWalkable = self:GetEntities("NotWalkable")
	local riverEnd = self:GetEntities("RiverEnd")
	local autoStarted = false
	
	for	i = 1, #entities do 
		
	--print("new entity, does it have UnitEntityId? ", world:EntityHasComponent(entities[i], "UnitEntityId"))
		if world:EntityHasComponent(entities[i], "HostSettings") then
			
		--print("host settings")
			self.AutoStart = world:GetComponent(entities[i], "HostSettings", "AutoStart"):GetInt()
		--print("host settings done")
			
		elseif world:EntityHasComponent(entities[i], "Spawn") and self.AutoStart == 1 then
			
		--print("do the auto start")
			Console.AddToCommandQueue("start")
			--Console.AddToCommandQueue("timescale 3")
			self.AutoStart = 0
		--print("auto start done")
			
		elseif world:EntityHasComponent(entities[i], "AI") and not world:EntityHasComponent(entities[i], "UnitEntityId") then
			
		--print("an ai without unit")
			local GameRunning = self:GetEntities("GameRunning")
			if #GameRunning > 0 then
				print("Trying to add AI when the game has already started.")
				world:KillEntity(entities[i])
				return
			end
			
		--print("game is not yet running")
			
			local counterEntities = self:GetEntities("PlayerCounter")			
			local mapSpecsEntities = self:GetEntities("MapSpecs")
			local noOfAIs = world:GetComponent(counterEntities[1], "PlayerCounter", "AIs"):GetInt(0)
			local noOfPlayers = world:GetComponent(counterEntities[1], "PlayerCounter", "Players"):GetInt(0)
			local noOfSpawnpoints = world:GetComponent(mapSpecsEntities[1], "MapSpecs", "NoOfSpawnpoints"):GetInt(0)
			local availableSpawnsLeft = noOfSpawnpoints - noOfPlayers
			
		--print("variables init")
			
			if availableSpawnsLeft > 0 then
				
			--print(availableSpawnsLeft, " spawnpoints left")
				local rng = math.random(1, #self.Names)
				local newName = "BOT " .. self.Names[rng]
				table.remove(self.Names, rng)
			--print("bot name added")

				local playerNumber = noOfPlayers + 1
				world:GetComponent(entities[i], "PlayerName", "Name"):SetString(newName)
				world:SetComponent(entities[i], "PlayerNumber", "Number", playerNumber)
				
				self:CounterComponentChanged(1, "Players")
				self:CounterComponentChanged(1, "AIs")
				availableSpawnsLeft = availableSpawnsLeft - 1
				
			--print("done with counter")
				world:CreateComponentAndAddTo("NeedUnit", entities[i])
				
				local onTheSpotValue = 0.0
				local weight = 1
				local length = 2
				local power = 2
				
			--print("added need unit")
				local bookIndex = DynamicScripting.LoadRuleBook("content/dynamicscripting/map.txt")
				DynamicScripting.SetRuleBook( bookIndex )
				
			--print("loaded rulebook")
				local fail = DynamicScripting.GenerateScript(playerNumber)
				--DynamicScripting.UpdateWeight(math.random())
			--print("generated script")
				
				local found, onTheSpotValue, weight, length, power = DynamicScripting.GetWeightFrom("Void", playerNumber)
			--print("got weight from void", found, onTheSpotValue, weight, length, power)
				self:PFstuff( found, voids, playerNumber, "Void", onTheSpotValue, weight, length, power )
			--print("inited pf void")
				
				local found, onTheSpotValue, weight, length, power = DynamicScripting.GetWeightFrom("NotWalkable", playerNumber)
			--print("got weight from not walkable", found, onTheSpotValue, weight, length, power)
				self:PFstuff( found, nonWalkable, playerNumber, "NotWalkable", onTheSpotValue, weight, length, power )
			--print("inited pf not walkable")
				
				local found, onTheSpotValue, weight, length, power = DynamicScripting.GetWeightFrom("RiverEnd", playerNumber)
			--print("got weight from riverEnd", found, onTheSpotValue, weight, length, power)
				self:PFstuff( found, riverEnd, playerNumber, "RiverEnd", onTheSpotValue, weight, length, power )
			--print("inited pf riverEnd")
				
				PotentialFieldHandler.SumPFs(playerNumber)
				
				io.write("AI ", i, " Added. Player Nr: ", playerNumber, "\n")
			else
				world:KillEntity(entities[i])
				print("Could not add AI, no spawnpoints left")
			end
		end
	end
end

AddAISystem.PFstuff = function(self, found, superstuff, playerNumber, object, onTheSpotValue, weight, length, power)
	if found then
		--print("found")
		local param = PFParam()
		--print("param created")
		for j = 1, #superstuff do
			
			--print("looping", j)
			--print("Entity: ", superstuff[j])
			--print("Entity has component: ", world:EntityHasComponent(superstuff[j], "MapPosition"))
			local mapPositionComponent = world:GetComponent(superstuff[j], "MapPosition", 0)
			--print("Getting int2")
			local x, y = mapPositionComponent:GetInt2()
			
			--print("got component", j)
			param:AddPosition(x, y)
		end
		
		--print("time to init pf")
		PotentialFieldHandler.InitPF(param, playerNumber, object, onTheSpotValue, weight, length, power)
		--print("inited pf")
		
	end
end

AddAISystem.CounterComponentChanged = function(self, _change, _component)
	
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", _component)
	local number = counterComp:GetInt(0)
	number = number + _change
	world:SetComponent(counterEntities[1], "PlayerCounter", _component, number)
end
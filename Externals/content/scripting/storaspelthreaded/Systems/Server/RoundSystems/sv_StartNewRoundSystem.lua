StartNewRoundSystem = System()
StartNewRoundSystem.CurrentRound = 0

StartNewRoundSystem.Initialize = function(self)
	--	Set Name
	self:SetName("StartNewRoundSystem")
	
	--	Toggle EntitiesAdded and Update
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("UnitSelectedCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotifyStartNewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
end

StartNewRoundSystem.EntitiesAdded = function(self, dt, entities)
	
	local onlyOnce = true
	local counterEntities = self:GetEntities("PlayerCounter")
	local counterComp = world:GetComponent(counterEntities[1], "PlayerCounter", 0)
	local noOfAIs = counterComp:GetInt(0)
	local noOfPlayers = counterComp:GetInt(1)
	
	local onlyAIs = false
	
	if noOfAIs == noOfPlayers then
		onlyAIs = true
	end
	
	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "NotifyStartNewRound") then
			--local numReadyUnits = #self:GetEntities("UnitSelectedCards")
			local readyUnits = self:GetEntities("UnitSelectedCards")
			local readyPlayerUnits = {}
			readyPlayerUnits.__mode = "k"
			local numReadyPlayerUnits = 0
			
			for unit = 1, #readyUnits do
				
				local playerEntity = world:GetComponent(readyUnits[unit], "PlayerEntityId", 0):GetInt(0)
				local isAI = world:EntityHasComponent(playerEntity, "AI", 0)
				
				if not isAI then
					--readyPlayerUnits[#readyPlayerUnits + 1] = readyUnits[unit]
					numReadyPlayerUnits = numReadyPlayerUnits + 1
				end
			end
			
			--local numReadyPlayerUnits = #readyPlayerUnits
			
			local numReadyUnits = 0
			
			if onlyAIs then
				--numReadyPlayerUnits = noOfPlayers
				numReadyUnits = noOfPlayers
			else
				--numReadyUnits = numReadyUnits + noOfAIs
				--numReadyPlayerUnits = numReadyPlayerUnits + noOfAIs
				numReadyUnits = numReadyPlayerUnits + noOfAIs
			end
			
			local units = self:GetEntities("Unit")
			
			--print("NumReadyUnits: " .. numReadyUnits)
			--print("NoOfPlayers: " .. noOfPlayers)
			
			--if noOfPlayers <= numReadyPlayerUnits and onlyOnce then
			if noOfPlayers <= numReadyUnits and onlyOnce then
			
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("NewRound", id)

				--for i = 1, #units do
				--	if world:EntityHasComponent(units[i], "UnitSelectedCards") then
				--		world:RemoveComponentFrom("UnitSelectedCards", units[i])
				--	end
				--end
				
				for i = 1, #readyUnits do
					world:RemoveComponentFrom("UnitSelectedCards", readyUnits[i])
				end
				
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("SetPickingPhaseTimer", newId)
				world:GetComponent(newId, "SetPickingPhaseTimer", "Amount"):SetFloat(0)
				
				onlyOnce = false
				--break	-- To make sure this is run only once.
				
				self.CurrentRound = self.CurrentRound + 1
				io.write("\n-------------------------- Round ", self.CurrentRound, " -------------------------\n")
				
			elseif not world:GetComponent(entity, "NotifyStartNewRound", "IsAI"):GetBool(0) then
			
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("AddToPickingPhaseTimer", newId)
				world:GetComponent(newId, "AddToPickingPhaseTimer", "Amount"):SetFloat(-5)
			end

			world:KillEntity(entity)
		end
	end
end

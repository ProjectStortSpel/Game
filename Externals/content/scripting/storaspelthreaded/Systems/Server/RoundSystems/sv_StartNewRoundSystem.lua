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
end

StartNewRoundSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	local onlyOnce = true
	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "NotifyStartNewRound") then
			local numReadyUnits = #self:GetEntities("UnitSelectedCards")
			local units = self:GetEntities("Unit")
	
			--print("NumReadyUnits: " .. numReadyUnits)
			--print("NumUnits: " .. #units)

			if numReadyUnits == #units and onlyOnce then
			
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("NewRound", id)

				for i = 1, #units do
					world:RemoveComponentFrom("UnitSelectedCards", units[i])
				end
				
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("SetPickingPhaseTimer", newId)
				world:GetComponent(newId, "SetPickingPhaseTimer", "Amount"):SetFloat(0)
				
				onlyOnce = false
				--break	-- To make sure this is run only once.
				
				self.CurrentRound = self.CurrentRound + 1
				--print("------------- Round --------------")
				--print()
				io.write("------------- Round ", self.CurrentRound, " --------------\n\n")
				
			elseif not world:GetComponent(entity, "NotifyStartNewRound", "IsAI"):GetBool(0) then
			
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("AddToPickingPhaseTimer", newId)
				world:GetComponent(newId, "AddToPickingPhaseTimer", "Amount"):SetFloat(-5)
			end

			world:KillEntity(entity)
		end
	end
end

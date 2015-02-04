StartNewRoundSystem = System()

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

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "NotifyStartNewRound") then
			local numReadyUnits = #self:GetEntities("UnitSelectedCards")
			local units = self:GetEntities("Unit")
	
			print("NumReadyUnits: " .. numReadyUnits)
			print("NumUnits: " .. #units)

			if numReadyUnits == #units then
			
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("NewRound", id)

				for i = 1, #units do
					world:RemoveComponentFrom("UnitSelectedCards", units[i])
				end
				print("StartNewRound Set says hi!")
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("SetPickingPhaseTimer", newId)
				world:GetComponent(newId, "SetPickingPhaseTimer", "Amount"):SetFloat(0)
				world:KillEntity(entity)
				break	-- To make sure this is run only once.
				
			elseif not world:EntityHasComponent(entity, "IsAI") then
				print("StartNewRound says hi!")
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("AddToPickingPhaseTimer", newId)
				world:GetComponent(newId, "AddToPickingPhaseTimer", "Amount"):SetFloat(-5)
			end

			world:KillEntity(entity)
		end
	end
end

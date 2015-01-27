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
			end

			world:KillEntity(entity)

		end
	end
end

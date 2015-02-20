TakeCardStepsFromUnitSystem = System()

TakeCardStepsFromUnitSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("TakeCardStepsFromUnitSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("TakeCardStepsFromUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)
end

TakeCardStepsFromUnitSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "TakeCardStepsFromUnit") then 
		
			local unit = world:GetComponent(entityId, "TakeCardStepsFromUnit", "Unit"):GetInt()
			local player = world:GetComponent(unit, "PlayerEntityId", "Id"):GetInt(0)
			local cardsteps = self:GetEntities("CardStep")
			
			local ai = world:EntityHasComponent(player, "AI")
			
			for i = 1, #cardsteps do
				local owner = world:GetComponent(cardsteps[i], "CardStep", "UnitEntityId"):GetInt()
				
				if owner == unit then
					world:RemoveComponentFrom("CardStep", cardsteps[i])
				end
				
				if ai then
					if world:EntityHasComponent(cardsteps[i], "AICard") then
						world:RemoveComponentFrom("AICard", cardsteps[i])
					end
				end
			end
		end
	end
end

TakeCardStepsFromUnitSystem = System()

TakeCardStepsFromUnitSystem.Initialize = function ( self )
	self:SetName("TakeCardStepsFromUnitSystem")
	self:AddComponentTypeToFilter("TakeCardStepsFromUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)

	print("TakeCardStepsFromUnitSystem Initialized")
end

TakeCardStepsFromUnitSystem.OnEntityAdded = function(self, entityId)
		
if world:EntityHasComponent( entityId, "TakeCardStepsFromUnit") then 
	
		print("TakeCardStepsFromUnitSystem")
	
		local unit = world:GetComponent(entityId, "TakeCardStepsFromUnit", "Unit"):GetInt()
		local cardsteps = self:GetEntities("CardStep")
		
		for i = 1, #cardsteps do
		
			local owner = world:GetComponent(cardsteps[i], "CardStep", "UnitEntityId"):GetInt()
			
			if owner == unit then
			
				world:RemoveComponentFrom("CardStep", cardsteps[i])
		
			end
		
		end
		
	end
	
end

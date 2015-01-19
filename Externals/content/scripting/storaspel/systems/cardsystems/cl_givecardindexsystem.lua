GiveCardIndexSystem = System()

GiveCardIndexSystem.Initialize = function ( self )
	self:SetName("GiveCardIndexSystem")
	self:AddComponentTypeToFilter("Card", FilterType.Mandatory)

	print("GiveCardIndexSystem Initialized")
end


GiveCardIndexSystem.OnEntityAdded = function( self, entityId )

	local cards = self:GetEntities()

	local indicies = {}
	for i = 1, #cards do
		
		if world:EntityHasComponent(cards[i], "CardIndex") then
			
			local index = world:GetComponent(cards[i], "CardIndex", "Index"):GetInt()
			indicies[index] = true

		end

	end

	local index = 1

	while indicies[index] do
		index = index + 1
	end

	world:CreateComponentAndAddTo("CardIndex", entityId)
	world:SetComponent(entityId, "CardIndex", "Index", index)

end

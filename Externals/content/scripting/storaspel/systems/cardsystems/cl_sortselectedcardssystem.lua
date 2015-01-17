SortSelectedCardSystem = System()

SortSelectedCardSystem.Initialize = function ( self )
	self:SetName("SortSelectedCardSystem")
	self:AddComponentTypeToFilter("SelectCard", FilterType.Mandatory)

	print("SortSelectedCardSystem Initialized")
end


SortSelectedCardSystem.OnEntityAdded = function( self, entityId )

	local cards = self:GetEntities()
	if #cards <= 5 then
		local index = 1
		for i = 1, #cards do
			
			if world:EntityHasComponent(cards[i], "CardSelected") then
				index = index + 1
			end

		end

		world:SetComponent(entityId, "SelectCard", "Index", index)
		world:CreateComponentAndAddTo("CardSelected", entityId)
	else
		world:RemoveComponentFrom("SelectCard", entityId)
	end
end


SortSelectedCardSystem.OnEntityRemoved = function( self, entityId )

	if world:EntityHasComponent(entityId, "CardSelected") then
		
		local index = world:GetComponent(entityId, "SelectCard", "Index"):GetInt()

		local cards = self:GetEntities()

		for i = 1, #cards do
			local index2 = world:GetComponent(cards[i], "SelectCard", "Index"):GetInt()

			if index2 > index then
				world:SetComponent(cards[i], "SelectCard", "Index", index2 - 1)
			end

		end
		
		world:RemoveComponentFrom("CardSelected", entityId)
	end
end
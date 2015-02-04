CardSelectSystem = System()

CardSelectSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CardSelectSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
end

CardSelectSystem.Update = function(self, dt, taskIndex, taskCount)

	if Input.GetTouchState(0) == InputState.Released then
		local entities = self:GetEntities()
		for i = 1, #entities do
			if world:EntityHasComponent(entities[i], "SelectCard") then
				world:RemoveComponentFrom("SelectCard", entities[i])
			else
				self:SelectCard(entities[i], 99)
			end
		end
	end

end

CardSelectSystem.SelectCard = function(self, card, index)
	world:CreateComponentAndAddTo("SelectCard", card)
	world:SetComponent(card, "SelectCard", "Index", index)
end
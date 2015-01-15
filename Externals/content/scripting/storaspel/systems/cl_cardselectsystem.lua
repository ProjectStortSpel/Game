CardSelectSystem = System()

CardSelectSystem.Update = function(self, dt)

	if Input.GetMouseButtonState(MouseButton.LeftButton) == InputState.Released then
	
		-- TODO gör så att de bara går att selecta i selection phase
		local entities = self:GetEntities()
		for i = 1, #entities do

			if world:EntityHasComponent(entities[i], "SelectCard") then
				world:RemoveComponentFrom("SelectCard", entities[i])
				world:RemoveComponentFrom("CardSelected", entities[i])
			else
				world:CreateComponentAndAddTo("SelectCard", entities[i])	
			end

		end
	
	end
		
end

CardSelectSystem.Initialize = function(self)
	self:SetName("Card select System")
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("Card select System initialized!")
end
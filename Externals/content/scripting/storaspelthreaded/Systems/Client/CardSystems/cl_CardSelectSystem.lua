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
	
	print("Card select System initialized!")
end

CardSelectSystem.Update = function(self, dt, taskIndex, taskCount)

	if Input.GetTouchState(0) == InputState.Released then
	
		-- TODO gör så att de bara går att selecta i selection phase
		local entities = self:GetEntities()
		for i = 1, #entities do

			if world:EntityHasComponent(entities[i], "SelectCard") then
				world:RemoveComponentFrom("SelectCard", entities[i])
			else
				world:CreateComponentAndAddTo("SelectCard", entities[i])	
			end

		end
	
	end
		
end
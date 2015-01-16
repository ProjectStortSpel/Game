ButtonPressedSystem = System()

ButtonPressedSystem.Update = function(self, dt)

	if Input.GetTouchState() == InputState.Released then
	
		-- TODO gör så att de bara går att selecta i selection phase
		local entities = self:GetEntities()
		if #entities > 0 then

			-- Kör entities[1]
			
		end
	
	end
		
end

ButtonPressedSystem.Initialize = function(self)
	self:SetName("ButtonPressedSystem")
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("ButtonPressedSystem initialized!")
end
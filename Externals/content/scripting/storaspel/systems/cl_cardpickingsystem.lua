CardHoverSystem = System()
CardHoverSystem.Scale = 0.6
CardHoverSystem.UpOffset = -0.2

CardHoverSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		local index = world:GetComponent(entities[i], "CardIndex", "Index"):GetInt()
		
		local scale = self:GetComponent(entity, "Scale", 0)
		
		local position = self:GetComponent(entity, "Position", 0)
		
		X, Y, Z = position:GetFloat3()
		
		mX, mY = Input.GetMousePosition()

		wX, wY = graphics:GetWindowSize()
		
		pX = 0.25*X*wX+wX/2
		pY = wY-0.25*Y*wY-wY/2
		print(pY)
		if mX > pX-50 and mX < pX+50 then
			if mY > pY-100 and mY < pY+100 then

				local scale = self:GetComponent(entity, "Scale", 0)
				scale:SetFloat3(self.Scale, self.Scale, self.Scale)
				
			end
		end
	end
end

CardHoverSystem.Initialize = function(self)
	self:SetName("Card hover System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
	
	print("Card hover System initialized!")
end
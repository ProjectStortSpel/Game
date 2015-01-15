SelectCards = false

SelectCardSystem = System()
SelectCardSystem.SelectCardSystem = false

SelectCardSystem.Initialize = function ( self )
	self:SetName("SelectCardSystem")
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)

	print("SelectCardSystem Initialized")
end

SelectCardSystem.Update = function( self, dt )

	if SelectCards then
		if Input.GetKeyState(Key._1) == InputState.Pressed then
			self:ToggleSelectCard(1)
		elseif Input.GetKeyState(Key._2) == InputState.Pressed then
			self:ToggleSelectCard(2)
		elseif Input.GetKeyState(Key._3) == InputState.Pressed then
			self:ToggleSelectCard(3)
		elseif Input.GetKeyState(Key._4) == InputState.Pressed then
			self:ToggleSelectCard(4)
		elseif Input.GetKeyState(Key._5) == InputState.Pressed then
			self:ToggleSelectCard(5)
		elseif Input.GetKeyState(Key._6) == InputState.Pressed then
			self:ToggleSelectCard(6)
		elseif Input.GetKeyState(Key._7) == InputState.Pressed then
			self:ToggleSelectCard(7)
		elseif Input.GetKeyState(Key._8) == InputState.Pressed then
			self:ToggleSelectCard(8)
		elseif Input.GetMouseButtonState(MouseButton.RightButton) == InputState.Released then
		
			local entities = self:GetEntities()
			for i = 1, #entities do
				local entity = entities[i]
			
				local index = world:GetComponent(entity, "CardIndex", "Index"):GetInt()
			
				local scale = self:GetComponent(entity, "Scale", 0)
			
				local position = self:GetComponent(entity, "Position", 0)
			
				X, Y, Z = position:GetFloat3()
			
				mX, mY = Input.GetMousePosition()

				wX, wY = graphics:GetWindowSize()
			
				pX = 0.25*X*wX+wX/2
				pY = wY-0.445*Y*wY-wY/2
				if mX > pX-50 and mX < pX+50 then
					if mY > pY-100 and mY < pY+100 then

						self:ToggleSelectCard(index)
						
					end
				end
			end
		
		end
	end

end


SelectCardSystem.ToggleSelectCard = function( self, index )
	
	local cards = self:GetEntities()

	for i = 1, #cards do
		local index2 = world:GetComponent(cards[i], "CardIndex", "Index"):GetInt()

		if index2 == index then

			if world:EntityHasComponent(cards[i], "SelectCard") then

				world:RemoveComponentFrom("SelectCard", cards[i])
				world:RemoveComponentFrom("CardSelected", cards[i])

			else
				
				world:CreateComponentAndAddTo("SelectCard", cards[i])
				
			end

			
		end

	end
end

SelectCardSystem.SendSelectedCards = function( self )
	
	local id = Net.StartPack("Server.SelectCards")
	
	
	local cards = self:GetEntities()

	local numselected = 0
	for i = 1, 8 do
		if self.SelectedCards[i] then
			numselected = numselected + 1
			Net.WriteInt(id, Net.ToServerID(cards[i]))

			if numselected == 5 then 
				break
			end
		end
	end

	Net.SendToServer(id)
	SelectCards = false
end

Net.Receive("Client.SelectCards", 
	function( id, ip, port )
	
		SelectCards = true

	end
)
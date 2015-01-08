SelectCards = false

SelectCardSystem = System()
SelectCardSystem.SelectCardSystem = false
SelectCardSystem.SelectedCards = {}

SelectCardSystem.Initialize = function ( self )
	self:SetName("SelectCardSystem")
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)

	for i = 1, 8 do
		table.insert(self.SelectedCards, false)
	end

	print("SelectCardSystem Initialized")
end

SelectCardSystem.Update = function( self, dt )

	if SelectCards then
		if Input.GetKeyState(Key._1) == InputState.Pressed then
			self.SelectedCards[1] = true
		elseif Input.GetKeyState(Key._2) == InputState.Pressed then
			self.SelectedCards[2] = true
		elseif Input.GetKeyState(Key._3) == InputState.Pressed then
			self.SelectedCards[3] = true
		elseif Input.GetKeyState(Key._4) == InputState.Pressed then
			self.SelectedCards[4] = true
		elseif Input.GetKeyState(Key._5) == InputState.Pressed then
			self.SelectedCards[5] = true
		elseif Input.GetKeyState(Key._6) == InputState.Pressed then
			self.SelectedCards[6] = true
		elseif Input.GetKeyState(Key._7) == InputState.Pressed then
			self.SelectedCards[7] = true
		elseif Input.GetKeyState(Key._8) == InputState.Pressed then
			self.SelectedCards[8] = true
		end

		local numselected = 0
		for i = 1, 8 do
			if self.SelectedCards[i] then
				numselected = numselected + 1
			end
		end

		if numselected >= 5 then
			self:SendSelectedCards()
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

Net.Receive("Client.SelectCards", function( id, ip, port )
	
	SelectCards = true

end)
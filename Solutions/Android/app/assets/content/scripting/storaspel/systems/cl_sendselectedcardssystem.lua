SelectCards = false

SendSelectCardSystem = System()
SendSelectCardSystem.SelectCardSystem = false

SendSelectCardSystem.Initialize = function ( self )
	self:SetName("SendSelectCardSystem")
	self:AddComponentTypeToFilter("SelectCard", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardSelected", FilterType.Mandatory)

	print("SendSelectCardSystem Initialized")
end

SendSelectCardSystem.OnEntityAdded = function( self, entityId )

	local cards = self:GetEntities()

	print("numselected: " .. #cards)

	if #cards >= 5 then
		self:SendSelectedCards()
		self:DeselectAll()
	end

end

SendSelectCardSystem.SendSelectedCards = function( self )
	
	local id = Net.StartPack("Server.SelectCards")
	
	
	local cards = self:GetEntities()

	for i = 1, 5 do	
		
		for j = 1, #cards do
			
			local index = world:GetComponent(cards[i], "SelectCard", "Index"):GetInt()

			if index == i then			
				Net.WriteInt(id, Net.ToServerID(cards[i]))
				break
			end

		end

			
	end

	Net.SendToServer(id)
	SelectCards = false
end

SendSelectCardSystem.DeselectAll = function( self )
	
	local cards = self:GetEntities()

	for i = 1, #cards do
		
		world:RemoveComponentFrom("CardSelected", cards[i])

	end

end
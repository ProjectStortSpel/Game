AiCardPickingSystem = System()

AiCardPickingSystem.Initialize = function(self)
	self:SetName("AI card picking System")
	
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AICard", FilterType.RequiresOneOf)
	
end

AiCardPickingSystem.Update = function(self, dt)
	
	local AIs = self:GetEntities("AI")
	local Cards = self:GetEntities("AICard")
	
	for i = 1, #AIs do
	
		local aisCard = {}
		
		for j = 1, #Cards do
			local card = self:GetComponent(Cards[j], "DealtCard", 0)
			local id = card:GetInt()
			local plyNr = self:GetComponent(id, "PlayerNumber", 0):GetInt()
			local aiNr = self:GetComponent(AIs[i], "PlayerNumber", 0):GetInt()
			
			if plyNr == aiNr then
				
				aisCard[#aisCard+1] = Cards[j]
			end
		end
		local pickedcards = {}
		for i = 1, 5 do
			
			if #aisCard > 1 then
				
				local cardNr = math.random(1, #aisCard)
				
				local pickedcard = aisCard[cardNr]
				
				pickedcards[#pickedcards + 1] = pickedcard
				
				table.remove(aisCard, cardNr)
			end
		end
		
		for i = 1, #aisCard do
			local cardName = self:GetComponent(pickedcards[i], "CardAction", 0):GetString()
			--print(cardName)
		end
		
	end
end

AiCardPickingSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent(entity, "DealtCard") then
		local playerid = self:GetComponent(entity, "DealtCard", 0)
		local id = playerid:GetInt()
		local plynum = self:GetComponent(id, "PlayerNumber", 0):GetInt()
		local card = self:GetComponent(entity, "CardAction", 0):GetString()
		--print ( plynum .. " gets a " .. card .. " Card" )
	else
		--print( "Ai added")
	end

end

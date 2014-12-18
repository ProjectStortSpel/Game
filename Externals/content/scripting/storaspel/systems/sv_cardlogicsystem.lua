
CardDeckSystem = System()

CardDeckSystem.NR_OF_CARDS_PER_PLAYER = 8
CardDeckSystem.NR_OF_PLAYERS = 4
CardDeckSystem.CardDeck = { }
CardDeckSystem.DealtCards = { }
CardDeckSystem.UsedCards = { }
CardDeckSystem.GameRunning = false


CardDeckSystem.Initialize = function ( self )
	self:SetName("CardDeckSystem")
	
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)

	print("Card Deck System Initialized")
end

CardDeckSystem.Update = function( self, dt )
	
	if GameRunning ~= CardDeckSystem.GameRunning then
		
		CardDeckSystem.GameRunning = GameRunning

		if GameRunning then 
			self:CreateDeck()
		end
	end

end

CardDeckSystem.CreateDeck = function (self)

	local prio = 0
	
	local mySeed = os.time() - 1418742000 -- dont ask
	
	math.randomseed(mySeed)
	
	self.NR_OF_PLAYERS = #self:GetEntities()

	for i = 1, 100 * self.NR_OF_PLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		
		local rand = math.random(1, 90)
		
		if rand < 30 then
			cardactioncomp:SetString("Forward")
		elseif rand < 50 then
			cardactioncomp:SetString("Backward")
		elseif rand < 65 then
			cardactioncomp:SetString("TurnRight")
		elseif rand < 80 then
			cardactioncomp:SetString("TurnLeft")
		elseif rand < 90 then
			cardactioncomp:SetString("TurnAround")
		else
			cardactioncomp:SetString("Forward")
			
		end
		
		table.insert(self.CardDeck, entity)
	end
end


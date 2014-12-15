CardDeck = { }
NROFPLAYERS = 4

CardDeckSystem = System()

CardDeckSystem.Update = function( self, dt )
	
end

CardDeckSystem.PostInitialize = function ( self )
	
	
	local prio = 0
	
	for i = 1, 10*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("Forward\0")
		
		table.insert(CardDeck, entity)
	end
	
	for i = 1, 8*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("Backward")
		
		table.insert(CardDeck, entity)
	end
	
	for i = 1, 5*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("TurnLeft")
		
		table.insert(CardDeck, entity)
	end
	
	for i = 1, 5*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("TurnRight")
		
		table.insert(CardDeck, entity)
	end
	
	for i = 1, #CardDeck do
		local entity = CardDeck[i]
		--local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		--local cardprio = cardpriocomp:GetInt()
		local cardaction = cardactioncomp:GetString()
		if i == 1 then
			Console.Print(cardaction)
			print(cardaction)
		end
	end
	
	print("Card Deck System Initialized")
end


--GenCarDeck = function(  )
--	local prio = 0
--	-- forward
--	for i = 1, 10*NROFPLAYERS do
--		prio = prio + 1
--		self:GetComponent(entity, "Position", 0)
--		local entity = world:CreateNewEntity()
--		local entity = world:CreateNewEntity("Player")
--		world:CreateComponentAndAddTo("Spawn", entity)
--		world:CreateComponentAndAddTo("SyncNetwork", entity)
--		local mapPos = {i,12}
--		--local mapPos = {i, 6}
--		self:SetPosition(entity, mapPos[1], 1.0, mapPos[2])
--		local comp = self:GetComponent(entity, "Spawn", 0)
--		comp:SetInt2(mapPos[1], mapPos[2])
--		
--	end
--	
--	--backward
--	
--	--turnright
--	
--	--turnleft
--
--end




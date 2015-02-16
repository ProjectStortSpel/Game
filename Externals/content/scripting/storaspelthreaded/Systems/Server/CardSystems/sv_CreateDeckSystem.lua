CreateDeckSystem = System()

CreateDeckSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CreateDeckSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Unit", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CreateDeck",	FilterType.RequiresOneOf)
end


CreateDeckSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
	
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "CreateDeck") then
			self:CreateDeck()
			world:KillEntity( entityId )

			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("DealCards", id)
			world:SetComponent(id, "DealCards", "NumCards", 8)
			print("DealCards id = " .. id)
		end
	end
end

CreateDeckSystem.CreateDeck = function (self)

	local NrOfPlayers = #self:GetEntities("Unit")
	local mySeed = os.time() - 1418742000 -- dont ask
	
	-- Setup Lists
	local PrioList = { }
	PrioList.__mode = "k"
	local CardAction = { }
	CardAction.__mode = "k"
	local NrOfCards = { }
	NrOfCards.__mode = "k"
	local CardPrio = { }
	CardPrio.__mode = "k"
	
	----------------------------------------------------------
	-- ADD NEW CARDS HERE
	CardAction[#CardAction+1] = "Forward"
	NrOfCards[#NrOfCards+1] = 4 * NrOfPlayers
	CardPrio[#CardPrio+1] = true
	
	CardAction[#CardAction+1] = "Backward"
	NrOfCards[#NrOfCards+1] = 4 * NrOfPlayers
	CardPrio[#CardPrio+1] = true
	
	CardAction[#CardAction+1] = "TurnRight"
	NrOfCards[#NrOfCards+1] = 2 * NrOfPlayers
	CardPrio[#CardPrio+1] = false

	CardAction[#CardAction+1] = "TurnLeft"
	NrOfCards[#NrOfCards+1] = 2 * NrOfPlayers
	CardPrio[#CardPrio+1] = false

	CardAction[#CardAction+1] = "TurnAround"
	NrOfCards[#NrOfCards+1] = 2 * NrOfPlayers
	CardPrio[#CardPrio+1] = false

	CardAction[#CardAction+1] = "Guard"
	NrOfCards[#NrOfCards+1] = 50 * NrOfPlayers
	CardPrio[#CardPrio+1] = false
	
	CardAction[#CardAction+1] = "Sprint"
	NrOfCards[#NrOfCards+1] = 1 * NrOfPlayers
	CardPrio[#CardPrio+1] = true

	CardAction[#CardAction+1] = "SlingShot"
	NrOfCards[#NrOfCards+1] = 1 * NrOfPlayers
	CardPrio[#CardPrio+1] = true
	----------------------------------------------------------

	-- GENERATE PRIO
	for i = 1, #NrOfCards do
		if CardPrio[i] then
			for j = 1, NrOfCards[i] do
				PrioList[#PrioList+1] = #PrioList+1
			end
		end
	end
	
	-- CREATE CARDS
	math.randomseed(mySeed)
	for i = 1, #CardAction do
		for j = 1, NrOfCards[i] do
			local prio = 0
			if CardPrio[i] then
				-- GET PRIO
				local rand = math.random(1, #PrioList)
				prio = PrioList[rand]
				table.remove(PrioList, rand)
			end
			
			-- CREATE CARD
			local entity = world:CreateNewEntity("Card")
			local cardpriocomp = world:GetComponent(entity, "CardPrio", 0)
			local cardactioncomp = world:GetComponent( entity, "CardAction", 0)
			cardpriocomp:SetInt(prio)
			cardactioncomp:SetText(CardAction[i])
		end
	end
end

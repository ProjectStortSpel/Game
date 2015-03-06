PlayCardSystem	=	System()
PlayCardSystem.CardsAbove 			= 	{}
PlayCardSystem.CardsAbove.__mode 	= 	"k"


PlayCardSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PlayCardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PlayCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardStep", FilterType.RequiresOneOf)
end

PlayCardSystem.GetCardsWithPrio = function(self, selectedStep, selectedPrio)

	local	allCards				=	self:GetEntities("CardStep")
	local	selectedCards			=	{}
			selectedCards.__mode	=	"k"
	for tIndex = 1, #allCards do
		local	tCard	=	allCards[tIndex]
		
		if selectedStep == world:GetComponent(tCard, "CardStep", "Step"):GetInt() then
			if selectedPrio == world:GetComponent(tCard, "CardPrio", "Prio"):GetInt() then
				selectedCards[#selectedCards+1]	=	tCard
			end
		end
	end
	
	
	return	selectedCards
end

PlayCardSystem.GetNextCard = function(self, allCards, currentStep)
	
	local	newCard		=	nil
	local	currentPrio	=	2147483647
	
	for cardIndex = 1, #allCards do
		local 	cardStep 	= 	world:GetComponent(allCards[cardIndex], "CardStep", "Step"):GetInt()

		if cardStep == currentStep then
			local 	tmpPrio	= 	world:GetComponent(allCards[cardIndex], "CardPrio", "Prio"):GetInt()
			
			if tmpPrio ~= 0 then
				
				if tmpPrio < currentPrio then
					newCard		=	allCards[cardIndex]
					currentPrio	=	tmpPrio
				end
				
			end
		end
	end
	
	return	newCard
end

PlayCardSystem.PlayCard = function(self, unitToPlay, cardToPlay)
	
	--	Check if the unit can play the action
	if not world:EntityHasComponent(unitToPlay, "UnitDead") then
	
		local	playerNumber	=	world:GetComponent(unitToPlay, "PlayerNumber", "Number" ):GetInt()
		local	cardAction		=	world:GetComponent(cardToPlay, "CardAction", "Action"):GetText()
		local	modelName		=	string.lower(cardAction)
				cardAction		=	"Unit" .. cardAction
				
		--	Give the action to the unit
		world:CreateComponentAndAddTo(cardAction, unitToPlay)
		
		local	playedCard	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("PlayedCard", playedCard)
		world:GetComponent(playedCard, "PlayedCard", "CardAction"):SetText(cardAction)
		world:GetComponent(playedCard, "PlayedCard", "CardPriority"):SetInt(world:GetComponent(cardToPlay, "CardPrio", "Prio"):GetInt())
		world:GetComponent(playedCard, "PlayedCard", "PlayerNumber"):SetInt(playerNumber)
		
		
		--	Create CardAboveHead
		local	cardAbove	=	world:CreateNewEntity("CardAboveHead")
		
		world:GetComponent(cardAbove, "Position", 0):SetFloat3(0.0, 2.0 + #self.CardsAbove*0.001, 0.0)
		world:GetComponent(cardAbove, "Rotation", 0):SetFloat3(1.5 * math.pi, math.pi, 0.0)
		world:GetComponent(cardAbove, "Scale", 0):SetFloat3(0,0,0)
		if not world:EntityHasComponent(cardAbove, "LerpScale") then
			world:CreateComponentAndAddTo("LerpScale", cardAbove)
			world:GetComponent(cardAbove, "LerpScale", "X"):SetFloat(1.5)
			world:GetComponent(cardAbove, "LerpScale", "Y"):SetFloat(1.5)
			world:GetComponent(cardAbove, "LerpScale", "Z"):SetFloat(1.5)
			world:GetComponent(cardAbove, "LerpScale", "Time"):SetFloat(0.15)
			world:GetComponent(cardAbove, "LerpScale", "Algorithm"):SetText("SmoothLerp")
		end
		world:GetComponent(cardAbove, "Model", "ModelName"):SetText(modelName)
		world:GetComponent(cardAbove, "Model", "ModelPath"):SetText("cards")
		world:GetComponent(cardAbove, "Model", "RenderType"):SetInt(1)
		world:GetComponent(cardAbove, "Parent", 0):SetInt(unitToPlay)
		
		local	R, G, B	=	world:GetComponent(unitToPlay, "Color", "X"):GetFloat3()
		world:GetComponent(cardAbove, "Color", 0):SetFloat3(R, G, B)
		
		world:GetComponent(cardAbove, "CardAboveHead", "CardOwner"):SetInt(unitToPlay)
		world:GetComponent(cardAbove, "CardAboveHead", "CardId"):SetInt(cardToPlay)
		world:GetComponent(cardAbove, "CardAboveHead", "CardDenied"):SetInt(0)
		
		
		if world:GetComponent(cardToPlay, "CardPrio", "Prio"):GetInt() ~= 0 then
			for n = 1, #self.CardsAbove do
				if not world:EntityHasComponent(self.CardsAbove[n], "LerpScale") then
					world:CreateComponentAndAddTo("LerpScale", self.CardsAbove[n])
					world:GetComponent(self.CardsAbove[n], "LerpScale", "X"):SetFloat(1.0)
					world:GetComponent(self.CardsAbove[n], "LerpScale", "Y"):SetFloat(1.0)
					world:GetComponent(self.CardsAbove[n], "LerpScale", "Z"):SetFloat(1.0)
					world:GetComponent(self.CardsAbove[n], "LerpScale", "Time"):SetFloat(0.2)
					world:GetComponent(self.CardsAbove[n], "LerpScale", "Algorithm"):SetText("SmoothLerp")
				end
				world:GetComponent(self.CardsAbove[n], "Color", 0):SetFloat3(0.4, 0.4, 0.4)
			end
		end
		self.CardsAbove[#self.CardsAbove+1]	=	cardAbove
	end
	
	world:RemoveComponentFrom("CardStep", cardToPlay)
	
	return 	1.5
end

PlayCardSystem.EntitiesAdded = function(self, dt, entities)
	
	for n = 1, #entities do
		local	tEntity	=	entities[n]
		
		if world:EntityHasComponent(tEntity, "PlayCard") then
			
			
			local	tStep	=	world:GetComponent(tEntity, "PlayCard", "Step"):GetInt()
			
			local	zeroPrio	=	self:GetCardsWithPrio(tStep, 0)
			local	cardTimer	=	0
			
		
			local	cardsToPlay			=	{}
					cardsToPlay.__mode	=	"k"
					
			if #zeroPrio == 0 then
				local	cardToPlay	=	self:GetNextCard(self:GetEntities("CardStep"), tStep)
				
				if cardToPlay then
					cardsToPlay[#cardsToPlay+1]	=	cardToPlay
				end
			else
				
				for iZero = 1, #zeroPrio do
					cardsToPlay[#cardsToPlay+1]	=	zeroPrio[iZero]
				end
				
			end
			
			
			if #cardsToPlay ~= 0 then
			
				for cardIndex = 1, #cardsToPlay do
					local unit = world:GetComponent( cardsToPlay[cardIndex], "CardStep", "UnitEntityId"):GetInt()

					cardTimer	=	self:PlayCard(unit, cardsToPlay[cardIndex])
					
				end
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("PlayCardTimer", id)
				world:GetComponent(id, "PlayCardTimer", "Time"):SetFloat(cardTimer)
				world:SetComponent(id, "PlayCardTimer", "Step", tStep)
			else
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("MoveRiver", id)
				world:CreateComponentAndAddTo("SyncNetwork", id)

				id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("StepTimer", id)
				world:GetComponent(id, "StepTimer", "Time"):SetFloat(1)
				
				for i = 1, #self.CardsAbove do
					local	tempCard	=	self.CardsAbove[i]
					--if not world:EntityHasComponent(tempCard, "LerpScale") then
					--	world:CreateComponentAndAddTo("LerpScale", tempCard)
					--end
					--world:GetComponent(tempCard, "LerpScale", "X"):SetFloat(0)
					--world:GetComponent(tempCard, "LerpScale", "Y"):SetFloat(0)
					--world:GetComponent(tempCard, "LerpScale", "Z"):SetFloat(0)
					--world:GetComponent(tempCard, "LerpScale", "Time"):SetFloat(0.15)
					--world:GetComponent(tempCard, "LerpScale", "Algorithm"):SetText("SmoothLerp")
					--
					world:KillEntity(tempCard)
				end
				self.CardsAbove 		= 	{ }
				self.CardsAbove.__mode 	= 	"k"
			end
			
			world:KillEntity(tEntity)
			
		end
	end
	
end



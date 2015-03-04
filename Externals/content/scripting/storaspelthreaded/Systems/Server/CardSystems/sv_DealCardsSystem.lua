DealCardsSystem = System()
DealCardsSystem.DealCard = false
DealCardsSystem.FirstDeal = true
DealCardsSystem.Arrows = {}
DealCardsSystem.Arrows.__mode = "k"

DealCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("DealCardsSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AI", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardAction", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealingSettings", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UsedCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("CardStep", FilterType.Excluded)
	self:AddComponentTypeToFilter("DealtCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

DealCardsSystem.ClearSelectedCards = function(self)
	
	local	selectedCards	=	self:GetEntities("ServerSelectedCard")
	for i = 1, #selectedCards do
		world:GetComponent(selectedCards[i], "ServerSelectedCard", "Index"):SetInt(0)
		world:RemoveComponentFrom("ServerSelectedCard", selectedCards[i])
	end

end

DealCardsSystem.EntitiesAdded = function(self, dt, entities)

	local dealCards = false

	for n = 1, #entities do
		local entityId = entities[n]

		if world:EntityHasComponent( entityId, "DealCards") then
			self:ClearSelectedCards()
			local numCards = world:GetComponent( entityId, "DealCards", "NumCards"):GetInt()
			self:DealCards(numCards)

			world:KillEntity( entityId )
			
			dealCards = true
		end
	end
	
	if dealCards then
		local audioId = Net.StartPack("Client.FadeInSoundC")
		Net.WriteString(audioId, "InGame")
		Net.WriteString(audioId, "PickingMusic")
		Net.WriteInt(audioId, 200)
		Net.WriteBool(audioId, false)
		Net.Broadcast(audioId)
	elseif #self.Arrows > 0 then
		self:RemoveArrows()
	end
end

DealCardsSystem.DealCards = function (self, numCards)

	local players = self:GetEntities("Player")
	local aiPlayers = self:GetEntities("AI")
	local cards = self:GetEntities("Card")
	local cardsLeft = #cards
	--print("DealCards")
	--print("Numplayer: " .. #players)
	--print("NumCards: " .. cardsLeft)
	--print("NumCardsToDeal: " .. numCards)

	--print("")
	
	for i = 1, #players do
		
		if world:EntityHasComponent(players[i], "HasSelectedCards") then
			world:RemoveComponentFrom("HasSelectedCards", players[i])
		end
	
		local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetText()
		local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()

		for j = 1, numCards do

			local cardIndex = math.random(1, cardsLeft)
			local card = cards[cardIndex]
			
			world:CreateComponentAndAddTo("DealtCard", card)
			world:SetComponent(card, "DealtCard", "PlayerEntityId", players[i])
			
			Net.SendEntity(card, ip, port)	

			table.remove(cards, cardIndex)
			cardsLeft = cardsLeft - 1
		end
		
		self:PlaceArrowAboveUnit(players[i])
		
		--Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
	end
	
	for i = 1, #aiPlayers do
		
		if world:EntityHasComponent(aiPlayers[i], "HasSelectedCards") then
			world:RemoveComponentFrom("HasSelectedCards", aiPlayers[i])
		end
		
		for j = 1, numCards do
			
			local cardIndex = math.random(1, cardsLeft)
			local card = cards[cardIndex]
			
			world:CreateComponentAndAddTo("DealtCard", card)
			if not world:EntityHasComponent(card, "AICard") then
				world:CreateComponentAndAddTo("AICard", card)
			end
			world:SetComponent(card, "DealtCard", "PlayerEntityId", aiPlayers[i])
			
			table.remove(cards, cardIndex)
			cardsLeft = cardsLeft - 1
		end
	end
	
	--	Notify players about timer
	local newId = world:CreateNewEntity();
	world:CreateComponentAndAddTo("OnPickingPhase", newId);
end
	  
DealCardsSystem.PlaceArrowAboveUnit = function(self, player)
	local unit = world:GetComponent(player, "UnitEntityId", "Id"):GetInt()

	local arrow = world:CreateNewEntity()
	world:CreateComponentAndAddTo("SyncNetwork", arrow)
	world:CreateComponentAndAddTo("Parent", arrow)
	world:CreateComponentAndAddTo("Model", arrow)
	world:CreateComponentAndAddTo("Position", arrow)
	world:CreateComponentAndAddTo("Rotation", arrow)
	world:CreateComponentAndAddTo("Scale", arrow)
	world:CreateComponentAndAddTo("LerpScale", arrow)
	world:GetComponent(arrow, "Position", 0):SetFloat3(0.0, 1.8, 0.0)
	world:GetComponent(arrow, "Rotation", 0):SetFloat3(1.5 * math.pi, math.pi, 0.0)
	world:GetComponent(arrow, "Scale", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(arrow, "Model", 0):SetModel("directionalarrow", "quad", 1)
	world:GetComponent(arrow, "Parent", 0):SetInt(unit)
	world:GetComponent(arrow, "LerpScale", "Time", 0):SetFloat4(0.2, 1.0, 1.0, 1.0)
	world:GetComponent(arrow, "LerpScale", "Algorithm", 0):SetText("SmoothLerp")
	world:GetComponent(arrow, "LerpScale", "KillWhenFinished", 0):SetBool(false)
	table.insert(self.Arrows, arrow)
end

DealCardsSystem.RemoveArrows = function(self)
	local arrowCount = #self.Arrows
	for i = 1, arrowCount do
		local arrow = self.Arrows[i]
		if not world:EntityHasComponent(arrow, "LerpScale") then
			world:CreateComponentAndAddTo("LerpScale", arrow)
		end
		world:GetComponent(arrow, "LerpScale", "Time", 0):SetFloat4(0.2, 0.0, 0.0, 0.0)
		world:GetComponent(arrow, "LerpScale", "Algorithm", 0):SetText("SmoothLerp")
		world:GetComponent(arrow, "LerpScale", "KillWhenFinished", 0):SetBool(true)
	end
	self.Arrows = { }
	self.Arrows.__mode = "k"
end

Net.Receive("Server.SelectCards",
	function( id, ip, port )
	
		--	Selected cards from player
		local	selectedCards			=	{}
				selectedCards.__mode	=	"k"
				
		--	Check all cards from the player
		local	currentPlayer, pIP, pPORT
		
		local DealingSettings = DealCardsSystem:GetEntities("DealingSettings")
		local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
		
		for i = 1, cardsToPick do
			
			--	Get the current card
			local	tempCard	=	Net.ReadInt(id)
			
			if world:EntityHasComponent(tempCard, "DealtCard") then
				currentPlayer	=	world:GetComponent( tempCard, "DealtCard", "PlayerEntityId"):GetInt()
				pIP 			= 	world:GetComponent( currentPlayer, "NetConnection", "IpAddress"):GetText()
				pPORT 			= 	world:GetComponent( currentPlayer, "NetConnection", "Port"):GetInt()
				
				--	Correct player
				if pIP == ip and pPORT == port then
					selectedCards[#selectedCards+1]	=	tempCard
				else
					print("TRYING TO PLAY A CARD THAT AINT THEIRS!!")
					Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
					return
				end	
				
				
			else
				print("TRYING TO PLAY A UNDEALT CARD! CHEATER")
				Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
				return
			end
		
		
		end
		
		--	All cards are legit
		--	start next phase
		
		--	Get the players unit
		local	pUnit	=	world:GetComponent(currentPlayer, "UnitEntityId", "Id"):GetInt()
		world:CreateComponentAndAddTo("HasSelectedCards", currentPlayer)
		
		for n = 1, cardsToPick do
			local	cAction	=	world:GetComponent(selectedCards[n], "CardAction", "Action"):GetText()
			local	cPrio	=	world:GetComponent(selectedCards[n], "CardPrio", "Prio"):GetInt()
			
			
			world:RemoveComponentFrom("DealtCard", selectedCards[n])
			world:CreateComponentAndAddTo("CardStep", selectedCards[n])
			
			world:GetComponent(selectedCards[n], "CardStep", "Step"):SetInt(n)
			world:GetComponent(selectedCards[n], "CardStep", "UnitEntityId"):SetInt(pUnit)
			
			Net.SendEntityKill(selectedCards[n], ip, port)
		end
		
		world:CreateComponentAndAddTo("UnitSelectedCards", pUnit)
		
		local	id	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("NotifyStartNewRound", id)
		
	end
)
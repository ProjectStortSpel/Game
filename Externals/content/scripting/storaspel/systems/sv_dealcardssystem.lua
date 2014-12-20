DealCardsSystem = System()
DealCardsSystem.DealCard = false
DealCardsSystem.FirstDeal = true

DealCardsSystem.Initialize = function ( self )
	self:SetName("DealCardsSystem")
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CardAction", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UsedCard", FilterType.Excluded)
	self:AddComponentTypeToFilter("DealtCard", FilterType.Excluded)

	print("DealCardsSystem Initialized")
end

DealCardsSystem.OnEntityAdded = function(self, entityId)
		
	if world:EntityHasComponent( entityId, "DealCards") then
		local numCards = world:GetComponent( entityId, "DealCards", "NumCards"):GetInt()
		self:DealCards(numCards)

		world:KillEntity( entityId )

	end
	
end

DealCardsSystem.DealCards = function (self, numCards)

	local players = self:GetEntities("Player")
	local cards = self:GetEntities("Card")
	local cardsLeft = #cards
	print("DealCards")
	print("Numplayer: " .. #players)
	print("NumCards: " .. cardsLeft)
	print("NumCardsToDeal: " .. numCards)

	print("")
	for i = 1, #players do

		local ip = world:GetComponent(players[i], "NetConnection", "IpAddress"):GetString()
		local port = world:GetComponent(players[i], "NetConnection", "Port"):GetInt()

		
		print("IP:   " .. ip)
		print("Port: " .. port)
		print("NumCards:" .. numCards)
		print("")
		for j = 1, numCards do

			local cardIndex = math.random(1, cardsLeft)

			local card = cards[cardIndex]
			world:CreateComponentAndAddTo("DealtCard", card)
			world:SetComponent(card, "DealtCard", "PlayerEntityId", players[i])
			
			Net.SendEntity(card, ip, port)	

			table.remove(cards, cardIndex)
			cardsLeft = cardsLeft - 1
		end
		
		Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
	end

end

--DealCardsSystem.GetCards = function (self)
	--
	--local cards = {}
	--local index = 1;
	--local entities = self:GetEntities()
	--for i = 1, #entities do
		--if self:EntityHasComponent(entities[i], "CardAction") then
			--cards[index] = entities[i]
			--index = index + 1
		--end
	--end
	--return cards
--end
--
--DealCardsSystem.GetPlayers = function (self)
	--
	--local players = {}
	--local index = 1;
	--local entities = self:GetEntities()
	--for i = 1, #entities do
		--if self:EntityHasComponent(entities[i], "Player") then
			--players[index] = entities[i]
			--index = index + 1
		--end
	--end
	--return players
--end

Net.Receive("Server.SelectCards", 
	function( id, ip, port )	

		print("Client selected cards:")
		print("")
		for i = 1, 5 do

			local card = Net.ReadInt(id)
			if  world:EntityHasComponent(card, "DealtCard") then
			
				local player = world:GetComponent(card, "DealtCard", "PlayerEntityId"):GetInt()
				local playerIp = world:GetComponent(player, "NetConnection", "IpAddress"):GetString()
				local playerPort = world:GetComponent(player, "NetConnection", "Port"):GetInt()

				
				if playerIp == ip and playerPort == port then
					local action = world:GetComponent(card, "CardAction", "Action"):GetString()
					local prio = world:GetComponent(card, "CardPrio", "Prio"):GetInt()
					print("Action: " .. action .. " - Prio: " .. prio)

					world:RemoveComponentFrom("DealtCard", card)
					world:CreateComponentAndAddTo("CardStep", card)
					world:SetComponent(card, "CardStep", "Step", i)
					local unit = world:GetComponent(player, "UnitEntityId", "Id"):GetInt()
					world:SetComponent(card, "CardStep", "UnitEntityId", unit)
				else
					Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
					return
				end

			else
				Net.Send(Net.StartPack("Client.SelectCards"), ip, port)
				return
			end

		end	
	end
)
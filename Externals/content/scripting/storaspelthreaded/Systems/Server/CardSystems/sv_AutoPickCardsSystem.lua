AutoPickCards = System()

AutoPickCards.Initialize = function(self)
	--	Set Name
	self:SetName("AutoPickCards")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("DealtCard", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("AutoPickCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("HasSelectedCards", FilterType.Excluded)
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
end

AutoPickCards.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent(entity, "AutoPickCards") then
		
			print("Time to steal cards!")
			
			local Players = self:GetEntities("Player")
			local Cards = self:GetEntities("DealtCard")
			
			for i = 1, #Players do
				
				local SelectedCards = 0
				local Player = Players[i]
				local unit = world:GetComponent(Player, "UnitEntityId", "Id"):GetInt()

				local playerIp = world:GetComponent(Player, "NetConnection", "IpAddress"):GetText()
				local playerPort = world:GetComponent(Player, "NetConnection", "Port"):GetInt()
				
				for n = 1, #Cards do
				
					local Owner = world:GetComponent(Cards[n], "DealtCard", "PlayerEntityId"):GetInt()
					
					if Owner == Player then
						
						local action = world:GetComponent(Cards[n], "CardAction", "Action"):GetText()
						local prio = world:GetComponent(Cards[n], "CardPrio", "Prio"):GetInt()
						print("Action: " .. action .. " - Prio: " .. prio)

						world:RemoveComponentFrom("DealtCard", Cards[n])
						world:CreateComponentAndAddTo("CardStep", Cards[n])
						world:SetComponent(Cards[n], "CardStep", "Step", SelectedCards + 1)
						world:SetComponent(Cards[n], "CardStep", "UnitEntityId", unit)



						Net.SendEntityKill(Cards[n], playerIp, playerPort)
						
						SelectedCards = SelectedCards + 1
						if SelectedCards == 5 then
							break
						end
					end
				end
			
				world:CreateComponentAndAddTo("UnitSelectedCards", unit)
			end
			
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NotifyStartNewRound", id)
			
			world:KillEntity(entity)
		end
	end
end
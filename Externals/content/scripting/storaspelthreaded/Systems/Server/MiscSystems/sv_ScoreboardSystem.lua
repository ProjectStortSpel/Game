ScoreboardSystem = System()

ScoreboardSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ScoreboardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	
	--	Set Filter
	self:AddComponentTypeToFilter("Player", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerNameChanged", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ScoreboardPlayer", FilterType.RequiresOneOf)
end

ScoreboardSystem.EntitiesAdded = function(self, dt, addedEntities)

	for n = 1, #addedEntities do
		
		if world:EntityHasComponent( addedEntities[n], "Player") then
		
			local ip 	= world:GetComponent(addedEntities[n], "NetConnection", "IpAddress"):GetText()
			local port 	= world:GetComponent(addedEntities[n], "NetConnection", "Port"):GetInt()
			local name = world:GetComponent(addedEntities[n], "PlayerName", "Name"):GetText()
			
			local scrbrdP = world:CreateNewEntity()
			world:CreateComponentAndAddTo("ScoreboardPlayer", scrbrdP)
			world:CreateComponentAndAddTo("SyncNetwork", scrbrdP)
			world:GetComponent(scrbrdP, "ScoreboardPlayer", "Name"):SetText(name)
			world:SetComponent(scrbrdP, "ScoreboardPlayer", "R", 1)
			world:SetComponent(scrbrdP, "ScoreboardPlayer", "G", 1)
			world:SetComponent(scrbrdP, "ScoreboardPlayer", "B", 1)
			world:SetComponent(scrbrdP, "ScoreboardPlayer", "IpAddress", ip)
			world:SetComponent(scrbrdP, "ScoreboardPlayer", "Port", port)
		
		elseif world:EntityHasComponent(addedEntities[n], "PlayerNameChanged") then
		
			local pnIp 		= world:GetComponent(addedEntities[n], "PlayerNameChanged", "IpAddress"):GetText()
			local pnPort 	= world:GetComponent(addedEntities[n], "PlayerNameChanged", "Port"):GetInt()
			local pnName 	= world:GetComponent(addedEntities[n], "PlayerNameChanged", "Name"):GetText()
	
			local scrbrdPlayers = self:GetEntities("ScoreboardPlayer")
			for i = 1, #scrbrdPlayers do
			
				local ip 	= world:GetComponent(scrbrdPlayers[i], "ScoreboardPlayer", "IpAddress"):GetText()
				local port 	= world:GetComponent(scrbrdPlayers[i], "ScoreboardPlayer", "Port"):GetInt()
			
				if pnIp == ip and pnPort == port then
					world:SetComponent(scrbrdPlayers[i], "ScoreboardPlayer", "Name", pnName)
					print(pnName)
				end
			
			end

		end
		
	end 
end
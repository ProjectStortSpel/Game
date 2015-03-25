FinishSystem = System()

FinishSystem.Initialize = function(self)
	--	Set Name
	self:SetName("FinishSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PlayerReachedFinish", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("PlayerCounter", FilterType.RequiresOneOf)
end

FinishSystem.EntitiesAdded = function(self, dt, entities)
	
	for n = 1, #entities do
		local entity = entities[n]
	
		if world:EntityHasComponent(entity, "PlayerReachedFinish") then
			
			local playerCounter = self:GetEntities("PlayerCounter")
			if #playerCounter <= 0 then -- Should never happend
				print("Warning: PlayerCounter does not exist")
				world:KillEntity(entity)
				return
			end

			local noPlayers = world:GetComponent(playerCounter[1], "PlayerCounter", "Players"):GetInt() -- Number of players still playing
			print("Number of players left:", noPlayers)
			if noPlayers <= 0 then
				print("Everyone has reached the finish, do a reload.")
				Console.AddToCommandQueue("reload")
			end
			world:KillEntity(entity)
		end
	
	end
	
end
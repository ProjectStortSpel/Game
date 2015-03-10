AIDynamicScripting = System()

AIDynamicScripting.LastCheckpoints = {}
AIDynamicScripting.LastCheckpoints.__mode = "k"

AIDynamicScripting.Initialize = function(self)
	self:SetName("AI dynamic scripting System")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewStep", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
end

AIDynamicScripting.EntitiesAdded = function(self, dt, entities)

	for i = 1, #entities 
	do
		if world:EntityHasComponent(entities[i], "NewRound")
		then
			local units = self:GetEntities("Unit");
			local nrOfPlayers = #units
			for j = 1, nrOfPlayers
			do
				local playerEntity	= world:GetComponent(units[j], "PlayerEntityId", 0):GetInt(0);
				if world:EntityHasComponent(playerEntity, "AI")
				then
					local plyNr = world:GetComponent(playerEntity, "PlayerNumber", 0):GetInt(0);
					self:UpdatePF( plyNr, "Unit" );
				end
			end
			
		elseif world:EntityHasComponent(entities[i], "NewStep")
		then
			local units = self:GetEntities("Unit");
			local nrOfPlayers = #units
			
			for j = 1, nrOfPlayers
			do
				--print("look at all the nr of players ", nrOfPlayers);
				local playerEntity	= world:GetComponent(units[j], "PlayerEntityId", 0):GetInt(0);
				
				if world:EntityHasComponent(playerEntity, "AI")
				then
					local plyNr 		= world:GetComponent(playerEntity, "PlayerNumber", 0):GetInt(0);
					local oldCP = self.LastCheckpoints[plyNr];
					local newCP = world:GetComponent(units[j], "TargetCheckpoint", 0):GetInt(0);
					if oldCP ~= newCP
					then 
						local superCounter = 0.0;
						for g = 1, #self.LastCheckpoints 
						do
							local a = self.LastCheckpoints[g];
							local b = newCP
							if a >= b
							then
								superCounter = superCounter + 1.0;
							end
						end
						local bookIndex = DynamicScripting.LoadRuleBook("content/dynamicscripting/map.txt")
						DynamicScripting.UseThisScript( plyNr );
						local toChange = 1 -( superCounter / (nrOfPlayers-1) );
						DynamicScripting.UpdateWeight(toChange, bookIndex);
						
						DynamicScripting.SetRuleBook( bookIndex );
						
						local fail = DynamicScripting.GenerateScript(plyNr);
						
						if not fail
						then
							print("updateing PF");
							
							self:UpdatePF( plyNr, "Void" );
							self:UpdatePF( plyNr, "NotWalkable" );
						end
					end
					
				end
			end
			
			for j = 1, nrOfPlayers
			do
				local playerEntity	= world:GetComponent(units[j], "PlayerEntityId", 0):GetInt(0);
				local newCP 		= world:GetComponent(units[j], "TargetCheckpoint", 0):GetInt(0);
				local plyNr 		= world:GetComponent(playerEntity, "PlayerNumber", 0):GetInt(0);
				self.LastCheckpoints[plyNr] = newCP;
			end
			
		elseif world:EntityHasComponent(entities[i], "Unit")
		then
		
			local playerEntity = world:GetComponent(entities[i], "PlayerEntityId", 0):GetInt(0);
			local playerNumber = world:GetComponent(playerEntity, "PlayerNumber", 0):GetInt(0);
			local newCP = world:GetComponent(entities[i], "TargetCheckpoint", 0):GetInt(0);
			self.LastCheckpoints[playerNumber] = newCP;
					
		end
	end

end

AIDynamicScripting.UpdatePF = function(self, playerNumber, object )
	local onTheSpotValue = 0.0;
	local length = 2;
	local power = 2;
	local found, weight = DynamicScripting.GetWeightFrom(object, playerNumber)
	if found 
	then
		local superstuff = self:GetEntities(object);
		local param = PFParam()
		for j = 1, #superstuff do
			
			local x, y = world:GetComponent(superstuff[j], "MapPosition", 0):GetInt2(0)
			
			param:AddPosition(x, y)
		end
		
		PotentialFieldHandler.UpdatePF(param, playerNumber, object, onTheSpotValue, weight, length, power)
		
	end
end
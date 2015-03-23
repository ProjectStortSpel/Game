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
					
	
					io.write("\n--- AI nr: ", plyNr, " uses these scripts ---\nOn the spot Weight Length Power\n")
					self:PrintMyScript(plyNr, "Void")
					self:PrintMyScript(plyNr, "NotWalkable")
					self:PrintMyScript(plyNr, "Unit")
					self:PrintMyScript(plyNr, "RiverEnd")
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
						
						--DynamicScripting.SetRuleBook( bookIndex );
						--
						--local fail = DynamicScripting.GenerateScript(plyNr);
						--
						--if not fail
						--then
						--	print("updateing PF");
						--	
						--	self:UpdatePF( plyNr, "Void" );
						--	self:UpdatePF( plyNr, "NotWalkable" );
						--end
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

AIDynamicScripting.PrintMyScript = function(self, _playerNumber, _object)
	
	local found, onTheSpotValue, weight, length, power = DynamicScripting.GetWeightFrom(_object, _playerNumber)
	
	local tab = ""
	if _object == "Void" or _object == "Unit" then
		tab = "\t\t"
	else
		tab = "\t"
	end
	
	io.write(_object, tab, "On the spot: ", onTheSpotValue, " Weight: ", weight, " Length: ", length, " Power: ", power, "\n")
end

AIDynamicScripting.UpdatePF = function(self, playerNumber, object )
	local found, onTheSpotValue, weight, length, power = DynamicScripting.GetWeightFrom(object, playerNumber)
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
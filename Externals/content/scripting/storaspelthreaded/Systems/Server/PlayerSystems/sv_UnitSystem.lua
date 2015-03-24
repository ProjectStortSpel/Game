UnitSystem = System()
UnitSystem.NextSlot = 1
UnitSystem.FreeSlots = {}
UnitSystem.FreeSlots.__mode = "k"
UnitSystem.Colors = {}
UnitSystem.Colors.__mode = "k"

UnitSystem.Initialize = function(self)
	self:SetName("UnitSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	self:PopulateColors()
	
	self:AddComponentTypeToFilter("NeedUnit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("RemoveUnit", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
end

--UnitSystem.FindEmptyId = function(self)
--
--	local currentPlayers = self:GetEntities()
--	local newPlayerId = 5
--	
--	local availableIds = {}
--	availableIds.__mode = "k"
--	for n = 1, 5 do
--		availableIds[n] = false
--	end
--	
--	for i = 1, #currentPlayers do
--		local playerId = world:GetComponent(currentPlayers[i], "PlayerNumber", "Number"):GetInt()
--		
--		if playerId ~= -1 then
--			availableIds[playerId] = true
--		end
--	end
--	
--	local newIndex = 1
--	while availableIds[newIndex] do
--		newIndex = newIndex + 1
--	end
--	
--	return newIndex
--end

UnitSystem.EntitiesAdded = function(self, dt, entities)
	
	local mySeed = os.time() - 1418742000 -- dont ask
	math.randomseed(mySeed)
	for n = 1, #entities do
		local entity = entities[n]
	
		if world:EntityHasComponent(entity, "NeedUnit") then
			
			world:RemoveComponentFrom("NeedUnit", entity)
			local playerNumber = nil
			if #self.FreeSlots ~= 0 then
				playerNumber = self.FreeSlots[1]
				table.remove(self.FreeSlots, 1)
			else
				playerNumber = self.NextSlot
				self.NextSlot = self.NextSlot + 1
			end
			
			local newEntityId = world:CreateNewEntity("Unit")
			
			world:SetComponent(newEntityId, "Model", "ModelName", "caveman");
			world:SetComponent(newEntityId, "Model", "ModelPath", "caveman");
			world:SetComponent(newEntityId, "Model", "RenderType", 0);
			
			local r, g, b = self:GetPlayerColor(playerNumber)
			if not world:EntityHasComponent(newEntityId, "Color") then
				world:CreateComponentAndAddTo("Color", newEntityId)
			end
			world:GetComponent(newEntityId, "Color", "X"):SetFloat3(r, g, b)
			
			--	Send to start fire on next checkpoint
			local	targetCheckpoint	=	1
			local	nCheckpoints		=	self:GetEntities("Checkpoint")
			local	X, Z, checkpointId	=	0,0,0
			for n = 1, #nCheckpoints do
				
				if world:GetComponent(nCheckpoints[n], "Checkpoint", "Number"):GetInt() == targetCheckpoint then
					checkpointId	=	nCheckpoints[n]
					X, Z			=	world:GetComponent(checkpointId, "MapPosition", "X"):GetInt2()
					break
				end
				
			end
			
			world:SetComponent(newEntityId, "PlayerNumber", "Number", playerNumber)
			world:SetComponent(newEntityId, "PlayerEntityId", "Id", entity)
			world:SetComponent(newEntityId, "TargetCheckpoint", "Id", targetCheckpoint)
			world:GetComponent(newEntityId, "Direction", 0):SetInt2(0, -1)
			world:CreateComponentAndAddTo("NeedSpawnLocation", newEntityId)
			--world:CreateComponentAndAddTo("Hide", newEntityId)
			
			world:SetComponent(entity, "PlayerNumber", "Number", playerNumber)
			world:CreateComponentAndAddTo("UnitEntityId", entity)
			world:SetComponent(entity, "UnitEntityId", "Id", newEntityId)
			
			world:CreateComponentAndAddTo("PlayerStats", newEntityId)
			world:GetComponent(newEntityId, "PlayerStats", "PlayerNumber"):SetInt(playerNumber)
			world:GetComponent(newEntityId, "PlayerStats", "CardsPlayed"):SetInt(0)
			world:GetComponent(newEntityId, "PlayerStats", "Deaths"):SetInt(0)
			world:GetComponent(newEntityId, "PlayerStats", "Place"):SetInt(0)
			world:GetComponent(newEntityId, "PlayerStats", "GoalCheckpoint"):SetInt(0)
			
			if world:EntityHasComponent(entity, "NetConnection") then
			  
				local ip = world:GetComponent(entity, "NetConnection", "IpAddress"):GetText()
				local port = world:GetComponent(entity, "NetConnection", "Port"):GetInt()
				local id = Net.StartPack("Client.SendMyUnitID")
				Net.WriteInt(id, newEntityId)
				Net.Send(id, ip, port)
				
				local audioId = Net.StartPack("Client.PlaySoundC")
				Net.WriteString(audioId, "Background")
				Net.WriteString(audioId, "Background")
				Net.WriteBool(audioId, true)
				Net.Send(audioId, ip, port)
			else
				-- CREATE TOTEM HAT ON AI
				local randomHat = math.random(0,100)
				local hatRequest = world:CreateNewEntity()
				world:CreateComponentAndAddTo("ThisHat", hatRequest)
				world:GetComponent(hatRequest, "ThisHat", "hatId"):SetInt(randomHat)
				world:GetComponent(hatRequest, "ThisHat", "unitId"):SetInt(newEntityId)
			end
						
		elseif world:EntityHasComponent(entity, "RemoveUnit") then
			local plyNum = world:GetComponent(entity, "RemoveUnit", "PlayerNo"):GetInt()
			self.FreeSlots[#self.FreeSlots + 1] = plyNum
			--table.insert(self.FreeSlots, plyNum)
			local unitId = world:GetComponent(entity, "RemoveUnit", "UnitEntityId"):GetInt()
			if world:EntityHasComponent(unitId, "LerpPosition") or world:EntityHasComponent(unitId, "LerpingPosition") then
				world:CreateComponentAndAddTo("KillAfterLerp", unitId)
			else
				world:KillEntity(unitId)
			end
			world:KillEntity(entity)
		end
		
	end
end

UnitSystem.GetPlayerColor = function(self, colorId)
	if colorId > #self.Colors then
		return 1, 1, 1
	end
	local hex = self.Colors[colorId]
    return tonumber("0x"..hex:sub(1,2))/255, tonumber("0x"..hex:sub(3,4))/255, tonumber("0x"..hex:sub(5,6))/255
end

UnitSystem.PopulateColors = function(self)
	self.Colors[#self.Colors + 1] = "00FF00"
	self.Colors[#self.Colors + 1] = "0000FF"
	self.Colors[#self.Colors + 1] = "FF0000"
	self.Colors[#self.Colors + 1] = "01FFFE"
	self.Colors[#self.Colors + 1] = "FFA6FE"
	self.Colors[#self.Colors + 1] = "FFDB66"
	self.Colors[#self.Colors + 1] = "006401"
	self.Colors[#self.Colors + 1] = "010067"
	self.Colors[#self.Colors + 1] = "95003A"
	self.Colors[#self.Colors + 1] = "007DB5"
	self.Colors[#self.Colors + 1] = "FF00F6"
	self.Colors[#self.Colors + 1] = "FFEEE8"
	self.Colors[#self.Colors + 1] = "774D00"
	self.Colors[#self.Colors + 1] = "90FB92"
	self.Colors[#self.Colors + 1] = "0076FF"
	self.Colors[#self.Colors + 1] = "D5FF00"
	self.Colors[#self.Colors + 1] = "FF937E"
	self.Colors[#self.Colors + 1] = "6A826C"
	self.Colors[#self.Colors + 1] = "FF029D"
	self.Colors[#self.Colors + 1] = "FE8900"
	self.Colors[#self.Colors + 1] = "7A4782"
	self.Colors[#self.Colors + 1] = "7E2DD2"
	self.Colors[#self.Colors + 1] = "85A900"
	self.Colors[#self.Colors + 1] = "FF0056"
	self.Colors[#self.Colors + 1] = "A42400"
	self.Colors[#self.Colors + 1] = "00AE7E"
	self.Colors[#self.Colors + 1] = "683D3B"
	self.Colors[#self.Colors + 1] = "BDC6FF"
	self.Colors[#self.Colors + 1] = "263400"
	self.Colors[#self.Colors + 1] = "BDD393"
	self.Colors[#self.Colors + 1] = "00B917"
	self.Colors[#self.Colors + 1] = "9E008E"
	self.Colors[#self.Colors + 1] = "001544"
	self.Colors[#self.Colors + 1] = "C28C9F"
	self.Colors[#self.Colors + 1] = "FF74A3"
	self.Colors[#self.Colors + 1] = "01D0FF"
	self.Colors[#self.Colors + 1] = "004754"
	self.Colors[#self.Colors + 1] = "E56FFE"
	self.Colors[#self.Colors + 1] = "788231"
	self.Colors[#self.Colors + 1] = "0E4CA1"
	self.Colors[#self.Colors + 1] = "91D0CB"
	self.Colors[#self.Colors + 1] = "BE9970"
	self.Colors[#self.Colors + 1] = "968AE8"
	self.Colors[#self.Colors + 1] = "BB8800"
	self.Colors[#self.Colors + 1] = "43002C"
	self.Colors[#self.Colors + 1] = "DEFF74"
	self.Colors[#self.Colors + 1] = "00FFC6"
	self.Colors[#self.Colors + 1] = "FFE502"
	self.Colors[#self.Colors + 1] = "620E00"
	self.Colors[#self.Colors + 1] = "008F9C"
	self.Colors[#self.Colors + 1] = "98FF52"
	self.Colors[#self.Colors + 1] = "7544B1"
	self.Colors[#self.Colors + 1] = "B500FF"
	self.Colors[#self.Colors + 1] = "00FF78"
	self.Colors[#self.Colors + 1] = "FF6E41"
	self.Colors[#self.Colors + 1] = "005F39"
	self.Colors[#self.Colors + 1] = "6B6882"
	self.Colors[#self.Colors + 1] = "5FAD4E"
	self.Colors[#self.Colors + 1] = "A75740"
	self.Colors[#self.Colors + 1] = "A5FFD2"
	self.Colors[#self.Colors + 1] = "FFB167"
	self.Colors[#self.Colors + 1] = "009BFF"
	self.Colors[#self.Colors + 1] = "E85EBE"
end
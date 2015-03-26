WeatherTornadoSystem					=	System()
WeatherTornadoSystem.TornadoIds			=	{}
WeatherTornadoSystem.TornadoIds.__mode 	= "k"
WeatherTornadoSystem.NoTornados			=	1

WeatherTornadoSystem.Initialize = function(self)
	--	Set Name
	self:SetName("WeatherTornadoSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("WeatherStep", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("WeatherTornado", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TestMoveSuccess",FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	
	-- Tiles
	self:AddComponentTypeToFilter("TileComp",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.Excluded)
	self:AddComponentTypeToFilter("Void",FilterType.Excluded)
	
end

WeatherTornadoSystem.EntitiesAdded = function(self, dt, newEntities)

	local units = self:GetEntities("Unit")

	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "WeatherTornado") then
			for i = 1, self.NoTornados do
				self.TornadoIds[#self.TornadoIds+1] = self:AddTornado()
			end
			world:KillEntity(tEntity)
		end
		
		if world:EntityHasComponent(tEntity, "WeatherStep") then
			for i = 1, #self.TornadoIds do
				self:MoveTornado(self.TornadoIds[i])
				
				for j = 1, #units do
					self:RemoveSpin(units[j])
					self:CheckCollision(self.TornadoIds[i], units[j])
				end
				
			end
		
		end
		
		if world:EntityHasComponent(tEntity, "TestMoveSuccess") then
			local unit = world:GetComponent(tEntity, "TestMoveSuccess", "Unit"):GetInt()
			self:RemoveSpin(unit)
			for i = 1, #self.TornadoIds do
				self:CheckCollision(self.TornadoIds[i], unit)
			end
		end
		
		if world:EntityHasComponent(tEntity, "DealCards") then
			for i = 1, #units do
				self:RemoveSpin(units[i])
			end
			
			for j = 1, #self.TornadoIds do
				self:CancelTornado(self.TornadoIds[j])
				self.TornadoIds[j] = nil
			end
			
		end
		
		
	end
	
end

WeatherTornadoSystem.RemoveSpin = function(self, unit)
	
	if world:EntityHasComponent(unit, "Spin") then 
	
		world:RemoveComponentFrom("Spin", unit)
	
		posX, posZ = world:GetComponent(unit, "MapPosition", 0):GetInt2()
	
		if not world:EntityHasComponent(unit, "LerpPosition") then
			world:CreateComponentAndAddTo("LerpPosition", unit)
		end
		world:GetComponent(unit, "LerpPosition", "X"):SetFloat(posX)
		world:GetComponent(unit, "LerpPosition", "Y"):SetFloat(0.5)
		world:GetComponent(unit, "LerpPosition", "Z"):SetFloat(posZ)
		world:GetComponent(unit, "LerpPosition", "Time"):SetFloat(0.5)
		world:GetComponent(unit, "LerpPosition", "Algorithm"):SetText("NormalLerp")
		
		local dirX, dirZ = world:GetComponent(unit, "Direction", 0):GetInt2()
		local lRotation = 0

		if dirX == 0 and dirZ == 1 then -- 0°
			lRotation = 0
		elseif dirX == 1 and dirZ == 0 then -- 90°
			lRotation = math.pi * 0.5
		elseif dirX == 0 and dirZ == -1 then -- 180°
			lRotation = math.pi
		elseif dirX == -1 and dirZ == 0 then -- 270°
			lRotation = 3 * math.pi / 2
		end
		
		if not world:EntityHasComponent(unit, "LerpRotation") then
			world:CreateComponentAndAddTo("LerpRotation", unit)
		end
		world:GetComponent(unit, "LerpRotation", "X"):SetFloat(0)
		world:GetComponent(unit, "LerpRotation", "Y"):SetFloat(lRotation)
		world:GetComponent(unit, "LerpRotation", "Z"):SetFloat(0)
		world:GetComponent(unit, "LerpRotation", "Time"):SetFloat(0.5)
		world:GetComponent(unit, "LerpRotation", "Algorithm"):SetText("NormalLerp")
		
	end

end

WeatherTornadoSystem.AddTornado = function(self)
	
	
	local id = world:CreateNewEntity("Tornado")
	local tiles = self:GetEntities("TileComp")
	local units = self:GetEntities("Unit")
	print("NO UNITS: " .. #units)
	local posX, posZ = 0
	
	repeat -- Prevent the tornado to spawn on a player
	
		local spawnTile = math.random(1, #tiles)
		posX, posZ = world:GetComponent(tiles[spawnTile], "MapPosition", 0):GetInt2()
		local canPlace = true
	
		for i = 1, #units do
		
			local playerX, playerZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
			
			if playerX == posX and playerZ == posZ then
				canPlace = false
			end
		
		end
	
	until canPlace
	
	--posX, posZ = world:GetComponent(units[1], "MapPosition", 0):GetInt2()
	
	world:GetComponent(id, "Position", 0):SetFloat3(posX, 0.5, posZ)
	world:GetComponent(id, "MapPosition", 0):SetInt2(posX, posZ)
	world:GetComponent(id, "Model", "ModelName"):SetText("tornado")
	world:GetComponent(id, "Model", "ModelPath"):SetText("tornado")
	world:GetComponent(id, "Model", "RenderType"):SetInt(1)
	
	world:CreateComponentAndAddTo("LerpScale", id)
	world:GetComponent(id, "LerpScale", "X"):SetFloat(0.5)
	world:GetComponent(id, "LerpScale", "Y"):SetFloat(0.5)
	world:GetComponent(id, "LerpScale", "Z"):SetFloat(0.5)
	world:GetComponent(id, "LerpScale", "Time"):SetFloat(2.5)
	world:GetComponent(id, "LerpScale", "Algorithm"):SetText("NormalLerp")
	
	return id
end

WeatherTornadoSystem.MoveTornado = function(self, id)
	
	local canMove = false
	local testX, testZ
	local tiles = self:GetEntities("TileComp")
	local testDirections = {1,2,3,4}
	testDirections.__mode = "k"
	local posX, posZ = world:GetComponent(id, "MapPosition", 0):GetInt2()
	
	repeat
	
		local i = math.random(#testDirections)
		local dir = table.remove(testDirections, i)	
		
		if dir == 1 then
			testX = posX - 1
			testZ = posZ		
		elseif dir == 2 then
			testX = posX + 1
			testZ = posZ			
		elseif dir == 3 then
			testX = posX
			testZ = posZ - 1			
		elseif dir == 4 then
			testX = posX
			testZ = posZ + 1			
		end
		
		for i = 1, #tiles do
			
			local tPosX, tPosZ = world:GetComponent(tiles[i], "MapPosition", 0):GetInt2()
			
			if tPosX == testX and tPosZ == testZ then
				
			
				if not world:EntityHasComponent(id, "LerpPosition") then
					world:CreateComponentAndAddTo("LerpPosition", id)
				end
				world:GetComponent(id, "LerpPosition", "X"):SetFloat(tPosX)
				world:GetComponent(id, "LerpPosition", "Y"):SetFloat(0.5)
				world:GetComponent(id, "LerpPosition", "Z"):SetFloat(tPosZ)
				world:GetComponent(id, "LerpPosition", "Time"):SetFloat(1.0)
				world:GetComponent(id, "LerpPosition", "Algorithm"):SetText("NormalLerp")
				
				if not world:EntityHasComponent(id, "LerpScale") then
					world:CreateComponentAndAddTo("LerpScale", id)
				end
				world:GetComponent(id, "LerpScale", "X"):SetFloat(1.0)
				world:GetComponent(id, "LerpScale", "Y"):SetFloat(1.0)
				world:GetComponent(id, "LerpScale", "Z"):SetFloat(1.0)
				world:GetComponent(id, "LerpScale", "Time"):SetFloat(1.0)
				world:GetComponent(id, "LerpScale", "Algorithm"):SetText("NormalLerp")
				
				
				world:GetComponent(id, "MapPosition", 0):SetInt2(tPosX, tPosZ)
				
				canMove = true
				break
				
			end
			
			
		end
		
	until canMove
	
end

WeatherTornadoSystem.CheckCollision = function(self, id, unit)
	
	local posX, posZ = world:GetComponent(id, "MapPosition", 0):GetInt2()
	
	local playerX, playerZ = world:GetComponent(unit, "MapPosition", 0):GetInt2()
	
	if posX == playerX and posZ == playerZ then
		print("COLLISION WITH A PLAYER. YOU SPIN ME RIGHT ROUND, BABY RIGHT ROUND!")
		
		if world:EntityHasComponent(unit, "ActionGuard") then
			-- SOUND
			local audioId = Net.StartPack("Client.PlaySoundC")
			Net.WriteString(audioId, "BlockVoice" .. math.random(1, 3))
			Net.WriteString(audioId, "BlockVoice" .. unit)
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
			local px, py, pz = world:GetComponent(unit, "Position", 0):GetFloat3()
			audioId = Net.StartPack("Client.SetSoundPosition")
			Net.WriteString(audioId, "BlockVoice" .. unit)
			Net.WriteFloat(audioId, px)
			Net.WriteFloat(audioId, py)
			Net.WriteFloat(audioId, pz)
			Net.Broadcast(audioId)
			audioId = Net.StartPack("Client.SetSoundVolume")
			Net.WriteString(audioId, "BlockVoice" .. unit)
			Net.WriteInt(audioId, 128)
			Net.Broadcast(audioId)
			return
		end
		
		if not world:EntityHasComponent(unit, "Spin") then
		
			world:CreateComponentAndAddTo("Spin", unit)
			world:GetComponent(unit, "Spin", 0):SetFloat3(0, 40, 0)
			
			if not world:EntityHasComponent(unit, "LerpPosition") then
				world:CreateComponentAndAddTo("LerpPosition", unit)
			end
			world:GetComponent(unit, "LerpPosition", "X"):SetFloat(posX)
			world:GetComponent(unit, "LerpPosition", "Y"):SetFloat(1.5)
			world:GetComponent(unit, "LerpPosition", "Z"):SetFloat(posZ)
			world:GetComponent(unit, "LerpPosition", "Time"):SetFloat(0.5)
			world:GetComponent(unit, "LerpPosition", "Algorithm"):SetText("NormalLerp")

			local dirRng = math.random(4)
			local dirX = 0
			local dirZ = 0

			if dirRng == 1 then -- 90°
				dirX = 1
				dirZ = 0
			elseif dirRng == 2 then -- 270°
				dirX = -1
				dirZ = 0
			elseif dirRng == 3 then -- 0°
				dirX = 0
				dirZ = 1
			elseif dirRng == 4 then -- 180°
				dirX = 0
				dirZ = -1
			end

			world:GetComponent(unit, "Direction", 0):SetInt2(dirX,dirZ)

		end
	end
	
	
end

WeatherTornadoSystem.CancelTornado = function(self, id)

	world:CreateComponentAndAddTo("LerpScale", id)
	world:GetComponent(id, "LerpScale", "X"):SetFloat(0.0)
	world:GetComponent(id, "LerpScale", "Y"):SetFloat(0.0)
	world:GetComponent(id, "LerpScale", "Z"):SetFloat(0.0)
	world:GetComponent(id, "LerpScale", "Time"):SetFloat(1.0)
	world:GetComponent(id, "LerpScale", "Algorithm"):SetText("NormalLerp")
	world:CreateComponentAndAddTo("KillAfterLerp", id)
	
end
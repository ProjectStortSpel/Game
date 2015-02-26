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
	self:AddComponentTypeToFilter("NewStep", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealCards", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("WeatherTornado", FilterType.RequiresOneOf)
	--self:AddComponentTypeToFilter("TestMoveSuccess",FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	
	-- Tiles
	self:AddComponentTypeToFilter("TileComp",FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NotWalkable",FilterType.Excluded)
	self:AddComponentTypeToFilter("Void",FilterType.Excluded)
	
end

WeatherTornadoSystem.EntitiesAdded = function(self, dt, newEntities)


	for n = 1, #newEntities do
		local	tEntity	=	newEntities[n]
		
		if world:EntityHasComponent(tEntity, "WeatherTornado") then
			
			for i = 1, self.NoTornados do
				self.TornadoIds[#self.TornadoIds + 1] = self:AddTornado()
			end
			world:KillEntity(tEntity)
			print("NO TORNADOS: " .. #self.TornadoIds)
		end
		
		if world:EntityHasComponent(tEntity, "NewStep") then
		
			for i = 1, #self.TornadoIds do
				self:MoveTornado(self.TornadoIds[i])
				self:CheckCollision(self.TornadoIds[i])
			end
			
		end
		if world:EntityHasComponent(tEntity, "DealCards") then
			
			for i = 1, #self.TornadoIds do
				self:CancelTornado(self.TornadoIds[i])
				self.TornadoIds[i] = nil
			end
		end
		
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
	world:GetComponent(id, "LerpScale", "X"):SetFloat(1.0)
	world:GetComponent(id, "LerpScale", "Y"):SetFloat(1.0)
	world:GetComponent(id, "LerpScale", "Z"):SetFloat(1.0)
	world:GetComponent(id, "LerpScale", "Time"):SetFloat(2.5)
	world:GetComponent(id, "LerpScale", "Algorithm"):SetText("NormalLerp")
	world:GetComponent(id, "LerpScale", "KillWhenFinished"):SetBool(false)
	
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
				
				world:CreateComponentAndAddTo("LerpPosition", id)
				world:GetComponent(id, "LerpPosition", "X"):SetFloat(tPosX)
				world:GetComponent(id, "LerpPosition", "Y"):SetFloat(0.5)
				world:GetComponent(id, "LerpPosition", "Z"):SetFloat(tPosZ)
				world:GetComponent(id, "LerpPosition", "Time"):SetFloat(2.0)
				world:GetComponent(id, "LerpPosition", "Algorithm"):SetText("NormalLerp")
				world:GetComponent(id, "LerpPosition", "KillWhenFinished"):SetBool(false)
				
				world:GetComponent(id, "MapPosition", 0):SetInt2(tPosX, tPosZ)
				
				canMove = true
				break
				
			end
			
			
		end
		
	until canMove
	
end

WeatherTornadoSystem.CheckCollision = function(self, id)

	local units = self:GetEntities("Unit")
	local posX, posZ = world:GetComponent(id, "MapPosition", 0):GetInt2()

	for i = 1, #units do
		
		local playerX, playerZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
		
		if posX == playerX and posZ == playerZ then
			print("COLLISION WITH A PLAYER. YOU SPIN ME RIGHT ROUND, BABY RIGHT ROUND!")
	
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
	world:GetComponent(id, "LerpScale", "KillWhenFinished"):SetBool(true)
	
end
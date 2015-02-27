VisualizeSelectedCards	=	System()
VisualizeSelectedCards.PlayerEntity	=	-1
VisualizeSelectedCards.GhostEntity	=	-1
VisualizeSelectedCards.PrintSimulation	=	0
VisualizeSelectedCards.MapSizeX	=	0
VisualizeSelectedCards.MapSizeZ	=	0

VisualizeSelectedCards.Initialize = function ( self )
	--	Set Name
	self:SetName("VisualizeSelectedCards")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("CardSelected", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TileComp", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitGhost", FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	
end

VisualizeSelectedCards.EntitiesAdded = function(self, dt, entities)
	
	for n = 1, #entities do
		local	newEntity	=	entities[n]
		if world:EntityHasComponent(newEntity, "CardSelected") then
			
			self:ReVisualizePath()
		end
		
		if world:EntityHasComponent(newEntity, "UnitGhost") then
		
			self.PlayerEntity	=	world:GetComponent(newEntity, "UnitGhost", "Id"):GetInt()
			self.GhostEntity	=	newEntity
			local	R, G, B		=	world:GetComponent(self.PlayerEntity, "Color", 0):GetFloat3()
			world:GetComponent(self.GhostEntity, "Color", "X"):SetFloat3(R, G, B)
			
			--self.GhostEntity	=	world:CreateNewEntity("Ghost")
			--world:CreateComponentAndAddTo("Hide", self.GhostEntity)
			--world:GetComponent(self.GhostEntity, "Model", "ModelName"):SetText("cavemenghost");
			--world:GetComponent(self.GhostEntity, "Model", "ModelPath"):SetText("caveman");
			--world:GetComponent(self.GhostEntity, "Model", "RenderType"):SetInt(1)
			
		end
		
		if world:EntityHasComponent(newEntity, "MapSpecs") then
			local tX, tZ = world:GetComponent(newEntity, "MapSpecs", "SizeX"):GetInt2()
			
			self.MapSizeX = tX
			self.MapSizeZ = tZ
		end
	end
	
	local	nCards	=	self:GetEntities("CardSelected")
	
	if #nCards ~= 0 then
		if world:EntityHasComponent(self.GhostEntity, "Hide") then
			world:RemoveComponentFrom("Hide", self.GhostEntity)
		end
	end

end

VisualizeSelectedCards.EntitiesRemoved = function(self, dt, entities)
	
	for n = 1, #entities do
		
		if world:EntityHasComponent(entities[n], "UnitGhost") then
			self.PlayerEntity	=	-1
			self.GhostEntity	=	-1
		end
	
	end
	
	
	self:ReVisualizePath()
	
	local	nCards	=	self:GetEntities("CardSelected")
	
	if #nCards == 0 and self.GhostEntity ~= -1 then
		if not world:EntityHasComponent(self.GhostEntity, "Hide") then
			world:CreateComponentAndAddTo("Hide", self.GhostEntity)
		end
	end
	
	

end

VisualizeSelectedCards.ReVisualizePath = function(self)

	if self.PlayerEntity == -1 or self.GhostEntity == -1 then
		return
	end

	local	selectedCards	=	self:GetEntities("CardSelected")
	
	if #selectedCards == 0 then
		return
	end
	
	local	cardsToSimulate			=	{}
			cardsToSimulate.__mode	=	"k"
			
	local	indexToFind		=	1
	while true do
		local	indexFound	=	false
		for iCard = 1, #selectedCards do
			local	currentCard		=	selectedCards[iCard]
			local	currentIndex	=	world:GetComponent(currentCard, "SelectCard", "Index"):GetInt()
			if currentIndex == indexToFind then
				indexToFind	=	indexToFind+1
				indexFound	=	true
				
				cardsToSimulate[#cardsToSimulate+1]	=	currentCard
				break
			end
			
		end
		
		if not indexFound then
			break
		end
	end
	
	if #cardsToSimulate > 0 then
		self:VisualizePath(cardsToSimulate)
	end
	
	
end

VisualizeSelectedCards.GetRotation = function(self, dirX, dirZ)

	local	returnRotation	=	0
	if dirX == 1 then
		returnRotation	=	math.pi/2
	end
	if dirX == -1 then
		returnRotation	=	-math.pi/2
	end
	if dirZ == -1 then
		returnRotation	=	-math.pi
	end
	if dirZ == 1 then
		returnRotation	=	0
	end
	
	
	return returnRotation
end

VisualizeSelectedCards.IsInsideWorld = function(self, X, Z)
	if X < 0 or X >= self.MapSizeX then
		return	false
	end
	
	if Z < 0 or Z >= self.MapSizeZ then
		return	false
	end
	
	return 	true
end

VisualizeSelectedCards.VisualizePath = function(self, cardsToVisualize)
	
	local	posX, posZ			=	world:GetComponent(self.PlayerEntity, "MapPosition", "X"):GetInt2()
	local	dirX, dirZ			=	world:GetComponent(self.PlayerEntity, "Direction", "X"):GetInt2()
	local	X, Y, Z				=	world:GetComponent(self.PlayerEntity, "Position", "X"):GetFloat3()
	local	rotX, rotY, rotZ	=	world:GetComponent(self.PlayerEntity, "Rotation", "X"):GetFloat3()

	local	fellDown	=	false
	fellDown, posX, posZ, dirX, dirZ	=	self:SimulateCardsFromPos(self.PlayerEntity, posX, posZ, dirX, dirZ, cardsToVisualize)
	
	if self:IsInsideWorld(posX, posZ) then
		local	tTile	=	self:GetEntities("TileComp")[posZ * self.MapSizeZ + posX+1]
		if world:EntityHasComponent(tTile, "TileOffset") then
			Y	=	world:GetComponent(tTile, "TileOffset", "Offset"):GetFloat()
		end
	else
		Y	=	-50
	end
	
	local	finalRotation	=	self:GetRotation(dirX, dirZ)
	world:GetComponent(self.GhostEntity, "Position", "X"):SetFloat3(posX, Y, posZ)
	world:GetComponent(self.GhostEntity, "Direction", "X"):SetInt2(dirX, dirZ)
	world:GetComponent(self.GhostEntity, "Rotation", 0):SetFloat3(rotX, finalRotation, rotZ)
end

Net.Receive("Client.GetPlayerEntityId", 
	function( id, ip, port )
		
		local	serverId	=	Net.ReadInt(id)
		
		print("SERVER: " .. serverId)
		
		local	newId	=	world:CreateNewEntity("Ghost")
		world:CreateComponentAndAddTo("UnitGhost", newId)
		world:GetComponent(newId, "UnitGhost", "Id"):SetInt(serverId)
		
		world:CreateComponentAndAddTo("Hide", newId)
		world:GetComponent(newId, "Model", "ModelName"):SetText("cavemenghost");
		world:GetComponent(newId, "Model", "ModelPath"):SetText("caveman");
		world:GetComponent(newId, "Model", "RenderType"):SetInt(1)
	end 
)










VisualizeSelectedCards.SimulateCardsFromPos = function(self, _unit, _posX, _posY, _dirX, _dirY, _pickedcards)
	
	local posX, posY, dirX, dirY = _posX, _posY, _dirX, _dirY
	
	local fellDown = false
	
	if self.PrintSimulation == 1 then
		print()
		print("----------------- NEW SIMULATION STARTED --------------------")
	end
	
	local cardName
	
	for i = 1, #_pickedcards do
		
		if type(_pickedcards[i]) == "string" then
			cardName = _pickedcards[i]
		else
			cardName = world:GetComponent(_pickedcards[i], "CardAction", 0):GetText()
		end
		
		if self.PrintSimulation == 1 then
			print()
			print("Card number:", i, "is", cardName)
			print()
		end
		
		if cardName == "Forward" or cardName == "Stone" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 1, false)
			
		elseif cardName == "Backward" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, false, false, 1, false)
			
		elseif cardName == "TurnLeft" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 1)
			
		elseif cardName == "TurnRight" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 3)
			
		elseif cardName == "TurnAround" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 2)
			
		elseif cardName == "Guard" or cardName == "SlingShot" then
			
			fellDown, posX, posY, dirX, dirY = self:SimulateTurnLeft(posX, posY, dirX, dirY, 0)
			
		elseif cardName == "Sprint" then
			
			fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, false, 2, false)
			
		--elseif cardName == "Leap" then
		--	
		--	fellDown, posX, posY = self:SimulateMoveForward(posX, posY, dirX, dirY, true, true, 2, false)
			
		else
		
			print("ERROR: CARD NOT ADDED IN SIMULATE CARDS", cardName)
		end
	end
	
	if self.PrintSimulation == 1 then
		print("I will end up at:", posX, posY, "with dir:", dirX, dirY)
	end
	
	return fellDown, posX, posY, dirX, dirY
end

VisualizeSelectedCards.SimulateMoveForward = function(self, _posX, _posY, _dirX, _dirY, _forwards, _jump, _iterations, _riverMove)
	
	local forward = -1
	if _forwards then
		forward = 1
	end
	
	local fellDown = false
	local posX = _posX
	local posY = _posY
				
	for i = 1, _iterations do
		
		posX = posX + _dirX * forward
		posY = posY + _dirY * forward
		
		if self:TileHasComponent("NotWalkable", posX, posY) then
			
			posX = posX - _dirX * forward
			posY = posY - _dirY * forward
			
		elseif self:TileHasComponent("Void", posX, posY) and not _jump then
			
			fellDown = true
			if self.PrintSimulation == 1 then
				print("I will fall down in", posX, posY)
			end
			break
		end
				
		if not _riverMove and i == _iterations and self:TileHasComponent("River", posX, posY) then
			
			local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
			
			for j = 1, waterSpeed do
			
				if self:TileHasComponent("River", posX + waterDirX, posY + waterDirY) then
					posX = posX + waterDirX
					posY = posY + waterDirY
					waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(posX, posY)
				else
					fellDown, posX, posY = self:SimulateMoveForward(posX, posY, waterDirX, waterDirY, true, false, 1, true)
					break
				end
				
				if self.PrintSimulation == 1 then
					print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
				end
			end
		end
	end
	
	if not _riverMove and self.PrintSimulation == 1 then
		print("Pos", posX, posY, "Forw:", _forwards)
	end
	
	return fellDown, posX, posY
end

VisualizeSelectedCards.SimulateTurnLeft = function(self, _posX, _posY, _dirX, _dirY, _iterations)
	
	local dirX = _dirX
	local dirY = _dirY
	local temp
	local posX = _posX
	local posY = _posY
	local fellDown = false
	
	for i = 1, _iterations do
		
		temp = dirY
		dirY = -dirX
		dirX = temp
	end
	
	if self:TileHasComponent("River", _posX, _posY) then
		
		local waterDirX, waterDirY, waterSpeed = self:GetRiverVariables(_posX, _posY)
		
		if self.PrintSimulation == 1 then
			print("I will move in river with X, Y, speed:", waterDirX, waterDirY, waterSpeed)
		end
		
		fellDown, posX, posY = self:SimulateMoveForward(_posX, _posY, waterDirX, waterDirY, true, false, waterSpeed, true)
		
		if self.PrintSimulation == 1 then
			print("Pos", posX, posY, "Dir", dirX, dirY, "with", _iterations)
		end
	elseif self.PrintSimulation == 1 then
		print("Dir", dirX, dirY, "with", _iterations, "turn left iterations")
	end
	
	return fellDown, posX, posY, dirX, dirY
end

VisualizeSelectedCards.TileHasComponent = function(self, _component, _posX, _posY)
	
	local mapSize = self:GetEntities("MapSpecs")
	local mapSizeComp = world:GetComponent(mapSize[1], "MapSpecs", "SizeX")
	local mapX, mapY = mapSizeComp:GetInt2()
	local tiles = self:GetEntities("TileComp")
	local returnValue
	
	if -1 < _posX and _posX < mapX and -1 < _posY and _posY < mapY then
		returnValue = world:EntityHasComponent(tiles[mapX * _posY + _posX + 1], _component)
	else
		print("ERROR, trying to get entity from tile outside the boundaries in AICardPickingSystem.TileHasComponent.")
		returnValue = false
	end
	
	return returnValue
end

VisualizeSelectedCards.GetRiverVariables = function(self, _posX, _posY)
	
	local mapSize = self:GetEntities("MapSpecs")
	local mapX = world:GetComponent(mapSize[1], "MapSpecs", "SizeX"):GetInt()
	local tiles = self:GetEntities("TileComp")
	local dirX, dirY, speed = world:GetComponent(tiles[mapX * _posY + _posX + 1], "River", 0):GetInt3()
	
	return dirX, dirY, speed
end
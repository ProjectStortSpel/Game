
CardDeckSystem = System()

CardDeckSystem.CardDeck = { }
CardDeckSystem.DealtCards = { }
CardDeckSystem.NR_OF_CARDS_PER_PLAYER = 8
CardDeckSystem.NR_OF_PLAYERS = 4

CardDeckSystem.Update = function( self, dt )
	
	if GameRunning == false then
		return;
	end
	
end

CardDeckSystem.PostInitialize = function ( self )

	self:CreateDeck()
	
	print("Card Deck System Initialized")
end

CardDeckSystem.CreateDeck = function (self)

	local prio = 0
	
	local mySeed = os.time() - 1418742000 -- dont ask
	
	math.randomseed(mySeed)
	
	for i = 1, 50 * self.NR_OF_PLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		
		local rand = math.random(1, 90)
		
		if rand < 30 then
			cardactioncomp:SetString("Forward")
		elseif rand < 50 then
			cardactioncomp:SetString("Backward")
		elseif rand < 65 then
			cardactioncomp:SetString("TurnRight")
		elseif rand < 80 then
			cardactioncomp:SetString("TurnLeft")
		elseif rand < 90 then
			cardactioncomp:SetString("TurnAround")
		else
			cardactioncomp:SetString("Forward")
			
		end
		
		table.insert(self.CardDeck, entity)
	end
end

CardDeckSystem.GetCards = function (self, noOfCards)
	
	local set = {}
	
	for i = 1, noOfCards do
		local card = self.CardDeck[1]
		set[i] = card
		--DeltCards[]
		table.insert(self.DealtCards, card)
		table.remove(self.CardDeck, 1)
	end
	
	return set
end

--GenCarDeck = function(  )
--	local prio = 0
--	-- forward
--	for i = 1, 10*NROFPLAYERS do
--		prio = prio + 1
--		self:GetComponent(entity, "Position", 0)
--		local entity = world:CreateNewEntity()
--		local entity = world:CreateNewEntity("Player")
--		world:CreateComponentAndAddTo("Spawn", entity)
--		world:CreateComponentAndAddTo("SyncNetwork", entity)
--		local mapPos = {i,12}
--		--local mapPos = {i, 6}
--		self:SetPosition(entity, mapPos[1], 1.0, mapPos[2])
--		local comp = self:GetComponent(entity, "Spawn", 0)
--		comp:SetInt2(mapPos[1], mapPos[2])
--		
--	end
--	
--	--backward
--	
--	--turnright
--	
--	--turnleft
--
--end





---------------------------- ForwardSystem

ForwardSystem = System()

ForwardSystem.Initialize = function(self)
	self:SetName("Forward System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Forward",FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	print("ForwardSystem initialized!")
end

ForwardSystem.OnEntityAdded = function(self, entity)

		if GameRunning == false then
		return;
	end

	local dir = self:GetComponent(entity, "Direction", 0)
	local mapPos = self:GetComponent(entity, "MapPosition", 0)
	
	local x, y = mapPos:GetInt2()
	local dirX, dirY = dir:GetInt2()
	
	local targetX = x + dirX
	local targetY = y + dirY
	
	local moved = PlayerMovementSystem:MoveTo(entity, targetX, targetY, dirX, dirY)
	
	world:RemoveComponentFrom("Forward", entity);
end


---------------------------- BackwardSystem

BackwardSystem = System()

BackwardSystem.Initialize = function(self)
	self:SetName("Backward System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Backward",FilterType.Mandatory)
	
	self:AddComponentTypeToFilter("TargetPosition",FilterType.Excluded)
	print("BackwardSystem initialized!")
end

BackwardSystem.OnEntityAdded = function(self, entity)

	if GameRunning == false then
		return;
	end

	local dir = self:GetComponent(entity, "Direction", 0)
	local mapPos = self:GetComponent(entity, "MapPosition", 0)
	
	local x, y = mapPos:GetInt2()
	local dirX, dirY = dir:GetInt2()
	
	local targetX = x - dirX
	local targetY = y - dirY
	
	local moved = PlayerMovementSystem:MoveTo(entity, targetX, targetY, -dirX, -dirY)
	
	world:RemoveComponentFrom("Backward", entity);
end

---------------------------- RightTurnSystem
RightTurnSystem = System()

RightTurnSystem.Initialize = function(self)
	self:SetName("Right Turn System")
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnRight",FilterType.Mandatory)
	
	print("RightTurnSystem initialized!")
end

RightTurnSystem.OnEntityAdded = function(self, entity)

	if GameRunning == false then
		return;
	end

	local dir = self:GetComponent(entity, "Direction", 0)
	local rot = self:GetComponent(entity, "Rotation", 4)
	local dx, dy = dir:GetInt2()
	local roty = rot:GetFloat()
	
	local tempdy = dx
	dx = -dy
	dy = tempdy
	
	dir:SetInt2(dx, dy)
	rot:SetFloat(roty - math.pi/2)
	
	world:RemoveComponentFrom("TurnRight", entity);
end

---------------------------- LeftTurnSystem

LeftTurnSystem = System()

LeftTurnSystem.Initialize = function(self)
	self:SetName("Left Turn System")
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnLeft",FilterType.Mandatory)
	
	print("LeftTurnSystem initialized!")
end

LeftTurnSystem.OnEntityAdded = function(self, entity)

	if GameRunning == false then
		return;
	end

	local dir = self:GetComponent(entity, "Direction", 0)
	local rot = self:GetComponent(entity, "Rotation", 4)
	local dx, dy = dir:GetInt2()
	local roty = rot:GetFloat()
	local tempdy = dx
	dx = dy
	dy = -tempdy
	
	dir:SetInt2(dx, dy)
	rot:SetFloat(roty + math.pi/2)
	
	world:RemoveComponentFrom("TurnLeft", entity);
end

---------------------------- TurnAroundSystem

TurnAroundSystem = System()

TurnAroundSystem.Initialize = function(self)
	self:SetName("Turn Around System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Direction",FilterType.Mandatory)
	self:AddComponentTypeToFilter("TurnAround",FilterType.Mandatory)
	
	print("TurnAroundSystem initialized!")
end

TurnAroundSystem.OnEntityAdded = function(self, entity)

	if GameRunning == false then
		return;
	end

	local dir = self:GetComponent(entity, "Direction", 0)
	local rot = self:GetComponent(entity, "Rotation", 4)
	local dx, dy = dir:GetInt2()
	local roty = rot:GetFloat()
	
	dx = -dx
	dy = -dy
	
	dir:SetInt2(dx, dy)
	rot:SetFloat(roty + math.pi)
	
	world:RemoveComponentFrom("TurnAround", entity);
end



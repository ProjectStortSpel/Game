CardDeck = { }
NROFPLAYERS = 4

CardDeckSystem = System()

CardDeckSystem.Update = function( self, dt )
	
end

CardDeckSystem.PostInitialize = function ( self )
	
	
	local prio = 0
	math.randomseed(os.time())
	
	for i = 1, 10*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("Forward")
		
		if i > 1 then
			table.insert(CardDeck, math.random(1, #CardDeck), entity)
		else
			table.insert(CardDeck, entity)
		end
	end
	
	for i = 1, 8*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("Backward")
		
		table.insert(CardDeck, math.random(1, #CardDeck), entity)
	end
	
	for i = 1, 5*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("TurnLeft")
		
		table.insert(CardDeck, math.random(1, #CardDeck), entity)
	end
	
	for i = 1, 5*NROFPLAYERS do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		cardactioncomp:SetString("TurnRight")
		
		table.insert(CardDeck, math.random(1, #CardDeck), entity)
	end
	
	for i = 1, #CardDeck do
		local entity = CardDeck[i]
		--local cardpriocomp = self:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = self:GetComponent( entity, "CardAction", 0)
		--local cardprio = cardpriocomp:GetInt()
		local cardaction = cardactioncomp:GetString()
		Console.Print(cardaction)
	end
	
	print("Card Deck System Initialized")
end

CardDeckSystem.GetCards = function (self, noOfCards)
	
	local set = {}
	
	for i = 1, noOfCards do
		local card = CardDeck[i]
		table.insert(set, card)
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



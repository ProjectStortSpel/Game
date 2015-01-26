CreateDeckSystem = System()

CreateDeckSystem.Initialize = function ( self )
	self:SetName("CreateDeckSystem")
	
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CreateDeck", FilterType.RequiresOneOf)
end


CreateDeckSystem.OnEntityAdded = function(self, entityId)
		
	if world:EntityHasComponent( entityId, "CreateDeck") then
		self:CreateDeck()
		world:KillEntity( entityId )

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("DealCards", id)
		world:SetComponent(id, "DealCards", "NumCards", 8)

	end
	
end

CreateDeckSystem.CreateDeck = function (self)
	
	local prio = 0
	
	local mySeed = os.time() - 1418742000 -- dont ask
	
	local test = 0
	math.randomseed(mySeed)
	for i = 1, 100 * #self:GetEntities("Unit") do
		
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

		test = test + 1

	end

	print("Created " .. test .. " cards!")
end

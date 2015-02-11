CreateDeckSystem = System()

CreateDeckSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CreateDeckSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Unit", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CreateDeck",	FilterType.RequiresOneOf)
end


CreateDeckSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
	
		local entityId = entities[n]
		if world:EntityHasComponent( entityId, "CreateDeck") then
			self:CreateDeck()
			world:KillEntity( entityId )

			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("DealCards", id)
			world:SetComponent(id, "DealCards", "NumCards", 8)
			print("DealCards id = " .. id)
		end
	end
end

CreateDeckSystem.CreateDeck = function (self)
	
	local prio = 0
	
	local mySeed = os.time() - 1418742000 -- dont ask
	
	math.randomseed(mySeed)
	for i = 1, 100 * #self:GetEntities("Unit") do
		
		prio = prio + 1
		local entity = world:CreateNewEntity("Card")
		
		local cardpriocomp = world:GetComponent(entity, "CardPrio", 0)
		local cardactioncomp = world:GetComponent( entity, "CardAction", 0)
		
		cardpriocomp:SetInt(prio)
		
		local rand = math.random(1, 109)
		
		if rand < 30 then
			cardactioncomp:SetString("Forward") -- 0-29
		elseif rand < 50 then
			cardactioncomp:SetString("Backward") -- 30-49
		elseif rand < 65 then
			cardactioncomp:SetString("TurnRight") -- 50-64
		elseif rand < 80 then
			cardactioncomp:SetString("TurnLeft") -- 65-79
		elseif rand < 90 then
			cardactioncomp:SetString("TurnAround") -- 80-89
		elseif rand < 100 then
			cardactioncomp:SetString("Sprint")	-- 90-99
		elseif rand < 110 then
			cardactioncomp:SetString("Sprint")	-- 100-109 
		else
			cardactioncomp:SetString("Forward")
			
		end

	end

end

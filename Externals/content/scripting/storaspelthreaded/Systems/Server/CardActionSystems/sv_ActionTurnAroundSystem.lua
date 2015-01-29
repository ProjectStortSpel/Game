ActionTurnAroundSystem = System()

ActionTurnAroundSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ActionTurnAroundSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnAround",FilterType.Mandatory)
end

ActionTurnAroundSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	print("TURN AROUND Number of entities: " .. #entities)
	for n = 1, #entities do
		local entity = entities[n]
		local dir = world:GetComponent(entity, "Direction", 0)
		local x, z = dir:GetInt2()
		x = x * -1
		z = z * -1
		dir:SetInt2(x, z)

		

		--rotate model
		local rotY = world:GetComponent(entity, "Rotation", "Y"):GetFloat()

		if	x == 0 and z == 1 then
			rotY = 0
		elseif  x == 0 and z == -1 then
			rotY = math.pi
		elseif  x == 1 then		
			rotY = math.pi / 2
		elseif  x == -1 then
			rotY = -math.pi / 2 
		end

		world:GetComponent(entity, "Rotation", "Y"):SetFloat(rotY)

		world:RemoveComponentFrom("UnitTurnAround", entity)
	end
end
ActionTurnLeftSystem = System()

ActionTurnLeftSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ActionTurnLeftSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("Unit",			FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnLeft",	FilterType.Mandatory)
end

ActionTurnLeftSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
	print("TURN LEFT Number of entities: " .. #entities)
	for n = 1, #entities do
		local entity = entities[n]
		local dir = world:GetComponent(entity, "Direction", 0)
		local x, z = dir:GetInt2()

		if	x == 0 and z == 1 then
			x = 1
			z = 0
			rotY = math.pi / 2
		elseif  x == 0 and z == -1 then
			x = -1
			z = 0
			rotY = -math.pi / 2
		elseif  x == 1 then
			x = 0
			z = -1
			rotY = math.pi
		elseif  x == -1 then
			x = 0
			z = 1
			rotY = 0
		end


		dir:SetInt2(x, z)
		world:GetComponent(entity, "Rotation", "Y"):SetFloat(rotY)

		world:RemoveComponentFrom("UnitTurnLeft", entity)
	end
end
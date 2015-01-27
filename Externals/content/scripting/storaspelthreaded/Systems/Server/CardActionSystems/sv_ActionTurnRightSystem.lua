ActionTurnRightSystem = System()

ActionTurnRightSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ActionTurnRightSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("Unit",			FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnRight",	FilterType.Mandatory)
end

ActionTurnRightSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local dir = world:GetComponent(entity, "Direction", 0)
		local x, z = dir:GetInt2()

		if	x == 0 and z == 1 then
			x = -1
			z = 0
			rotY = -math.pi / 2
		elseif  x == 0 and z == -1 then
			x = 1
			z = 0
			rotY = math.pi / 2
		elseif  x == 1 then
			x = 0
			z = 1
			rotY = 0
		elseif  x == -1 then
			x = 0
			z = -1
			rotY = math.pi
		end


		dir:SetInt2(x, z)
		world:GetComponent(entity, "Rotation", "Y"):SetFloat(rotY)

		world:RemoveComponentFrom("UnitTurnRight", entity)
	end
end
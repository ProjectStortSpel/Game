
TurnLeftSystem = System()

TurnLeftSystem.Initialize = function(self)
	--	Set Name
	self:SetName("TurnLeftSystem")
	
	--	Toggle EntitiesAdded and Update
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Unit",FilterType.Mandatory)
	self:AddComponentTypeToFilter("UnitTurnLeft",FilterType.Mandatory)
end

TurnLeftSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		local dir = world:GetComponent(entity, "Direction", 0)
		local rot = world:GetComponent(entity, "Rotation", 0)
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
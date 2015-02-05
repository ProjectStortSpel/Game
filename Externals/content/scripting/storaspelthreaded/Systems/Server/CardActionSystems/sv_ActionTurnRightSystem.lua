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
		if world:EntityHasComponent(entity, "Stunned") then
			world:SetComponent(entity, "NoSubSteps", "Counter", 1)
			world:RemoveComponentFrom("UnitTurnRight", entity)
			print("I AM A STUNNED UNIT WITH ID: " .. entity)
			return
		end
		
		local dir = world:GetComponent(entity, "Direction", 0)
		local x, z = dir:GetInt2()
		local temp = x
		x = -z
		z = temp
		dir:SetInt2(x, z)
		
		local rot = world:GetComponent(entity, "Rotation", 0)
		local rot_x, rot_y, rot_z = rot:GetFloat3()

		if not world:EntityHasComponent(entity, "LerpRotation") then
			world:CreateComponentAndAddTo("LerpRotation", entity)
		end
		world:GetComponent(entity, "LerpRotation", "X"):SetFloat(rot_x)
		world:GetComponent(entity, "LerpRotation", "Y"):SetFloat(rot_y - math.pi/2)
		world:GetComponent(entity, "LerpRotation", "Z"):SetFloat(rot_z)
		world:GetComponent(entity, "LerpRotation", "Time"):SetFloat(1)
		world:GetComponent(entity, "LerpRotation", "Algorithm"):SetString("SmoothLerp")
		
		world:SetComponent(entity, "NoSubSteps", "Counter", 1)
		world:RemoveComponentFrom("UnitTurnRight", entity)

	end
end
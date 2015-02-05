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

		print("ha")
		local dir = world:GetComponent(entity, "Direction", 0)
		local rot = world:GetComponent(entity, "Rotation", 0)
		print("ha1")
		local rot_x, rot_y, rot_z = rot:GetFloat3()

		if not world:EntityHasComponent(entity, "SlerpRotation") then
			world:CreateComponentAndAddTo("SlerpRotation", entity)
		end 
		
		local x, z = dir:GetInt2()
		print("hi")
		world:GetComponent(entity, "SlerpRotation", "fromX"):SetFloat( 0 )
		world:GetComponent(entity, "SlerpRotation", "fromY"):SetFloat( 1 )
		world:GetComponent(entity, "SlerpRotation", "fromZ"):SetFloat( 0 )
		print("hi")
		if	x == 0 and z == 1 then
			world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(0)
			x = 1
			z = 0
			rotY = math.pi / 2
		elseif  x == 0 and z == -1 then
			world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(math.pi)
			x = -1
			z = 0
			rotY = -math.pi / 2
		elseif  x == 1 then
			world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(math.pi / 2)
			x = 0
			z = -1
			rotY = math.pi
		elseif  x == -1 then
			world:GetComponent(entity, "SlerpRotation", "fromW"):SetFloat(-math.pi / 2)
			x = 0
			z = 1
			rotY = 0
		end
		
		world:GetComponent(entity, "SlerpRotation", "toX"):SetFloat( 0 )
		world:GetComponent(entity, "SlerpRotation", "toY"):SetFloat( 1 )
		world:GetComponent(entity, "SlerpRotation", "toZ"):SetFloat( 0 )
		world:GetComponent(entity, "SlerpRotation", "toW"):SetFloat(math.pi/2)
		
		dir:SetInt2(x, z)
		world:SetComponent(entity, "NoSubSteps", "Counter", 1)
		world:RemoveComponentFrom("UnitTurnLeft", entity)
		
	end
end
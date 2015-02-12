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

	--print("TURN AROUND Number of entities: " .. #entities)
	for i = 1, #entities do
		if world:EntityHasComponent(entities[i], "Stunned") then
			world:SetComponent(entities[i], "NoSubSteps", "Counter", 1)
			world:RemoveComponentFrom("UnitTurnAround", entities[i])
			print("I AM A STUNNED UNIT WITH ID: " .. entities[i])
			return
		end
		
		local dir = world:GetComponent(entities[i], "Direction", 0)
		local x, z = dir:GetInt2()
		x = x * -1
		z = z * -1
		dir:SetInt2(x, z)
		local rot = world:GetComponent(entities[i], "Rotation", 0)
		
		local rot_x, rot_y, rot_z = rot:GetFloat3()
		
		--if not world:EntityHasComponent(entities[i], "SlerpRotation") then
		--	world:CreateComponentAndAddTo("SlerpRotation", entities[i])
		--end 
		--
		--local rotY = world:GetComponent(entities[i], "Rotation", "Y"):GetFloat()
		--print("hi")
		--world:GetComponent(entities[i], "SlerpRotation", "fromX"):SetFloat( 0 )
		--world:GetComponent(entities[i], "SlerpRotation", "fromY"):SetFloat( 1 )
		--world:GetComponent(entities[i], "SlerpRotation", "fromZ"):SetFloat( 0 )
		--print("hi")
		--if	x == 0 and z == 1 then
		--	world:GetComponent(entities[i], "SlerpRotation", "fromW"):SetFloat( math.pi )
		--	rotY = 0
		--elseif  x == 0 and z == -1 then
		--	world:GetComponent(entities[i], "SlerpRotation", "fromW"):SetFloat( 0 )
		--	rotY = math.pi
		--elseif  x == 1 then		
		--	world:GetComponent(entities[i], "SlerpRotation", "fromW"):SetFloat( -math.pi / 2 )
		--	rotY = math.pi / 2
		--elseif  x == -1 then
		--	world:GetComponent(entities[i], "SlerpRotation", "fromW"):SetFloat( math.pi / 2 )
		--	rotY = -math.pi / 2 
		
		if not world:EntityHasComponent(entities[i], "LerpRotation") then
			world:CreateComponentAndAddTo("LerpRotation", entities[i])
		end
		world:GetComponent(entities[i], "LerpRotation", "X"):SetFloat(rot_x)
		world:GetComponent(entities[i], "LerpRotation", "Y"):SetFloat(rot_y + math.pi)
		world:GetComponent(entities[i], "LerpRotation", "Z"):SetFloat(rot_z)
		world:GetComponent(entities[i], "LerpRotation", "Time"):SetFloat(1)
		world:GetComponent(entities[i], "LerpRotation", "Algorithm"):SetText("SmoothLerp")
		
		world:SetComponent(entities[i], "NoSubSteps", "Counter", 1)
		world:RemoveComponentFrom("UnitTurnAround", entities[i])
	end
end
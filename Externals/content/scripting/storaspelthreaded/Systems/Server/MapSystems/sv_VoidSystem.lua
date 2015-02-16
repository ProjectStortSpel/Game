VoidSystem = System()

VoidSystem.Initialize = function(self)

	--	Set Name
	self:SetName("VoidSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckVoid", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Void", FilterType.RequiresOneOf)
end

VoidSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
	
		local entity = entities[n]
		if world:EntityHasComponent( entity, "CheckVoid") then

			local units = self:GetEntities("Unit")
			local voids = self:GetEntities("Void")

			for i = 1, #units do
				
				-- Get the MapPosition of the unit
				local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2() 
				-- Get the number of substeps
				local noSteps = world:GetComponent(units[i], "NoSubSteps", 0):GetInt()
				-- Get the direction of the unit
				local dirX, dirZ = world:GetComponent(units[i], "Direction", 0):GetInt2()
				
				-- Normalize the direction
				if dirX ~= 0 then
					dirX  = dirX / math.abs(dirX)
				end
				if dirZ ~= 0 then
					dirZ = dirZ / math.abs(dirZ)
				end

				-- Go through all substeps
				local hitVoid = false
				for tmp = noSteps - 1, 0, -1 do 
				
					if hitVoid then
						break
					end
				
					local tmpX = unitX - (dirX * tmp)
					local tmpZ = unitZ - (dirZ * tmp)
					
					-- Go through all voids
					for j = 1, #voids do
					
						-- Get the current voids' MapPosition
						local voidX, voidZ = world:GetComponent(voids[j], "MapPosition", 0):GetInt2() 

						-- If the voids' position is the same as the unit
						if tmpX == voidX and tmpZ == voidZ then
							-- Add Unit dead component
							if not world:EntityHasComponent(units[i], "UnitDead") then
								world:CreateComponentAndAddTo("UnitDead", units[i])
							end
							
							--	Spawn rating entity
						
							local	unitDied	=	world:CreateNewEntity()
							world:CreateComponentAndAddTo("UnitDied", unitDied)
							world:GetComponent(unitDied, "UnitDied", "PlayerNumber"):SetInt(world:GetComponent(units[i], "PlayerNumber", 0):GetInt())
							world:GetComponent(unitDied, "UnitDied", "X"):SetInt(voidX)
							world:GetComponent(unitDied, "UnitDied", "Z"):SetInt(voidZ)
							
							--	Move the unit
							if not world:EntityHasComponent(units[i], "LerpPosition") then
								world:CreateComponentAndAddTo("LerpPosition", units[i])
							end
							world:GetComponent(units[i], "LerpPosition", "X"):SetFloat(tmpX)
							world:GetComponent(units[i], "LerpPosition", "Y"):SetFloat(-10)
							world:GetComponent(units[i], "LerpPosition", "Z"):SetFloat(voidZ)
							world:GetComponent(units[i], "LerpPosition", "Time"):SetFloat(1)
							world:GetComponent(units[i], "LerpPosition", "Algorithm"):SetText("ExponentialLerp")
							
							hitVoid = true
							
							break
						end

					end	
				end
			end
			
			world:KillEntity( entity )
		end
	end
end

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
	self:AddComponentTypeToFilter("FallDownSound", FilterType.RequiresOneOf)
end

VoidSystem.EntitiesAdded = function(self, dt, entities)

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
								print("UNIT IS NOW DEAD!")
								world:CreateComponentAndAddTo("UnitDead", units[i])
								
								--	Spawn rating entity
								local	unitDied	=	world:CreateNewEntity()
								world:CreateComponentAndAddTo("UnitDied", unitDied)
								world:GetComponent(unitDied, "UnitDied", "PlayerNumber"):SetInt3(world:GetComponent(units[i], "PlayerNumber", 0):GetInt(), voidX, voidZ)
							end
							

							
							--	Move the unit
							if not world:EntityHasComponent(units[i], "LerpPosition") then
								world:CreateComponentAndAddTo("LerpPosition", units[i])
								world:CreateComponentAndAddTo("AddEntityAfterLerp", units[i])
							end
							world:GetComponent(units[i], "LerpPosition", "Time"):SetFloat4(1, tmpX, -10, voidZ)
							world:GetComponent(units[i], "LerpPosition", "Algorithm"):SetText("ExponentialLerp")
							
							world:GetComponent(units[i], "AddEntityAfterLerp", "ComponentName"):SetText("FallDownSound")
							
							if not world:EntityHasComponent(units[i], "Animation") then
								world:CreateComponentAndAddTo("Animation", units[i])
							end
							world:GetComponent(units[i], "Animation", "Id"):SetInt(7)
							world:GetComponent(units[i], "Animation", "FrameTime"):SetFloat(0.01)
							world:GetComponent(units[i], "Animation", "Time"):SetFloat(1)
							

							hitVoid = true
							
							break
						end

					end	
				end
			end
			
			world:KillEntity( entity )
			
		elseif world:EntityHasComponent( entity, "FallDownSound") then
		
			local x, y, z = world:GetComponent(entity, "FallDownSound", 0):GetFloat3()
		
			local audioId = Net.StartPack("Client.PlaySoundC")
			Net.WriteString(audioId, "FallingDown" .. math.random(1,3))
			Net.WriteString(audioId, "FallDownChannel")
			Net.WriteBool(audioId, false)
			Net.Broadcast(audioId)
			
			audioId = Net.StartPack("Client.SetSoundVolume")
			Net.WriteString(audioId, "FallDownChannel")
			Net.WriteInt(audioId, 20)
			Net.Broadcast(audioId)
			
			audioId = Net.StartPack("Client.SetSoundPosition")
			Net.WriteString(audioId, "FallDownChannel")
			Net.WriteFloat(audioId, x)
			Net.WriteFloat(audioId, y)
			Net.WriteFloat(audioId, z)
			Net.Broadcast(audioId)
			
			world:KillEntity( entity )
			
		end
	end
end
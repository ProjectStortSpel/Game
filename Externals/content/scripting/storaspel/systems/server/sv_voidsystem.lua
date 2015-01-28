VoidSystem = System()

VoidSystem.Initialize = function(self)
	self:SetName("VoidSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckVoid", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Void", FilterType.RequiresOneOf)
end

VoidSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "CheckVoid") then

		local units = self:GetEntities("Unit")
		local voids = self:GetEntities("Void")

		for i = 1, #units do
			
			local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
			local noSteps = world:GetComponent(units[i], "NoSubSteps", 0):GetInt()
			local dirX, dirZ = world:GetComponent(units[i], "Direction", 0):GetInt2()			

			if dirX ~= 0 then
				dirX = dirX / math.abs(dirX)
			end

			if dirZ ~= 0 then
				dirZ = dirZ / math.abs(dirZ)
			end
			
			for tmp = noSteps - 1, 0, -1 do
					
				local tmpX = unitX - (dirX * tmp)
				local tmpZ = unitZ - (dirZ * tmp)

				for j = 1, #voids do
					
					local voidX, voidZ = world:GetComponent(voids[j], "MapPosition", 0):GetInt2() 

					if tmpX == voidX and tmpZ == voidZ then
						
						print("Unit walked into a void.", voidX, voidZ)

						if not world:EntityHasComponent(units[i], "UnitDead") then
							world:CreateComponentAndAddTo("UnitDead", units[i])
						end

						if not world:EntityHasComponent(units[i], "Hide") then
							world:CreateComponentAndAddTo("Hide", units[i])
						end
						break

					end

				end
			end
		end

		world:KillEntity( entity )

		

	end
end

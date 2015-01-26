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

			for j = 1, #voids do
				
				local voidX, voidZ = world:GetComponent(voids[j], "MapPosition", 0):GetInt2() 

				if unitX == voidX and unitZ == voidZ then
					
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

		world:KillEntity( entity )

		

	end
end

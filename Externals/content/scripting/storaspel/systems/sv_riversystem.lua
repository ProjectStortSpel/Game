RiverSystem = System()

RiverSystem.Initialize = function(self)
	self:SetName("RiverSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MoveRiver", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("River", FilterType.RequiresOneOf)

	print("RiverSystem initialized!")
end

RiverSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "MoveRiver") then
		print("Move River")
		local units = self:GetEntities("Unit")
		local rivers = self:GetEntities("River")

		for i = 1, #units do
			
			local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2() 

			for j = 1, #rivers do
				
				local riverX, riverZ = world:GetComponent(rivers[j], "MapPosition", 0):GetInt2() 

				if unitX == riverX and unitZ == riverZ then
					local riverDirX, riverDirZ = world:GetComponent(rivers[j], "River", 0):GetInt2()
					world:SetComponent(units[i], "MapPosition", "X", unitX + riverDirX)
					world:SetComponent(units[i], "MapPosition", "Z", unitZ + riverDirZ)

					local pos = world:GetComponent(units[i], "Position", 0)
					local X, Y, Z = pos:GetFloat3()
					--pos:SetFloat3(X + riverDirX, Y, Z + riverDirZ)


					--world:SetComponent(units[i], "Position", "X", unitX + riverDirX)
					--world:SetComponent(units[i], "Position", "Z", unitZ + riverDirZ)
					break

				end

			end

		end

		world:KillEntity( entity )

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("PostMove", id)

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("StepTimer", id)
		world:GetComponent(id, "StepTimer", "Time"):SetFloat(1)



	end
end

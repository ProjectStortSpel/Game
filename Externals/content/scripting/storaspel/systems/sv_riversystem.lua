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
					local posX = unitX + riverDirX
					local posZ = unitZ + riverDirZ

					world:GetComponent(units[i], "MapPosition", 0):SetInt2(posX, posZ)
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

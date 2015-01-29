RiverSystem = System()

RiverSystem.Initialize = function(self)
	--	Set Name
	self:SetName("RiverSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Unit", 		FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MoveRiver", 	FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("River", 		FilterType.RequiresOneOf)
end

RiverSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent( entity, "MoveRiver") then
			print("Move River")

			--local file = File.Append("gamelog.txt")
			--File.WriteLine(file, "MoveRiver")
			--File.Close(file)

			local units = self:GetEntities("Unit")
			local rivers = self:GetEntities("River")

			for i = 1, #units do
				
				local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2() 

				for j = 1, #rivers do
					
					local riverX, riverZ = world:GetComponent(rivers[j], "MapPosition", 0):GetInt2() 

					if unitX == riverX and unitZ == riverZ then

						local riverDirX, riverDirZ = world:GetComponent(rivers[j], "River", 0):GetInt2()
						local posXs = unitX + riverDirX
						local posZs = unitZ + riverDirZ

						--world:GetComponent(units[i], "MapPosition", 0):SetInt2(posX, posZ)

						--print(entity)

						local id = world:CreateNewEntity()
						world:CreateComponentAndAddTo("TestRiver", id)
						world:SetComponent(id, "TestRiver", "Unit", units[i])
						world:SetComponent(id, "TestRiver", "PosX", posXs)
						world:SetComponent(id, "TestRiver", "PosZ", posZs)

						break

					end

				end

			end

			world:KillEntity( entity )

			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PostMove", id)

			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("StepTimer", id)
			world:GetComponent(id, "StepTimer", "Time"):SetFloat(0.7)


		end
	end
end
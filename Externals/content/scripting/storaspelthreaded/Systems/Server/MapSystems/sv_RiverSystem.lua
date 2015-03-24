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

RiverSystem.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
		local entity = entities[n]
		
		if world:EntityHasComponent( entity, "MoveRiver") then

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

						--local id = world:CreateNewEntity()
						--world:CreateComponentAndAddTo("TestRiver", id)
						--world:SetComponent(id, "TestRiver", "Unit", units[i])
						--world:SetComponent(id, "TestRiver", "PosX", posXs)
						--world:SetComponent(id, "TestRiver", "PosZ", posZs)
						
						local id = world:CreateNewEntity()
						world:CreateComponentAndAddTo("SimultaneousMove", id)
						world:GetComponent(id, "SimultaneousMove", "Unit"):SetInt(units[i])
						world:GetComponent(id, "SimultaneousMove", "PosX"):SetInt(posXs)
						world:GetComponent(id, "SimultaneousMove", "PosZ"):SetInt(posZs)
						world:GetComponent(id, "SimultaneousMove", "DirX"):SetInt(riverDirX)
						world:GetComponent(id, "SimultaneousMove", "DirZ"):SetInt(riverDirZ)
						world:GetComponent(id, "SimultaneousMove", "Steps"):SetInt(1)
						world:GetComponent(id, "SimultaneousMove", "SlerpTime"):SetFloat(0.5)

						break

					end

				end

			end

			world:KillEntity( entity )
		end
	end
end
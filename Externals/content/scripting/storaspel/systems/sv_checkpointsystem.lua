CheckpointSystem = System()

CheckpointSystem.Initialize = function(self)
	self:SetName("CheckpointSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckCheckpoint", FilterType.RequiresOneOf)

	print("CheckpointSystem initialized!")
end

CheckpointSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "CheckCheckpoint") then
		
		local units = self:GetEntities("Unit")
		local checkpoints = self:GetEntities("Checkpoint")
		local nextCP = nil

		for i = 1, #units do
			
			local targetId = world:GetComponent(units[i], "TargetCheckpoint", "Id"):GetInt()

			
			for j = 1, #checkpoints do
				
				local cpId = world:GetComponent(checkpoints[j], "Checkpoint", "Number"):GetInt()

				if cpId == targetId then
					local checkpointX, checkpointZ = world:GetComponent(checkpoints[j], "MapPosition", 0):GetInt2() 
					local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2() 

					if unitX == checkpointX and unitZ == checkpointZ then
						
						print("Unit reached a checkpoint")

						world:SetComponent(units[i], "TargetCheckpoint", "Id", targetId + 1)
						world:GetComponent(units[i], "Spawnpoint", 0):SetInt2(checkpointX, checkpointZ)

						break
					end
				end
			end
		end

		world:KillEntity( entity )

	end
end

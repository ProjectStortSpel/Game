FinishSystem = System()
FinishSystem.TotemCount = {}

FinishSystem.Initialize = function(self)
	self:SetName("FinishSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Finish", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckFinish", FilterType.RequiresOneOf)

	print("FinishSystem initialized!")
end

FinishSystem.Update = function(self, dt)

	--print("NoOfEntities: " .. #self:GetEntities())

end

FinishSystem.AddTotemPole = function(self, playerId, currentCP, noCP, X, Y, Z)

	local head = world:CreateNewEntity("Head")
	local rotation 	= world:GetComponent(head, "Rotation", 0)
	local position 	= world:GetComponent(head, "Position", 0)
	local scale		= world:GetComponent(head, "Scale", 0)
	local axis = math.pi
	local setScale = 0.5
	
	-- Position
	if self.TotemCount[currentCP] == nil then
		table.insert(self.TotemCount, currentCP, 1)
	end

	rotation:SetFloat3(0, axis, 0)
	position:SetFloat3(X, self.TotemCount[currentCP] * setScale, Z)
	scale:SetFloat3(setScale, setScale, setScale)
	
	world:SetComponent(head, "Model", "ModelName", "ply" .. playerId);
	world:SetComponent(head, "Model", "ModelPath", "head");
	world:SetComponent(head, "Model", "RenderType", 0);
	self.TotemCount[currentCP] = self.TotemCount[currentCP] + 1
end

FinishSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent( entity, "CheckFinishpoint") then
		
		local units = self:GetEntities("Unit")
		local checkpoints = self:GetEntities("Finish")
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
						
						
						local playerId = world:GetComponent(units[i], "PlayerNumber", 0):GetInt()
						self.AddTotemPole(self, playerId, j, #checkpoints, checkpointX, Y, checkpointZ)
												
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

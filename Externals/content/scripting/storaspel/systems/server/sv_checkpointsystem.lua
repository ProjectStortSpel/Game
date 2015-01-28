CheckpointSystem = System()
CheckpointSystem.TotemCount = {}

CheckpointSystem.Initialize = function(self)
	self:SetName("CheckpointSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Checkpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckCheckpoint", FilterType.RequiresOneOf)
end

CheckpointSystem.Update = function(self, dt)

	--print("NoOfEntities: " .. #self:GetEntities())

end

CheckpointSystem.AddTotemPole = function(self, playerId, currentCP, noCP, X, Y, Z)

	-- TODO: FIX THIS SO IT DOESN'T USE ARRAYS FOR TOTEMS. DOESN'T WORK IN DEBUG

	local head = world:CreateNewEntity("Head")
	local rotation 	= world:GetComponent(head, "Rotation", 0)
	local position 	= world:GetComponent(head, "Position", 0)
	local scale		= world:GetComponent(head, "Scale", 0)
	local axis = math.pi
	local setScale = 0.25
	
	-- Position
	if self.TotemCount[currentCP] == nil then
		self.TotemCount[currentCP] = 1
	end

	rotation:SetFloat3(0, axis, 0)
	position:SetFloat3(X, 0.4 + self.TotemCount[currentCP] * setScale, Z)
	scale:SetFloat3(setScale, setScale, setScale)
	
	world:SetComponent(head, "Model", "ModelName", "ply" .. playerId);
	world:SetComponent(head, "Model", "ModelPath", "head");
	world:SetComponent(head, "Model", "RenderType", 0);
	self.TotemCount[currentCP] = self.TotemCount[currentCP] + 1
end

CheckpointSystem.OnEntityAdded = function(self, entity)

	-- If the entity has a CheckCheckpoint component
	if world:EntityHasComponent( entity, "CheckCheckpoint") then

		-- Get all units currently on the playfield
		local units = self:GetEntities("Unit")
		-- Get all checkpoints on the map
		local checkPoints = self:GetEntities("Checkpoint")
		
		for i = 1, #units do
		
			-- Get the id of the unit's next checkpoint
			local targetId = world:GetComponent(units[i], "TargetCheckpoint", "Id"):GetInt()
			
			if targetId > #checkPoints then
			
				local newId = world:CreateNewEntity()
				world:CreateComponentAndAddTo("CheckFinishpoint", newId)
				world:CreateComponentAndAddTo("UnitEntityId", newId)
				world:SetComponent(newId, "UnitEntityId", "Id", units[i])
			
			else
				local noSteps = world:GetComponent(units[i], "NoSubSteps", 0):GetInt()
				local dirX, dirZ = world:GetComponent(units[i], "Direction", 0):GetInt2()
				
				if dirX ~= 0 then
					dirX = dirX / math.abs(dirX)
				end

				if dirZ ~= 0 then
					dirZ = dirZ / math.abs(dirZ)
				end
				
				-- Get the unit's MapPosition
				local unitPosX, unitPosZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
								
				for tmp = noSteps - 1, 0, -1 do
					print("TMP: " .. tmp)
					print("noSteps: " .. noSteps)
					
					--if n > 0 then os.execute("ping -n " .. tonumber(1000+1) .. " localhost > NUL") end
					
					
					local tmpX = unitPosX - (dirX * tmp)
					local tmpZ = unitPosZ - (dirZ * tmp)
					
					-- Go through all checksoints
					for j = 1, #checkPoints do
					
						-- Get the current checkpoints Id
						local cpId = world:GetComponent(checkPoints[j], "Checkpoint", "Number"):GetInt()
						
						-- If the id is the same as the unit's target checkpoint
						if cpId == targetId then
							
							-- Get the checkpoint's MapPosition
							local cpPosX, cpPosZ = world:GetComponent(checkPoints[j], "MapPosition", 0):GetInt2()
							
							-- If the unit's position and the checkpoint's position is the same
							if tmpX == cpPosX and tmpZ == cpPosZ then
							
								-- Get the number of the player controlling the unit
								local playerNum = world:GetComponent(units[i], "PlayerNumber", 0):GetInt()
							
								print("Player#" .. playerNum .. " reached checkpoint#" .. cpId)
								
								local totemPieceId = world:CreateNewEntity()
								world:CreateComponentAndAddTo("AddTotemPiece", totemPieceId)
								world:CreateComponentAndAddTo("PlayerNumber", totemPieceId)
								world:CreateComponentAndAddTo("CheckpointId", totemPieceId)
								print("CreateComponentAndAddTo done")
								world:SetComponent(totemPieceId, "PlayerNumber", "Number", playerNum)
								print("PlayerNumber done")
								world:SetComponent(totemPieceId, "CheckpointId", "Id", cpId)
								print("CheckpointId done")
								
								-- Set the new target checkpoint
								world:SetComponent(units[i], "TargetCheckpoint", "Id", targetId + 1)
								-- Set the units spawnpoint
								world:GetComponent(units[i], "Spawnpoint", 0):SetInt2(cpPosX, cpPosZ)
							
								break
							
							end
						
						end
					
					end
					
				
				end
			
			end
		
		end
		
		world:KillEntity(entity)
		
	end

end

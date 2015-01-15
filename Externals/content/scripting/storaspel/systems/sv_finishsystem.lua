FinishSystem = System()
FinishSystem.TotemCount = {}

FinishSystem.Initialize = function(self)
	self:SetName("FinishSystem System")

	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Finishpoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckFinishpoint", FilterType.RequiresOneOf)

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
	local setScale = 0.25
	
	-- Position
	if self.TotemCount[currentCP] == nil then
		table.insert(self.TotemCount, currentCP, 1)
	end

	rotation:SetFloat3(0, axis, 0)
	position:SetFloat3(X, 0.4 + self.TotemCount[currentCP] * setScale, Z)
	scale:SetFloat3(setScale, setScale, setScale)
	
	world:SetComponent(head, "Model", "ModelName", "ply" .. playerId);
	world:SetComponent(head, "Model", "ModelPath", "head");
	world:SetComponent(head, "Model", "RenderType", 0);
	self.TotemCount[currentCP] = self.TotemCount[currentCP] + 1
end

FinishSystem.OnEntityAdded = function(self, entity)

	if world:EntityHasComponent( entity, "CheckFinishpoint") then
	
		print("FinishSystem.OnEntityAdded")
	
		local units = self:GetEntities("Unit")
		local finishpoints = self:GetEntities("Finishpoint")
		local nextCP = nil

		for i = 1, #units do
			
			local unitX, unitZ = world:GetComponent(units[i], "MapPosition", 0):GetInt2()
			
			for j = 1, #finishpoints do
			
				local finishpointX, finishpointZ = world:GetComponent(finishpoints[j], "MapPosition", 0):GetInt2()
				
				if unitX == finishpointX and unitZ == finishpointZ then
					print("Unit reached a finishpoint")
					
					local playerId = world:GetComponent(units[i], "PlayerEntityId", "Id"):GetInt()
					print("PlayerId: " .. playerId)
					
					local id = world:CreateNewEntity()
					world:CreateComponentAndAddTo("TakeCardsFromPlayer", id)
					world:GetComponent(id, "TakeCardsFromPlayer", "Player"):SetInt(playerId)
					
					id = world:CreateNewEntity()
					world:CreateComponentAndAddTo("TakeCardStepsFromUnit", id)
					world:GetComponent(id, "TakeCardStepsFromUnit", "Unit"):SetInt(units[i])
					
					world:CreateComponentAndAddTo("IsSpectator", playerId)
					world:KillEntity(units[i])
					
				end
				
			
			end
			
		end

		world:KillEntity( entity )

	end
	
end

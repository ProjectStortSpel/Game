TrueTestMoveSystem = System()

TrueTestMoveSystem.Initialize = function(self)

	--	Set Name
	self:SetName("TrueTestMoveSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()

	--	Set Filter
	self:AddComponentTypeToFilter("MapPosition",FilterType.Mandatory)
	self:AddComponentTypeToFilter("Unit", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Position",FilterType.Mandatory)

end

TrueTestMoveSystem.Update = function(self, dt, taskIndex, taskCount)
	local entities = self:GetEntities()

	for i = 1, #entities do

		local entity = entities[i]
		local mapPositionX, mapPositionZ = world:GetComponent(entity, "MapPosition", 0):GetInt2()
		local pos = world:GetComponent(entity, "Position", 0)
		local newPosX, newPosY, newPosZ = pos:GetFloat3()
		
		if mapPositionX ~= newPosX or mapPositionZ ~= newPosZ 
		then
			local dirposX = mapPositionX - newPosX;
			local dirposZ = mapPositionZ - newPosZ;
			local tempX = math.abs(dirposX)
			local tempZ = math.abs(dirposZ)
			local total = math.sqrt(dirposX*dirposX + dirposZ*dirposZ)

			dirposX = dirposX/total
			dirposZ = dirposZ/total

			newPosX = newPosX + dirposX * dt * 1.5
			newPosZ = newPosZ + dirposZ * dt * 1.5

			if tempX <= 0.05 then
				newPosX = mapPositionX
			end
			if tempZ <= 0.05 then
				newPosZ = mapPositionZ
			end

			world:GetComponent(entity, "Position", 0):SetFloat3(newPosX, newPosY, newPosZ, false)
		end

	end
end
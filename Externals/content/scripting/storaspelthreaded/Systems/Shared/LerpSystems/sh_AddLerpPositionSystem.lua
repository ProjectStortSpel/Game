AddLerpPositionSystem = System()

AddLerpPositionSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddLerpPositionSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpPosition", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingPosition", FilterType.Excluded)
end

AddLerpPositionSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Lerp Position
		local tX = world:GetComponent(entity, "LerpPosition", "X"):GetFloat(0)
		local tY = world:GetComponent(entity, "LerpPosition", "Y"):GetFloat(0)
		local tZ = world:GetComponent(entity, "LerpPosition", "Z"):GetFloat(0)
		local lerpTime = world:GetComponent(entity, "LerpPosition", "Time"):GetFloat(0)
		local lerpAlgorithm = world:GetComponent(entity, "LerpPosition", "Algorithm"):GetString(0)

		-- Get Current Position
		local position = world:GetComponent(entity, "Position", 0)
		local X, Y, Z = position:GetFloat3(0)
		
		-- Create Lerping
		world:CreateComponentAndAddTo("LerpingPosition", entity)
		world:GetComponent(entity, "LerpingPosition", "sX"):SetFloat(X)
		world:GetComponent(entity, "LerpingPosition", "sY"):SetFloat(Y)
		world:GetComponent(entity, "LerpingPosition", "sZ"):SetFloat(Z)
		world:GetComponent(entity, "LerpingPosition", "tX"):SetFloat(tX)
		world:GetComponent(entity, "LerpingPosition", "tY"):SetFloat(tY)
		world:GetComponent(entity, "LerpingPosition", "tZ"):SetFloat(tZ)
		world:GetComponent(entity, "LerpingPosition", "Time"):SetFloat(lerpTime)
		world:GetComponent(entity, "LerpingPosition", "Timer"):SetFloat(0)
		world:GetComponent(entity, "LerpingPosition", "Algorithm"):SetString(lerpAlgorithm)
		
		world:RemoveComponentFrom("LerpPosition", entity)
	end
end
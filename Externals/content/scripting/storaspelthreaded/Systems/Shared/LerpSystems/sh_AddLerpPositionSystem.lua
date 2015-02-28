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

AddLerpPositionSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Lerp Position
		local lerpTime, tX, tY, tZ = world:GetComponent(entity, "LerpPosition", "Time"):GetFloat4(0)
		local lerpAlgorithm = world:GetComponent(entity, "LerpPosition", "Algorithm"):GetText(0)
		local killWhenFinished = world:GetComponent(entity, "LerpPosition", "KillWhenFinished"):GetBool()

		-- Get Current Position
		local X, Y, Z = world:GetComponent(entity, "Position", 0):GetFloat3(0)
		
		-- Create Lerping
		world:CreateComponentAndAddTo("LerpingPosition", entity)
		world:GetComponent(entity, "LerpingPosition", "Time"):SetFloat8(lerpTime, 0, X, Y, Z, tX, tY, tZ)
		world:GetComponent(entity, "LerpingPosition", "Algorithm"):SetText(lerpAlgorithm)
		world:GetComponent(entity, "LerpingPosition", "KillWhenFinished"):SetBool(killWhenFinished)
		
		world:RemoveComponentFrom("LerpPosition", entity)
	end
end
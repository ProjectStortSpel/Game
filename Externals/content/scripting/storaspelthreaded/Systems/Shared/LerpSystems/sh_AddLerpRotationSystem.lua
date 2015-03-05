AddLerpRotationSystem = System()

AddLerpRotationSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddLerpRotationSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Rotation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpRotation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingRotation", FilterType.Excluded)
end

AddLerpRotationSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Lerp Rotation
		local lerpTime, tX, tY, tZ =  world:GetComponent(entity, "LerpRotation", "Time"):GetFloat4(0)
		local lerpAlgorithm = world:GetComponent(entity, "LerpRotation", "Algorithm"):GetText(0)
		
		-- Get Current Rotation
		local X, Y, Z = world:GetComponent(entity, "Rotation", 0):GetFloat3(0)
		
		-- Create Lerping
		world:CreateComponentAndAddTo("LerpingRotation", entity)
		world:GetComponent(entity, "LerpingRotation", "Time"):SetFloat8(lerpTime, 0, X, Y, Z, tX, tY, tZ)
		world:GetComponent(entity, "LerpingRotation", "Algorithm"):SetText(lerpAlgorithm)
		
		world:RemoveComponentFrom("LerpRotation", entity)
	end
end
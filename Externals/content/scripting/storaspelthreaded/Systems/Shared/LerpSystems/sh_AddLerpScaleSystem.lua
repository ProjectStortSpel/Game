AddLerpScaleSystem = System()

AddLerpScaleSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddLerpScaleSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpScale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingScale", FilterType.Excluded)
end

AddLerpScaleSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]
		
		-- Get Lerp Scale
		local lerpTime, tX, tY, tZ = world:GetComponent(entity, "LerpScale", "Time"):GetFloat4(0)
		local lerpAlgorithm = world:GetComponent(entity, "LerpScale", "Algorithm"):GetText(0)

		-- Get Current Scale
		local X, Y, Z = world:GetComponent(entity, "Scale", 0):GetFloat3(0)
		
		-- Create Lerping
		world:CreateComponentAndAddTo("LerpingScale", entity)
		world:GetComponent(entity, "LerpingScale", "Time"):SetFloat8(lerpTime, 0, X, Y, Z, tX, tY, tZ)
		world:GetComponent(entity, "LerpingScale", "Algorithm"):SetText(lerpAlgorithm)
		
		world:RemoveComponentFrom("LerpScale", entity)
	end
end
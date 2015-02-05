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

AddLerpScaleSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Lerp Scale
		local tX = world:GetComponent(entity, "LerpScale", "X"):GetFloat(0)
		local tY = world:GetComponent(entity, "LerpScale", "Y"):GetFloat(0)
		local tZ = world:GetComponent(entity, "LerpScale", "Z"):GetFloat(0)
		local lerpTime = world:GetComponent(entity, "LerpScale", "Time"):GetFloat(0)
		local lerpAlgorithm = world:GetComponent(entity, "LerpScale", "Algorithm"):GetString(0)

		-- Get Current Scale
		local scale = world:GetComponent(entity, "Scale", 0)
		local X, Y, Z = scale:GetFloat3(0)
		
		-- Create Lerping
		world:CreateComponentAndAddTo("LerpingScale", entity)
		world:GetComponent(entity, "LerpingScale", "sX"):SetFloat(X)
		world:GetComponent(entity, "LerpingScale", "sY"):SetFloat(Y)
		world:GetComponent(entity, "LerpingScale", "sZ"):SetFloat(Z)
		world:GetComponent(entity, "LerpingScale", "tX"):SetFloat(tX)
		world:GetComponent(entity, "LerpingScale", "tY"):SetFloat(tY)
		world:GetComponent(entity, "LerpingScale", "tZ"):SetFloat(tZ)
		world:GetComponent(entity, "LerpingScale", "Time"):SetFloat(lerpTime)
		world:GetComponent(entity, "LerpingScale", "Timer"):SetFloat(0)
		world:GetComponent(entity, "LerpingScale", "Algorithm"):SetString(lerpAlgorithm)
		
		world:RemoveComponentFrom("LerpScale", entity)
	end
end
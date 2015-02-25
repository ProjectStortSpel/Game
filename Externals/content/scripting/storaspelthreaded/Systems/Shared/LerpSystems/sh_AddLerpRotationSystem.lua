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
		local tX = world:GetComponent(entity, "LerpRotation", "X"):GetFloat(0)
		local tY = world:GetComponent(entity, "LerpRotation", "Y"):GetFloat(0)
		local tZ = world:GetComponent(entity, "LerpRotation", "Z"):GetFloat(0)
		local lerpTime = world:GetComponent(entity, "LerpRotation", "Time"):GetFloat(0)
		local lerpAlgorithm = world:GetComponent(entity, "LerpRotation", "Algorithm"):GetText(0)
		local killWhenFinished = world:GetComponent(entity, "LerpRotation", "KillWhenFinished"):GetBool()
		
		-- Get Current Rotation
		local rotation = world:GetComponent(entity, "Rotation", 0)
		local X, Y, Z = rotation:GetFloat3(0)
		
		-- Create Lerping
		world:CreateComponentAndAddTo("LerpingRotation", entity)
		world:GetComponent(entity, "LerpingRotation", "sX"):SetFloat(X)
		world:GetComponent(entity, "LerpingRotation", "sY"):SetFloat(Y)
		world:GetComponent(entity, "LerpingRotation", "sZ"):SetFloat(Z)
		world:GetComponent(entity, "LerpingRotation", "tX"):SetFloat(tX)
		world:GetComponent(entity, "LerpingRotation", "tY"):SetFloat(tY)
		world:GetComponent(entity, "LerpingRotation", "tZ"):SetFloat(tZ)
		world:GetComponent(entity, "LerpingRotation", "Time"):SetFloat(lerpTime)
		world:GetComponent(entity, "LerpingRotation", "Timer"):SetFloat(0)
		world:GetComponent(entity, "LerpingRotation", "Algorithm"):SetText(lerpAlgorithm)
		world:GetComponent(entity, "LerpingRotation", "KillWhenFinished"):SetBool(killWhenFinished)
		
		world:RemoveComponentFrom("LerpRotation", entity)
		
	end
end
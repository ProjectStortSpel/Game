CameraMovePhaseSystem = System()

CameraMovePhaseSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CameraMovePhaseSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("NewRound", FilterType.Mandatory)
end

CameraMovePhaseSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("CameraInterestPoint", id)
	world:GetComponent(id, "CameraInterestPoint", "UpX"):SetFloat(-1)
	world:GetComponent(id, "CameraInterestPoint", "UpZ"):SetFloat(-1)
	world:GetComponent(id, "CameraInterestPoint", "AtX"):SetFloat(6.5)
	world:GetComponent(id, "CameraInterestPoint", "AtZ"):SetFloat(6.5)
	world:GetComponent(id, "CameraInterestPoint", "Distance"):SetFloat(1.2)
end
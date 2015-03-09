CameraMovePhaseSystem = System()

CameraMovePhaseSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("CameraMovePhaseSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	
end

CameraMovePhaseSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		if world:EntityHasComponent(entities[n], "NewRound") then
		
			local mapSize = self:GetEntities("MapSpecs")
			if #mapSize > 0 then
				mapX, mapZ = world:GetComponent(mapSize[1], "MapSpecs", "SizeX"):GetInt2()
			end

			local distance = math.max(mapX,mapZ)/10
			
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("CameraInterestPoint", id)
			world:GetComponent(id, "CameraInterestPoint", "UpX"):SetFloat(-0.5)
			world:GetComponent(id, "CameraInterestPoint", "UpZ"):SetFloat(-0.5)
			world:GetComponent(id, "CameraInterestPoint", "AtX"):SetFloat(mapX/2 + distance)
			world:GetComponent(id, "CameraInterestPoint", "AtZ"):SetFloat(mapZ/2 + distance)
			world:GetComponent(id, "CameraInterestPoint", "Distance"):SetFloat(distance)
			
		end
	end
end
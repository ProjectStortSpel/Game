FlowingWaterSystem = System()

FlowingWaterSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("FlowingWaterSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("River", FilterType.Mandatory)
end

FlowingWaterSystem.PostInitialize = function ( self )
--[[
	local newEntityId = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", newEntityId)
	world:CreateComponentAndAddTo("Rotation", newEntityId)
	world:CreateComponentAndAddTo("Position", newEntityId)
	world:CreateComponentAndAddTo("Scale", newEntityId)
	local rotation = world:GetComponent(newEntityId, "Rotation", 0)
	local position = world:GetComponent(newEntityId, "Position", 0)
	local scale = world:GetComponent(newEntityId, "Scale", 0)
	rotation:SetFloat3(0, 0, 0)
	position:SetFloat3(0, 0, 0)
	scale:SetFloat3(1, 1, 1)
	
	world:SetComponent(newEntityId, "Model", "ModelName", "island");
	world:SetComponent(newEntityId, "Model", "ModelPath", "island");
	world:SetComponent(newEntityId, "Model", "RenderType", 0);
--]]
end

FlowingWaterSystem.EntitiesAdded = function(self, dt, entities)
	
	for i = 1, #entities do
		local	X, Z	=	world:GetComponent(entities[i], "MapPosition", "X"):GetInt2()
		local 	modelname = world:GetComponent(entities[i], "Model", "ModelName"):GetText()
		
		local	newFlowingTile	=	world:CreateNewEntity("FlowingWater")
		
		local comp = world:GetComponent(newFlowingTile, "Rotation", 0)
		local currentRotation = comp:GetFloat(1)
		
		local ix, iy, iz = world:GetComponent(entities[i], "River", 0):GetInt3(0)
		
		if ix == 1 then
			currentRotation = -math.pi/2
		elseif iy == 1 then
			currentRotation = -iy*math.pi
		elseif iy == -1 then
			currentRotation = 0
		else
			currentRotation = math.pi/2
		end
		
		if modelname == "rivercorner" then
		
			local dir = world:GetComponent(entities[i], "RiverCornerDir", "Dir"):GetText()

			if dir == "Right" then
				world:SetComponent(newFlowingTile, "Model", "ModelName", "watercornerright")
				world:SetComponent(newFlowingTile, "Model", "ModelPath", "watercornerright")
				currentRotation = currentRotation + math.pi/2
			else
				world:SetComponent(newFlowingTile, "Model", "ModelName", "watercornerleft")
				world:SetComponent(newFlowingTile, "Model", "ModelPath", "watercornerleft")
				currentRotation = currentRotation - math.pi/2
			end
			
			world:GetComponent(newFlowingTile, "Position", "X"):SetFloat3(X, 0.40, Z)
			world:SetComponent(newFlowingTile, "Model", "RenderType", 6);
		
		else
			world:SetComponent(newFlowingTile, "Model", "ModelName", "waterplane")
			world:SetComponent(newFlowingTile, "Model", "ModelPath", "water")
			
			world:GetComponent(newFlowingTile, "Position", "X"):SetFloat3(X, 0.40, Z)
			world:SetComponent(newFlowingTile, "Model", "RenderType", 5)		

		end
		
		
		comp:SetFloat3(0, currentRotation, 0)
		
		

		
	end
	
end
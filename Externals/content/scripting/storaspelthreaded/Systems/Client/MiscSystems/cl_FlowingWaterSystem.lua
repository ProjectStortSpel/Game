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
		local	newFlowingTile	=	world:CreateNewEntity("FlowingWater")
		world:SetComponent(newFlowingTile, "Model", "ModelName", "waterplane");
		world:SetComponent(newFlowingTile, "Model", "ModelPath", "water");
		world:SetComponent(newFlowingTile, "Model", "RenderType", 5);
		
		world:GetComponent(newFlowingTile, "Position", "X"):SetFloat3(X, 0.40, Z)
		
		local comp = world:GetComponent(entities[i], "River", 0)
        local ix, iy, iz = comp:GetInt3(0)
		
		if ix == 1 then
			world:GetComponent(newFlowingTile, "Rotation", "X"):SetFloat3(0, math.pi, 0)
		elseif iy ~= 0 then
			world:GetComponent(newFlowingTile, "Rotation", "X"):SetFloat3(0, iy*math.pi/2, 0)
		else
			world:GetComponent(newFlowingTile, "Rotation", "X"):SetFloat3(0, 0, 0)
		end
		
	end
	
end
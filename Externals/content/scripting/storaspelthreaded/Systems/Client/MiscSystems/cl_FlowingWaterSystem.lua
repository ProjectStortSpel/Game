FlowingWaterSystem = System()

FlowingWaterSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("FlowingWaterSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Filters
	self:AddComponentTypeToFilter("River", FilterType.Mandatory)
end

FlowingWaterSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
	for i = 1, #entities do
		local	X, Z	=	world:GetComponent(entities[i], "MapPosition", "X"):GetInt2()
		local	newFlowingTile	=	world:CreateNewEntity("FlowingWater")
		world:SetComponent(newFlowingTile, "Model", "ModelName", "waterplane");
		world:SetComponent(newFlowingTile, "Model", "ModelPath", "water");
		world:SetComponent(newFlowingTile, "Model", "RenderType", 1);
		
		world:GetComponent(newFlowingTile, "Position", "X"):SetFloat3(X, 0.40, Z)
		
	end
	
end
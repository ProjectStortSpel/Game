PointlightSystem = System()
PointlightSystem.TotalTime = 1.0

PointlightSystem.Initialize = function(self)
	--	Set Name
	self:SetName("PointlightSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Pointlight", FilterType.Mandatory)
end

PointlightSystem.PostInitialize = function(self)
	for n = 1, 6 do
		local entity = world:CreateNewEntity()
		world:CreateComponentAndAddTo("Pointlight", entity)
		world:CreateComponentAndAddTo("SyncNetwork", entity)
		local newPointlight = self:GetComponent(entity, "Pointlight", 0)
		newPointlight:SetPointlight(4, 2, 4*n, 0.3, 0.7, 0.7, 0.7, 0.75, 0.85, 3)
	end
end

PointlightSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

end
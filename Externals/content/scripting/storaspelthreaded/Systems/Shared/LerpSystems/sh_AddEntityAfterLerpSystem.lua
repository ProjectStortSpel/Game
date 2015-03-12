AddEntityAfterLerpSystem = System()

AddEntityAfterLerpSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddEntityAfterLerpSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()

	--	Set Filter
	self:AddComponentTypeToFilter("AddEntityAfterLerp", FilterType.Mandatory)
	--self:AddComponentTypeToFilter("LerpingPosition", FilterType.Mandatory)
end

AddEntityAfterLerpSystem.EntitiesRemoved = function(self, dt, entities)
	for i = 1, #entities do
		local entity = entities[i]
		
		
	end
end
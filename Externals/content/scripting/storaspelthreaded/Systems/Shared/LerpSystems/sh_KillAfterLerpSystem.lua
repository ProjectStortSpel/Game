KillAfterLerpSystem = System()

KillAfterLerpSystem.Initialize = function(self)
	--	Set Name
	self:SetName("KillAfterLerpSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesRemoved()

	--	Set Filter
	self:AddComponentTypeToFilter("KillAfterLerp", FilterType.Mandatory)
	self:AddComponentTypeToFilter("LerpingPosition", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LerpingScale", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LerpingRotation", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LerpPosition", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LerpScale", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("LerpRotation", FilterType.RequiresOneOf)
end

KillAfterLerpSystem.EntitiesRemoved = function(self, dt, entities)
	for i = 1, #entities do
		print("Kill after lerp!")
		world:KillEntity(entities[i])
	end
end
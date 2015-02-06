MoveTotemPieceSystem = System()

MoveTotemPieceSystem.Initialize = function(self)
	--	Set Name
	self:SetName("MoveTotemPieceSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("TotemPole", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TotemPiece", FilterType.RequiresOneOf)
end

MoveTotemPieceSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, addedEntities)

	for n = 1, #addedEntities do

	
	end 
end
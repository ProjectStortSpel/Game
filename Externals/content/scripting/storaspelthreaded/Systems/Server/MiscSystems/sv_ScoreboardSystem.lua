ScoreboardSystem = System()

ScoreboardSystem.Initialize = function(self)
	--	Set Name
	self:SetName("ScoreboardSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Player", FilterType.Mandatory)
end

ScoreboardSystem.EntitiesAdded = function(self, dt, addedEntities)

	for n = 1, #addedEntities do
		
		local scrbrdP = world:CreateNewEntity()
		world:CreateComponentAndAddTo("ScoreboardPlayer", scrbrdP)
		world:CreateComponentAndAddTo("SyncNetwork", scrbrdP)
		world:GetComponent(scrbrdP, "ScoreboardPlayer", "Name"):SetText("TEMP")
		world:SetComponent(scrbrdP, "ScoreboardPlayer", "R", 1)
		world:SetComponent(scrbrdP, "ScoreboardPlayer", "G", 1)
		world:SetComponent(scrbrdP, "ScoreboardPlayer", "B", 1)
		
	end 
end
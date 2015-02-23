StageSystem = System()

StageSystem.Initialize = function(self)
	--	Set Name
	self:SetName("StageSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("GameStage",	FilterType.RequiresOneOf)
end

StageSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	
end

StageSystem.PostInitialize = function(self)
	local entity = world:CreateNewEntity()
	world:CreateComponentAndAddTo("GameStage", entity)
	world:CreateComponentAndAddTo("JoiningStage", entity)
	world:CreateComponentAndAddTo("SyncNetwork", entity)
end

Console.AddCommand("Start", 
	function (command, ...)
		
		Console.Print("Game started");

		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("GameRunning", id)
		world:CreateComponentAndAddTo("SyncNetwork", id)
		
		id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CreateDeck", id)
		
		Console.Print("NewGame");

	end 
)
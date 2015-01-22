TestSystem = System()

TestSystem.Initialize = function(self)
	self:SetName("ButtonPressedSystem")
	
	self:SetUpdateTaskCount(1)
	self:SetEntitiesAddedTaskCount(1)
	self:SetEntitiesRemovedTaskCount(1)
	self:AddComponentTypeToFilter("Model", FilterType.Mandatory)
	
	print("ButtonPressedSystem initialized!")
end

TestSystem.Update = function(self, dt, taskIndex, taskCount)
	--print("SPAM Update")
end

TestSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	print("SPAM EntitiesAdded")
end

TestSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)
	print("SPAM EntitiesRemoved")
end
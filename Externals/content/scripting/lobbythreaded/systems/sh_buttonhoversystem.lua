ButtonHoverSystem = System()
ButtonHoverSystem.Scale = 1.1

ButtonHoverSystem.Initialize = function(self)
	self:SetName("ButtonHoverSystem")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("ButtonHoverSystem initialized!")
end

ButtonHoverSystem.Update = function(self, dt, taskIndex, taskCount)
	--local bla = { }
	--for i = 1, 1000 do
	--	bla[i] = i
	--end
end

ButtonHoverSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for i = 1, #entities do
		local scale = self:GetComponent(entities[i], "Scale", 0)
		scale:SetFloat3(self.Scale, 0.5 * self.Scale, self.Scale)
	end
		
end

ButtonHoverSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local scale = self:GetComponent(entities[i], "Scale", 0)
		scale:SetFloat3(1, 0.5, 1)
	end
		
end
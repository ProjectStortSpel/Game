ButtonHoverSystem = System()
ButtonHoverSystem.Scale = 1.1

ButtonHoverSystem.Initialize = function(self)
	self:SetName("ButtonHoverSystem")
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("ButtonHoverSystem initialized!")
end

ButtonHoverSystem.OnEntityAdded = function(self, entityId)

	local scale = self:GetComponent(entityId, "Scale", 0)
	scale:SetFloat3(self.Scale, 0.5 * self.Scale, self.Scale)
		
end

ButtonHoverSystem.OnEntityRemoved = function(self, entityId)

	local scale = self:GetComponent(entityId, "Scale", 0)
	scale:SetFloat3(1, 0.5, 1)
		
end
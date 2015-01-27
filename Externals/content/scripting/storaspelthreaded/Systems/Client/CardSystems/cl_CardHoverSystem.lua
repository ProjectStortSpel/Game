CardHoverSystem = System()
CardHoverSystem.HoverScale = 1
CardHoverSystem.DefaultScale = 0.9

CardHoverSystem.Initialize = function(self)
	self:SetName("Card hover System")
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("Card hover System initialized!")
end

CardHoverSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
    
		local action = self:GetComponent(entity, "CardAction", 0):GetString()
		local prio = self:GetComponent(entity, "CardPrio", 0):GetInt()
		
		graphics:RenderSimpleText(action, 0,44)
		graphics:RenderSimpleText(prio, 15,44)
		
	end
		
end



CardHoverSystem.OnEntityAdded = function(self, entityId)

	local action = self:GetComponent(entityId, "CardAction", 0):GetString()
	local scale = self:GetComponent(entityId, "Scale", 0)
	local prio = self:GetComponent(entityId, "CardPrio", 0):GetInt()
	
	graphics:RenderSimpleText(action, 0,44)
	graphics:RenderSimpleText(prio, 15,44)

	scale:SetFloat3(self.HoverScale, self.HoverScale, self.HoverScale)
		
end

CardHoverSystem.OnEntityRemoved = function(self, entityId)

	local scale = self:GetComponent(entityId, "Scale", 0)

	scale:SetFloat3(self.DefaultScale, self.DefaultScale, self.DefaultScale)
		
end
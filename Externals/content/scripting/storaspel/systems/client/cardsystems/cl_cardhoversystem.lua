CardHoverSystem = System()
CardHoverSystem.HoverScale = 1
CardHoverSystem.DefaultScale = 0.9
CardHoverSystem.Rot = 0.0

CardHoverSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		self.Rot = self.Rot + dt*2
		if self.Rot > 6.2832 then
			self.Rot = self.Rot - 6.2832
		end
		
		local rotation = world:GetComponent(entity, "Rotation", 0)
		rotation:SetFloat3(math.cos(self.Rot)*0.05-0.1, math.sin(self.Rot)*0.08, 0)
	end
		
end

CardHoverSystem.Initialize = function(self)
	self:SetName("Card hover System")
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("Card hover System initialized!")
end

CardHoverSystem.OnEntityAdded = function(self, entityId)

	local action = world:GetComponent(entityId, "CardAction", 0):GetString()
	local scale = world:GetComponent(entityId, "Scale", 0)
	local rotation = world:GetComponent(entityId, "Rotation", 0)
	local prio = world:GetComponent(entityId, "CardPrio", 0):GetInt()
	
	graphics:RenderSimpleText(action, 0,44)
	graphics:RenderSimpleText(prio, 15,44)

	rotation:SetFloat3(0, 0, 0)
	scale:SetFloat3(self.HoverScale, self.HoverScale, self.HoverScale)
		
end

CardHoverSystem.OnEntityRemoved = function(self, entityId)

	local scale = world:GetComponent(entityId, "Scale", 0)
	local rotation = world:GetComponent(entityId, "Rotation", 0)
	
	self.Rot = 0
	
	rotation:SetFloat3(0, 0, 0)
	scale:SetFloat3(self.DefaultScale, self.DefaultScale, self.DefaultScale)
		
end
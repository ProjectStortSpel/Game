CardPrintSelectionSystem = System()

CardPrintSelectionSystem.Update = function(self, dt)
    --
	--local entities = self:GetEntities()
	--for i = 1, #entities do
	--	local entity = entities[i]
    --
	--	local action = self:GetComponent(entity, "CardAction", 0):GetString()
	--	local prio = self:GetComponent(entity, "CardPrio", 0):GetInt()
	--	
	--	graphics:RenderSimpleText(action, 0,44)
	--	graphics:RenderSimpleText(prio, 15,44)
	--	
	--end
	--	
end

CardPrintSelectionSystem.Initialize = function(self)
	self:SetName("Card hover System")
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("Card hover System initialized!")
end
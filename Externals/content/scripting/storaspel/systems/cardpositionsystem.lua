CardPositionSystem = System()

CardPositionSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		local halfentities = #entities/2
		local entity = entities[i]
		local position = self:GetComponent(entity, "Position", 0)
		position:SetFloat3(7+i*0.8-halfentities, 3.0, 13.0)
	end
end

CardPositionSystem.Initialize = function(self)
	self:SetName("Card position System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	
	print("Card position System initialized!")
end
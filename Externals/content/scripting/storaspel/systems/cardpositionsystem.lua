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

CardPositionSystem.OnEntityAdded = function(self, entityId)

	local model = self:GetComponent(entityId, "Model", 0)
	local action = self:GetComponent(entityId, "CardAction", 0)
	if action == "Forward" then
		model:SetModel("forward", "cards")
	elseif action == "Backward" then
		model:SetModel("back", "cards")
	elseif action == "TurnRight" then
		model:SetModel("turnright", "cards")
	elseif action == "TurnLeft" then
		model:SetModel("turnleft", "cards")
	elseif action == "TurnAround" then
		model:SetModel("turnaround", "cards")
	else
		model:SetModel("dodge", "cards")
	end
end
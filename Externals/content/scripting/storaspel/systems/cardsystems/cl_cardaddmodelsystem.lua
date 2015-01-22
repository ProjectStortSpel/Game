CardAddModelSystem = System()
CardAddModelSystem.UpOffset = -0.2

CardAddModelSystem.Initialize = function(self)
	self:SetName("Card add model System")
	
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	
	print("Card add model System initialized!")
end

CardAddModelSystem.OnEntityAdded = function(self, entityId)

	print("CardAddModel.OnEntityAdded")

	world:CreateComponentAndAddTo("Model", entityId)
	world:CreateComponentAndAddTo("Position", entityId)
	world:CreateComponentAndAddTo("Rotation", entityId)
	world:CreateComponentAndAddTo("Scale", entityId)
	world:CreateComponentAndAddTo("PickBox", entityId)
	
	local pickbox = self:GetComponent(entityId, "PickBox", 0)
	pickbox:SetFloat2(0.6, 1.0)
	
	local position = self:GetComponent(entityId, "Position", 0)
	position:SetFloat3(0, -3, -4)
	
	local scale = self:GetComponent(entityId, "Scale", 0)
	scale:SetFloat3(0.9, 0.9, 0.9)
	
	local rotation = self:GetComponent(entityId, "Rotation", 0)
	rotation:SetFloat3(3.14159265358979*0.5, 0, 3.14159265358979)
	--rotation:SetFloat3(0, 90, 0)
	
	local model = self:GetComponent(entityId, "Model", 0)
	local action = self:GetComponent(entityId, "CardAction", 0):GetString()
	if action == "Forward" then
		model:SetModel("forward", "cards", 2)
	elseif action == "Backward" then
		model:SetModel("back", "cards", 2)
	elseif action == "TurnRight" then
		model:SetModel("turnright", "cards", 2)
	elseif action == "TurnLeft" then
		model:SetModel("turnleft", "cards", 2)
	elseif action == "TurnAround" then
		model:SetModel("turnaround", "cards", 2)
	else
		model:SetModel("dodge", "cards", 2)
	end
end
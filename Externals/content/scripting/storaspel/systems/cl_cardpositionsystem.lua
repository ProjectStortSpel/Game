CardPositionSystem = System()
CardPositionSystem.Scale = 0.5
CardPositionSystem.UpOffset = -0.2

CardPositionSystem.Update = function(self, dt)

	local entities = self:GetEntities()
	for i = 1, #entities do
		
		local index = world:GetComponent(entities[i], "CardIndex", "Index"):GetInt()
		
		local offsetYfactor = 1.5

		local entity = entities[i]

		if world:EntityHasComponent(entity, "SelectCard") then

			local data = self:GetComponent(entity, "SelectCard", "Index"):GetInt()
			offsetYfactor = 1.2 - (data*0.08)

		end

		local halfentities = #entities/2
		px = (-halfentities + index - 0.5) * self.Scale * 0.8
		py = -offsetYfactor * self.Scale
		pz = -2
		
		local position = self:GetComponent(entity, "Position", 0)
		position:SetFloat3(px, py, pz)
		
		local rotation = self:GetComponent(entity, "Rotation", 0)
		rotation:SetFloat3(0, 3.14159265358979, 1.51 * 3.14159265358979)
		
		local scale = self:GetComponent(entity, "Scale", 0)
		scale:SetFloat3(self.Scale, self.Scale, self.Scale)
	end
end

CardPositionSystem.Initialize = function(self)
	self:SetName("Card position System")
	--self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
	
	print("Card position System initialized!")
end

CardPositionSystem.OnEntityAdded = function(self, entityId)

	print("CardPositionSystem.OnEntityAdded")

	world:CreateComponentAndAddTo("Model", entityId)
	world:CreateComponentAndAddTo("Position", entityId)
	world:CreateComponentAndAddTo("Rotation", entityId)
	world:CreateComponentAndAddTo("Scale", entityId)

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
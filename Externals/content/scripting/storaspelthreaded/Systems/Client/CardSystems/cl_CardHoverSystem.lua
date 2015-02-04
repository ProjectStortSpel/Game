CardHoverSystem = System()
CardHoverSystem.HoverScale = 1
CardHoverSystem.DefaultScale = 0.9
CardHoverSystem.Rot = 0.0

CardHoverSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CardHoverSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
	
	print("Card hover System initialized!")
end

CardHoverSystem.Update = function(self, dt, taskIndex, taskCount)
	local entities = self:GetEntities()
	for i = 1, #entities do
		local entity = entities[i]
		
		self.Rot = self.Rot + dt*2
		if self.Rot > 6.2832 then
			self.Rot = self.Rot - 6.2832
		end
		
		local rotation = self:GetComponent(entity, "Rotation", 0)
		rotation:SetFloat3(math.cos(self.Rot)*0.05-0.1, math.sin(self.Rot)*0.08, 0)
	end	
end



CardHoverSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		
		local action = self:GetComponent(entityId, "CardAction", 0):GetString()
		local rotation = self:GetComponent(entityId, "Rotation", 0)
		local prio = self:GetComponent(entityId, "CardPrio", 0):GetInt()
		
		GraphicDevice.RenderSimpleText(action, 0,44)
		GraphicDevice.RenderSimpleText(prio, 15,44)

		rotation:SetFloat3(0, 0, 0)

		
		if not world:EntityHasComponent(entityId, "LerpScale") then
			world:CreateComponentAndAddTo("LerpScale", entityId)
		end
		world:GetComponent(entityId, "LerpScale", "X"):SetFloat(self.HoverScale)
		world:GetComponent(entityId, "LerpScale", "Y"):SetFloat(self.HoverScale)
		world:GetComponent(entityId, "LerpScale", "Z"):SetFloat(self.HoverScale)
		world:GetComponent(entityId, "LerpScale", "Time"):SetFloat(0.5)
		world:GetComponent(entityId, "LerpScale", "Algorithm"):SetString("NormalLerp")
		
	end
end

CardHoverSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		local scale = self:GetComponent(entityId, "Scale", 0)
		local rotation = self:GetComponent(entityId, "Rotation", 0)
		
		self.Rot = 0
		
		rotation:SetFloat3(0, 0, 0)

		if not world:EntityHasComponent(entityId, "LerpScale") then
			world:CreateComponentAndAddTo("LerpScale", entityId)
		end
		world:GetComponent(entityId, "LerpScale", "X"):SetFloat(self.DefaultScale)
		world:GetComponent(entityId, "LerpScale", "Y"):SetFloat(self.DefaultScale)
		world:GetComponent(entityId, "LerpScale", "Z"):SetFloat(self.DefaultScale)
		world:GetComponent(entityId, "LerpScale", "Time"):SetFloat(0.1)
		world:GetComponent(entityId, "LerpScale", "Algorithm"):SetString("NormalLerp")
	end	
end
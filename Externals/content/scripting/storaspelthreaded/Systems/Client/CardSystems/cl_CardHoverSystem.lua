CardHoverSystem = System()
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
		
		local rotation = world:GetComponent(entity, "Rotation", 0)
		rotation:SetFloat3(math.cos(self.Rot)*0.05-0.1, math.sin(self.Rot)*0.08, 0)
	end	
end



CardHoverSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		local rotation = world:GetComponent(entityId, "Rotation", 0)
		rotation:SetFloat3(0, 0, 0)
	end
end

CardHoverSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		local entityId = entities[n]
		local rotation = world:GetComponent(entityId, "Rotation", 0)
		self.Rot = 0
		rotation:SetFloat3(0, 0, 0)
	end	
end
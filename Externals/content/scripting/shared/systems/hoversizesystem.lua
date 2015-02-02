HoverSizeSystem = System()

HoverSizeSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		local scale = self:GetComponent(entityId, "Scale", 0)
		local hscale = self:GetComponent(entityId, "HoverSize", 0)
		local sx, sy, sz = scale:GetFloat3()
		local hx, hy, hz = hscale:GetFloat3()
		scale:SetFloat3(hx, hy, hz)
		hscale:SetFloat3(sx, sy, sz)
	end
end

HoverSizeSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		local scale = self:GetComponent(entityId, "Scale", 0)
		local hscale = self:GetComponent(entityId, "HoverSize", 0)
		local sx, sy, sz = scale:GetFloat3()
		local hx, hy, hz = hscale:GetFloat3()
		scale:SetFloat3(hx, hy, hz)
		hscale:SetFloat3(sx, sy, sz)
	end
end

HoverSizeSystem.Initialize = function(self)
	self:SetName("HoverSizeSystem")
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	self:AddComponentTypeToFilter("HoverSize", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
end
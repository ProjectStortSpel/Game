HoverSizeSystem = System()

HoverSizeSystem.OnEntityAdded = function(self, entityId)
	local scale = world:GetComponent(entityId, "Scale", 0)
	local hscale = world:GetComponent(entityId, "HoverSize", 0)
	local sx, sy, sz = scale:GetFloat3()
	local hx, hy, hz = hscale:GetFloat3()
	scale:SetFloat3(hx, hy, hz)
	hscale:SetFloat3(sx, sy, sz)
	
end

HoverSizeSystem.OnEntityRemoved = function(self, entityId)
	local scale = world:GetComponent(entityId, "Scale", 0)
	local hscale = world:GetComponent(entityId, "HoverSize", 0)
	local sx, sy, sz = scale:GetFloat3()
	local hx, hy, hz = hscale:GetFloat3()
	scale:SetFloat3(hx, hy, hz)
	hscale:SetFloat3(sx, sy, sz)
end

HoverSizeSystem.Initialize = function(self)
	self:SetName("HoverSizeSystem")
	self:AddComponentTypeToFilter("HoverSize", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
end
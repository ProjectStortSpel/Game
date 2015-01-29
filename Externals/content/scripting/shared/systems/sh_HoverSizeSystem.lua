HoverSizeSystem = System()

HoverSizeSystem.Initialize = function(self)
	--	Set Name
	self:SetName("HoverSizeSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("HoverSize", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Scale", FilterType.Mandatory)
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.Mandatory)
end


HoverSizeSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		local scale = self:GetComponent(entityId, "Scale", 0)
		local hscale = self:GetComponent(entityId, "HoverSize", 0)
		local sx, sy, sz = scale:GetFloat3()
		local hx, hy, hz = hscale:GetFloat3()
		scale:SetFloat3(hx, hy, hz)
		hscale:SetFloat3(sx, sy, sz)
	end
end

HoverSizeSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		local scale = self:GetComponent(entityId, "Scale", 0)
		local hscale = self:GetComponent(entityId, "HoverSize", 0)
		local sx, sy, sz = scale:GetFloat3()
		local hx, hy, hz = hscale:GetFloat3()
		scale:SetFloat3(hx, hy, hz)
		hscale:SetFloat3(sx, sy, sz)
	end
end


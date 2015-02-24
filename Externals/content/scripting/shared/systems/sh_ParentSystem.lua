ParentSystem = System()

ParentSystem.Update = function(self, dt)

	local children = self:GetEntities("Parent")
	for i = 1, #children do
		local child = children[i]
		local parentId = world:GetComponent(child, "Parent", 0):GetInt(0)

		local x, y, z = world:GetComponent(child, "Position", 0):GetFloat3(0)
		world:GetComponent(child, "Position", 0):SetFloat3(x, y, z) -- FULHAX FÖR ATT FÅ DEN ATT UPDATERA HELA TIDEN (FUCK EFFEKTIVITET)
		--local x, y, z = world:GetComponent(parentId, "Scale", 0):GetFloat3(0)
		--world:GetComponent(child, "Scale", 0):SetFloat3(x, y, z)
	end
	
end

ParentSystem.EntitiesAdded = function(self, dt, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		if world:EntityHasComponent(entityId, "Parent") then
			local parent = world:GetComponent(entityId, "Parent", 0)
			local parentEntity = parent:GetInt(0)
			if world:EntityHasComponent(parentEntity, "Render") then
				if not world:EntityHasComponent(parentEntity, "IsParent") then
					world:CreateComponentAndAddTo("IsParent", parentEntity)
				end
			else
				world:RemoveComponentFrom("Parent", entityId)
				print("Parent doesn't have RenderComponent")
			end
		end
	end
end

ParentSystem.EntitiesRemoved = function(self, dt, entities)
	for i = 1, #entities do
		local entityId = entities[i]
		--if world:EntityHasComponent(entityId, "IsParent") then
			local children = self:GetEntities("Parent")
			for i = 1, #children do
				local child = children[i]
				local parentId = world:GetComponent(child, "Parent", 0):GetInt(0)
				if parentId == entityId then
					world:KillEntity(child)
				end
			end
		--end
	end
end

ParentSystem.Initialize = function(self)
	self:SetName("Parent System")
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	self:AddComponentTypeToFilter("Parent", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("IsParent", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Render", FilterType.Mandatory)
end
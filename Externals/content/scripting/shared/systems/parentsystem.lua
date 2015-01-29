ParentSystem = System()

ParentSystem.OnEntityAdded = function(self, entityId)

	if world:EntityHasComponent(entityId, "Parent") then
		local parent = self:GetComponent(entityId, "Parent", 0)
		local parentEntity = Parent:GetInt(0)
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

ParentSystem.OnEntityRemoved = function(self, entityId)
	if world:EntityHasComponent(entityId, "IsParent") then
		local children = self:GetEntities("Parent")
		for i = 1, #children do
			local child = children[i]
			local parentId = self:GetComponent(child, "Parent", 0):Parent:GetInt(0)
			if parentId == entityId then
				world:RemoveComponentFrom("Parent", child)
			end
		end
	end
end

ParentSystem.Initialize = function(self)
	self:SetName("ParentSystem System")
	self:AddComponentTypeToFilter("Parent", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("IsParent", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("Render", FilterType.Mandatory)
end
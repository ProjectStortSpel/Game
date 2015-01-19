CardPositionSystem = System()
CardPositionSystem.UpOffset = -1.6

CardPositionSystem.OnEntityAdded = function( self, entityId )

	local entities = self:GetEntities()
	--for i = 1, #entities do
		local entity = entityId--entities[i]
		
		local index = world:GetComponent(entity, "CardIndex", "Index"):GetInt()
		
		local halfentities = 8/2
		local px = (-halfentities + index - 0.5) * 0.7
		local py = self.UpOffset
		local pz = -4
		
		if not world:EntityHasComponent(entity, "LerpTargetPosition") then
			world:CreateComponentAndAddTo("LerpTargetPosition", entity)
		end
		
		local position = self:GetComponent(entity, "LerpTargetPosition", 0)
		position:SetFloat3(px, py, pz)
		
	--end
	
end


CardPositionSystem.Initialize = function(self)
	self:SetName("Card position System")
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Rotation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardIndex", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardSelected", FilterType.Excluded)
	
	print("Card position System initialized!")
end
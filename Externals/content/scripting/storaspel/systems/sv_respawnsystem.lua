RespawnSystem = System()

RespawnSystem.Initialize = function(self)
	self:SetName("RespawnSystem System")

	self:AddComponentTypeToFilter("UnitDead", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("RespawnUnits", FilterType.RequiresOneOf)

	print("RespawnSystem initialized!")
end

RespawnSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "RespawnUnits") then
		
		local units = self:GetEntities("UnitDead")

		for i = 1, #units do
			
			world:RemoveComponentFrom("UnitDead", units[i])
			world:CreateComponentAndAddTo("Spawn", units[i])

		end

		world:KillEntity( entity )
	end
end

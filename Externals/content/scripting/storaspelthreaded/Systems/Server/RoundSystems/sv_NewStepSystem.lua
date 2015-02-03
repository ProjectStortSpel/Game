NewStepSystem = System()
NewStepSystem.Step = 1

NewStepSystem.Initialize = function(self)
	--	Set Name
	self:SetName("NewStepSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewStep", FilterType.RequiresOneOf)
end

NewStepSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent( entity, "NewRound") then
			--print("\n\nNEW ROUND!")

			self.Step = 1
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NewStep", id)

		elseif world:EntityHasComponent( entity, "NewStep") then
			
			if self.Step <= 5 then
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("PlayCard", id)
				world:SetComponent(id, "PlayCard", "Step", self.Step)

				--print("\nNEW STEP: " .. self.Step)

				self.Step = self.Step + 1
			else
				--local id = world:CreateNewEntity()
				--world:CreateComponentAndAddTo("EndRound", id)
				
				--Respawn
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("RespawnUnits", id)

				--Deal cards
				id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("DealCards", id)
				world:SetComponent(id, "DealCards", "NumCards", 5)

				id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("RemoveEffects", id)
				
			end
		end

		world:KillEntity( entity )
	end
end

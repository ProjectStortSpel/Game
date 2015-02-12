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
	self:AddComponentTypeToFilter("Unit", FilterType.RequiresOneOf)
end

NewStepSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entity = entities[n]
		if world:EntityHasComponent( entity, "NewRound") then

			self.Step = 1
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("NewStep", id)
			world:KillEntity( entity )

		elseif world:EntityHasComponent( entity, "NewStep") then
			--print("\n\nNEW STEP!")
			
			if self.Step <= 5 then
			
				io.write("Step ", self.Step, " ")
				
				local units = self:GetEntities("Unit")
				for i = 1, #units do
					world:SetComponent(units[i], "NoSubSteps", "Counter", 1)
				end

				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("PlayCard", id)
				world:SetComponent(id, "PlayCard", "Step", self.Step)

				--print("\nNEW STEP: " .. self.Step)

				self.Step = self.Step + 1
			else
				--local id = world:CreateNewEntity()
				--world:CreateComponentAndAddTo("EndRound", id)
				io.write("\n")
				
				--Respawn
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("RespawnUnits", id)

				--Deal cards
				id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("DealCards", id)
				world:SetComponent(id, "DealCards", "NumCards", 5)
				
			end
			world:KillEntity( entity )
		end

	end
end

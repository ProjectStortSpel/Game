NewStepSystem = System()
NewStepSystem.Step = 1

NewStepSystem.Initialize = function(self)
	self:SetName("NewStepSystem System")
	self:AddComponentTypeToFilter("NewRound", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewStep", FilterType.RequiresOneOf)
	print("NewStepSystem initialized!")
end

NewStepSystem.OnEntityAdded = function(self, entity)
	
	if world:EntityHasComponent( entity, "NewRound") then

		self.Step = 1
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("NewStep", id)

	else if world:EntityHasComponent( entity, "NewStep") then
		
		if self.Step <= 5 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("HandleStep", id)
			world:SetComponent(id, "HandleStep", "Step", self.Step)
			self.Step = self.Step + 1
		else
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("EndRound", id)
		end

		

	end

	world:KillEntity( entity )

end

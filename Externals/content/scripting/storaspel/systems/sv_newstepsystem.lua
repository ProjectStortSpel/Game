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
		print("NEW ROUND!")
		self.Step = 1
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("NewStep", id)

	elseif world:EntityHasComponent( entity, "NewStep") then
		
		if self.Step <= 5 then
			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("PlayCards", id)
			world:SetComponent(id, "PlayCards", "Step", self.Step)

			print("NEW STEP: " .. self.Step)

			self.Step = self.Step + 1
		else
			--local id = world:CreateNewEntity()
			--world:CreateComponentAndAddTo("EndRound", id)

			local id = world:CreateNewEntity()
			world:CreateComponentAndAddTo("DealCards", id)
			world:SetComponent(id, "DealCards", "NumCards", 5)

		end
	end

	world:KillEntity( entity )
end

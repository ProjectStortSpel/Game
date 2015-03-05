AddAnimationSystem = System()

AddAnimationSystem.Initialize = function(self)
	--	Set Name
	self:SetName("AddAnimationSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("Animation", FilterType.Mandatory)
	self:AddComponentTypeToFilter("Animating", FilterType.Excluded)
end

AddAnimationSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		local entity = entities[n]

		-- Get Animation
		local Id = world:GetComponent(entity, "Animation", "Id"):GetInt(0)
		local frameTime = world:GetComponent(entity, "Animation", "FrameTime"):GetFloat(0)
		local animationTime = world:GetComponent(entity, "Animation", "Time"):GetFloat(0)

		-- Create Animating
		world:CreateComponentAndAddTo("Animating", entity)
		world:GetComponent(entity, "Animating", "Id"):SetInt(Id)
		world:GetComponent(entity, "Animating", "FrameTime"):SetFloat(frameTime)
		world:GetComponent(entity, "Animating", "Time"):SetFloat(animationTime)
		world:GetComponent(entity, "Animating", "Timer"):SetFloat(0)

		print("Play Animation: "..Id)
		
		world:RemoveComponentFrom("Animation", entity)
	end
end
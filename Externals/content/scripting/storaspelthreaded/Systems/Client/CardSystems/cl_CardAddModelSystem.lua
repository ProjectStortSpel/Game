CardAddModelSystem = System()
CardAddModelSystem.UpOffset = -0.2
CardAddModelSystem.Arrow = -1

CardAddModelSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CardAddModelSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
end

CardAddModelSystem.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		
		world:CreateComponentAndAddTo("Model", entityId)
		world:CreateComponentAndAddTo("Position", entityId)
		world:CreateComponentAndAddTo("Rotation", entityId)
		world:CreateComponentAndAddTo("Scale", entityId)
		world:CreateComponentAndAddTo("PickBox", entityId)
		
		local pickbox = world:GetComponent(entityId, "PickBox", 0)
		pickbox:SetFloat2(0.6, 1.0)
		
		local position = world:GetComponent(entityId, "Position", 0)
		position:SetFloat3(0, -3, -4)
		
		local scale = world:GetComponent(entityId, "Scale", 0)
		scale:SetFloat3(0.9, 0.9, 0.9)
		
		local rotation = world:GetComponent(entityId, "Rotation", 0)
		rotation:SetFloat3(0, 0, 0)
		
		local model = world:GetComponent(entityId, "Model", 0)
		local action = world:GetComponent(entityId, "CardAction", 0):GetText()
		local	modelName		=	string.lower(action)
		model:SetModel(modelName, "cards", 2)
		
		-- HOVERSCALE
		self:AddHoverSize(1.1, entityId)
		
		
		-- PRIO TEXT WOOOOOT?!?!?!?
		local prio = world:GetComponent(entityId, "CardPrio", 0):GetInt()
		
		local id = world:CreateNewEntity()
		world:CreateComponentAndAddTo("Model", id)
		world:CreateComponentAndAddTo("Parent", id)
		world:CreateComponentAndAddTo("KillWhenOrphan", id)
		world:CreateComponentAndAddTo("Position", id)
		world:CreateComponentAndAddTo("Rotation", id)
		world:CreateComponentAndAddTo("Scale", id)
		local model = world:GetComponent(id, "Model", 0)
		model:SetModel("right", "text", 3, false)
		local parent = world:GetComponent(id, "Parent", 0)
		parent:SetInt(entityId)
		local position = world:GetComponent(id, "Position", 0)
		position:SetFloat3(0.24, 0.47, 0.01)
		local scale = world:GetComponent(id, "Scale", 0)
		scale:SetFloat3(0.30, 0.15, 1)
		local rotation = world:GetComponent(id, "Rotation", 0)
		rotation:SetFloat3(0, 0, 0)

		world:CreateComponentAndAddTo("TextTexture", id)
		world:GetComponent(id, "TextTexture", "Name"):SetText("prio"..prio) -- TODO: NAME CANT BE MORE THAN 3 CHARS? WTF?
		world:GetComponent(id, "TextTexture", "Text"):SetText(prio)
		world:GetComponent(id, "TextTexture", "FontIndex"):SetInt(0)
		world:GetComponent(id, "TextTexture", "R"):SetFloat(0)
		world:GetComponent(id, "TextTexture", "G"):SetFloat(0)
		world:GetComponent(id, "TextTexture", "B"):SetFloat(0)
	end
	
	local unit = world:GetComponent(entities[1], "Card", "Unit"):GetInt()
	self:CreateArrow(unit)
end

CardAddModelSystem.EntitiesRemoved = function(self, dt, entities)
	self:RemoveArrow()
end

CardAddModelSystem.CreateArrow = function(self, unit)
	if self.Arrow == -1 then
		local arrow = world:CreateNewEntity()
		world:CreateComponentAndAddTo("Parent", arrow)
		world:CreateComponentAndAddTo("Model", arrow)
		world:CreateComponentAndAddTo("Position", arrow)
		world:CreateComponentAndAddTo("Rotation", arrow)
		world:CreateComponentAndAddTo("Scale", arrow)
		world:CreateComponentAndAddTo("LerpScale", arrow)
		world:GetComponent(arrow, "Position", 0):SetFloat3(0.0, 1.5, 0.05)
		world:GetComponent(arrow, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
		world:GetComponent(arrow, "Scale", 0):SetFloat3(0.0, 0.0, 0.0)
		world:GetComponent(arrow, "Model", 0):SetModel("arrow", "arrow", 1, false)
		world:GetComponent(arrow, "Parent", 0):SetInt(unit)
		world:GetComponent(arrow, "LerpScale", "Time", 0):SetFloat4(0.2, 0.4, 0.4, 0.4)
		world:GetComponent(arrow, "LerpScale", "Algorithm", 0):SetText("SmoothLerp")
		self.Arrow = arrow
	end
end

CardAddModelSystem.RemoveArrow = function(self)
	if self.Arrow ~= -1 then
		if not world:EntityHasComponent(self.Arrow, "LerpScale") then
			world:CreateComponentAndAddTo("LerpScale", self.Arrow)
		end
		world:CreateComponentAndAddTo("KillAfterLerp", self.Arrow)
		world:GetComponent(self.Arrow, "LerpScale", "Time", 0):SetFloat4(0.2, 0.0, 0.0, 0.0)
		world:GetComponent(self.Arrow, "LerpScale", "Algorithm", 0):SetText("SmoothLerp")
		self.Arrow = -1
	end
end

CardAddModelSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end
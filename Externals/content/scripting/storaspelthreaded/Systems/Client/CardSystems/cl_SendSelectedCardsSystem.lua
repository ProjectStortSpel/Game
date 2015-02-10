SendSelectedCardsSystem = System()

SendSelectedCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("SendSelectCardsSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardSelected", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ReadyButton", FilterType.RequiresOneOf)
end

SendSelectedCardsSystem.Update = function(self, dt, taskIndex, taskCount)

	local button = self:GetEntities("ReadyButton")

	if #button > 0 then
		if world:EntityHasComponent(button[1], "OnPickBoxHit") and Input.GetTouchState(0) == InputState.Released then
		
			self:SendSelectedCards()
			self:DeselectAll()
		end
		
		local cardsSelected = self:GetEntities("CardSelected")
		if #cardsSelected < 5 then
			for n = 1, #button do
				world:KillEntity(button[n])
			end
		end
	end
	
end

SendSelectedCardsSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "CardSelected") then
			local cards = self:GetEntities("CardSelected")
			
			if #cards == 5 then
				--self:SendSelectedCards()
				--self:DeselectAll()
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("Model", id)
				world:CreateComponentAndAddTo("Position", id)
				world:CreateComponentAndAddTo("Rotation", id)
				world:CreateComponentAndAddTo("Scale", id)
				world:CreateComponentAndAddTo("PickBox", id)
				world:CreateComponentAndAddTo("ReadyButton", id)
				
				local model = world:GetComponent(id, "Model", 0)
				model:SetModel("readybutton", "quad", 2)
				
				local position = world:GetComponent(id, "Position", 0)
				position:SetFloat3(3, 0, -4)
				
				local scale = world:GetComponent(id, "Scale", 0)
				scale:SetFloat3(1, 0.5, 1)
				
				local pickbox = world:GetComponent(id, "PickBox", 0)
				pickbox:SetFloat2(1, 1)

			end
		end
	end
end

SendSelectedCardsSystem.SendSelectedCards = function( self )
	
	local id = Net.StartPack("Server.SelectCards")
	local cards = self:GetEntities("CardSelected")

	for i = 1, 5 do	
		
		for j = 1, #cards do
			
			local index = world:GetComponent(cards[i], "SelectCard", "Index"):GetInt()

			if index == i then			
				Net.WriteInt(id, Net.ToServerID(cards[i]))
				break
			end

		end

			
	end
	Net.SendToServer(id)
end

SendSelectedCardsSystem.DeselectAll = function( self )
	
	local cards = self:GetEntities("CardSelected")

	for i = 1, #cards do
		world:RemoveComponentFrom("CardSelected", cards[i])
	end
	
	local buttons = self:GetEntities("ReadyButton")
	for n = 1, #buttons do
		world:KillEntity(buttons[n])
	end
end
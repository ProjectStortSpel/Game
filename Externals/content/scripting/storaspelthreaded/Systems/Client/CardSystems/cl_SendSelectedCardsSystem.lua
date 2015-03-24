SendSelectedCardsSystem = System()
SendSelectedCardsSystem.RequestRelease = false

SendSelectedCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("SendSelectCardsSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("CardSelected", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ReadyButton", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("DealingSettings", FilterType.RequiresOneOf)
end

SendSelectedCardsSystem.Update = function(self, dt)

	local button = self:GetEntities("ReadyButton")

	if #button > 0 then
		if world:EntityHasComponent(button[1], "OnPickBoxHit") and self.RequestRelease then
		
			self:SendSelectedCards()
			self:DeselectAll()
		end
		
		local DealingSettings = self:GetEntities("DealingSettings")
		local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
		
		local cardsSelected = self:GetEntities("CardSelected")
		if #cardsSelected < cardsToPick then
			for n = 1, #button do
				world:KillEntity(button[n])
			end
		end
		
		if Input.GetTouchState(0) == InputState.Released then
			self.RequestRelease = true
		else
			self.RequestRelease = false
		end
	end
	
end

SendSelectedCardsSystem.EntitiesAdded = function(self, dt, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "CardSelected") then
		
		
			--Audio.PlaySound("Cards2", "Cards2"..entityId, false)
			
			
			local cards = self:GetEntities("CardSelected")
			
			local DealingSettings = self:GetEntities("DealingSettings")
			local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)
			
			if #cards == cardsToPick then
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
				
				local rotation = world:GetComponent(id, "Rotation", 0)
				rotation:SetFloat3(0.0, 0.0, 0.0)
				
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
	
	local DealingSettings = self:GetEntities("DealingSettings")
	local cardsPerHand, cardsToPick = world:GetComponent(DealingSettings[1], "DealingSettings", 0):GetInt2(0)

	for i = 1, cardsToPick do	
		
		for j = 1, #cards do
			
			local index = world:GetComponent(cards[j], "SelectCard", "Index"):GetInt()
			if index == i then			
				Net.WriteInt(id, Net.ToServerID(cards[j]))
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
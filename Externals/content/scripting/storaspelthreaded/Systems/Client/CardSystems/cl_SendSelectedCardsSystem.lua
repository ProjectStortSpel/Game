SendSelectedCardsSystem = System()
SendSelectedCardsSystem.SelectCardSystem = false

SendSelectedCardsSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("SendSelectCardsSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()
	self:UsingEntitiesRemoved()
	
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
	
	end
	
end

SendSelectedCardsSystem.EntitiesRemoved = function(self, dt, taskIndex, taskCount, entities)
	local button = self:GetEntities("ReadyButton")
	for i = 1, #button do
		
		world:KillEntity(button[i])

	end
end


SendSelectedCardsSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)

	for n = 1, #entities do
		local entityId = entities[n]
		if world:EntityHasComponent(entityId, "CardSelected") then
			local cards = self:GetEntities("CardSelected")

			print("numselected: " .. #cards)

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
				
				local model = self:GetComponent(id, "Model", 0)
				model:SetModel("readybutton", "quad", 2)
				
				local position = self:GetComponent(id, "Position", 0)
				position:SetFloat3(3, 0, -4)
				
				local scale = self:GetComponent(id, "Scale", 0)
				scale:SetFloat3(1, 0.5, 1)
				
				local pickbox = self:GetComponent(id, "PickBox", 0)
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

end
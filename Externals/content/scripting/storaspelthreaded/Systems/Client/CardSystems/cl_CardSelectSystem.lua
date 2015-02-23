CardSelectSystem = System()
CardSelectSystem.Hold = -1
CardSelectSystem.Holding = false
CardSelectSystem.startX = 0
CardSelectSystem.startY = 0

CardSelectSystem.Initialize = function(self)
	--	Set Name
	self:SetName("CardSelectSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesRemoved()
	
	--	Set Filter
	self:AddComponentTypeToFilter("Position", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardPrio", FilterType.Mandatory)
	self:AddComponentTypeToFilter("CardAction", FilterType.Mandatory)
end

CardSelectSystem.EntitiesRemoved = function(self, dt, newEntities)
	local cards = self:GetEntities("CardHolding")
	if #cards > 0 then
	else
		self.Holding = false
	end
end

CardSelectSystem.Update = function(self, dt)

	if Input.GetTouchState(0) == InputState.Released then
		local mX, mY = GraphicDevice.GetTouchPosition()
		local aspectX, aspectY = GraphicDevice.GetAspectRatio()
		local rX = mX * aspectX * 7
		local rY = mY * aspectY * 7
		local deltaX = self.startX - rX
		local deltaY = self.startY - rY
		local length = math.sqrt(deltaX*deltaX + deltaY*deltaY)
		if length > 0.1 then
			self:DropCard()
		else
			self:PickCard()
		end
	end
	if Input.GetTouchState(0) == InputState.Down then
		local mX, mY = GraphicDevice.GetTouchPosition()
		local aspectX, aspectY = GraphicDevice.GetAspectRatio()
		if not self.Holding then
			local cards = self:GetEntities("OnPickBoxHit")
			if #cards > 0 then
				self.startX = mX * aspectX * 7
				self.startY = mY * aspectY * 7
				local card = cards[1]
				self.Hold = card
				self.Holding = true
				if world:EntityHasComponent(card, "SelectCard") then
					world:RemoveComponentFrom("SelectCard", card)
				end
				world:CreateComponentAndAddTo("CardHolding", card)
			end
		else
			local rX = mX * aspectX * 7
			local rY = mY * aspectY * 7
			world:GetComponent(self.Hold, "Position", "X"):SetFloat(rX)
			world:GetComponent(self.Hold, "Position", "Y"):SetFloat(rY)
			world:GetComponent(self.Hold, "Position", "Z"):SetFloat(-3.8)
		end
	end
end

CardSelectSystem.PickCard = function(self)
	if self.Holding then
		local mX, mY = GraphicDevice.GetTouchPosition()
		local aspectX, aspectY = GraphicDevice.GetAspectRatio()
		local rX = mX * aspectX * 12
		local rY = mY * aspectY * 12
		
		if rY < -1.5 then
			self:SelectCard(self.Hold, 99)
		end
		
		self.Holding = false
		if world:EntityHasComponent(self.Hold, "CardHolding") then
			world:RemoveComponentFrom("CardHolding", self.Hold)
		end
	end
end

CardSelectSystem.DropCard = function(self)
	if self.Holding then
		local mX, mY = GraphicDevice.GetTouchPosition()
		local aspectX, aspectY = GraphicDevice.GetAspectRatio()
		local rX = mX * aspectX * 12
		local rY = mY * aspectY * 12
		
		if rY > -1.5 then
			local cards = self:GetEntities("CardSelected")
			local index = 1
			for i = 1, #cards do
				local card = cards[i]
				local X = world:GetComponent(card, "Position", "X"):GetFloat()
				if rX > X then
					index = index + 1
				end
			end
			self:SelectCard(self.Hold, index)
		end
		
		self.Holding = false
		if world:EntityHasComponent(self.Hold, "CardHolding") then
			world:RemoveComponentFrom("CardHolding", self.Hold)
		end
	end
end

CardSelectSystem.SelectCard = function(self, card, index)
	world:CreateComponentAndAddTo("SelectCard", card)
	world:SetComponent(card, "SelectCard", "Index", index)
end
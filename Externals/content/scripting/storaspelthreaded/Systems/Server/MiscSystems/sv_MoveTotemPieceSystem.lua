MoveTotemPieceSystem = System()

MoveTotemPieceSystem.Initialize = function(self)
	--	Set Name
	self:SetName("MoveTotemPieceSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("TotemPole", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("TotemPiece", FilterType.RequiresOneOf)
end

MoveTotemPieceSystem.MovePieceUp = function(self, totemPieceId)
	
	local	currentHeight	=	world:GetComponent(totemPieceId, "TotemPiece", "CurrentHeight"):GetInt()
	local	X,Y,Z			=	world:GetComponent(totemPieceId, "Position", "X"):GetFloat3()
	local	newHeight		=	0.95 + (currentHeight)*0.27
	
	if not world:EntityHasComponent(totemPieceId, "LerpPosition") then
		world:CreateComponentAndAddTo("LerpPosition", totemPieceId)
	end
	world:GetComponent(totemPieceId, "LerpPosition", "X"):SetFloat(X)
	world:GetComponent(totemPieceId, "LerpPosition", "Y"):SetFloat(newHeight)
	world:GetComponent(totemPieceId, "LerpPosition", "Z"):SetFloat(Z)
	world:GetComponent(totemPieceId, "LerpPosition", "Time"):SetFloat(0.4)
	world:GetComponent(totemPieceId, "LerpPosition", "Algorithm"):SetText("ExponentialLerp")

end

MoveTotemPieceSystem.MovePiecesUp = function(self, totemPoleId)

	local	totemPieces 	= 	self:GetEntities("TotemPiece")
	local	highestPiece 	= 	-1
	local	highestPieceId	=	-1
	for i = 1, #totemPieces do
		
		local	currentPiece		=	totemPieces[i]
		local	currentPiecePoleId	=	world:GetComponent(currentPiece, "TotemPiece", "TotemPoleId"):GetInt()
		
		if currentPiecePoleId == totemPoleId then
			local	tpHeight	=	world:GetComponent(currentPiece, "TotemPiece", "CurrentHeight")
			
			self:MovePieceUp(currentPiece)
			tpHeight:SetInt(tpHeight:GetInt()+1)
			
			if tpHeight:GetInt() > highestPiece then
				highestPieceId = currentPiece
				highestPiece = tpHeight:GetInt()
			end
		end
	end
	
	if highestPieceId ~= -1 then
		world:GetComponent(highestPieceId, "LerpPosition", "Time"):SetFloat(0.8)
		world:GetComponent(highestPieceId, "LerpPosition", "Algorithm"):SetText("OvershotLerp")
	end

	
end

MoveTotemPieceSystem.EntitiesAdded = function(self, dt, addedEntities)

	for n = 1, #addedEntities do
		local	newEntity	=	addedEntities[n]
		
		if world:EntityHasComponent(newEntity, "TotemPiece") then
		
			self:MovePiecesUp(world:GetComponent(newEntity, "TotemPiece", "TotemPoleId"):GetInt())
		end
	
	end 
end
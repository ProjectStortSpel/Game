FadeTreeSystem = System()
FadeTreeSystem.TreeTransparent	=	false

FadeTreeSystem.Initialize = function(self)
	--	Set Name
	self:SetName("FadeTreeSystem")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()
	
	--	Set Filter
	self:AddComponentTypeToFilter("IsTree", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ToggleTree", FilterType.RequiresOneOf)
	
end

FadeTreeSystem.EntitiesAdded = function(self, dt, addedEntities)
	
	for n = 1, #addedEntities do
		--	Get entity ID
		local newEntity = addedEntities[n]
		
		--	Add Totem Piece logic
		if world:EntityHasComponent(newEntity, "IsTree") then
			
		end
		
		--	OnPickingPhase, change to transparent
		if world:EntityHasComponent(newEntity, "ToggleTree") then
			local	allTrees	=	self:GetEntities("IsTree")
			if self.TreeTransparent then
				self.TreeTransparent	=	false
				for i = 1, #allTrees do
					local	modelId	=	world:GetComponent(allTrees[i], "Render", "ModelId"):GetInt()
					GraphicDevice.ChangeModelTexture(modelId, "models/tree/tree.png")
				end
			else
				self.TreeTransparent	=	true
				for i = 1, #allTrees do
					local	modelId	=	world:GetComponent(allTrees[i], "Render", "ModelId"):GetInt()
					GraphicDevice.ChangeModelTexture(modelId, "models/tree/treetrans.png")
				end
			end
			
			world:KillEntity(newEntity)
		end

	end 
end

Net.Receive("Client.ToggleTrees", 
	function( id, ip, port )
		local	newEntity	=	world:CreateNewEntity()
		world:CreateComponentAndAddTo("ToggleTree", newEntity)
	end 
)
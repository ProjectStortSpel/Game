PlayerIndicatorSystem = System()
PlayerIndicatorSystem.spin = 0.1

PlayerIndicatorSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("PlayerIndicatorSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	
	--	Filters
	self:AddComponentTypeToFilter("PlayerIndicator", FilterType.Mandatory)
end


PlayerIndicatorSystem.Update = function(self, dt, taskIndex, taskCount)
	self.spin = self.spin + dt
	local entities = self:GetEntities()
	for i = 1, #entities do
		local rotation = world:GetComponent(entities[i], "Rotation", 0)
		rotation:SetFloat3(0, 90 + self.spin, 0)
	end
end

Net.Receive("Client.SendPlayerUnitId", 
	function( id, ip, port )

	local playerId = Net.ReadInt(id)
	print(playerId)

	local newEntityId = world:CreateNewEntity("Head")


	local rotation = world:GetComponent(newEntityId, "Rotation", 0)
	local position = world:GetComponent(newEntityId, "Position", 0)
	local scale = world:GetComponent(newEntityId, "Scale", 0)

	rotation:SetFloat3(0, math.pi, 0)
	position:SetFloat3(0, 1, 0)
	scale:SetFloat3(0.75, 0.75, 0.75)

	world:CreateComponentAndAddTo("PlayerIndicator", newEntityId)
	world:CreateComponentAndAddTo("PlayerNumber", newEntityId)

	local rotation = world:GetComponent(newEntityId, "Rotation", 0)
	local position = world:GetComponent(newEntityId, "Position", 0)
	rotation:SetFloat3(0, 90, 0)
	position:SetFloat3(4, 2.8, -7)
	world:SetComponent(newEntityId, "Model", "ModelName", "ply");
	world:SetComponent(newEntityId, "Model", "ModelPath", "ply");
	world:SetComponent(newEntityId, "Model", "RenderType", 2);

	end 
)
PlayerIndicatorSystem = System()
PlayerIndicatorSystem.spin = 0.1

PlayerIndicatorSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("PlayerIndicatorSystem")
	
	--	Filters
	self:AddComponentTypeToFilter("PlayerIndicator", FilterType.Mandatory)
end

Net.Receive("Client.SendPlayerUnitColor", 
	function( id, ip, port )

	local r = Net.ReadFloat(id)
	local g = Net.ReadFloat(id)
	local b = Net.ReadFloat(id)

	local newEntityId = world:CreateNewEntity("Head")

	local rotation = world:GetComponent(newEntityId, "Rotation", 0)
	local position = world:GetComponent(newEntityId, "Position", 0)
	local scale = world:GetComponent(newEntityId, "Scale", 0)
	if not world:EntityHasComponent(newEntityId, "Color") then
		world:CreateComponentAndAddTo("Color", newEntityId)
	end
	world:GetComponent(newEntityId, "Color", "X"):SetFloat3(r, g, b)

	world:CreateComponentAndAddTo("PlayerIndicator", newEntityId)
	world:CreateComponentAndAddTo("PlayerNumber", newEntityId)

	rotation:SetFloat3(0, 0, 0)
	position:SetFloat3(-3.8, 2.2, -5)
	scale:SetFloat3(1, 0.5, 1)
	world:SetComponent(newEntityId, "Model", "ModelName", "totem");
	world:SetComponent(newEntityId, "Model", "ModelPath", "quad");
	world:SetComponent(newEntityId, "Model", "RenderType", 2);

	end 
)
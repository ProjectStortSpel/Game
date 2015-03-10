PlayerIndicatorSystem = System()

PlayerIndicatorSystem.Initialize = function ( self )
	--	Set Name
	self:SetName("PlayerIndicatorSystem")
	
	--	Filters
	self:AddComponentTypeToFilter("PlayerIndicator", FilterType.Mandatory)
end

Net.Receive("Client.SendMyUnitID", 

	function( id, ip, port )
	local entityId = Net.ToClientID(Net.ReadInt(id))
	world:CreateComponentAndAddTo("MyUnit", entityId)
	
	end 
)
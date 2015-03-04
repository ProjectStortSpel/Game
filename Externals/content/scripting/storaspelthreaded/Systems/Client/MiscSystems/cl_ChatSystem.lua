ChatSystem = System()

ChatSystem.Initialize = function(self)
	
	--	Set Name
	self:SetName("ChatSystem")
	self:InitializeNetworkEvents()
	self:AddComponentTypeToFilter("NULL", FilterType.Mandatory)
end

Net.Receive("SERVER_BROADCAST_CHAT_MESSAGE", 
	function( id, ip, port )

	local sender = Net.ReadString(id)
	local message = Net.ReadString(id)
	print(sender .. ": " .. message)
	--Console.AddToHistory(sender .. ": " .. message)
	
	end
)
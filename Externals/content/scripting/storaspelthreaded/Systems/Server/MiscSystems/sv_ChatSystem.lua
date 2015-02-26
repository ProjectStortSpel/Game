ChatSystem = System()

ChatSystem.Initialize = function(self)
	
	--	Set Name
	self:SetName("ChatSystem")
	self:InitializeNetworkEvents()
	self:AddComponentTypeToFilter("NULL", FilterType.Mandatory)
end

Net.Receive("CLIENT_SEND_CHAT_MESSAGE", 
	function( id, ip, port )

	local message = Net.ReadString(id)
	print("Server received message: " .. message)
	
	local id = Net.StartPack("SERVER_BROADCAST_CHAT_MESSAGE")
	Net.WriteString(id, ip)
	Net.WriteString(id, message)
	Net.Broadcast(id, ip, port)
	
	end
)
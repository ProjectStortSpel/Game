ServerUsernameSystem = System()
ServerUsernameSystem.Names = {}

ServerUsernameSystem.Initialize = function(self)
	self:SetName("ServerUsername System")
	self:AddComponentTypeToFilter("PlayerName", FilterType.Mandatory)	
	self:AddComponentTypeToFilter("IsSpectator", FilterType.Excluded)
	
	table.insert(self.Names, "Player1")
	table.insert(self.Names, "Player2")
	table.insert(self.Names, "Player3")
	table.insert(self.Names, "Player4")
	table.insert(self.Names, "Player5")
	table.insert(self.Names, "Player6")
	table.insert(self.Names, "Player7")
	table.insert(self.Names, "Player8")
	table.insert(self.Names, "Player9")
	table.insert(self.Names, "Player10")
	table.insert(self.Names, "Player11")
	table.insert(self.Names, "Player12")
	table.insert(self.Names, "Player13")
	table.insert(self.Names, "Player14")
	table.insert(self.Names, "Player15")
end

ServerUsernameSystem.Update = function(self, dt)
end

ServerUsernameSystem.OnEntityAdded = function(self, entityId)
	local players = self:GetEntities()
	print("NoPlayers: " .. #players)
end


Net.Receive("Server.SetUsername", 
	function( id, ip, port )

	print("Server.SetUsername")
	
	local pId = Net.ToServerID(Net.ReadInt(id))
	print("ServerID: " .. pId)
	local uname = Net.ReadString(id)
	print("Username: " .. uname)
	
	--table.insert(self.ServerId, pId)
	--table.insert(self.ServerName, uname)
	
	--world:SetComponent(pId, "PlayerName", "Name", uname)
	
	end
)
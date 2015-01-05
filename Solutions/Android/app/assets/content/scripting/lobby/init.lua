GameRunning = false;


-- Components

-- Position Component
local PositionComponent = ComponentType()
PositionComponent.Name = "Position"
PositionComponent.TableType = TableType.Array
PositionComponent:AddVariable("X", ByteSize.Float)
PositionComponent:AddVariable("Y", ByteSize.Float)
PositionComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(PositionComponent)

-- Scale Component
local ScaleComponent = ComponentType()
ScaleComponent.Name = "Scale"
ScaleComponent.TableType = TableType.Array
ScaleComponent:AddVariable("X", ByteSize.Float)
ScaleComponent:AddVariable("Y", ByteSize.Float)
ScaleComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(ScaleComponent)

-- Rotation Component
local RotationComponent = ComponentType()
RotationComponent.Name = "Rotation"
RotationComponent.TableType = TableType.Array
RotationComponent:AddVariable("X", ByteSize.Float)
RotationComponent:AddVariable("Y", ByteSize.Float)
RotationComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(RotationComponent)

-- Velocity Component
local VelocityComponent = ComponentType()
VelocityComponent.Name = "Velocity"
VelocityComponent.TableType = TableType.Array
VelocityComponent:AddVariable("X", ByteSize.Float)
VelocityComponent:AddVariable("Y", ByteSize.Float)
VelocityComponent:AddVariable("Z", ByteSize.Float)
worldCreator:AddComponentType(VelocityComponent)

-- Render Component
local RenderComponent = ComponentType()
RenderComponent.Name = "Render"
RenderComponent.SyncNetwork = false
RenderComponent.TableType = TableType.Array
RenderComponent:AddVariable("ModelId", ByteSize.Int)
RenderComponent:AddVariable("Mat", ByteSize.Matrix)
worldCreator:AddComponentType(RenderComponent)

--	Model Component
local ModelComponent = ComponentType()
ModelComponent.Name = "Model"
ModelComponent.TableType = TableType.Map
ModelComponent:AddVariable("ModelName", ByteSize.Text)
ModelComponent:AddVariable("ModelPath", ByteSize.Text)
worldCreator:AddComponentType(ModelComponent)

-- Username Component
local NameComponent = ComponentType()
NameComponent.Name = "Username"
NameComponent.TableType = TableType.Map
NameComponent:AddVariable("Name", ByteSize.Text)
worldCreator:AddComponentType(NameComponent)

-- NetConnection Component
local NetConnection = ComponentType()
NetConnection.Name = "NetConnection"
NetConnection.TableType = TableType.Map
NetConnection:AddVariable("IpAddress", ByteSize.Text)
NetConnection:AddVariable("Port", ByteSize.Int)
NetConnection:AddVariable("Active", ByteSize.Bool)
worldCreator:AddComponentType(NetConnection)

-- Systems
local TotalTime = 0

StartUpSystem = System()
StartUpSystem.Update = function(self, dt)

end

StartUpSystem.Initialize = function(self)
	self:SetName("Start Up System")
	
	print("Start Up System initialized!")
end

StartUpSystem.PostInitialize = function(self)

end

StartUpSystem.OnEntityAdded = function(self, entityId)

end

StartUpSystem.OnEntityRemoved = function(self, entityId)
end

--if Server then
	ServerLobbySystem = System()

ServerLobbySystem.m_noConnections = 0;
ServerLobbySystem.m_maxConnections = 3;

ServerLobbySystem.Initialize = function(self)
	self:SetName("ServerLobbySystem System");

	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	ServerLobbySystem.m_maxConnections = Net.MaxConnections();
	
	print("ServerLobbySystem initialized!");
end

ServerLobbySystem.Update = function(self, dt)
	
	if GameRunning then
		return
	end
	
	if Net.IsRunning() then
		ServerLobbySystem:UpdateServerOnline();
	else
		--ServerLobbySystem:UpdateServerOffline();
	end

end

ServerLobbySystem.UpdateServerOnline = function(self)

	local text = "Server started. " .. tostring(ServerLobbySystem.m_noConnections) .. "/" .. tostring(ServerLobbySystem.m_maxConnections) .. " connected. Connected players are: ";	
	graphics:RenderSimpleText(text, 55, 5);
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local username = self:GetComponent(entities[i], "Username", "Name");
		graphics:RenderSimpleText("Player" .. tostring(i) .. ": " .. username:GetString(), 55, 6 + (i * 2));
	end
	
	if Input.GetKeyState(Key.Return) == InputState.Pressed and Console.IsOpen() == false then
		Console.Print("Game started");
		GameRunning = true;
		local id = Net.StartPack("NewGame");
		Net.Broadcast(id);
		Console.Print("NewGame");
	end
	
	
	
end

ServerLobbySystem.OnEntityAdded = function(self, entityId)
	
	if GameRunning then
		return
	end

	ServerLobbySystem.m_noConnections = ServerLobbySystem.m_noConnections + 1;
end
ServerLobbySystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

	ServerLobbySystem.m_noConnections = ServerLobbySystem.m_noConnections - 1;
end
	ServerConnectSystem = System()

ServerConnectSystem.Initialize = function(self)
	self:SetName("ServerConnectSystem System");
	
	self:InitializeNetworkEvents();
	
	Net.Receive("Username", ServerConnectSystem.OnUsername);
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	print("ServerConnectSystem initialized!");
end

ServerConnectSystem.Update = function(self, dt)

	if GameRunning then
		return
	end

end

ServerConnectSystem.OnEntityAdded = function(self, entityId)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnEntityAdded");
	local match = false;
	local matchId = 0;
	
	local username 	= self:GetComponent(entityId, "Username", "Name"):GetString();
	local ipAddress = self:GetComponent(entityId, "NetConnection", "IpAddress"):GetString();
	local port 		= self:GetComponent(entityId, "NetConnection", "Port"):GetInt();

	local entities = self:GetEntities();
	for i = 1, #entities do
	
		if entityId ~= entities[i] then
			local uname = self:GetComponent(entities[i], "Username", "Name"):GetString();
			
			if username == uname then
				match = true;
				matchId = entities[i];
				break;
			end
			
			
		end
	
	end
	
	if match then
		
		local setActive = true;
		local isActive = self:GetComponent(matchId, "NetConnection", "Active"):GetBool();
		
		if isActive then
			local oldIp 	= self:GetComponent(matchId, "NetConnection", "IpAddress"):GetString();
			local oldPort	= self:GetComponent(matchId, "NetConnection", "Port"):GetInt();
			local reason	= "ServerConnectSystem kicked you.";
			
			Net.Kick(oldIp, oldPort, reason);
		end
		
		world:SetComponent(matchId, "NetConnection", "IpAddress", ipAddress);
		world:SetComponent(matchId, "NetConnection", "Port", port);
		world:SetComponent(matchId, "NetConnection", "Active", setActive);
		world:KillEntity(entityId);
		
	else
	
		local setActive = true;
		world:SetComponent(entityId, "NetConnection", "Active", setActive);

	end
	
	
	
end

ServerConnectSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnEntityRemoved");
end

ServerConnectSystem.OnPlayerTimedOut = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnPlayerTimedOut");
	ServerConnectSystem:RemovePlayer(_ip, _port);

end

ServerConnectSystem.OnPlayerDisconnected = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnPlayerDisconnected");
	ServerConnectSystem:RemovePlayer(_ip, _port);

end

ServerConnectSystem.OnUsername = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.OnUsername");
	
	local name = _ip .. _port;
	local active = false;
	
	local eId = world:CreateNewEntity("User");
	world:SetComponent(eId, "Username", "Name", name);
	world:SetComponent(eId, "NetConnection", "IpAddress", _ip);
	world:SetComponent(eId, "NetConnection", "Port", _port);
	world:SetComponent(eId, "NetConnection", "Active", active);
	
end



ServerConnectSystem.RemovePlayer = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.RemovePlayer");
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local eIp 	= self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString();
		local ePort = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt();

		if _ip == eIp and _port == ePort then
			world:KillEntity(entities[i]);
		end
		
	end

end
--end

--if Client then
	ClientLobbySystem = System()

ClientLobbySystem.Initialize = function(self)
	self:SetName("ClientLobbySystem System")

	Net.Receive("NewGame", ClientLobbySystem.NewGame);
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	print("ClientLobbySystem initialized!")
end

ClientLobbySystem.Update = function(self, dt)
	
	if GameRunning then
		return
	end
	
	
	if Net.IsConnected() then
	
		local text = "Connected to server.";	
		graphics:RenderSimpleText(text, 55, 5);
	
	end

end

ClientLobbySystem.OnEntityAdded = function(self, entityId)

	if GameRunning then
		return
	end

end
ClientLobbySystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

end


ClientLobbySystem.NewGame = function(self, _ip, _port)
	Console.Print("ClientLobbySystem.NewGame");
end
	

ClientConnectSystem = System()

ClientConnectSystem.Update = function(self, dt)

	if GameRunning then
		return
	end

end

ClientConnectSystem.Initialize = function(self)
	self:SetName("ClientConnectSystem System");
	
	self:InitializeNetworkEvents();
	
	self:AddComponentTypeToFilter("Username", FilterType.Mandatory);
	self:AddComponentTypeToFilter("NetConnection", FilterType.Mandatory);
	
	print("ClientConnectSystem initialized!");
end

ClientConnectSystem.OnEntityAdded = function(self, entityId)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnConnectedToServer");
end

ClientConnectSystem.OnEntityRemoved = function(self, entityId)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnConnectedToServer");
end

ClientConnectSystem.OnConnectedToServer = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnConnectedToServer");
	
	local id = Net.StartPack("Username");
	Net.WriteString(id, "USERNAME_ClientConnectSystem");
	Net.SendToServer(id);
end

ClientConnectSystem.OnDisconnectedFromServer = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnDisconnectedFromServer");
end

ClientConnectSystem.OnTimedOutFromServer = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnTimedOutFromServer");
end

ClientConnectSystem.OnRemotePlayerConnected = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnRemotePlayerConnected");
	
	
end

ClientConnectSystem.OnRemotePlayerDisconnected = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnRemotePlayerDisconnected");
end

ClientConnectSystem.OnRemotePlayerKicked = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ClientConnectSystem.OnRemotePlayerKicked");
end



ServerConnectSystem.RemovePlayer = function(self, _ip, _port)

	if GameRunning then
		return
	end

	Console.Print("ServerConnectSystem.RemovePlayer");
	
	local entities = self:GetEntities();
	for i = 1, #entities do
		local eIp 	= self:GetComponent(entities[i], "NetConnection", "IpAddress"):GetString();
		local ePort = self:GetComponent(entities[i], "NetConnection", "Port"):GetInt();

		if _ip == eIp and _port == ePort then
			world:KillEntity(entities[i]);
		end
		
	end

end
--end

-- Templates

local Head = EntityTemplate()
Head.Name = "Head"
Head:SetFloat3("Position", 0.0, 0.0, 0.0)
Head:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Head:SetFloat3("Scale", 1.0, 1.0, 1.0)
Head:SetModel("Model", "head", "head")
EntityTemplateManager:AddTemplate(Head)


local User = EntityTemplate()
User.Name = "User"
User:SetString("Username", "DefaultPlayer")
User:SetString("NetConnection", "0.0.0.0", "IpAddress")
User:SetInt("NetConnection", 0, "Port")
User:SetBool("NetConnection", false, "Active")
EntityTemplateManager:AddTemplate(User)

worldCreator:AddSystemGroup()
worldCreator:AddSystemToCurrentGroup(StartUpSystem)

--if Server then
	worldCreator:AddSystemToCurrentGroup(ServerLobbySystem)
	worldCreator:AddSystemToCurrentGroup(ServerConnectSystem)
--end

--if Client then
	worldCreator:AddSystemToCurrentGroup(ClientLobbySystem)
	worldCreator:AddSystemToCurrentGroup(ClientConnectSystem)
--end
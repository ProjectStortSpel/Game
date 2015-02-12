-- Author: Marcus
IPConnectSystem = System()
IPConnectSystem.Name = "IPConnect"
IPConnectSystem.IsActive = false
IPConnectSystem.TextInput = ""
IPConnectSystem.TextEntity = -1
IPConnectSystem.BackgroundEntity = -1
IPConnectSystem.BackgroundConnectText = -1
IPConnectSystem.BackgroundExampleText = -1
IPConnectSystem.ConnectEntity = -1

IPConnectSystem.Initialize = function(self)
	self:SetName("IPConnectSystem");
	
	self:UsingUpdate();
	self:UsingEntitiesAdded();
	
	self:AddComponentTypeToFilter("IPConnectEntry", FilterType.Mandatory)
end

IPConnectSystem.Update = function(self, dt, taskIndex, taskCount)
	if self.IsActive then
		if Input.GetTextInput() ~= self.TextInput then
			self:UpdateText()
		end
		if Input.GetKeyState(Key.Return) == InputState.Pressed then
			Console.AddToCommandQueue("connect " .. self.TextInput)
			self:Deactivate()
		end
		
		if Input.GetTouchState(0) == InputState.Released then
			local pressedButtons = self:GetEntities("OnPickBoxHit")
			if #pressedButtons > 0 then
				local pressedButton = pressedButtons[1]
				if world:EntityHasComponent(pressedButton, "HoverSize") then
					local textInput = self.TextInput
					if string.find(textInput, ":") ~= nil then
						local colonStart, colonEnd = string.find(textInput, ":")
						local ip = string.sub(textInput, 1, colonStart - 1)
						local port = string.sub(textInput, colonEnd + 1, string.len(textInput))
						Console.AddToCommandQueue("connect " .. ip .. " " .. port)
					else
						Console.AddToCommandQueue("connect " .. self.TextInput)
					end
					self:Deactivate()
				end
			else
				self:Deactivate()
				
				local id = world:CreateNewEntity()
				world:CreateComponentAndAddTo("ConnectMenu", id)
			end
		end
	end
end

IPConnectSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	if not self.IsActive then
		self:Activate()
	end
end

IPConnectSystem.Activate = function(self)
	Input.StartTextInput()
	Input.SetTextInput("")
	
	self.IsActive = true
	self.TextInput = ""
	
	self:CreateBackground()
	self:CreateConnectButton()
	self:UpdateText()
end

IPConnectSystem.Deactivate = function(self)
	Input.SetTextInput("")
	Input.StopTextInput()

	self.IsActive = false
	self.TextInput = ""
	
	self:DeleteTextEntity()
	self:DeleteConnectButton()
	self:DeleteBackground()
end

IPConnectSystem.UpdateText = function(self)
	local textInput = self:GetValidatedInputString()
	
	self:DeleteTextEntity()

	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("TextTexture", id)
	
	world:GetComponent(id, "Model", 0):SetModel("left", "text", 3)
	world:GetComponent(id, "Position", 0):SetFloat3(-0.57, 0.21, -2.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(2.0, 0.095, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(id, "TextTexture", "Name"):SetText("IPConnectText")
	world:GetComponent(id, "TextTexture", "Text"):SetText(textInput .. "_")
	world:GetComponent(id, "TextTexture", "FontIndex"):SetInt(0)
	world:GetComponent(id, "TextTexture", "R"):SetFloat(0.0)
	world:GetComponent(id, "TextTexture", "G"):SetFloat(0.0)
	world:GetComponent(id, "TextTexture", "B"):SetFloat(0.0)

	self.TextEntity = id
	self.TextInput = textInput
end

IPConnectSystem.GetValidatedInputString = function(self)
	local textInput = Input.GetTextInput()
	
	local dotCount = 0
	local numberCount = 0
	local colonCount = 0
	local byteIndex = 1
	local prevByte = 46
	
	while textInput ~= nil and byteIndex <= string.len(textInput) do
		local byte = string.byte(textInput, byteIndex)
		
		-- DotKolonn
		if byte == 46 and prevByte ~= 46 and dotCount < 3 then
			dotCount = dotCount + 1
			numberCount = 0
		-- Number before colon
		elseif byte >= 48 and byte <= 57 and numberCount < 3 and colonCount == 0 then
			numberCount = numberCount + 1
		-- Number after colon
		elseif byte >= 48 and byte <= 57 and numberCount < 5 and colonCount == 1 then
			numberCount = numberCount + 1
		-- Colon
		elseif byte == 58 and colonCount == 0 and dotCount == 3 and numberCount > 0 then
			colonCount = colonCount + 1
			numberCount = 0
		-- Abort
		else
			break
		end
		
		prevByte = byte
	  
		byteIndex = byteIndex + 1
	end
	
	print(textInput)
	textInput = string.sub(textInput, 1, byteIndex - 1)
	Input.SetTextInput(textInput)
	return textInput
end

IPConnectSystem.DeleteTextEntity = function(self)
	if self.TextEntity ~= -1 then
		world:KillEntity(self.TextEntity)
		self.TextEntity = -1
	end
end

IPConnectSystem.CreateBackground = function(self)
	local id = world:CreateNewEntity()
	
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("IPConnectEntry", id)
	
	world:GetComponent(id, "Model", 0):SetModel("ipconnectbackground", "quad", 2)
	world:GetComponent(id, "Position", 0):SetFloat3(0.0, 0.0, -2.1)
	world:GetComponent(id, "Scale", 0):SetFloat3(1.5, 1.5, 1.0)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1.0, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	
	self.BackgroundEntity = id	
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("TextTexture", id)
	
	world:GetComponent(id, "Model", 0):SetModel("left", "text", 3)
	world:GetComponent(id, "Position", 0):SetFloat3(-0.59, 0.45, -2.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(2.0, 0.15, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(id, "TextTexture", "Name"):SetText("IPConnectBackgroundConnectText")
	world:GetComponent(id, "TextTexture", "Text"):SetText("Connect via IP")
	world:GetComponent(id, "TextTexture", "FontIndex"):SetInt(0)
	world:GetComponent(id, "TextTexture", "R"):SetFloat(0.306)
	world:GetComponent(id, "TextTexture", "G"):SetFloat(0.224)
	world:GetComponent(id, "TextTexture", "B"):SetFloat(0.157)
	
	self.BackgroundConnectText = id
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("TextTexture", id)
	
	world:GetComponent(id, "Model", 0):SetModel("left", "text", 3)
	world:GetComponent(id, "Position", 0):SetFloat3(-0.55, 0.08, -2.0)
	world:GetComponent(id, "Scale", 0):SetFloat3(5.0, 0.055, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	world:GetComponent(id, "TextTexture", "Name"):SetText("IPConnectBackgroundExampleText")
	world:GetComponent(id, "TextTexture", "Text"):SetText("ex. 127.0.0.1:6112")
	world:GetComponent(id, "TextTexture", "FontIndex"):SetInt(0)
	world:GetComponent(id, "TextTexture", "R"):SetFloat(0.306)
	world:GetComponent(id, "TextTexture", "G"):SetFloat(0.224)
	world:GetComponent(id, "TextTexture", "B"):SetFloat(0.157)
	
	self.BackgroundExampleText = id
end

IPConnectSystem.DeleteBackground = function(self)
	if self.BackgroundEntity ~= -1 then
		world:KillEntity(self.BackgroundEntity)
		world:KillEntity(self.BackgroundConnectText)
		world:KillEntity(self.BackgroundExampleText)
		self.BackgroundEntity = -1
		self.BackgroundConnectText = -1
		self.BackgroundExampleText = -1
	end
end

IPConnectSystem.CreateConnectButton = function(self)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("IPConnectEntry", id)
	
	world:GetComponent(id, "Model", 0):SetModel("connect", "quad", 2)
	world:GetComponent(id, "Position", 0):SetFloat3(0.0, -0.35, -1.9)
	world:GetComponent(id, "Scale", 0):SetFloat3(0.5, 0.2, 1.0)
	world:GetComponent(id, "PickBox", 0):SetFloat2(1.0, 1.0)
	world:GetComponent(id, "Rotation", 0):SetFloat3(0.0, 0.0, 0.0)
	
	local scale = world:GetComponent(id, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", id)
	local hoversize = world:GetComponent(id, "HoverSize", 0)
	hoversize:SetFloat3(sx * 1.1, sy * 1.1, sz * 1.1)

	self.ConnectEntity = id	
end

IPConnectSystem.DeleteConnectButton = function(self)
	if self.ConnectEntity ~= -1 then
		world:KillEntity(self.ConnectEntity)
		self.ConnectEntity = -1
	end
end
ButtonPressedSystem = System()

ButtonPressedSystem.Update = function(self, dt)

	if Input.GetTouchState(0) == InputState.Released then
	
		-- TODO gör så att de bara går att selecta i selection phase
		local pressedButtons = self:GetEntities("OnPickBoxHit")
		if #pressedButtons > 0 then
			
			local pressedButton = pressedButtons[1]
			local buttonCommands = self:GetEntities("ButtonCommand")
			
			local commands = {}
			
			for i = 1, #buttonCommands do
				
				local owner = self:GetComponent(buttonCommands[i], "ButtonCommand", "Button"):GetInt()
				if (owner == pressedButton) then
					
					local command = self:GetComponent(buttonCommands[i], "ButtonCommand", "Command"):GetString()
					local index = self:GetComponent(buttonCommands[i], "ButtonCommand", "Index"):GetInt()
					commands[index] = command
					
				end
				
			end
			
			for i = 1, #commands do
				Console.AddToCommandQueue(commands[i])
			end
						
		end
	
	end
		
end

ButtonPressedSystem.Initialize = function(self)
	self:SetName("ButtonPressedSystem")
	self:AddComponentTypeToFilter("OnPickBoxHit", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("ButtonCommand", FilterType.RequiresOneOf)
	
	print("ButtonPressedSystem initialized!")
end

ButtonPressedSystem.CreateButton = function(self, object, folder, posx, posy)
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("Button", id)
	
	local model = self:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 2)
	
	local position = self:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, -4)
	
	local scale = self:GetComponent(id, "Scale", 0)
	scale:SetFloat3(1, 0.5, 1)
	
	local pickbox = self:GetComponent(id, "PickBox", 0)
	pickbox:SetFloat2(1, 1)
	
	local numcommands = self:GetComponent(id, "Button", "NumCommands")
	numcommands:SetInt(0)
	
	print("ADASDnumcommands: " .. numcommands:GetInt())
	
	return id	

end

ButtonPressedSystem.AddCommandToButton = function(self, command, button)
	
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("ButtonCommand", id)
	
	local numcommands = self:GetComponent(button, "Button", "NumCommands"):GetInt() + 1
	self:GetComponent(button, "Button", "NumCommands"):SetInt(numcommands)
	
	world:GetComponent(id, "ButtonCommand", "Command"):SetString(command)
	world:GetComponent(id, "ButtonCommand", "Button"):SetInt(button)
	world:GetComponent(id, "ButtonCommand", "Index"):SetInt(numcommands)
	
end

ButtonPressedSystem.PostInitialize = function(self)
	
	--host
	local button = self:CreateButton("host", "quad", -2, 1)
	self:AddCommandToButton("host", button)
	self:AddCommandToButton("gamemode storaspel", button)
	
	--connect localhost
	button = self:CreateButton("localhost", "quad", 0, 1)
	self:AddCommandToButton("connect", button)
	
	--connect server
	button = self:CreateButton("server", "quad", 2, 1)
	self:AddCommandToButton("connect 194.47.150.44", button)
	
	--connect erik
	button = self:CreateButton("erik", "quad", -2, 0)
	self:AddCommandToButton("connect 194.47.150.5", button)
	
	--connect niklas
	button = self:CreateButton("niklas", "quad", 0, 0)
	self:AddCommandToButton("connect 194.47.150.29", button)
	
	--connect marcus
	button = self:CreateButton("marcus", "quad", 2, 0)
	self:AddCommandToButton("connect 194.47.150.48", button)
	
	--connect christian
	button = self:CreateButton("christian", "quad", -2, -1)
	self:AddCommandToButton("connect 194.47.150.57", button)
	
	--connect pontus
	button = self:CreateButton("pontus", "quad", 0, -1)
	self:AddCommandToButton("connect 194.47.150.128", button)
	
	--connect anders
	button = self:CreateButton("anders", "quad", 2, -1)
	self:AddCommandToButton("connect 194.47.150.100", button)
	
	
	
	
	print("ButtonPressedSystem post initialized!")
end
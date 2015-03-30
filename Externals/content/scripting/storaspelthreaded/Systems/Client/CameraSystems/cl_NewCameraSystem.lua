NewCameraSystem = System()
NewCameraSystem.Camera = nil
NewCameraSystem.CameraUpX = 0
NewCameraSystem.CameraUpZ = -1
NewCameraSystem.CameraLookAtX = 6.5
NewCameraSystem.CameraLookAtZ = 6.5
NewCameraSystem.CameraDistance = 1
NewCameraSystem.mouseX = 0
NewCameraSystem.mouseY = 0
NewCameraSystem.TouchSprite1 = nil
NewCameraSystem.TouchSprite2 = nil
NewCameraSystem.TouchScreen = nil
NewCameraSystem.CameraButton = -1
NewCameraSystem.Pressed = false
NewCameraSystem.Moved = false
NewCameraSystem.CameraSetting = 0
NewCameraSystem.Help = false


NewCameraSystem.Initialize = function(self)
	--	Set Name
	self:SetName("NewCameraSystem")
	
	--	Toggle EntitiesAdded
	self:UsingUpdate()
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("CameraSystemComponent", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CameraInterestPoint", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CameraElement", FilterType.RequiresOneOf)
	--self:AddComponentTypeToFilter("MapSize", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MapSpecs", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("GameRunning", FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("MyUnit", FilterType.RequiresOneOf)
end

NewCameraSystem.EntitiesAdded = function(self, dt, entities)
	for n = 1, #entities do
		if world:EntityHasComponent(entities[n], "GameRunning") then
			self.CameraButton = self:CreateInterfaceElement("camerabutton"..self.CameraSetting, "quad", 3.3, 1.4, -4, 0.45, 0.45)
			self:AddEntityCommandToButton("CameraSystemComponent", self.CameraButton)
			self:AddHoverSize(1.5, self.CameraButton)
		elseif world:EntityHasComponent(entities[n], "CameraSystemComponent") then
			self.CameraSetting = (self.CameraSetting + 1) % 4
			self:RemoveElements()
			if self.CameraSetting == 0 then
				GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
															self.CameraUpX,0,self.CameraUpZ,
															self.CameraLookAtX,0.5,self.CameraLookAtZ,
															1)
			elseif self.CameraSetting == 1 then
				local aspectX, aspectY = GraphicDevice.GetAspectRatio()
				self.Help = true
				local element = nil
				local text = nil
				element = self:CreateElement("touchside", "quad", 0, aspectY*2.2, -2.2, 4*aspectX, 0.5*aspectX)
				text = self:CreateElement("touchside", "quad", 0, aspectY*2, -2.1, 2, 0.08)
				self:AddTextToTexture("touchside1", "ZOOM IN", 0, 1, 1, 1, text)
				
				element = self:CreateElement("touchside", "quad", 0, -aspectY*2.2, -2.2, 4*aspectX, 0.5*aspectX)
				world:GetComponent(element, "Rotation", 0):SetFloat3(0, 0, 3.14159265359)
				text = self:CreateElement("touchside", "quad", 0, -aspectY*2, -2.1, 2, 0.08)
				self:AddTextToTexture("touchside2", "ZOOM OUT", 0, 1, 1, 1, text)
				
				element = self:CreateElement("touchside", "quad", aspectX*2.2, 0, -2.2, 4*aspectY, 0.5*aspectY)
				world:GetComponent(element, "Rotation", 0):SetFloat3(0, 0, -3.14159265359*0.5)
				text = self:CreateElement("touchside", "quad", aspectX*2, 0, -2.1, 2, 0.08)
				self:AddTextToTexture("touchside3", "ROTATE RIGHT", 0, 1, 1, 1, text)
				world:GetComponent(text, "Rotation", 0):SetFloat3(0, 0, -3.14159265359*0.5)
				
				element = self:CreateElement("touchside", "quad", -aspectX*2.2, 0, -2.2, 4*aspectY, 0.5*aspectY)
				world:GetComponent(element, "Rotation", 0):SetFloat3(0, 0, 3.14159265359*0.5)
				text = self:CreateElement("touchside", "quad", -aspectX*2, 0, -2.1, 2, 0.08)
				self:AddTextToTexture("touchside4", "ROTATE LEFT", 0, 1, 1, 1, text)
				world:GetComponent(text, "Rotation", 0):SetFloat3(0, 0, 3.14159265359*0.5)
				
				self.CameraUpX = 0
				self.CameraUpZ = -1
				self.CameraDistance = 1
				GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
															self.CameraUpX,0,self.CameraUpZ,
															self.CameraLookAtX,0.5,self.CameraLookAtZ,
															1)
			elseif self.CameraSetting == 2 then
				local mapSize = self:GetEntities("MapSpecs")
				if #mapSize > 0 then
					local mapX, mapZ = world:GetComponent(mapSize[1], "MapSpecs", "SizeX"):GetInt2()
					
					local aspectX, aspectY = GraphicDevice.GetAspectRatio()
					local distX = aspectY * 0.1
					local distZ = aspectX * 0.1
					
					if mapX >= mapZ then
						self.CameraUpX = 0
						self.CameraUpZ = -1
						distX = mapX * distX
						distZ = mapZ * distZ
					else
						self.CameraUpX = -1
						self.CameraUpZ = 0
						distX = mapZ * distX
						distZ = mapX * distZ
					end
					
					self.CameraDistance = math.max(distX, distZ)
					
					GraphicDevice.GetCamera():MoveToAndLookAt(	mapX*0.5-0.5-self.CameraUpX,self.CameraDistance*10+0.5,mapZ*0.5-0.5-self.CameraUpZ,
																self.CameraUpX,0,self.CameraUpZ,
																mapX*0.5-0.5-self.CameraUpX,0.5,mapZ*0.5-0.5-self.CameraUpZ,
																1)
				end
			elseif self.CameraSetting == 3 then
				self.CameraUpX = 0
				self.CameraUpZ = -1
				self.CameraDistance = 0.8
				GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
															self.CameraUpX,0,self.CameraUpZ,
															self.CameraLookAtX,0.5,self.CameraLookAtZ,
															1)
			end
			
			-- UPDATE BUTTON
			world:KillEntity(self.CameraButton)
			self.CameraButton = self:CreateInterfaceElement("camerabutton"..self.CameraSetting, "quad", 3.3, 1.4, -4, 0.45, 0.45)
			self:AddEntityCommandToButton("CameraSystemComponent", self.CameraButton)
			self:AddHoverSize(1.5, self.CameraButton)
			
			world:KillEntity(entities[n])
		elseif world:EntityHasComponent(entities[n], "CameraInterestPoint") then
			if self.CameraSetting == 0 then
				self:DoCIP(entities[n])
			end
			world:KillEntity(entities[n])
		elseif world:EntityHasComponent(entities[n], "MapSpecs") then
		
			local mapX, mapZ = world:GetComponent(entities[n], "MapSpecs", "SizeX"):GetInt2()
			
			GraphicDevice.SetShadowmapBounds(mapX-2, mapZ-2, mapX*0.5, 0.5, mapZ*0.5)
		
		end
	end
end

NewCameraSystem.Update = function(self, dt)
	if self.CameraSetting == 1 then
		self:DoFreeCam(dt)
	elseif self.CameraSetting == 3 then
		self:DoTPC()
	else
		
	end
end


NewCameraSystem.PostInitialize = function(self)

	self.TouchScreen = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Position", self.TouchScreen)
	world:CreateComponentAndAddTo("Rotation", self.TouchScreen)
	world:CreateComponentAndAddTo("Scale", self.TouchScreen)
	world:CreateComponentAndAddTo("PickBox", self.TouchScreen)
	local rposition = world:GetComponent(self.TouchScreen, "Position", 0)
	rposition:SetFloat3(0, 0, -100)		
	local rscale = world:GetComponent(self.TouchScreen, "Scale", 0)
	rscale:SetFloat3(200, 200, 200)	
	local pickbox = world:GetComponent(self.TouchScreen, "PickBox", 0)
	pickbox:SetFloat2(1.0, 1.0)
	
	self.TouchSprite1 = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", self.TouchSprite1)
	local model = world:GetComponent(self.TouchSprite1, "Model", 0)
	model:SetModel("touch", "quad", 3)
	world:CreateComponentAndAddTo("Position", self.TouchSprite1)
	world:CreateComponentAndAddTo("Rotation", self.TouchSprite1)
	world:CreateComponentAndAddTo("Scale", self.TouchSprite1)
	local rposition = world:GetComponent(self.TouchSprite1, "Position", 0)
	rposition:SetFloat3(0, 0, 1)		
	local rscale = world:GetComponent(self.TouchSprite1, "Scale", 0)
	rscale:SetFloat3(0.1, 0.1, 0.1)	
	
	self.TouchSprite2 = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", self.TouchSprite2)
	local model = world:GetComponent(self.TouchSprite2, "Model", 0)
	model:SetModel("touch", "quad", 3)
	world:CreateComponentAndAddTo("Position", self.TouchSprite2)
	world:CreateComponentAndAddTo("Rotation", self.TouchSprite2)
	world:CreateComponentAndAddTo("Scale", self.TouchSprite2)	
	local lposition = world:GetComponent(self.TouchSprite2, "Position", 0)
	lposition:SetFloat3(0, 0, 1)		
	local lscale = world:GetComponent(self.TouchSprite2, "Scale", 0)
	lscale:SetFloat3(0.05, 0.05, 0.05)	
	
	GraphicDevice.GetCamera():MoveToAndLookAt(	-15,20,-35,
												0,1,0.5,
												0,0,0,
												0)
end

NewCameraSystem.DoTPC = function(self, entityId)
	local unitPosition = self:GetEntities("MyUnit")
	if #unitPosition > 0 then
		if world:EntityHasComponent(unitPosition[1], "MapPosition") then
			local unitX, unitZ = world:GetComponent(unitPosition[1], "MapPosition", 0):GetInt2()
			local unitDirX, unitDirZ = world:GetComponent(unitPosition[1], "Direction", "X"):GetInt2()
			if self.CameraUpX ~= unitDirX or self.CameraUpZ ~= unitDirZ or self.CameraLookAtX ~= unitX or self.CameraLookAtZ ~= unitZ then
				self.CameraUpX = unitDirX
				self.CameraUpZ = unitDirZ
				self.CameraLookAtX = unitX
				self.CameraLookAtZ = unitZ
				GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
										self.CameraUpX,0,self.CameraUpZ,
										self.CameraLookAtX,0.5,self.CameraLookAtZ,
										1)
			end
			return
		end
	end
	local noUnit = world:CreateNewEntity()
	world:CreateComponentAndAddTo("CameraSystemComponent", noUnit)
end

NewCameraSystem.DoCIP = function(self, entityId)

	self.CameraUpX = world:GetComponent(entityId, "CameraInterestPoint", "UpX"):GetFloat(0)
	self.CameraUpZ = world:GetComponent(entityId, "CameraInterestPoint", "UpZ"):GetFloat(0)
	self.CameraLookAtX = world:GetComponent(entityId, "CameraInterestPoint", "AtX"):GetFloat(0)
	self.CameraLookAtZ = world:GetComponent(entityId, "CameraInterestPoint", "AtZ"):GetFloat(0)
	self.CameraDistance = world:GetComponent(entityId, "CameraInterestPoint", "Distance"):GetFloat(0)
	local lerptime = world:GetComponent(entityId, "CameraInterestPoint", "Time"):GetFloat(0)

	GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
									self.CameraUpX,0,self.CameraUpZ,
									self.CameraLookAtX,0.5,self.CameraLookAtZ,
									lerptime)
end

NewCameraSystem.DoFreeCam = function(self, dt)

	if Input.GetMouseButtonState(MouseButton.Mouse4) == InputState.Released then
		self.CameraDistance = self.CameraDistance + 0.2
		if self.CameraDistance > 1.8 then
			self.CameraDistance = 1.8
		end
		
		GraphicDevice.GetCamera():MoveToAndLookAt(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
												self.CameraUpX,0,self.CameraUpZ,
												self.CameraLookAtX,0.5,self.CameraLookAtZ,
												0.5)
	elseif Input.GetMouseButtonState(MouseButton.Mouse5) == InputState.Released then
		self.CameraDistance = self.CameraDistance - 0.2
		if self.CameraDistance < 0.4 then
			self.CameraDistance = 0.4
		end
		GraphicDevice.GetCamera():MoveToAndLookAt(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
												self.CameraUpX,0,self.CameraUpZ,
												self.CameraLookAtX,0.5,self.CameraLookAtZ,
												0.5)
	end

	if world:EntityHasComponent(self.TouchScreen, "OnPickBoxHit") then
		local move = false
		local mX, mY = GraphicDevice.GetTouchPosition()
		local aspectX, aspectY = GraphicDevice.GetAspectRatio()
		local rX = mX * aspectX * 2
		local rY = mY * aspectY * 2
		
		local deltaaspectX = aspectY / aspectX
		if Input.GetTouchState(0) == InputState.Down and self.Pressed == true then
			local dX = mX - self.mouseX
			local dZ = mY - self.mouseY
			local x, y, z = GraphicDevice.GetCamera():GetPosition()
			if dX > 0.02 * deltaaspectX then
				self.CameraLookAtX = self.CameraLookAtX - dt * dX * 25 * self.CameraUpZ
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dX * 25 * self.CameraUpX
				move = true                                    
			end                                                
			if dX < -0.02 * deltaaspectX then                       
				self.CameraLookAtX = self.CameraLookAtX - dt * dX * 25 * self.CameraUpZ
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dX * 25 * self.CameraUpX
				move = true                                    
			end                                                
			if dZ > 0.02 then                                  
				self.CameraLookAtX = self.CameraLookAtX + dt * dZ * 25 * self.CameraUpX
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dZ * 25 * self.CameraUpZ
				move = true                                    
			end                                                
			if dZ < -0.02 then                                 
				self.CameraLookAtX = self.CameraLookAtX + dt * dZ * 25 * self.CameraUpX
				self.CameraLookAtZ = self.CameraLookAtZ + dt * dZ * 25 * self.CameraUpZ
				move = true
			end

			if move == true then
				local rposition = world:GetComponent(self.TouchScreen, "Position", 0)
				rposition:SetFloat3(0, 0, -0.11)
				local rposition = world:GetComponent(self.TouchSprite1, "Position", 0)
				rposition:SetFloat3(self.mouseX * aspectX * 2, self.mouseY * aspectY * 2, -1.0001)			
				self.Moved = true
				local mapX = 0
				local mapZ = 0
				local mapSize = self:GetEntities("MapSpecs")
				if #mapSize > 0 then
					mapX, mapZ = world:GetComponent(mapSize[1], "MapSpecs", "SizeX"):GetInt2()
				end
				if self.CameraLookAtX < 0 then
					self.CameraLookAtX = 0
				end
				if self.CameraLookAtZ < 0 then
					self.CameraLookAtZ = 0
				end
				if self.CameraLookAtX > mapX-1 then
					self.CameraLookAtX = mapX-1
				end
				if self.CameraLookAtZ > mapZ-1 then
					self.CameraLookAtZ = mapZ-1
				end
				GraphicDevice.GetCamera():SetPosition(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5, y, self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5)
			end
			local rposition = world:GetComponent(self.TouchSprite2, "Position", 0)
			rposition:SetFloat3(rX, rY, -1)		
		end
		if Input.GetTouchState(0) == InputState.Released and self.Pressed == true then
			self.Pressed = false
			local rposition = world:GetComponent(self.TouchScreen, "Position", 0)
			rposition:SetFloat3(0, 0, -100)
			if self.Moved == false then
				local x, y, z = GraphicDevice.GetCamera():GetPosition()
				
				if mX > 0.5 - 0.1 * deltaaspectX then
					local temp = self.CameraUpX
					self.CameraUpX = self.CameraUpZ
					self.CameraUpZ = -1*temp
					move = true
				end
				if mX < -0.5 + 0.1 * deltaaspectX then
					local temp = self.CameraUpX
					self.CameraUpX = self.CameraUpZ*-1
					self.CameraUpZ = temp
					move = true
				end
				if mY > 0.5 - 0.1 then
					self.CameraDistance = self.CameraDistance - 0.2
					if self.CameraDistance < 0.4 then
						self.CameraDistance = 0.4
					end
					move = true
				end
				if mY < -0.5 + 0.1 then
					self.CameraDistance = self.CameraDistance + 0.2
					if self.CameraDistance > 1.8 then
						self.CameraDistance = 1.8
					end
					move = true
				end
				if move == true then
					GraphicDevice.GetCamera():MoveToAndLookAt(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
												self.CameraUpX,0,self.CameraUpZ,
												self.CameraLookAtX,0.5,self.CameraLookAtZ,
												0.5)
				end	
			end
			local rposition = world:GetComponent(self.TouchSprite1, "Position", 0)
			rposition:SetFloat3(0, 0, 1)
			local rposition = world:GetComponent(self.TouchSprite2, "Position", 0)
			rposition:SetFloat3(0, 0, 1)
		elseif Input.GetTouchState(0) == InputState.Released and self.Pressed == false then
			local rposition = world:GetComponent(self.TouchScreen, "Position", 0)
			rposition:SetFloat3(0, 0, -100)
			
			local x, y, z = GraphicDevice.GetCamera():GetPosition()
			
			if mX > 0.5 - 0.1 * deltaaspectX then
				local temp = self.CameraUpX
				self.CameraUpX = self.CameraUpZ
				self.CameraUpZ = -1*temp
				move = true
			end
			if mX < -0.5 + 0.1 * deltaaspectX then
				local temp = self.CameraUpX
				self.CameraUpX = self.CameraUpZ*-1
				self.CameraUpZ = temp
				move = true
			end
			if mY > 0.5 - 0.1 then
				self.CameraDistance = self.CameraDistance - 0.2
				if self.CameraDistance < 0.4 then
					self.CameraDistance = 0.4
				end
				move = true
			end
			if mY < -0.5 + 0.1 then
				self.CameraDistance = self.CameraDistance + 0.2
				if self.CameraDistance > 1.8 then
					self.CameraDistance = 1.8
				end
				move = true
			end
			if move == true then
				GraphicDevice.GetCamera():MoveToAndLookAt(self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
											self.CameraUpX,0,self.CameraUpZ,
											self.CameraLookAtX,0.5,self.CameraLookAtZ,
											0.5)
			end	
			
			local rposition = world:GetComponent(self.TouchSprite1, "Position", 0)
			rposition:SetFloat3(0, 0, 1)
			local rposition = world:GetComponent(self.TouchSprite2, "Position", 0)
			rposition:SetFloat3(0, 0, 1)
		end
		if Input.GetTouchState(0) == InputState.Pressed  then
			self.Pressed = true
			self.Moved = false
			self.mouseX, self.mouseY = GraphicDevice.GetTouchPosition()
		end
	end
end

Net.Receive("Client.SendCIP", 
	function(id, ip, port)
		local entity = world:CreateNewEntity()
		world:CreateComponentAndAddTo("CameraInterestPoint", entity)
		local AtX = Net.ReadFloat(id)
		local AtZ = Net.ReadFloat(id)
		local UpX = Net.ReadFloat(id)
		local UpZ = Net.ReadFloat(id)
		local Distance = Net.ReadFloat(id)
		local Time = Net.ReadFloat(id)
		world:GetComponent(entity, "CameraInterestPoint", "AtX"):SetFloat(AtX)
		world:GetComponent(entity, "CameraInterestPoint", "AtZ"):SetFloat(AtZ)
		world:GetComponent(entity, "CameraInterestPoint", "UpX"):SetFloat(UpX)
		world:GetComponent(entity, "CameraInterestPoint", "UpZ"):SetFloat(UpZ)
		world:GetComponent(entity, "CameraInterestPoint", "Distance"):SetFloat(Distance)
		world:GetComponent(entity, "CameraInterestPoint", "Time"):SetFloat(Time)
	end 
)

NewCameraSystem.CreateElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("CameraElement", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 3)
	local position = world:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = world:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1)
	local rotation = world:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end

NewCameraSystem.AddTextToTexture = function(self, n, text, font, r, g, b, button)
	world:CreateComponentAndAddTo("TextTexture", button)
	world:GetComponent(button, "TextTexture", "Name"):SetText(n)
	world:GetComponent(button, "TextTexture", "Text"):SetText(text)
	world:GetComponent(button, "TextTexture", "FontIndex"):SetInt(font)
	world:GetComponent(button, "TextTexture", "R"):SetFloat(r)
	world:GetComponent(button, "TextTexture", "G"):SetFloat(g)
	world:GetComponent(button, "TextTexture", "B"):SetFloat(b)
end

NewCameraSystem.RemoveElements = function(self)
	if self.Help == true then
		local e = self:GetEntities("CameraElement")
		for i = 1, #e do
			world:KillEntity(e[i])
		end
		self.Help = false
	end
end

NewCameraSystem.CreateInterfaceElement = function(self, object, folder, posx, posy, posz, scalex, scaley)
	local id = world:CreateNewEntity()
	world:CreateComponentAndAddTo("Model", id)
	world:CreateComponentAndAddTo("Position", id)
	world:CreateComponentAndAddTo("Rotation", id)
	world:CreateComponentAndAddTo("Scale", id)
	world:CreateComponentAndAddTo("PickBox", id)
	world:CreateComponentAndAddTo("GameInterfaceElement", id)
	local model = world:GetComponent(id, "Model", 0)
	model:SetModel(object, folder, 3)
	local position = world:GetComponent(id, "Position", 0)
	position:SetFloat3(posx, posy, posz)
	local scale = world:GetComponent(id, "Scale", 0)
	scale:SetFloat3(scalex, scaley, 1)
	local pickbox = world:GetComponent(id, "PickBox", 0)
	pickbox:SetFloat2(1, 1)
	local rotation = world:GetComponent(id, "Rotation", 0)
	rotation:SetFloat3(0, 0, 0)
	return id	
end

NewCameraSystem.AddConsoleCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuConsoleCommand", button)
	world:GetComponent(button, "MenuConsoleCommand", "Command"):SetText(command)
end

NewCameraSystem.AddEntityCommandToButton = function(self, command, button)
	world:CreateComponentAndAddTo("MenuEntityCommand", button)
	world:GetComponent(button, "MenuEntityCommand", "ComponentName"):SetText(command)
end

NewCameraSystem.AddHoverSize = function(self, deltascale, button)
	local scale = world:GetComponent(button, "Scale", 0)
	local sx, sy, sz = scale:GetFloat3()
	world:CreateComponentAndAddTo("HoverSize", button)
	local hoversize = world:GetComponent(button, "HoverSize", 0)
	hoversize:SetFloat3(sx*deltascale, sy*deltascale, sz*deltascale)
end
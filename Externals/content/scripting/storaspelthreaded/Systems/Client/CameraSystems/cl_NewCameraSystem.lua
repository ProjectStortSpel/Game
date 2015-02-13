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
NewCameraSystem.Pressed = false
NewCameraSystem.Moved = false
NewCameraSystem.FreeCam = false
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
	
end

NewCameraSystem.EntitiesAdded = function(self, dt, taskIndex, taskCount, entities)
	for n = 1, #entities do
		if world:EntityHasComponent(entities[n], "CameraSystemComponent") then
			self.FreeCam = not self.FreeCam
			if self.FreeCam == true then
				local aspectX, aspectY = GraphicDevice.GetAspectRatio()
				local deltaaspectX = aspectY / aspectX
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
				
			else
				self:RemoveElements()
			end
			world:KillEntity(entities[n])
		end
		if world:EntityHasComponent(entities[n], "CameraInterestPoint") then
			if self.FreeCam == false then
				self:DoCIP(entities[n])
			end
			world:KillEntity(entities[n])
		end
	end
end

NewCameraSystem.Update = function(self, dt, taskIndex, taskCount)
	if self.FreeCam == true then
		self:DoFreeCam(dt)
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
			
	GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
									self.CameraUpX,0,self.CameraUpZ,
									self.CameraLookAtX,0.5,self.CameraLookAtZ,
									1)
	
end

NewCameraSystem.DoCIP = function(self, entityId)

	self.CameraUpX = world:GetComponent(entityId, "CameraInterestPoint", "UpX"):GetFloat(0)
	self.CameraUpZ = world:GetComponent(entityId, "CameraInterestPoint", "UpZ"):GetFloat(0)
	self.CameraLookAtX = world:GetComponent(entityId, "CameraInterestPoint", "AtX"):GetFloat(0)
	self.CameraLookAtZ = world:GetComponent(entityId, "CameraInterestPoint", "AtZ"):GetFloat(0)
	self.CameraDistance = world:GetComponent(entityId, "CameraInterestPoint", "Distance"):GetFloat(0)

	GraphicDevice.GetCamera():MoveToAndLookAt(	self.CameraLookAtX-self.CameraUpX*self.CameraDistance*7.5,self.CameraDistance*10-2,self.CameraLookAtZ-self.CameraUpZ*self.CameraDistance*7.5,
									self.CameraUpX,0,self.CameraUpZ,
									self.CameraLookAtX,0.5,self.CameraLookAtZ,
									0.5)
end

NewCameraSystem.DoFreeCam = function(self, dt)
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
				rposition:SetFloat3(0, 0, -0.1001)
				local rposition = world:GetComponent(self.TouchSprite1, "Position", 0)
				rposition:SetFloat3(self.mouseX * aspectX * 2, self.mouseY * aspectY * 2, -1)			
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
		world:GetComponent(entity, "CameraInterestPoint", "AtX"):SetFloat(AtX)
		world:GetComponent(entity, "CameraInterestPoint", "AtZ"):SetFloat(AtZ)
		world:GetComponent(entity, "CameraInterestPoint", "UpX"):SetFloat(UpX)
		world:GetComponent(entity, "CameraInterestPoint", "UpZ"):SetFloat(UpZ)
		world:GetComponent(entity, "CameraInterestPoint", "Distance"):SetFloat(Distance)
		print("move camera")
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
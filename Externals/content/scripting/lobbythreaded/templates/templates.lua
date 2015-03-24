local User = EntityTemplate()
User.Name = "User"
User:SetString("Username", "DefaultPlayer")
User:SetString("NetConnection", "0.0.0.0", "IpAddress")
User:SetInt("NetConnection", 0, "Port")
User:SetBool("NetConnection", false, "Active")
EntityTemplateManager:AddTemplate(User)

local Button = EntityTemplate()
Button.Name = "Button"
Button:AddComponent("Model")
Button:AddComponent("Position")
Button:AddComponent("Rotation")
Button:AddComponent("Scale")
Button:AddComponent("PickBox")
Button:AddComponent("HoverSound")
Button:SetString("HoverSound", "Click1")
EntityTemplateManager:AddTemplate(Button)

local Texture = EntityTemplate()
Texture.Name = "Texture"
Texture:AddComponent("Model")
Texture:AddComponent("Position")
Texture:AddComponent("Rotation")
Texture:AddComponent("Scale")
EntityTemplateManager:AddTemplate(Texture)

local Text = EntityTemplate()
Text.Name = "Text"
Text:AddComponent("Model")
Text:AddComponent("Position")
Text:AddComponent("Rotation")
Text:AddComponent("Scale")
EntityTemplateManager:AddTemplate(Text)

local Checkbox = EntityTemplate()
Checkbox.Name = "Checkbox"
Checkbox:AddComponent("Model")
Checkbox:AddComponent("Position")
Checkbox:AddComponent("Rotation")
Checkbox:AddComponent("Scale")
Checkbox:AddComponent("PickBox")
Checkbox:AddComponent("Checkbox")
EntityTemplateManager:AddTemplate(Checkbox)
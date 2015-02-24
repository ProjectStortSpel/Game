local User = EntityTemplate()
User.Name = "User"
User:SetString("Username", "DefaultPlayer")
User:SetString("NetConnection", "0.0.0.0", "IpAddress")
User:SetInt("NetConnection", 0, "Port")
User:SetBool("NetConnection", false, "Active")
EntityTemplateManager:AddTemplate(User)

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
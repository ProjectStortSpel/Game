
local Head = EntityTemplate()
Head.Name = "Head"
Head:AddComponent("Model")
--	Model data
Head:SetFloat3("Position", 0.0, 1.0, 0.0)
Head:SetFloat3("Rotation", 0.0, math.pi, 0.0)
Head:SetFloat3("Scale", 1, 1, 1)
Head:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Head)

local SlingShot = EntityTemplate()
SlingShot.Name = "SlingShot"
SlingShot:SetFloat3("Position", 0.0, 0.0, 0.0)
SlingShot:SetFloat3("Rotation", 0.0, 0.0, 0.0)
SlingShot:SetFloat3("Scale", 0.25, 0.25, 0.25)
SlingShot:SetModel("Model", "head", "head" , 0)
SlingShot:AddComponent("SyncNetwork")
SlingShot:AddComponent("SlingShotComponent")
EntityTemplateManager:AddTemplate(SlingShot)




local TotemHead = EntityTemplate()
TotemHead.Name = "TotemPiece"
TotemHead:AddComponent("Model")
--	Model data
TotemHead:SetFloat3("Position", 0.0, 1.0, 0.0)
TotemHead:SetFloat3("Rotation", 0.0, math.pi, 0.0)
TotemHead:SetFloat3("Scale", 1, 1, 1)
TotemHead:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(TotemHead)

local Hole = EntityTemplate()
Hole.Name = "Hole"
Hole:SetFloat3("Position", 0.0, 0.0, 0.0)
Hole:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Hole:SetFloat3("Scale", 1.0, 1.0, 1.0)
Hole:SetModel("Model", "hole_test", "hole" , 0)
EntityTemplateManager:AddTemplate(Hole)

local Stone = EntityTemplate()
Stone.Name = "Stone"
Stone:SetFloat3("Position", 0.0, 0.0, 0.0)
Stone:SetFloat3("Rotation", 0.0, 0.0, 0.0)
Stone:SetFloat3("Scale", 0.6, 0.3, 0.6)
Stone:SetModel("Model", "stone", "stone", 0)
EntityTemplateManager:AddTemplate(Stone)

local GrassTile = EntityTemplate()
GrassTile.Name = "Grass"
GrassTile:SetFloat3("Position", 0.0, 0.0, 0.0)
GrassTile:SetFloat3("Rotation", 0.0, 0.0, 0.0)
GrassTile:SetFloat3("Scale", 1.0, 1.0, 1.0)
GrassTile:SetModel("Model", "grass", "grass", 0)
EntityTemplateManager:AddTemplate(GrassTile)

local User = EntityTemplate()
User.Name = "User"
User:SetString("Username", "DefaultPlayer")
User:SetString("NetConnection", "0.0.0.0", "IpAddress")
User:SetInt("NetConnection", 0, "Port")
User:SetBool("NetConnection", false, "Active")
EntityTemplateManager:AddTemplate(User)

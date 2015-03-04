
local template = EntityTemplate()
template.Name = "Head"
template:AddComponent("Model")
--	Model data
template:SetFloat3("Position", 0.0, 1.0, 0.0)
template:SetFloat3("Rotation", 0.0, math.pi, 0.0)
template:SetFloat3("Scale", 1, 1, 1)
template:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "TotemPiece"
template:AddComponent("Model")
template:AddComponent("TotemPiece")
--	Model data
template:SetFloat3("Position", 0.0, 1.0, 0.0)
template:SetFloat3("Rotation", 0.0, math.pi, 0.0)
template:SetFloat3("Scale", 1, 1, 1)
template:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "Hole"
template:SetFloat3("Position", 0.0, 0.0, 0.0)
template:SetFloat3("Rotation", 0.0, 0.0, 0.0)
template:SetFloat3("Scale", 1.0, 1.0, 1.0)
template:SetModel("Model", "hole_test", "hole" , 0)
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "Stone"
template:SetFloat3("Position", 0.0, 0.0, 0.0)
template:SetFloat3("Rotation", 0.0, 0.0, 0.0)
template:SetFloat3("Scale", 1.0, 1.0, 1.0)
template:SetModel("Model", "stone", "stone", 0)
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "Grass"
template:SetFloat3("Position", 0.0, 0.0, 0.0)
template:SetFloat3("Rotation", 0.0, 0.0, 0.0)
template:SetFloat3("Scale", 1.0, 1.0, 1.0)
template:SetModel("Model", "grass", "grass", 0)
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "User"
template:SetString("Username", "DefaultPlayer")
template:SetString("NetConnection", "0.0.0.0", "IpAddress")
template:SetInt("NetConnection", 0, "Port")
template:SetBool("NetConnection", false, "Active")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "SlingShotProjectile"
template:SetFloat3("Position", 0.0, 0.0, 0.0)
template:SetFloat3("Rotation", 0.0, 0.0, 0.0)
template:SetFloat3("Scale", 1, 1, 1)
template:SetModel("Model", "pebble", "pebble" , 0)
template:AddComponent("SyncNetwork")
template:AddComponent("SlingShotProjectile")
template:AddComponent("LerpPosition")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "FlowingWater"
template:AddComponent("Model")
--	Model data
template:SetFloat3("Position", 0.0, 1.0, 0.0)
template:SetFloat3("Rotation", 0.0, math.pi, 0.0)
template:SetFloat3("Scale", 1, 1, 1)
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "Button"
template:AddComponent("Model")
template:AddComponent("Position")
template:AddComponent("Rotation")
template:AddComponent("Scale")
template:AddComponent("PickBox")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "Text"
template:AddComponent("Model")
template:AddComponent("Position")
template:AddComponent("Rotation")
template:AddComponent("Scale")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "Checkbox"
template:AddComponent("Model")
template:AddComponent("Position")
template:AddComponent("Rotation")
template:AddComponent("Scale")
template:AddComponent("PickBox")
template:AddComponent("Checkbox")
EntityTemplateManager:AddTemplate(template)

template = EntityTemplate()
template.Name = "FlowingWater"
template:AddComponent("Model")
template:SetFloat3("Position", 0.0, 1.0, 0.0)
template:SetFloat3("Rotation", 0.0, 0, 0.0)
template:SetFloat3("Scale", 1, 1, 1)
EntityTemplateManager:AddTemplate(template)
--	Tornado template
local Hat = EntityTemplate()
Hat.Name = "totemHat"
Hat:SetFloat3("Position", -0.14, 0.2, 0.1)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 1.4, 1.4, 1.4)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("Parent")
Hat:AddComponent("ParentJoint")
Hat:AddComponent("KillWhenOrphan")
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "smallstoneHat"
Hat:SetFloat3("Position", -0.14, 0.2, 0.1)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 0.14, 0.14, 0.14)
Hat:SetFloat3("Color", 1, 1, 1)
Hat:AddComponent("Parent")
Hat:AddComponent("ParentJoint")
Hat:AddComponent("KillWhenOrphan")
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)
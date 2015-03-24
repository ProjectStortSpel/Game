local Hat = EntityTemplate()
Hat.Name = "totemHat"
Hat:SetModel("Model", "totemhat", "totemhat", 0)
Hat:SetFloat3("Position", -0.14, 0.2, 0.1)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 1.4, 1.4, 1.4)
Hat:SetFloat3("Color", 1, 1, 1) 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "eggMan"
Hat:SetModel("Model", "stone", "stone", 0)
Hat:SetFloat3("Position", 0, 0, 0)
Hat:SetFloat3("Rotation", 0, 0, -4.14)
Hat:SetFloat3("Scale", 1, 1, 1)
Hat:SetFloat3("Color", 0, 0, 0) 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 1) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "stoneHat"
Hat:SetModel("Model", "smallstone", "smallstone", 0)
Hat:SetFloat3("Position", -0.14, 0.2, 0.1)
Hat:SetFloat3("Rotation", 0.2, 0, 0.6)
Hat:SetFloat3("Scale", 0.14, 0.14, 0.14)
Hat:SetFloat3("Color", 1, 1, 1) 		-- ADDS PLAYER COLOR
Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)

Hat = EntityTemplate()
Hat.Name = "arrowHat"
Hat:SetModel("Model", "arrow", "arrow", 0)
Hat:SetFloat3("Position", 0.0, 1.5, 0.05)
Hat:SetFloat3("Rotation", 0, 0, 0)
Hat:SetFloat3("Scale", 0.4, 0.4, 0.4)
Hat:SetFloat3("Color", 1, 1, 1) 		-- ADDS PLAYER COLOR
--Hat:SetInt("ParentJoint", 5) 			-- OPTIONAL: ADDS HAT TO HEAD JOINT
Hat:AddComponent("SyncNetwork")
EntityTemplateManager:AddTemplate(Hat)
local Card = EntityTemplate()
Card.Name = "Card"
Card:AddComponent("Card")
Card:AddComponent("CardPrio")
Card:AddComponent("CardAction")
EntityTemplateManager:AddTemplate(Card)


local CardAboveHead = EntityTemplate()
CardAboveHead.Name = "CardAboveHead"
CardAboveHead:AddComponent("SyncNetwork")
CardAboveHead:AddComponent("Parent")
CardAboveHead:AddComponent("Model")
CardAboveHead:AddComponent("NoShadow")
CardAboveHead:AddComponent("CardAboveHead")
CardAboveHead:AddComponent("Position")
CardAboveHead:AddComponent("Rotation")
CardAboveHead:AddComponent("Scale")
CardAboveHead:AddComponent("Color")
CardAboveHead:AddComponent("KillWhenOrphan")
EntityTemplateManager:AddTemplate(CardAboveHead)

local StoneTimer = EntityTemplate()
StoneTimer.Name = "StoneTimer"
StoneTimer:AddComponent("SyncNetwork")
StoneTimer:AddComponent("Model")
StoneTimer:AddComponent("Position")
StoneTimer:AddComponent("Rotation")
StoneTimer:AddComponent("Scale")
StoneTimer:AddComponent("Color")
StoneTimer:AddComponent("StoneTimer")
EntityTemplateManager:AddTemplate(StoneTimer)
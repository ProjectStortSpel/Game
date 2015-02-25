--	 __  __          _____  	 _____         _______ ______ _____  
--	|  \/  |   /\   |  __ \ 	|  __ \     /\|__   __|  ____|  __ \ 
--	| \  / |  /  \  | |__) |	| |__) |   /  \  | |  | |__  | |__) |
--	| |\/| | / /\ \ |  ___/ 	|  _  /   / /\ \ | |  |  __| |  _  / 
--	| |  | |/ ____ \| |     	| | \ \  / ____ \| |  | |____| | \ \ 
--	|_|  |_/_/    \_\_|     	|_|  \_\/_/    \_\_|  |______|_|  \_\
--	
MapRater	=	System()
MapRater.SEPARATOR				=	"	"
MapRater.PlayersLeftToWin		=	0
MapRater.NumberOfPlayers		=	0
MapRater.CurrentRound			=	0


MapRater.NumberOfCheckpoints				=	0
MapRater.CheckpointRoundDifferences			=	{}	--	Each player will have their own list ranging from 1 to Checkpoints [Player][Checkpoint]
MapRater.CheckpointRoundDifferences.__mode	=	"k"	--	Each entry (CheckpointNumber) will contain which round the player reached that checkpoint on

MapRater.PlayersDeaths			=	{}
MapRater.PlayersDeaths.__mode	=	"k"

MapRater.PlayersCardCount			=	{}
MapRater.PlayersCardCount.__mode	=	"k"

MapRater.Initialize = function(self)
	--	Set Name
	self:SetName("MapRater")
	
	--	Toggle EntitiesAdded
	self:UsingEntitiesAdded()

	--	Set Filter
	self:AddComponentTypeToFilter("PlayedCard",			FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("CheckpointReached",	FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("NewRound",			FilterType.RequiresOneOf)
	self:AddComponentTypeToFilter("UnitDied",			FilterType.RequiresOneOf)
	
	self:AddComponentTypeToFilter("MapSpecs",			FilterType.RequiresOneOf)
end


--	 ______ _   _ _______ _____ _______ _____ ______  _____ 	          _____  _____  ______ _____  
--	|  ____| \ | |__   __|_   _|__   __|_   _|  ____|/ ____|	    /\   |  __ \|  __ \|  ____|  __ \ 
--	| |__  |  \| |  | |    | |    | |    | | | |__  | (___  	   /  \  | |  | | |  | | |__  | |  | |
--	|  __| | . ` |  | |    | |    | |    | | |  __|  \___ \ 	  / /\ \ | |  | | |  | |  __| | |  | |
--	| |____| |\  |  | |   _| |_   | |   _| |_| |____ ____) |	 / ____ \| |__| | |__| | |____| |__| |
--	|______|_| \_|  |_|  |_____|  |_|  |_____|______|_____/ 	/_/    \_\_____/|_____/|______|_____/ 
MapRater.EntitiesAdded = function(self, dt, newEntities)

	for n = 1, #newEntities do
		local	newEntity	=	newEntities[n]
		
		if world:EntityHasComponent(newEntity, "PlayedCard") then
			self:CardPlayed(newEntity)
			world:KillEntity(newEntity)
		elseif world:EntityHasComponent(newEntity, "CheckpointReached") then
			self:CheckpointReached(newEntity)
			world:KillEntity(newEntity)
			
			if self.PlayersLeftToWin == 0 then
				self:LogWorldData()
				self:PrintInfo()
			end
		elseif world:EntityHasComponent(newEntity, "UnitDied") then
			local	playerNumber	=	world:GetComponent(newEntity, "UnitDied", "PlayerNumber"):GetInt()
			self.PlayersDeaths[playerNumber]	=	self.PlayersDeaths[playerNumber] + 1
			world:KillEntity(newEntity)
		elseif world:EntityHasComponent(newEntity, "NewRound") then
			self.CurrentRound	=	self.CurrentRound+1
		elseif world:EntityHasComponent(newEntity, "MapSpecs") then
			self:SetupMapSpecs(newEntity)
		end
	end
	
end

MapRater.SetupMapSpecs = function(self, mapSpecs)
	
	self.NumberOfCheckpoints	=	world:GetComponent(mapSpecs, "MapSpecs", "NoOfCheckpoints"):GetInt()
	self.NumberOfPlayers		=	world:GetComponent(mapSpecs, "MapSpecs", "NoOfSpawnpoints"):GetInt()
	self.PlayersLeftToWin		=	self.NumberOfPlayers
	
	for i = 1, self.NumberOfPlayers do
	
		self.PlayersDeaths[#self.PlayersDeaths+1]		=	0
		self.PlayersCardCount[#self.PlayersCardCount+1]	=	0
		
		self.CheckpointRoundDifferences[#self.CheckpointRoundDifferences+1]			=	{}
		self.CheckpointRoundDifferences[#self.CheckpointRoundDifferences].__mode	=	"k"
		
		for j = 1, self.NumberOfCheckpoints do
			self.CheckpointRoundDifferences[i][j]	=	0
		end
		
	end
end




--	  _____          _____  _____   _____ 	 _____  _           __     ________ _____  
--	 / ____|   /\   |  __ \|  __ \ / ____|	|  __ \| |        /\\ \   / /  ____|  __ \ 
--	| |       /  \  | |__) | |  | | (___  	| |__) | |       /  \\ \_/ /| |__  | |  | |
--	| |      / /\ \ |  _  /| |  | |\___ \ 	|  ___/| |      / /\ \\   / |  __| | |  | |
--	| |____ / ____ \| | \ \| |__| |____) |	| |    | |____ / ____ \| |  | |____| |__| |
--	 \_____/_/    \_\_|  \_\_____/|_____/ 	|_|    |______/_/    \_\_|  |______|_____/ 
--	Counting the cards played and also who played what
MapRater.CardPlayed = function(self, cardEntity)
	--	Extract the data
	local	playerNumber	=	world:GetComponent(cardEntity, "PlayedCard", "PlayerNumber"):GetInt()
	local 	cardAction 		= 	world:GetComponent(cardEntity, "PlayedCard", "CardAction"):GetText()
	local 	cardPriority	= 	world:GetComponent(cardEntity, "PlayedCard", "CardPriority"):GetInt()
	
	self.PlayersCardCount[playerNumber]	=	self.PlayersCardCount[playerNumber]+1
end


--	  _____ _    _ ______ _____ _  _______   ____ _____ _   _ _______ 	 _____  ______          _____ _    _ ______ _____  
--	 / ____| |  | |  ____/ ____| |/ /  __ \ / __ \_   _| \ | |__   __|	|  __ \|  ____|   /\   / ____| |  | |  ____|  __ \ 
--	| |    | |__| | |__ | |    | ' /| |__) | |  | || | |  \| |  | |   	| |__) | |__     /  \ | |    | |__| | |__  | |  | |
--	| |    |  __  |  __|| |    |  < |  ___/| |  | || | | . ` |  | |   	|  _  /|  __|   / /\ \| |    |  __  |  __| | |  | |
--	| |____| |  | | |___| |____| . \| |    | |__| || |_| |\  |  | |   	| | \ \| |____ / ____ \ |____| |  | | |____| |__| |
--	 \_____|_|  |_|______\_____|_|\_\_|     \____/_____|_| \_|  |_|   	|_|  \_\______/_/    \_\_____|_|  |_|______|_____/ 
--	

MapRater.CheckpointReached = function(self, checkpointEntity)
	--	Extract the data
	local	playerNumber		=	world:GetComponent(checkpointEntity, "CheckpointReached", "PlayerNumber"):GetInt()
	local 	checkpointNumber	= 	world:GetComponent(checkpointEntity, "CheckpointReached", "CheckpointNumber"):GetInt()
	
	self.CheckpointRoundDifferences[playerNumber][checkpointNumber]	=	self.CurrentRound
	if checkpointNumber == self.NumberOfCheckpoints then
		self.PlayersLeftToWin	=	self.PlayersLeftToWin-1
	end
end

MapRater.GetCheckpointListIndex = function(self, Player, Checkpoint)
	return (Player * self.NumberOfPlayers + Checkpoint)
end




--	 _____  _____  _____ _   _ _______ 	 _____ _   _ ______ ____  
--	|  __ \|  __ \|_   _| \ | |__   __|	|_   _| \ | |  ____/ __ \ 
--	| |__) | |__) | | | |  \| |  | |   	  | | |  \| | |__ | |  | |
--	|  ___/|  _  /  | | | . ` |  | |   	  | | | . ` |  __|| |  | |
--	| |    | | \ \ _| |_| |\  |  | |   	 _| |_| |\  | |   | |__| |
--	|_|    |_|  \_\_____|_| \_|  |_|   	|_____|_| \_|_|    \____/ 
MapRater.PrintInfo = function(self)
	
	print("\nMAP RATER DATA BELOW!\n")
	local	dataString	=	""
	for playerIndex = 1, self.NumberOfPlayers do
		
		--	Player index
		dataString	=	dataString .. playerIndex .. self.SEPARATOR
		
		--	Cards played
		dataString	=	dataString .. self.PlayersCardCount[playerIndex] .. self.SEPARATOR
		
		--	Number of deaths
		dataString	=	dataString .. self.PlayersDeaths[playerIndex] .. self.SEPARATOR
		
		--	Checkpoint data
		for i = 1, self.NumberOfCheckpoints do
			dataString	=	dataString .. self.CheckpointRoundDifferences[playerIndex][i] .. self.SEPARATOR
		end
		
		dataString	=	dataString .. "\n"
	end
	
	print(dataString)
	print("\nEND OF MAP RATER DATA\n")
end

MapRater.PrintCardsData = function(self)

	--	First entry will be all cards played, then each players number of cards
	local	cardData	=	""
	
	for i = 1, self.NumberOfPlayers do
		cardData	=	cardData .. self.PlayersCardCount[i]
		if i < self.NumberOfPlayers then
			cardData	=	cardData .. self.SEPARATOR
		end
	end
	
	return	cardData
end

MapRater.PrintDeathData = function(self)

	local	deathData	=	""
	for i = 1, self.NumberOfPlayers do
		deathData	=	deathData .. self.PlayersDeaths[i]
		if i < self.NumberOfPlayers then
			deathData	=	deathData .. self.SEPARATOR
		end
	end
	
	return	deathData
end

--	This method will print information for each checkpoint
MapRater.PrintCheckpointInfo = function(self)

	local	checkpointMessage	=	""
	for n = 1, self.NumberOfCheckpoints do
	
		local	earliestRound	=	self.CurrentRound+1
		local	earliestPlayer	=	-1
		local	latestRound		=	0
		local	latestPlayer	=	-1	
		
		checkpointMessage	=	checkpointMessage .. n .. self.SEPARATOR
		
		for i = 1, self.NumberOfPlayers do
			
			local	roundInfo	=	self.CheckpointRoundDifferences[i][n]
			
			checkpointMessage	=	checkpointMessage .. roundInfo
			if i < self.NumberOfPlayers then
				checkpointMessage	=	checkpointMessage .. self.SEPARATOR
			end
		end
		if n < self.NumberOfCheckpoints then
			checkpointMessage	=	checkpointMessage .. "\n"
		end
	end
	
	return	checkpointMessage
end








































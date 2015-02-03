#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include "Logger/Managers/Logger.h"
#include "Game/Network/ClientDatabase.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

int main(int argc, char** argv)
{
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
    std::cout << "Current Path: " << path << std::endl;
#endif
    
    
    
#if defined(__OSX__) || defined(__IOS__)
    //signal(SIGPIPE, SIG_IGN);
#endif
    
#ifdef __IOS__
    printf("Home dir: %s\n\n", getenv("HOME"));
#endif
    
	Logger::GetInstance().AddGroup("Game");
	GameCreator* newGame = new GameCreator();


	newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeThreads();
	newGame->InitializeWorld("lobbythreaded");
	 

	newGame->StartGame(argc, argv);
	delete newGame;

	delete(&ClientDatabase::GetInstance());
	delete(&Logger::GetInstance());
	
#ifdef __IOS__
    exit(0);
#endif
	return 0;
}
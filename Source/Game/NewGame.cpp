#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include "Logger/Managers/Logger.h"
#include "Game/Network/ClientDatabase.h"
#include "Game/HomePath.h"
#include "FileSystem/MD5.h"
#include "FileSystem/Directory.h"
#include "FileSystem/File.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

int main(int argc, char** argv)
{
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char Cpath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)Cpath, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(Cpath);
    std::cout << "Current Path: " << Cpath << std::endl;
#endif
    
    
    
#if defined(__OSX__) || defined(__IOS__)
    //signal(SIGPIPE, SIG_IGN);
#endif
    
#ifdef __IOS__
    printf("Home dir: %s\n\n", getenv("HOME"));
#endif
    
	std::string path = HomePath::GetSecondaryHomePath();
	path.append("data/");
	Logger::GetInstance().SetPath(path);
	Logger::GetInstance().AddGroup("Game");
	Logger::GetInstance().AddGroup("Network");
	GameCreator* newGame = new GameCreator();


 //   std::string path2 = HomePath::GetHomePath();
 //  // path2.append("models/");
 //   
	//SDL_Log("Path: %s", path2.c_str());

 //   std::vector<std::string> temp = FileSystem::Directory::GetAllFiles(path2);
 //   
 //   
 //   for (int i = 0; i < temp.size(); ++i)
 //   {
 //       std::string path = path2;
 //       path.append(temp[i]);
	//	SDL_Log("File: %s\n", path.c_str());
 //       FileSystem::MD5::MD5_Print(FileSystem::MD5::MD5_File(path));
 //   }

	//newGame->InitializeLua();
	newGame->InitializeGraphics();
	newGame->InitializeInput();
	newGame->InitializeNetwork();
	newGame->InitializeThreads();
    
	newGame->StartGame(argc, argv);
	delete newGame;

	delete(&ClientDatabase::GetInstance());
	delete(&Logger::GetInstance());
	
#ifdef __IOS__
    exit(0);
#endif
	return 0;
}
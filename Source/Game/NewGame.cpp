#include "GameCreator.h"

#ifdef WIN32
#ifdef _DEBUG
#include <VLD/vld.h>
#endif
#endif

#include "Logger/Managers/Logger.h"
#include "Game/Network/ClientDatabase.h"
#include "Game/HomePath.h"
//#include "FileSystem/MD5.h"
//#include "FileSystem/File.h"

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
    
	std::string path = HomePath::GetSecondaryHomePath();
	path.append("data/");
	Logger::GetInstance().SetPath(path);
	Logger::GetInstance().AddGroup("Game");
	GameCreator* newGame = new GameCreator();


	////Test Md5
	//std::string path2 = HomePath::GetSecondaryHomePath();
	//path2.append("models/caveman/cavemanwave.amesh");
	//SDL_RWops* test;
	//FileSystem::File::Open(path2, &test);
	//Sint64 length = FileSystem::File::GetFileSize(test);
	//char* data = FileSystem::File::Read(test, length);

	//FileSystem::MD5::MD5_CTX ctx;
	//FileSystem::MD5::MD5_Init(&ctx);
	//FileSystem::MD5::MD5_Update(&ctx, data, length);
	//unsigned char res[16];
	//FileSystem::MD5::MD5_Final(res, &ctx);
	//FileSystem::MD5::MD5_Print(res);

	//delete data;

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
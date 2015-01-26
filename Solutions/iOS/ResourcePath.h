//
//  ResourcePath.h
//  StoraSpel
//
//  Created by Erik Nilsson on 2015-01-24.
//  Copyright (c) 2015 CardBordGames. All rights reserved.
//

#ifndef StoraSpel_ResourcePath_h
#define StoraSpel_ResourcePath_h

#include <string>
class ResourcePath
{
    
public:
    
    ResourcePath();
    ~ResourcePath(){};
    
    std::string getResourcePath(void);
    
};

std::string getResourcePath(void);

#endif

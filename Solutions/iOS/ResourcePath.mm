//
//  ResourcePath.m
//  StoraSpel
//
//  Created by Erik Nilsson on 2015-01-24.
//  Copyright (c) 2015 CardBordGames. All rights reserved.
//

#include "ResourcePath.h"
#import <Foundation/Foundation.h>

ResourcePath::ResourcePath()
{
}



std::string ResourcePath::getResourcePath(void)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    
    if (mainBundle != nil)
    {
        NSString* path = [mainBundle resourcePath];
        return [path UTF8String] + std::string("/");
    }
    
    return "";
}

std::string getResourcePath(void)
{
    NSBundle* mainBundle = [NSBundle mainBundle];
    
    if (mainBundle != nil)
    {
        NSString* path = [mainBundle resourcePath];
        return [path UTF8String] + std::string("/");
    }
    
    return "";
}

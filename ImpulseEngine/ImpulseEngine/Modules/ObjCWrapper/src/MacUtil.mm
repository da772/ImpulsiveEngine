#include "ObjCWrapper/MacUtil.h"
#include <AppKit/AppKit.h>

std::string __MASCOSX_GET_APPLICATION_SUPPORT() {
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *applicationSupportDirectory = [paths firstObject];
	const char* ch = [applicationSupportDirectory UTF8String];
	std::string s = ch;
    return s;
    
}


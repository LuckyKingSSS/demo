#import <Cocoa/Cocoa.h>
#include "GetFoldPath.h"
#include "Linux32_Typedef.h"
#include "strconv.h"
#include "WSLog.h"
#undef BOOL

bool g_appstore = false;

@interface NSString (GetFolderPath)
- (NSString *)searchPathByType:(int) type;
@end

int GetFoldPath(int nFlag, wchar_t* szPath, int nPathBufferSize)
{
    if(nPathBufferSize > 0)
    {
        wmemset(szPath, L'\0', nPathBufferSize );
    }
    
	NSDictionary *dict=[[NSBundle mainBundle] infoDictionary];
	NSString *appId=[dict valueForKey:(NSString*)kCFBundleNameKey];

	int ret=1;
	
	NSString *path=[NSString string];
	NSArray *array=nil;
	if ([path respondsToSelector:@selector(searchPathByType:)]) {
		path=[path searchPathByType:nFlag];
	}else {
		switch ((MacDirectoryType)nFlag) {
			case MacUserHome:
				path=NSHomeDirectory();
				break;
			case MacTempDir:
				path=NSTemporaryDirectory();
				break;
			case MacApplicationLibrary:
				array=NSSearchPathForDirectoriesInDomains(NSLibraryDirectory,NSUserDomainMask,YES);
				if ([array count]>0) {
					path=[array objectAtIndex:0];
					path=[path stringByAppendingPathComponent:appId];
				}
				break;
			case MacApplicationCache:
				array=NSSearchPathForDirectoriesInDomains(NSCachesDirectory,NSUserDomainMask,YES);
				if ([array count]>0) {
					path=[array objectAtIndex:0];
					path=[path stringByAppendingPathComponent:appId];
				}
				break;
			case MacApplicationSupport:
				array=NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory,NSUserDomainMask,YES);
				if ([array count]>0) {
					path=[array objectAtIndex:0];
					path=[path stringByAppendingPathComponent:appId];
				}
				break;
            case MacAppResourcePath:
                path = [[NSBundle mainBundle] resourcePath];
                break;
//            case MacDocumentDirectPath:
//            {
//                GetDocumentPath(szPath, 512);
//                return 1;
//            }
//                break;
			default:
				break;
		}
	}

	if (path==nil||[path isEqualToString:@""]) {
		path=NSTemporaryDirectory();
		path=[path stringByAppendingPathComponent:appId];
		ret=0;
	}
	
	BOOL isDirectory=NO;
	BOOL isExist=[[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory];
	if (!isDirectory) {
		[[NSFileManager defaultManager] removeItemAtPath:path error:nil];
		isExist=[[NSFileManager defaultManager] fileExistsAtPath:path];
	}
	if (!isExist) {
		[[NSFileManager defaultManager] createDirectoryAtPath:path withIntermediateDirectories:YES attributes:nil error:nil];
	}
	
	isExist=[[NSFileManager defaultManager] fileExistsAtPath:path isDirectory:&isDirectory];
	if (!isExist || !isDirectory ) {
		ret=0;
	}
	
	[path getCString:(char*)szPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
	return ret;
}

int CopyDocmentMedias(const wchar_t* szSrcFilePath, const wchar_t* szDstFilePath)
{
	USES_CONVERSION;
	NSString* strSrcFilePath = [NSString stringWithUTF8String:W2A(szSrcFilePath)];
	NSString* strDstFilePath = [NSString stringWithUTF8String:W2A(szDstFilePath)];
	NSLog(@"CopyDocmentMedias strSrcFilePath %@, ---------, strDstFilePath %@", strSrcFilePath, strDstFilePath);
	
	NSString *strPath = strDstFilePath;
	strPath = [strPath stringByDeletingLastPathComponent];
	BOOL isDir = NO;

	if(![[NSFileManager defaultManager] fileExistsAtPath:strPath isDirectory:&isDir] ||
	   !isDir)
	{
		BOOL isSuc = [[NSFileManager defaultManager] createDirectoryAtPath:strPath attributes:nil];
        NSLog(@"createDirectoryAtPath:-----------%@---%d********%@", strPath, isSuc);
        if (![[NSFileManager defaultManager] fileExistsAtPath:strPath]) {
            NSLog(@"!!!!!!!!!!!!!!!createDirectoryAtPath failed");
        }
	}
	
	NSError* error = nil;
	[[NSFileManager defaultManager] linkItemAtPath:strSrcFilePath toPath:strDstFilePath error:&error];
	if (error) {
		NSLog(@"----------linkItemAtPath error: %@", error);
		[[NSFileManager defaultManager] copyItemAtPath:strSrcFilePath toPath:strDstFilePath error:&error];
		if (error) {
			NSLog(@"----------copyItemAtPath error: %@", error);
		}
	}
	return YES;
}

MacStoreFileType storeFileURL(const wchar_t* szSrcFilePath) {
    if (!g_appstore) {
        return MacStoreFailedType;
    }
	USES_CONVERSION;
    NSString* strFile = [NSString stringWithUTF8String:W2A(szSrcFilePath)];
    //非线编自身资源不使用bookmark
    NSDictionary *dict=[[NSBundle mainBundle] infoDictionary];
    NSString *appId=[dict valueForKey:(NSString*)kCFBundleNameKey];
    NSString* strResPath = [NSHomeDirectory() stringByAppendingFormat:@"/Library/Application Support/%@",appId];
    if ([strFile length] >= [strResPath length] && [strResPath isEqualToString:[strFile substringToIndex:[strResPath length]]]) {
        return MacStoreFailedType;
    }
    //
    
	NSURL* fileURL = [NSURL fileURLWithPath:strFile];
	NSError *error = nil;
	
	NSData *bookmarkData = nil;
	SInt32 MacMajorVersion;
	SInt32 MacMinorVersion;
	OSErr err  = Gestalt(gestaltSystemVersionMajor, &MacMajorVersion);
	if(err  != noErr)
	{
		NSLog(@"osVersion Error");
		return MacStoreFailedType;
	}
	err = Gestalt(gestaltSystemVersionMinor, &MacMinorVersion);
	if(err  != noErr)
	{
		NSLog(@"osVersion Error");
		return MacStoreFailedType;
	}
	if (MacMajorVersion >= 10 && MacMinorVersion >= 7 && MacMinorVersion < 9) {
		SInt32 MacBugFixVersion;
		err  = Gestalt(gestaltSystemVersionBugFix, &MacBugFixVersion);
		if(err  != noErr)
		{
			NSLog(@"osVersion Error");
			return MacStoreFailedType;
		}
		if (MacMinorVersion == 7) {
			if (MacBugFixVersion >= 3) {
				bookmarkData = [fileURL bookmarkDataWithOptions:1UL << 11//NSURLBookmarkCreationWithSecurityScope
								 includingResourceValuesForKeys:nil
												  relativeToURL:nil
														  error:&error];
			}else
			{
				bookmarkData = [fileURL bookmarkDataWithOptions:1UL << 8
								 includingResourceValuesForKeys:nil
												  relativeToURL:nil
														  error:&error];
			}
		}else{
			bookmarkData = [fileURL bookmarkDataWithOptions:1UL << 11//NSURLBookmarkCreationWithSecurityScope
							 includingResourceValuesForKeys:nil
											  relativeToURL:nil
													  error:&error];
		}
	}else{
		bookmarkData = [fileURL bookmarkDataWithOptions:1UL << 8
						 includingResourceValuesForKeys:nil
										  relativeToURL:nil
												  error:&error];   
	}
	//        bookmarkData = [fileURL bookmarkDataWithOptions:NSURLBookmarkCreationMinimalBookmark
	//                     includingResourceValuesForKeys:nil
	//                                      relativeToURL:nil
	//                                              error:&error];
	if (error || !bookmarkData) {
		NSLog(@"store bookmark error %@",fileURL.path);
		NSLog(@"bookmark data %@",bookmarkData);
		//NSError *anError = [NSError errorWithDomain:[error domain] code:[error code] userInfo:[error userInfo]];
		
		NSLog(@"book mark error %@",[error description]);
		if (![[NSFileManager defaultManager] isReadableFileAtPath:fileURL.path] || 
			![[NSFileManager defaultManager] isWritableFileAtPath:fileURL.path]) {
			return MacStoreNoReadorWriteFileType;
		}
		return MacStoreFailedType;
	}
	NSDictionary *urlDict = [[NSUserDefaults standardUserDefaults] objectForKey:@"wsVideoEditor25BookmarkURL"];
	if (!urlDict) {
		urlDict = [NSMutableDictionary dictionaryWithObject:bookmarkData forKey:fileURL.path];
	}else{
		NSMutableDictionary *mutUrlDict = [[urlDict mutableCopy] autorelease];
		[mutUrlDict setObject:bookmarkData forKey:fileURL.path];
		urlDict = mutUrlDict;
	}
	[[NSUserDefaults standardUserDefaults] setObject:urlDict forKey:@"wsVideoEditor25BookmarkURL"];
	//   });
	[[NSUserDefaults standardUserDefaults] synchronize];
	
	return MacStoreSucessType;
}

void getSongPathStoreBookmark(const wchar_t* szSrcFilePath, wchar_t* szSongPath) {
	USES_CONVERSION;
	NSString* songPath = [NSString stringWithUTF8String:W2A(szSrcFilePath)];
	NSDictionary *bookmarkDict = [[NSUserDefaults standardUserDefaults] objectForKey:@"wsVideoEditor25BookmarkURL"];
	NSData *data = [bookmarkDict objectForKey:songPath];
    NSError *error = nil;
	//NSLog(@"getSongPathStoreBookmark-------------");
	if (data) {
		NSURL *url = nil;
        SInt32 MacMajorVersion;
        SInt32 MacMinorVersion;
        OSErr err  = Gestalt(gestaltSystemVersionMajor, &MacMajorVersion);
        if(err  != noErr)
        {
            NSLog(@"osVersion Error");
			[songPath getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
            return;
        }
        err = Gestalt(gestaltSystemVersionMinor, &MacMinorVersion);
        if(err  != noErr)
        {
            NSLog(@"osVersion Error");
			[songPath getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
            return;
        }
        if (MacMajorVersion >= 10 && MacMinorVersion >= 7 && MacMinorVersion < 9)
        {
			//NSLog(@"MacMajorVersion:>10.7");
            SInt32 MacBugFixVersion;
            OSErr err  = Gestalt(gestaltSystemVersionBugFix, &MacBugFixVersion);
            if(err  != noErr)
            {
                NSLog(@"osVersion Error");
				[songPath getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
                return;
            }
            if (MacMinorVersion == 7) {
                if (MacBugFixVersion >= 3) {
                    url = [NSURL URLByResolvingBookmarkData:data options:1UL << 10 relativeToURL:nil bookmarkDataIsStale:nil error:nil];
                }else
                {
                    url =  [NSURL URLByResolvingBookmarkData:data options:1UL << 8 relativeToURL:nil bookmarkDataIsStale:nil error:nil];
                }
            }else
            {
                url = [NSURL URLByResolvingBookmarkData:data options:1UL << 10 relativeToURL:nil bookmarkDataIsStale:nil error:nil];
            }
        }else
        {
			//NSLog(@"MacMajorVersion < 10.7##########");
            url =  [NSURL URLByResolvingBookmarkData:data options:1UL << 8 relativeToURL:nil bookmarkDataIsStale:nil error:nil];
        }
		// url =  [NSURL URLByResolvingBookmarkData:data options:(NSURLBookmarkResolutionWithoutUI|NSURLBookmarkResolutionWithoutMounting) relativeToURL:nil bookmarkDataIsStale:nil error:&error]; 
        if (error) {
            NSLog(@"get store URL fail %@---%@---url %@",[error description],songPath,url);
			[songPath getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
            return;
        }
		if (url) {
			if ([url respondsToSelector:@selector(startAccessingSecurityScopedResource)])
            {
                [url performSelector:@selector(startAccessingSecurityScopedResource)];
            }
			//NSLog(@"+++++++++++++++++++ISWriteable:%d", [[NSFileManager defaultManager] isWritableFileAtPath:url.path]);
			//NSLog(@"-------------------ISReadable:%d", [[NSFileManager defaultManager] isReadableFileAtPath:url.path]);
			[[url path] getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
			return;
		}else
        {
			//NSLog(@"url is nil777777777777777777777");
			[songPath getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
            return;
        }
	}else
    {
		//NSLog(@"data nil8888888888888888");
		[songPath getCString:(char*)szSongPath maxLength:MAX_PATH*sizeof(wchar_t) encoding:NSUTF32StringEncoding];
        return;
    }
}
void setMacAppstore(bool appstore) {
    g_appstore = appstore;
}

int isAuthoredPath(const wchar_t*szSrcFilePath) {
    //mac网站版工程文档若只是使用保存路径则return 1，若要使用硬链接则return 0即可
    if (!g_appstore) {
        return 1;//return 0;//return 1;
    }
    USES_CONVERSION;
    NSString* strFile = [NSString stringWithUTF8String:W2A(szSrcFilePath)];
    //非线编自身资源
    NSDictionary *dict=[[NSBundle mainBundle] infoDictionary];
    NSString *appId=[dict valueForKey:(NSString*)kCFBundleNameKey];
    NSString* appSupportPath = [NSHomeDirectory() stringByAppendingFormat:@"/Library/Application Support/%@",appId];
    NSString* movePath = [NSHomeDirectory() stringByAppendingFormat:@"/Movies"];
    NSString* photoPath = [NSHomeDirectory() stringByAppendingFormat:@"/Pictures"];
    NSString* musicPath = [NSHomeDirectory() stringByAppendingFormat:@"/Music"];
    
    NSArray* arrResPath = [NSArray arrayWithObjects:appSupportPath, movePath, photoPath, musicPath, nil];
    for (NSString* path in arrResPath) {
        if ([strFile length] >= [path length] && [path isEqualToString:[strFile substringToIndex:[path length]]]) {
            return 1;
        }
    }
    
    return 0;
}
bool NLEIsFileExist(const wchar_t* pwszFileName)
{
    @autoreleasepool {
        if (pwszFileName == NULL) {
            return false;
        }
        
        USES_CONVERSION
        BOOL isDir = YES;
        BOOL isExist = [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:W2A(pwszFileName)] isDirectory:&isDir];
        
        return isExist && !isDir;
    }
}

wchar_t* NLEGetFileName(const wchar_t* pwszFilePath) {
    USES_CONVERSION
    NSString* strFilePath = [NSString stringWithCString:W2A(pwszFilePath) encoding:NSUTF8StringEncoding];
    NSString* strFileName = [[strFilePath lastPathComponent] stringByDeletingPathExtension];
    
    wchar_t* wszVal = (wchar_t*)[strFileName cStringUsingEncoding:NSUTF32StringEncoding];
    int size=[strFileName length];
	wszVal[size]=0;
    return wszVal;
}

void GetSystemVersion(int* majorVersion, int* minorVersion, int* bugFixVersion) {
    SInt32 MacMajorVersion;
    SInt32 MacMinorVersion;
    SInt32 MacBugFixVersion;
    Gestalt(gestaltSystemVersionMajor, &MacMajorVersion);
    Gestalt(gestaltSystemVersionMinor, &MacMinorVersion);
    Gestalt(gestaltSystemVersionBugFix, &MacBugFixVersion);
    
    if (majorVersion) {
        *majorVersion = MacMajorVersion;
    }
    
    if (minorVersion) {
        *minorVersion = MacMinorVersion;
    }
    
    if (bugFixVersion) {
        *bugFixVersion = MacBugFixVersion;
    }
}
void GetScreenSize(int* width, int* height) {
    NSRect rect = [[NSScreen mainScreen] frame];
    if (width) {
        *width = rect.size.width;
    }
    
    if (height) {
        *height = rect.size.height;
    }
}

/*
 *@brief  创建目录，如果已存在，返回true,若不存在则创建
 *@param szPath 路径名
 *@param nFlag 保留字
 *@remind 例如在临时目录下创建名为Wondershare的目录,调用如下
 * CreateDirectory(L"/var/folders/_4/x8f_wzs527x97701czhtx5f40000gn/T/Wondershare", 0)
 
*/

bool CreateDirectory(const wchar_t* szPath,int nFlag)
{
    USES_CONVERSION
    NSString* strPath = [NSString stringWithUTF8String:W2A(szPath)];
    NSFileManager* fileMgr = [[NSFileManager alloc] init];
    BOOL isDir = NO;
    BOOL isRet = YES;
    if (![fileMgr fileExistsAtPath:strPath isDirectory:&isDir] || isDir) {
        isRet = [fileMgr createDirectoryAtPath:strPath
                   withIntermediateDirectories:YES
                                    attributes:[NSDictionary dictionaryWithObjectsAndKeys:NSFileTypeDirectory, NSFileType, nil]
                                         error:nil];
    }
    
    [fileMgr release];
    fileMgr = nil;
    
    return isRet;
}

void copyStringToClippoard(const wchar_t* wszVal) {
    NSPasteboard* pBord = [NSPasteboard pasteboardWithName:NSGeneralPboard];
    [pBord declareTypes:[NSArray arrayWithObject:NSStringPboardType] owner:nil];
    
    USES_CONVERSION
    NSString* strVal = [NSString stringWithUTF8String:W2A(wszVal)];
    [pBord setString:strVal forType:NSStringPboardType];
}

wchar_t* stringFromClippoard() {
    NSPasteboard* pBoard = [NSPasteboard pasteboardWithName:NSGeneralPboard];
    NSString* strVal = [pBoard stringForType:NSPasteboardTypeString];
    
    wchar_t* wszVal = (wchar_t*)[strVal cStringUsingEncoding:NSUTF32StringEncoding];
    int size=[strVal length];
	wszVal[size]=0;
    
    return wszVal;
}

bool NLECopyFile(const wchar_t* szSrcFilePath, const wchar_t* szDstFilePath) {
    NSFileManager* fileMgr = [[[NSFileManager alloc] init] autorelease];
    
    USES_CONVERSION
    NSString* strSrcFile = [NSString stringWithUTF8String:W2A(szSrcFilePath)];
    NSString* strDstFile = [NSString stringWithUTF8String:W2A(szDstFilePath)];
    
    if (![fileMgr fileExistsAtPath:strSrcFile]) {
        return false;
    }
    
    NSError* error = nil;
    if ([fileMgr fileExistsAtPath:strDstFile]) {
        [fileMgr removeItemAtPath:strDstFile error:&error];
    }
    
    if (error) {
        return false;
    }
    
    [fileMgr copyItemAtPath:strSrcFile toPath:strDstFile error:&error];
    
    return error == nil ? true : false;
}

bool NLEDeleteFile(const wchar_t* szFilePath) {
    NSFileManager* fileMgr = [[[NSFileManager alloc] init] autorelease];
    
    USES_CONVERSION
    NSString* strFile = [NSString stringWithUTF8String:W2A(szFilePath)];
    
    NSError* error = nil;
    if ([fileMgr fileExistsAtPath:strFile]) {
        [fileMgr removeItemAtPath:strFile error:&error];
    }
    
    return error == nil ? true : false;
}

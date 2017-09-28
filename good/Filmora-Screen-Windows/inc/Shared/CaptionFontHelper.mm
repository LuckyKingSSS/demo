//
//  CaptionFontHelper.m
//  NLEPlatform_Mac
//
//  Created by Tancui on 1/25/11.
//  Copyright 2011 Joan Smith. All rights reserved.
//
#import <Cocoa/Cocoa.h>
#include "Linux32_Typedef.h"
#include "strconv.h"
int getWidthFrom(NSAttributedString * str)
{
	int width=0;
	NSSize size=[str size];
	if (size.width>0.001&&size.width<1) {
		width=1;
	}else if (size.width>=1) {
		width=size.width;
	}
	return width;
}
int getHeightFrom(NSAttributedString * str)
{
	int height=0;
	NSSize size=[str size];
	if (size.height>0.001&&size.height<1) {
		height=1;
	}else if (size.height>=1) {
		height=size.height;
	}
	return height;
}
NSAttributedString *getAttributedString(const wchar_t* pName, int nSize, wchar_t wChar)
{
	USES_CONVERSION;
	char *cstr=T2A(pName);
	NSString *fontName=[NSString stringWithUTF8String:cstr];
	wchar_t word[2]={'\0','\0'};
	word[0]=wChar;
	cstr=T2A(word);
	NSString *str=[NSString stringWithUTF8String:cstr];;
	NSFont *font = [NSFont fontWithName:fontName size:nSize];
	if (font==nil) {
		font = [NSFont systemFontOfSize:nSize];
	}
		//NSFont *font = [NSFont fontWithName:fontName size:nSize];
	NSDictionary *attrsDictionary =
	                     [NSDictionary dictionaryWithObjectsAndKeys:
						  font,NSFontAttributeName,
						  [NSColor blueColor],NSForegroundColorAttributeName,
						  [NSColor clearColor],NSBackgroundColorAttributeName,
						  nil];

	NSAttributedString *attrString =
    [[NSAttributedString alloc] initWithString:str
									attributes:attrsDictionary];
	return [attrString autorelease];
}
int  GetCharWidth(const wchar_t* pName, int nSize, wchar_t wChar)
{
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	NSAttributedString *attrString=getAttributedString(pName,nSize,wChar);
	int width=getWidthFrom(attrString);
	[pool release];
	return width;
}
int  GetCharHeight(const wchar_t* pName, int nSize)
{
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	wchar_t wChar=L'a';
	NSAttributedString *attrString=getAttributedString(pName,nSize,wChar);
	int height=getHeightFrom(attrString);
	[pool release];
	return height;
}

BOOL GetCharImage(const wchar_t* pName, int nSize, wchar_t wChar, unsigned char* pBuf, int nBufSize)
{
	NSAutoreleasePool *pool=[[NSAutoreleasePool alloc] init];
	NSAttributedString *attrString=getAttributedString(pName,nSize,wChar);
	NSSize size=NSZeroSize;
	size.width=getWidthFrom(attrString);
	size.height=getHeightFrom(attrString);
	
	NSData *data=nil;
	if (size.width>=1 &&size.height>=1) {
		NSImage *image=[[NSImage alloc] initWithSize:size];
		[image lockFocus];
		[attrString drawInRect:NSMakeRect(0, 0, size.width, size.height)];
		[image unlockFocus];
		NSBitmapImageRep *bmp=[NSBitmapImageRep imageRepWithData:[image TIFFRepresentation]];
		data=[bmp representationUsingType:NSBMPFileType properties:nil];
		[image release];
	}


	 BOOL isOK=NO;
	if(nBufSize!=[data length]){
		nBufSize=nBufSize>[data length]?[data length]:nBufSize;
		NSLog(@"error,The Image buffer size is not ok!");
	}else if (nBufSize>0) {
		[data getBytes:pBuf length:nBufSize];
		isOK=YES;
		unsigned char *rgba=pBuf+54;
		int length=nBufSize-54;
		for (int i=0; i<length; i+=4) {
			unsigned char *b=rgba+i;
			unsigned char *g=b+1;
			unsigned char *r=b+2;
			unsigned char *a=b+3;
			if (*b==0xFF&&*g==0xFF&&*r==0xFF||*b==0&&*g==0&&*r==0) {
				*b=0;
				*g=0;
				*r=0;
				*a=0;
			}else {
				*b=0xFF;
				*g=0;
				*r=0;
				*a=0xFF;
			}
		}
	}

	[pool release];
	return isOK;
}

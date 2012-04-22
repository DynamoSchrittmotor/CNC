//
//  click2.m
//  
//
//  Created by Alex Peller on 04.11.11.
//  Copyright 2011 one man. All rights reserved.
//
// File: 
// click.m
//
// Compile with: 
// gcc -o click2 click2.m -framework ApplicationServices -framework Foundation
//
// Usage:
// ./click -x pixels -y pixels -f finger -p pressed -d drag
// At the given coordinates it will click and release.

//#include <ApplicationServices/ApplicationServices.h>
//#include <unistd.h>
#import <Foundation/Foundation.h>
#import <ApplicationServices/ApplicationServices.h>


int main(int argc, char *argv[]) {
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	NSUserDefaults *args = [NSUserDefaults standardUserDefaults];
	
	
	// grabs command line arguments -x and -y
	//
	int p = [args integerForKey:@"p"]; //0:not pressed, 1:pressed
	int f = [args integerForKey:@"f"]; //0:left,		1:right
	int d = [args integerForKey:@"d"];
	int x = [args integerForKey:@"x"];
	int y = [args integerForKey:@"y"];
	
	
	// The data structure CGPoint represents a point in a two-dimensional
	// coordinate system.  Here, X and Y distance from upper left, in pixels.
	//
	//CGPoint pt;
	//pt.x = x;
	//pt.y = y;
	
	
	// This is where the magic happens.  See CGRemoteOperation.h for details.
	//
	// CGPostMouseEvent( CGPoint        mouseCursorPosition,
	//                   boolean_t      updateMouseCursorPosition,
	//                   CGButtonCount  buttonCount,
	//                   boolean_t      mouseButtonDown, ... )
	//
	// So, we feed coordinates to CGPostMouseEvent, put the mouse there,
	// then click and release.
	//
	//CGPostMouseEvent( pt, 1, f, p );

	
	
	
	//[pool release];
	//return 0;
	// Move to 200x200
    CGEventRef move1 = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved,
											   CGPointMake(x, y),
											   kCGMouseButtonLeft // ignored
											   );
    // Left button down at 250x250
    CGEventRef click1_down = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown,
													 CGPointMake(x, y),
													 kCGMouseButtonLeft
													 );
	// Left button drag at 250x250
    CGEventRef click1_drag = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged,
													 CGPointMake(x, y),
													 kCGMouseButtonLeft
													 );
	
    // Left button up at 250x250
    CGEventRef click1_up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp,
												   CGPointMake(x, y),
												   kCGMouseButtonLeft
												   );
	// Right button down at 250x250
    CGEventRef click2_down = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDown,
													 CGPointMake(x, y),
													 kCGMouseButtonRight
													 );
    // Right button up at 250x250
    CGEventRef click2_up = CGEventCreateMouseEvent(NULL, kCGEventRightMouseUp,
												   CGPointMake(x, y),
												   kCGMouseButtonRight
												   );
	
	CGEventRef scroll_up = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 50, -80, 50);
	
	CGEventRef scroll_down = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitPixel, 50, 80, 50);
	
    // Now, execute these events with an interval to make them noticeable
	if (f == 0){
		CGEventPost(kCGHIDEventTap, move1);
		//CFRelease(move1);
	}
	if (f == 1){
		CGEventPost(kCGHIDEventTap, click1_down);
		//CFRelease(click1_down);
	}
	if (f == 2){
		CGEventPost(kCGHIDEventTap, click1_drag);
		//CFRelease(click1_drag);
	}
	if (f == 3){
		CGEventPost(kCGHIDEventTap, click1_up);
		//CFRelease(click1_up);
	}
	if (f == 4){
		CGEventPost(kCGHIDEventTap, click2_down);
		//CFRelease(click2_down);
	}
	if (f == 5){
		CGEventPost(kCGHIDEventTap, click2_up);
		//CFRelease(click2_up);
	}
	if (f == 6){
		CGEventPost(kCGHIDEventTap, scroll_up);
		//CFRelease(scroll_up);
	}
	if (f == 7){
		CGEventPost(kCGHIDEventTap, scroll_down);
		//CFRelease(scroll_down);
	}

	
	[pool release];
    return 0;
}
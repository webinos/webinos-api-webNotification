/*******************************************************************************
 *  Code contributed to the webinos project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2013 Martin Lasak, Fraunhofer FOKUS
 ******************************************************************************/

#import <Cocoa/Cocoa.h>
#import <notifications.h>

#import "FakeBundle.h"

@interface NotificationCenterDelegate : NSObject<NSUserNotificationCenterDelegate> @end

@implementation NotificationCenterDelegate

-(void) userNotificationCenter:(NSUserNotificationCenter*) center didActivateNotification:(NSUserNotification*) notification {
    [center removeDeliveredNotification:notification];
}

@end

bool Notifications::show(string title, string body, string dir, string lang, string tag, string icon) {
    NSString* _title = [[NSString alloc] initWithUTF8String:title.c_str()];
    NSString* _body = [[NSString alloc] initWithUTF8String:body.c_str()];
    NSString* _dir = [[NSString alloc] initWithUTF8String:dir.c_str()];
    NSString* _lang = [[NSString alloc] initWithUTF8String:lang.c_str()];
    NSString* _tag = [[NSString alloc] initWithUTF8String:tag.c_str()];
    NSString* _icon = [[NSString alloc] initWithUTF8String:icon.c_str()];

    @autoreleasepool {
        if (![FakeBundle installNSBundleHook]) return NO;
        NSUserNotificationCenter* nc = [NSUserNotificationCenter defaultUserNotificationCenter];
        NotificationCenterDelegate* ncDelegate = [[NotificationCenterDelegate alloc] init];
        nc.delegate = ncDelegate;
        NSUserNotification *note = [[NSUserNotification alloc] init];
        note.hasActionButton = NO;
        note.title = _title;
        note.informativeText = _body;
        [nc deliverNotification: note];
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow: 1]];
    }

    return YES;
}

bool Notifications::setup(){
	return YES;
}
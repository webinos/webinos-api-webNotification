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

#import "FakeBundle.h"

@implementation NSBundle(swizle)

-(NSString*) __bundleIdentifier {
    if (self == [NSBundle mainBundle]) {
        return @"com.apple.terminal";
    } else {
        return [self __bundleIdentifier];
    }
}

@end

@implementation FakeBundle

+(BOOL) installNSBundleHook {
    Class clazz = objc_getClass("NSBundle");
    if (clazz) {
        method_exchangeImplementations(class_getInstanceMethod(clazz, @selector(bundleIdentifier)),
                                       class_getInstanceMethod(clazz, @selector(__bundleIdentifier)));
        return YES;
    }
	return NO;
}

@end
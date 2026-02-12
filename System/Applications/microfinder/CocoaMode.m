#import <Cocoa/Cocoa.h>

@interface CocoaMode : NSObject
- (void)launchCocoaApp:(NSString *)appName;
@end

@implementation CocoaMode
- (void)launchCocoaApp:(NSString *)appName {
    NSString *openCommand = [NSString stringWithFormat:@"open -a \"%@\"", appName];
    NSTask *task = [[NSTask alloc] init];
    [task setLaunchPath:@"/bin/bash"];
    [task setArguments:@[@"-c", openCommand]];
    [task launch];
    [task waitUntilExit];
}
@end

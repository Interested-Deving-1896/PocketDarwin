/* 
        ==[X]===========================================
        =                                              =
        =                  MicroFinder                 =
        =                                              =
        = The OpenSource Darwin Commandline Experience =
        =                                              =
        =               Finder version 0.1             =
        =                                              =
        =       TM and (c) 2025-6 PocketDarwin         =
        =                                              =
        ================================================

        =============================
        = Finder Entry Point        =
        =============================

*/

#import <Foundation/Foundation.h>
#import <mach/mach.h>
#import <mach/mach_vm.h>
#import <mach/thread_act.h>
#import <mach/mach_init.h>
#import <unistd.h>
#import <ncursesw/curses.h>

@interface MicroFinder : NSObject
- (pid_t)findProcessByName:(NSString *)processName;
@end

@implementation MicroFinder
- (pid_t)findProcessByName:(NSString *)processName {
    NSArray *runningApps = [[NSWorkspace sharedWorkspace] runningApplications];
    for (NSRunningApplication *app in runningApps) {
        if ([[app localizedName] isEqualToString:processName]) {
            return [app processIdentifier];
        }
    }
    return -1; // Process not found
}
@end

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        MicroFinder *finder = [[MicroFinder alloc] init];
  //      NSString *processName = @"Safari"; // Example process name
        pid_t pid = [finder findProcessByName:processName];
        
        if (pid != -1) {
            NSLog(@"Found process %@ with PID: %d", processName, pid);
        } else {
            NSLog(@"Process %@ not found", processName);
        }
    }
    return 0;
}
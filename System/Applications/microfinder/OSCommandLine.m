#import <Foundation/Foundation.h>
#import <unistd.h>

#if NEEDED_
  #import <Cocoa/Cocoa.h>
#endif

@interface OSCommandLine : NSObject
- (void)executeCommand:(NSString *)command;
@end

// set the shell main entry (while statement)
@implementation OSCommandLine
- (void)executeCommand:(NSString *)command {
    // Create a new process to execute the command
    NSTask *task = [[NSTask alloc] init];
    while (true) {
        // Read user input
        char input[256];
        printf("(finder)% ");
        fgets(input, sizeof(input), stdin);
        
        // Remove newline character from input
        NSString *command = [NSString stringWithUTF8String:input];
        command = [command stringByTrimmingCharactersInSet:[NSCharacterSet newlineCharacterSet]];
        
        // Check for exit command
        if ([command isEqualToString:@"exit"]) {
            NSLog(@"Exiting Finder...");
            break;
        } else if ([command isEqualToString:@"help"]) {
            NSLog(@"Available commands:");
            NSLog(@"- help: Show this help message");
            NSLog(@"- open <app>: Open the specified application (e.g., open Installer.app)");
            NSLog(@"- exit: Exit the Finder");
            // Add more commands as needed
        } else if ([command hasPrefix:@"open"]) {
            // Example: open Safari
            NSString *appName = [command substringFromIndex:5]; // Get the app name after "open "
            NSString *openCommand = [NSString stringWithFormat:@"open -a \"%@\"", appName];
            [task setLaunchPath:@"/bin/bash"];
            [task setArguments:@[@"-c", openCommand]];
            [task launch];
            [task waitUntilExit];
        } else if ([command hasPrefix:@"kill"]) {
            // Example: kill Safari
            NSString *appName = [command substringFromIndex:5]; // Get the app name after "kill "
            NSString *killCommand = [NSString stringWithFormat:@"pkill -f \"%@\"", appName];
            [task setLaunchPath:@"/bin/bash"];
            [task setArguments:@[@"-c", killCommand]];
            [task launch];
            [task waitUntilExit];
        } else if ([command hasPrefix:@"mode"]) {
            // modes: tui, cli, cocoa(limited)
            NSString *mode = [command substringFromIndex:5]; // Get the mode after "mode "
            if ([mode isEqualToString:@"tui"]) {
                NSLog(@"Switching to TUI mode...");
                // Implement TUI mode logic here
            } else if ([mode isEqualToString:@"cli"]) {
                NSLog(@"Switching to CLI mode...");
                // Implement CLI mode logic here
            } else if ([mode isEqualToString:@"cocoa"]) {
                // use macro NEEDED_ to conditionally compile Cocoa code
                runCocoaMode();
                NSLog(@"Switching to Cocoa mode...");
                // Implement Cocoa mode logic here (limited functionality)
            }     
        } else {
            NSLog(@"Unknown command: %@", command);
        }
        
        // Set up the task to execute the command
        [task setLaunchPath:@"/bin/bash"];
        [task setArguments:@[@"-c", command]];
        
        // Launch the task
        [task launch];
        [task waitUntilExit];
    }
}
@end


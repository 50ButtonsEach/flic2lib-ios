//
//  FLICManagerSimulator.m
//  flic2lib
//
//  Created by Anton Meier on 2019-12-10.
//  Copyright © 2020 Shortcut Labs. All rights reserved.
//

#import <flic2lib/flic2lib.h>

#if TARGET_OS_SIMULATOR

NSString * const FLICErrorDomain = @"com.shortcutlabs.flic2lib";
NSString * const FLICButtonScannerErrorDomain = @"com.shortcutlabs.flic2lib.buttonscanner";

@implementation FLICManager

static FLICManager *_sharedManager = nil;

+ (instancetype _Nullable)sharedManager;
{
    return _sharedManager;
}

+ (instancetype _Nullable)configureWithDelegate:(id<FLICManagerDelegate> _Nullable)delegate
                                 buttonDelegate:(id<FLICButtonDelegate> _Nullable)buttonDelegate
                                     background:(BOOL)background;
{
    static dispatch_once_t onceToken;
    
    dispatch_once(&onceToken, ^{
        _sharedManager = [[FLICManager alloc] init];
        _sharedManager.delegate = delegate;
        _sharedManager.buttonDelegate = buttonDelegate;
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            if ([FLICManager sharedManager].delegate && [[FLICManager sharedManager].delegate respondsToSelector:@selector(managerDidRestoreState:)])
            {
                [[FLICManager sharedManager].delegate managerDidRestoreState:[FLICManager sharedManager]];
            }
        });
        
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1.0f * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            if ([FLICManager sharedManager].delegate && [[FLICManager sharedManager].delegate respondsToSelector:@selector(manager:didUpdateState:)])
            {
                [[FLICManager sharedManager].delegate manager:[FLICManager sharedManager] didUpdateState:FLICManagerStatePoweredOn];
            }
        });
    });
    
    return _sharedManager;
}

- (NSArray<FLICButton *> *)buttons;
{
    return @[];
}

- (void)forgetButton:(FLICButton *)button completion:(void (^)(NSUUID *uuid, NSError * _Nullable error))completion;
{
    dispatch_async(dispatch_get_main_queue(), ^{
        completion(button.identifier, nil);
    });
}

- (void)scanForButtonsWithStateChangeHandler:(void (^)(FLICButtonScannerStatusEvent event))stateHandler
                                  completion:(void (^)(FLICButton * _Nullable button, NSError * _Nullable error))completion;
{
    dispatch_async(dispatch_get_main_queue(), ^{
        completion(nil, [NSError errorWithDomain:FLICButtonScannerErrorDomain code:FLICButtonScannerErrorCodeUnknown userInfo:@{NSLocalizedDescriptionKey: @"You can not scan for Flic buttons while running on the iOS Simulator."}]);
    });
}

- (void)stopScan;
{
    
}

@end

#endif

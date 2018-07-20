//
//  ESMethodMonitor.h
//  ESMethodMonitor
//
//  Created by 晨风 on 2018/7/12.
//  Copyright © 2018年 晨风. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import "ESMethodInvocation.h"

//! Project version number for ESMethodMonitor.
FOUNDATION_EXPORT double ESMethodMonitorVersionNumber;

//! Project version string for ESMethodMonitor.
FOUNDATION_EXPORT const unsigned char ESMethodMonitorVersionString[];

// In this header, you should import all the public headers of your framework using statements like #import <ESMethodMonitor/PublicHeader.h>

#import "ESMethodInvocationEntity.h"

@class ESMethodMonitor;

@protocol ESMethodMonitorDelegate <NSObject>

@optional
- (BOOL)methodMonitor:(ESMethodMonitor *)mehtodMonitor ignoreInvocation:(ESMethodInvocationEntity *)invocation;
- (void)methodMonitor:(ESMethodMonitor *)methodMonitor recordInvocation:(ESMethodInvocationEntity *)invocation;

@end

@interface ESMethodMonitor : NSObject

/// ms
@property (nonatomic, assign) double minTimeCost;

@property (nonatomic, weak) id<ESMethodMonitorDelegate> delegate;

+ (instancetype)shareInstance;

- (void)start;
- (void)stop;

- (void)addIgnoreQueue:(dispatch_queue_t)queue;
- (void)removeIgnoreQueue:(dispatch_queue_t)queue;

@end

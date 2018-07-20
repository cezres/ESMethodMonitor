//
//  ESMethodInvocationEntity.h
//  ESMethodMonitor
//
//  Created by 晨风 on 2018/7/17.
//  Copyright © 2018年 晨风. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ESMethodInvocationEntity : NSObject

@property (nonatomic, strong) NSString *className;
@property (nonatomic, strong) NSString *cmdName;
@property (nonatomic, assign) NSTimeInterval time;  // ms
@property (nonatomic, strong) NSString *stack;

@end

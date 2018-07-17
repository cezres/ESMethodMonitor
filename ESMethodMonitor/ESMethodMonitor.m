//
//  ESMethodMonitor.m
//  ESMethodMonitor
//
//  Created by 晨风 on 2018/7/12.
//  Copyright © 2018年 晨风. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ESMethodMonitor.h"
#import "ESMethodMonitorCore.h"

static dispatch_queue_t _queue;
static int _kQueueSpecific;
static CFStringRef _queueSpecificValue = CFSTR("ESMethodMonitor");


@interface ESMethodMonitor ()

@property (nonatomic, strong) NSMutableDictionary<NSString *, NSMutableArray<NSValue *> *> *stack;

- (void)recored:(ESMethodInvocation *)invocation;

@end

static inline int methodMonitorIgnore(ESMethodInvocation *invocation) {
    CFStringRef retrievedValue = dispatch_get_specific(&_kQueueSpecific);
    if (retrievedValue && CFStringCompare(retrievedValue, _queueSpecificValue, kCFCompareCaseInsensitive) == kCFCompareEqualTo) {
        /// 忽略的线程
        return 1;
    }
    unsigned long len = strlen(invocation->className);
    if (len < 2) {
        return 0;
    }
    if (invocation->className[0] == 'O' && invocation->className[1] == 'S') {
        return 1;
    }
    return 0;
}

static inline void methodMonitorRecord(ESMethodInvocation *invocation) {
    ESMethodInvocation *newInvocation = ESMethodInvocationCopy(invocation);
    if (newInvocation->depth > 0) {
        newInvocation->topUUid = malloc(strlen(invocation->top->uuid) + 1);
        memcpy(newInvocation->topUUid, invocation->top->uuid, strlen(invocation->top->uuid) + 1);
    }
    dispatch_async(_queue, ^{
        [[ESMethodMonitor shareInstance] recored:newInvocation];
    });
}


@implementation ESMethodMonitor

+ (instancetype)shareInstance {
    static id obj;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        obj = [[self alloc] init];
    });
    return obj;
}


- (instancetype)init {
    if (self = [super init]) {
        _queue = dispatch_queue_create("ESMethodMonitor", DISPATCH_QUEUE_SERIAL);
        dispatch_queue_set_specific(_queue, &_kQueueSpecific, (void *)_queueSpecificValue, (dispatch_function_t)CFRelease);
        self.minTimeCost = 0;
        
        ESMethodMonitorSetIgnoreHandler(methodMonitorIgnore);
        ESMethodMonitorSetRecordHandler(methodMonitorRecord);
        
        _stack = [NSMutableDictionary dictionary];
    }
    return self;
}

- (void)start {
    ESMethodMonitorStart();
}

- (void)stop {
    ESMethdMonitorStop();
}

- (void)recored:(ESMethodInvocation *)invocation {
    NSString *uuid = [NSString stringWithUTF8String:invocation->uuid];
    NSString *topUuid = nil;
    if (invocation->depth > 0) {
        topUuid = [NSString stringWithUTF8String:invocation->topUUid];
        NSMutableArray<NSValue *> *childs = [_stack objectForKey:topUuid];
        if (!childs) {
            childs = [NSMutableArray array];
            [_stack setObject:childs forKey:topUuid];
        }
        [childs addObject:[NSValue valueWithPointer:invocation]];
    }
    
    NSMutableArray *childs = [_stack objectForKey:uuid];
    if (childs && [childs isKindOfClass:[NSMutableArray class]]) {
        invocation->childs = malloc(sizeof(void *) * childs.count);
        invocation->child_count = (unsigned int)childs.count;
        for (int i=0; i<childs.count; i++) {
            invocation->childs[i] = [childs[i] pointerValue];
        }
        [_stack removeObjectForKey:uuid];
    }
    else {
        invocation->child_count = 0;
        invocation->childs = NULL;
    }
    
    if (invocation->depth == 0) {
        /// 记录
        NSMutableString *desc = [NSMutableString string];
        [self descriptionMethodCall:invocation fromString:desc];
        NSLog(@"\n%@", desc);
    }
    
}

- (void)descriptionMethodCall:(ESMethodInvocation *)invocation fromString:(NSMutableString *)string {
    /// self
    [string appendString:@">"];
    for (int d=0; d<invocation->depth; d++) {
        [string appendString:@"  "];
    }
    if (invocation->isClassMethod) {
        [string appendString:@"+ "];
    }
    else {
        [string appendString:@"- "];
    }
    [string appendFormat:@"[%s %s]\t%.2lfms\n", invocation->className, invocation->cmdName, invocation->time / 1000.0];
    /// childs
    for (int i=0; i<invocation->child_count; i++) {
        [self descriptionMethodCall:invocation->childs[i] fromString:string];
    }
    free(invocation->uuid);
    free(invocation->childs);
    free(invocation);
}


#pragma mark -

- (void)setMinTimeCost:(double)minTimeCost {
    _minTimeCost = minTimeCost;
    ESMethodMonitorSetMinTimeCost(minTimeCost * 1000);
}

@end

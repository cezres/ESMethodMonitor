//
//  ESMethodMonitorUtils.h
//  ESMethodMonitor
//
//  Created by 晨风 on 2018/7/16.
//  Copyright © 2018年 晨风. All rights reserved.
//

#ifndef ESMethodMonitorUtils_h
#define ESMethodMonitorUtils_h

#include <stdio.h>
#include <objc/runtime.h>
#include <objc/message.h>
#include <stdlib.h>

#include <time.h>

struct ESMethodInvocation {
    const char *className;
    const char *cmdName;
    bool isClassMethod;
    
    ///
    void *object;
    SEL cmd;
    uintptr_t lr;
    uint64_t time;  // us
    
    char *uuid;
    
    ///
    uint depth;
    
    struct ESMethodInvocation *top;
};
typedef struct ESMethodInvocation ESMethodInvocation;

struct ESMethodInvocationStack {
    bool isMainThread;
    int index;
    ESMethodInvocation *currentInvocation;
};
typedef struct ESMethodInvocationStack ESMethodInvocationStack;


static inline ESMethodInvocation * ESMethodInvocationCreate(void *object, SEL cmd, uintptr_t lr) {
    ESMethodInvocation *invocation = malloc(sizeof(ESMethodInvocation));
    invocation->object = object;
    invocation->cmd = cmd;
    invocation->lr = lr;
    invocation->uuid = NULL;
    return invocation;
}

static inline uintptr_t ESMethodInvocationRelease(ESMethodInvocation *invocation) {
    uintptr_t lr = invocation->lr;
    if (invocation->uuid) {
        free(invocation->uuid);
    }
    free(invocation);
    return lr;
}



#endif /* ESMethodMonitorUtils_h */

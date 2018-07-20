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
#include <string.h>

#include <time.h>

struct ESMethodInvocation {
    const char *className;
    const char *cmdName;
    bool isClassMethod;
    uint64_t time;  // us
    uint depth;
    
    char *uuid;
    struct ESMethodInvocation *top;
    char *topUUid;
    
    struct ESMethodInvocation **childs;  /// 子级指针数组
    unsigned int child_count;   ///
    
    void *object;
    Class cls;
    SEL cmd;
    uintptr_t lr;
};
typedef struct ESMethodInvocation ESMethodInvocation;

struct ESMethodInvocationStack {
    bool isMainThread;
    int index;
    ESMethodInvocation *currentInvocation;
};
typedef struct ESMethodInvocationStack ESMethodInvocationStack;


static inline ESMethodInvocation * ESMethodInvocationInit() {
    ESMethodInvocation *invocation = malloc(sizeof(ESMethodInvocation));
    invocation->className = NULL;
    invocation->cmdName = NULL;
    invocation->isClassMethod = false;
    invocation->time = 0;
    invocation->depth = 0;
    
    invocation->uuid = NULL;
    invocation->top = NULL;
    invocation->topUUid = NULL;
    
    invocation->childs = NULL;
    invocation->child_count = 0;
    
    invocation->object = NULL;
    invocation->cmd = NULL;
    invocation->lr = 0;
    return invocation;
}

static inline ESMethodInvocation * ESMethodInvocationCreate(void *object, SEL cmd, uintptr_t lr) {
    ESMethodInvocation *invocation = ESMethodInvocationInit();
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
    if (invocation->childs) {
        free(invocation->childs);
    }
    free(invocation);
    return lr;
}

static inline ESMethodInvocation * ESMethodInvocationCopy(ESMethodInvocation *invocation) {
    ESMethodInvocation *newInvocation = ESMethodInvocationInit();
    newInvocation->className = invocation->className;
    newInvocation->cmdName = invocation->cmdName;
    newInvocation->isClassMethod = invocation->isClassMethod;
    newInvocation->time = invocation->time;
    newInvocation->uuid = malloc(sizeof(char) * strlen(invocation->uuid));
    strcpy(newInvocation->uuid, invocation->uuid);
    newInvocation->depth = invocation->depth;
    return newInvocation;
}


#endif /* ESMethodMonitorUtils_h */

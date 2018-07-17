//
//  ESMethodMonitorCore.h
//  ESMethodMonitor
//
//  Created by 晨风 on 2018/7/13.
//  Copyright © 2018年 晨风. All rights reserved.
//

#ifndef ESMethodMonitorCore_h
#define ESMethodMonitorCore_h

#include <stdio.h>
#include "ESMethodMonitorUtils.h"

typedef int (*ESMethodMonitorIgnore)(ESMethodInvocation *invocation);
typedef void (*ESMethodMonitorRecord)(ESMethodInvocation *invocation);

void ESMethodMonitorStart(void);
void ESMethdMonitorStop(void);
void ESMethodMonitorSetIgnoreHandler(ESMethodMonitorIgnore ignore);
void ESMethodMonitorSetRecordHandler(ESMethodMonitorRecord record);
void ESMethodMonitorSetMinTimeCost(unsigned long minTimeCost);  /// us

#endif /* ESMethodMonitorCore_h */

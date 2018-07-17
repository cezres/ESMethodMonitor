//
//  ViewController.m
//  MethodMonitorExample
//
//  Created by 晨风 on 2018/7/12.
//  Copyright © 2018年 晨风. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

struct ViewController {
    const char *str;
};

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    [self setupUI];
}


- (void)setupUI {
    UIButton *button = [UIButton buttonWithType:UIButtonTypeSystem];
    [button setTitle:@"Button" forState:UIControlStateNormal];
    button.layer.borderColor = [UIColor redColor].CGColor;
    button.layer.borderWidth = 1;
    button.frame = CGRectMake(100, 100, 80, 40);
//    button.center = self.view.center;
    [button addTarget:self action:@selector(buttonAction:) forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:button];
}


- (void)buttonAction:(UIButton *)button {
    NSLog(@"%@",self.view);
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end

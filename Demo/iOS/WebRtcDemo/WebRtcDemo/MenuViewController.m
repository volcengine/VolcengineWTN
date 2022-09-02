//
//  MenuViewController.m
//  veRTC_Demo
//
//  Created by bytedance on 2021/5/18.
//  Copyright © 2021 . All rights reserved.
//

#import "MenuViewController.h"
#import "HomeViewController.h"


@interface MenuViewController ()


@property (nonatomic, strong) HomeViewController *homeViewController;


@end

@implementation MenuViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    
    [self addChildViewController:self.homeViewController];
    [self.view addSubview:self.homeViewController.view];
    [self.homeViewController.view mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.view);
    }];
    
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self.navigationController setNavigationBarHidden:YES animated:NO];
    [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    self.navigationController.interactivePopGestureRecognizer.enabled = NO;
}


#pragma mark - getter
- (HomeViewController *)homeViewController {
    if (!_homeViewController) {
        _homeViewController = [[HomeViewController alloc] init];
    }
    return _homeViewController;
}

@end

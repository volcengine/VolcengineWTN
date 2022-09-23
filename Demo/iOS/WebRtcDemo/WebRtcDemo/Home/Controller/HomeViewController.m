//
//  HomeViewController.m
//  WebRtcDemo
//
//  Created by ByteDance on 2022/6/12.
//

#import "HomeViewController.h"
#import "ScenesItemButton.h"
#import "PushStreamingViewController.h"
#import "InputStreamingUrlViewController.h"

@interface HomeViewController ()

@property (nonatomic, strong) UILabel *titleLabel;
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) NSMutableArray *dataArray;

@end

@implementation HomeViewController


- (void)viewDidLoad {
    [super viewDidLoad];
    [self addBgGradientLayer];
    
    [self.view addSubview:self.titleLabel];
    _titleLabel.text = @"请选择推拉流";
    [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(20);
        make.right.mas_equalTo(-20);
        make.height.mas_equalTo(20);
        make.top.mas_equalTo(45 + [DeviceInforTool getStatusBarHight]);
    }];
    
    // scrollView
    [self.view addSubview:self.scrollView];
    [self.scrollView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(20);
        make.right.mas_equalTo(-20);
        make.top.mas_equalTo(95 + [DeviceInforTool getStatusBarHight]);
        make.bottom.mas_equalTo(-100);
    }];
    
    UIView *contenView = [[UIView alloc] init];
    [self.scrollView addSubview:contenView];
    [contenView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.scrollView);
    }];
    
    // add buttons
    for (int i = 0; i < self.dataArray.count; i++) {
        ScenesItemButton *button = [[ScenesItemButton alloc] init];
        button.tag = 100+i;
        [contenView addSubview:button];
        
        SceneButtonModel *model = self.dataArray[i];
        button.model = model;
        
        [button addTarget:self action:@selector(sceneButtonAction:) forControlEvents:UIControlEventTouchUpInside];
        [button mas_makeConstraints:^(MASConstraintMaker *make) {
            make.right.left.equalTo(contenView);
            make.top.mas_equalTo(i*(120 + 20));
            make.height.mas_equalTo(120);
        }];
    }
    
    CGFloat scrollviewHeight = self.dataArray.count * (120 + 20);
    [contenView mas_updateConstraints:^(MASConstraintMaker *make) {
        make.height.mas_equalTo(scrollviewHeight);
        make.width.equalTo(self.scrollView).offset(0);
    }];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

#pragma mark - Touch Action

- (void)sceneButtonAction:(ScenesItemButton *)button {
    if(button.tag == 100) {
        PushStreamingViewController *pushStreamingViewController  = [[PushStreamingViewController alloc]init];
        [self.navigationController pushViewController:pushStreamingViewController animated:YES];
    }else {
        InputStreamingUrlViewController *inputStreamingUrlViewController  = [[InputStreamingUrlViewController alloc]init];
        [self.navigationController pushViewController:inputStreamingUrlViewController animated:YES];
    }
    
}

#pragma mark - Private Action
- (void)addBgGradientLayer {
    UIColor *startColor = [UIColor colorFromHexString:@"#30394A"];
    UIColor *endColor = [UIColor colorFromHexString:@"#1D2129"];
    CAGradientLayer *gradientLayer = [CAGradientLayer layer];
    gradientLayer.frame = self.view.bounds;
    gradientLayer.colors = @[(__bridge id)[startColor colorWithAlphaComponent:1.0].CGColor,
                             (__bridge id)[endColor colorWithAlphaComponent:1.0].CGColor];
    gradientLayer.startPoint = CGPointMake(.0, .0);
    gradientLayer.endPoint = CGPointMake(.0, 1.0);
    [self.view.layer addSublayer:gradientLayer];
}

#pragma mark - getter

- (UIScrollView *)scrollView {
    if (_scrollView == nil) {
        _scrollView = [[UIScrollView alloc] init];
        _scrollView.canCancelContentTouches = NO;
        _scrollView.showsVerticalScrollIndicator = NO;
        _scrollView.showsHorizontalScrollIndicator = NO;
    }
    return _scrollView;
}

- (UILabel *)titleLabel {
    if (!_titleLabel) {
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.font = [UIFont systemFontOfSize:24];
        _titleLabel.textAlignment = NSTextAlignmentLeft;
        _titleLabel.textColor =[UIColor whiteColor];
    }
    return _titleLabel;
}

- (NSMutableArray *)dataArray {
    if (_dataArray == nil) {
        _dataArray = [NSMutableArray array];
        
        SceneButtonModel *pushStreamingModel = [[SceneButtonModel alloc] init];
        pushStreamingModel.title = @"推流";
        pushStreamingModel.descTitle = @"便捷接入实现标准WebRTC推流";
        pushStreamingModel.iconName = @"menu_pushStreaming";
        [_dataArray addObject:pushStreamingModel];
        
        SceneButtonModel *pullStreamingModel = [[SceneButtonModel alloc] init];
        pullStreamingModel.descTitle = @"200ms延时标准WebRTC拉流";
        pullStreamingModel.title = @"拉流";
        pullStreamingModel.iconName = @"menu_pullStreaming";
        [_dataArray addObject:pullStreamingModel];
    }
    return _dataArray;
}

@end

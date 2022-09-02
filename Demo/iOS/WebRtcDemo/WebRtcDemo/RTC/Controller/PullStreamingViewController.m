//
//  PullStreamingViewController.m
//  WebRtcDemo
//
//  Created by ByteDance on 2022/6/12.
//

#import "PullStreamingViewController.h"
#import "PullStreamingFailView.h"
#import <veWTN/Subscriber.h>

@interface PullStreamingViewController ()<SubscriberDelegate>

@property (nonatomic, strong) UIView *headerView;
@property (nonatomic, strong) UILabel *titleLabel;
@property (nonatomic, strong) UIButton *stopPushStreamingBtn;

@property (nonatomic, strong) UIView *remoteVideoView;

@property (nonatomic, strong) Subscriber *subscriber;

@property (nonatomic, strong) PullStreamingFailView *pullStreamingFailView;



@end

@implementation PullStreamingViewController

- (void)dealloc{
    /// 销毁引擎
    [self.subscriber destroy];

    self.subscriber = nil;
    

}


- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addBgGradientLayer];
    
    [self buildUI];
    
    [self initSubscriber];
    
    
    [self showToastWithMessage:@"正在加载..."  icon:@"" isloading:YES];

}

- (void)buildUI{
    
    UIEdgeInsets edgeInsets = UIEdgeInsetsZero;
    if (@available(iOS 11.0, *)) {
        edgeInsets = [UIApplication sharedApplication].keyWindow.safeAreaInsets;
    }
    
    [self.view addSubview:self.headerView];
    [self.headerView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(self.view.mas_top);
        make.left.right.equalTo(self.view);
        make.height.mas_equalTo(64);
    }];
    
    [self.headerView addSubview:self.titleLabel];
    self.titleLabel.text = @"拉流";
    [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.headerView.mas_centerX);
        make.bottom.equalTo(self.headerView.mas_bottom).mas_equalTo(-8);
        make.size.mas_equalTo(CGSizeMake(300 / 2, 29));
    }];
    
    [self.headerView addSubview:self.stopPushStreamingBtn];
    [self.stopPushStreamingBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.mas_equalTo(-20);
        make.size.mas_equalTo(24);
        make.bottom.equalTo(self.headerView.mas_bottom).mas_equalTo(-10);
    }];
    
    [self.view addSubview:self.remoteVideoView];
    [self.remoteVideoView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.equalTo(self.view);
        make.top.equalTo(self.headerView.mas_bottom);
        make.bottom.equalTo(self.view.mas_bottom);
    }];
    
    [self.view addSubview:self.pullStreamingFailView];
    [self.pullStreamingFailView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.equalTo(self.view);
        make.top.equalTo(self.view).mas_equalTo(125);
        make.height.mas_equalTo(250);
    }];
    self.pullStreamingFailView.hidden = YES;
}

#pragma mark -- subscriber
- (void)initSubscriber {
    ///初始化拉流引擎
    self.subscriber = [Subscriber create:self];
    
    ///设置远端视频view
    [self.subscriber setRemoteView:self.remoteVideoView];
    
    [self.subscriber setEnableSpeaker:YES];
    
    [self.subscriber startSubscribe:self.pullStreamingUrl];
        
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

#pragma mark - Private Method
- (void)stopPushStreaming:(UIButton *)button{
    button.selected = !button.selected;
    [self showAlert:@""];
}

/// showToast
- (void)showToastWithMessage:(NSString *)message icon:(NSString *)icon isloading:(BOOL )isloading {
    
    UIViewController *windowVC = [DeviceInforTool topViewController];
    [[ToastComponents shareToastComponents] showWithMessage:message view:windowVC.view keep:NO isLoading:isloading icon:icon block:^(BOOL result) {
        
    }];
}

- (void)showAlert:(NSString *)message {
    UIAlertController *alert = [UIAlertController
                                alertControllerWithTitle:@"确定停止拉流么？"
                                message:message
                                preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *sureAlertAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction *_Nonnull action) {
        [self.subscriber stopSubscribe];
        [self.navigationController popViewControllerAnimated:YES];
    }];
    UIAlertAction *cancelAlertAction = [UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:^(UIAlertAction *_Nonnull action) {
    }];
    [alert addAction:sureAlertAction];
    [alert addAction:cancelAlertAction];

    dispatch_async(dispatch_get_main_queue(), ^{
        [self presentViewController:alert animated:YES completion:nil];
    });
    
}

#pragma mark - SubscriberDelegate
- (void)subscriber:(Subscriber *)subscriber onSubscribeStateChanged:(SubscribeState)state reason:(Reason)reason {
    if (reason != Success || ![subscriber isSubscribed]) {
        self.remoteVideoView.hidden = YES;
        self.pullStreamingFailView.hidden = NO;
    }
}
#pragma mark - getter
- (UIView *)headerView{
    if(!_headerView){
        _headerView = [[UIView alloc] init];
    }
    return _headerView;
}

- (UILabel *)titleLabel{
    if(!_titleLabel){
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.textAlignment = NSTextAlignmentCenter;
        _titleLabel.textColor =[UIColor whiteColor];
        _titleLabel.font = [UIFont systemFontOfSize:18];
    }
    return _titleLabel;
}

- (UIButton *)stopPushStreamingBtn{
    if(!_stopPushStreamingBtn){
        _stopPushStreamingBtn = [[UIButton alloc] init];
        [_stopPushStreamingBtn setImage:[UIImage imageNamed:@"stopPushStreaming"] forState:UIControlStateNormal];
        _stopPushStreamingBtn.imageView.contentMode =UIViewContentModeScaleAspectFit;
        [_stopPushStreamingBtn addTarget:self action:@selector(stopPushStreaming:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _stopPushStreamingBtn;
}

- (UIView *)remoteVideoView{
    if(!_remoteVideoView){
        _remoteVideoView = [[UIView alloc] init];
    }
    return _remoteVideoView;
}

- (PullStreamingFailView *)pullStreamingFailView{
    if(!_pullStreamingFailView){
        _pullStreamingFailView = [[PullStreamingFailView alloc] init];
        __weak __typeof(self) wself = self;
        _pullStreamingFailView.retryBlock = ^{
            [wself.subscriber stopSubscribe];
            [wself.subscriber startSubscribe:wself.pullStreamingUrl];
        };
    }
    return _pullStreamingFailView;
}

@end

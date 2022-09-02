//
//  PushStreamingViewController.m
//  WebRtcDemo
//
//  Created by ByteDance on 2022/6/12.
//

#import "PushStreamingViewController.h"
#import "UserLiveView.h"
#import "MenuItemButton.h"
#import <veWTN/Publisher.h>


@interface PushStreamingViewController ()<PublisherDelegate>


@property (nonatomic, strong) UIView *headerView;
@property (nonatomic, strong) UIButton *switchCameraBtn;
@property (nonatomic, strong) UILabel *titleLabel;
@property (nonatomic, strong) UIButton *stopPushStreamingBtn;


@property (nonatomic, strong) UIButton *startPushStreamingBtn;

@property (nonatomic, strong) UIView *foooterView;
@property (nonatomic, strong) MenuItemButton *localAudioBtn;
@property (nonatomic, strong) MenuItemButton *localVideoBtn;

@property (nonatomic, strong) UIView *containerView;
@property (nonatomic, strong) UserLiveView *localView;


@property (nonatomic, strong) UIButton *pushStreamingBtn;

@property (nonatomic, strong) Publisher *publisher;

@property (nonatomic, assign) BOOL  isVideoOpen;


@end

@implementation PushStreamingViewController

- (void)dealloc{
    /// 销毁引擎
    [self.publisher destroy];

    self.publisher = nil;
    
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self addBgGradientLayer];

    [self buildUI];
    
    [self initPublisher];
    
}

- (void)buildUI{
    
    UIEdgeInsets edgeInsets = UIEdgeInsetsZero;
    if (@available(iOS 11.0, *)) {
        edgeInsets = [UIApplication sharedApplication].keyWindow.safeAreaInsets;
    }
    
    [self.view addSubview:self.headerView];
    [self.headerView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(edgeInsets.top);
        make.left.right.equalTo(self.view);
        make.height.mas_equalTo(49);
    }];
    
    self.switchCameraBtn.frame = CGRectMake(22, 14, 26, 26);
    [self.headerView addSubview:self.switchCameraBtn];
    [self.switchCameraBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(22);
        make.size.mas_equalTo(26);
        make.centerY.equalTo(self.headerView);
    }];
    
    [self.headerView addSubview:self.titleLabel];
    [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.center.equalTo(self.headerView);
        make.left.greaterThanOrEqualTo(self.switchCameraBtn.mas_right).mas_offset(22);
    }];
    self.titleLabel.text = @"推流";
    
    [self.headerView addSubview:self.stopPushStreamingBtn];
    [self.stopPushStreamingBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.mas_equalTo(-20);
        make.size.mas_equalTo(24);
        make.centerY.equalTo(self.headerView);
    }];
    
    [self.view addSubview: self.foooterView];
    [self.foooterView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.equalTo(self.view);
        make.bottom.equalTo(self.view).offset(-edgeInsets.bottom);
        make.height.mas_equalTo(64);
    }];
    
    [self.foooterView addSubview:self.localAudioBtn];
    [self.localAudioBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.width.equalTo(self.view).multipliedBy(0.5);
        make.height.mas_equalTo(49);
        make.left.mas_equalTo(0);
        make.centerY.equalTo(self.foooterView);
    }];
   
    [self.foooterView addSubview:self.localVideoBtn];
    [self.localVideoBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.width.equalTo(self.view).multipliedBy(0.5);
        make.height.mas_equalTo(49);
        make.right.mas_equalTo(0);
    }];
    
        
    [self.view addSubview:self.containerView];
    [self.containerView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.right.equalTo(self.view);
        make.top.equalTo(self.headerView.mas_bottom);
        make.bottom.equalTo(self.foooterView.mas_top);
    }];
    
    [self.containerView addSubview:self.localView];
    [self.localView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.left.top.bottom.equalTo(self.containerView);
    }];
    
    [self.containerView addSubview:self.pushStreamingBtn];
    [self.pushStreamingBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(50);
        make.right.mas_equalTo(-50);
        make.bottom.equalTo(self.containerView.mas_bottom).mas_equalTo(-20);
        make.height.mas_equalTo(50);
    }];
}

#pragma mark - RTC Method

- (void)initPublisher{
    /// 创建引擎
    self.publisher = [Publisher create:self];

    /// 设置视频发布参数
    VideoConfig *videoConfig = [[VideoConfig alloc]init];
    videoConfig.width = 640;
    videoConfig.height = 960;
    videoConfig.fps =15;
    [self.publisher setVideoConfig:videoConfig];

    [self setLocalRenderView];

    /// 开启本地音频采集
    [self.publisher startAudioCapture];

    /// 开启本地视频采集
    [self.publisher startVideoCapture];
    
    self.isVideoOpen = YES;
    
}

- (void)setLocalRenderView{
    self.localView.uid = @"";
    /// 设置本地视频渲染视图
    [self.publisher setLocalView:self.localView];
}

#pragma mark - PublisherDelegate
- (void)publisher:(Publisher *)publisher onPublishStateChanged:(PublishState)state reason:(Reason)reason {
}


#pragma mark - alert
- (void)showAlert:(NSString *)message {
    UIAlertController *alert = [UIAlertController
                                alertControllerWithTitle:@"确定停止推流么？"
                                message:message
                                preferredStyle:UIAlertControllerStyleAlert];
    
    UIAlertAction *sureAlertAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction *_Nonnull action) {
        [self.publisher stopPublish];
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


#pragma mark - Private Method
- (void)pushStreamingBtnClick:(UIButton *)sender  {
        self.pushStreamingBtn.hidden = YES;
    [self.publisher startPublish:PushStreamUrl];
}

- (void)switchCamera:(UIButton *)button{
    button.selected = !button.selected;
    /// 切换前置/后置摄像头（默认使用前置摄像头）
    if(self.isVideoOpen) {
        [self.publisher switchCamera];
    }
}

- (void)stopPushStreaming:(UIButton *)button{
    button.selected = !button.selected;
    if ([self.publisher isPublishing]) {
        [self showAlert:@""];
    } else {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)enableLocalAudio:(UIButton *)button{
    button.selected = !button.selected;
    if (button.selected) {
        /// 关闭本地音频发送
        [self.publisher stopAudioCapture];
    }else{
        /// 开启本地音频发送
        [self.publisher startAudioCapture];
    }
}

- (void)enableLocalVideo:(UIButton *)button{
    button.selected = !button.selected;
    if (button.selected) {
        /// 关闭本地视频发送
        self.isVideoOpen = NO;
        [self.publisher stopVideoCapture];
    }else{
        /// 开启本地视频发送
        self.isVideoOpen = YES;
        [self.publisher startVideoCapture];
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
#pragma mark - Getter

- (UIView *)headerView{
    if(!_headerView){
        _headerView = [[UIView alloc] init];
    }
    return _headerView;
}

- (UIButton *)switchCameraBtn{
    if(!_switchCameraBtn){
        _switchCameraBtn = [[UIButton alloc] init];
        [_switchCameraBtn setImage:[UIImage imageNamed:@"switch_camera"] forState:UIControlStateNormal];
        [_switchCameraBtn addTarget:self action:@selector(switchCamera:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _switchCameraBtn;
}

- (UILabel *)titleLabel{
    if(!_titleLabel){
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.textColor = [UIColor whiteColor];
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

- (UIButton *)pushStreamingBtn{
    if(!_pushStreamingBtn){
        _pushStreamingBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        _pushStreamingBtn.backgroundColor = [UIColor whiteColor];
        _pushStreamingBtn.layer.backgroundColor = [UIColor colorWithRed:0.086 green:0.365 blue:1 alpha:1].CGColor;
        [_pushStreamingBtn setTitle:@"开始推流" forState:UIControlStateNormal];
        [_pushStreamingBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
        _pushStreamingBtn.layer.cornerRadius = 25;
        _pushStreamingBtn.layer.masksToBounds = YES;
        
        [_pushStreamingBtn addTarget:self action:@selector(pushStreamingBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _pushStreamingBtn;
}

- (UIView *)foooterView{
    if(!_foooterView){
        _foooterView = [[UIView alloc] init];
    }
    return _foooterView;
}

- (MenuItemButton *)localAudioBtn{
    if(!_localAudioBtn){
        _localAudioBtn = [[MenuItemButton alloc] init];
        _localAudioBtn.backgroundColor = [UIColor clearColor];
        [_localAudioBtn addTarget:self action:@selector(enableLocalAudio:) forControlEvents:UIControlEventTouchUpInside];
        
        _localAudioBtn.imageEdgeInsets = UIEdgeInsetsMake(4, 0, 20, 0);
        _localAudioBtn.imageView.contentMode = UIViewContentModeScaleAspectFit;
        _localAudioBtn.desTitle = @"麦克风";
        _localAudioBtn.isAction = NO;
        
        [_localAudioBtn setImage:[UIImage imageNamed:@"normal_audio"] forState:UIControlStateNormal];
        [_localAudioBtn setImage:[UIImage imageNamed:@"mute_audio"] forState:UIControlStateSelected];
    }
    return _localAudioBtn;
}

- (MenuItemButton *)localVideoBtn{
    if(!_localVideoBtn){
        
        _localVideoBtn = [[MenuItemButton alloc] init];
        _localVideoBtn.backgroundColor = [UIColor clearColor];
        [_localVideoBtn addTarget:self action:@selector(enableLocalVideo:) forControlEvents:UIControlEventTouchUpInside];
        _localVideoBtn.imageEdgeInsets = UIEdgeInsetsMake(4, 0, 20, 0);
        _localVideoBtn.imageView.contentMode = UIViewContentModeScaleAspectFit;
        _localVideoBtn.desTitle = @"摄像头";
        _localVideoBtn.isAction = NO;
        [_localVideoBtn setImage:[UIImage imageNamed:@"normal_video"] forState:UIControlStateNormal];
        [_localVideoBtn setImage:[UIImage imageNamed:@"mute_video"] forState:UIControlStateSelected];
        
    }
    return _localVideoBtn;
}

- (UIView *)containerView{
    if(!_containerView){
        _containerView = [[UIView alloc] init];
        _containerView.backgroundColor = [UIColor lightGrayColor];
    }
    return _containerView;
}

- (UserLiveView *)localView{
    if(!_localView){
        _localView = [[UserLiveView alloc] init];
    }
    return _localView;
}

@end

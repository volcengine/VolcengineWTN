//
//  InputStreamingUrlViewController.m
//  WebRtcDemo
//
//  Created by ByteDance on 2022/6/12.
//

#import "InputStreamingUrlViewController.h"
#import "PullStreamingViewController.h"

@interface InputStreamingUrlViewController ()<UITextViewDelegate>

@property (nonatomic, strong) UIButton *backBtn;

@property (nonatomic, strong) UILabel *titleLabel;

@property (nonatomic, strong) UITextView *streamingUrlInputView;
@property (nonatomic, strong) UIButton *copStreamingUrlBtn;

@property (nonatomic, strong) UIButton *pullStreamingBtn;



@end

@implementation InputStreamingUrlViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self addBgGradientLayer];
    
    [self buildUI];

}

- (void)buildUI{
    
    UIEdgeInsets edgeInsets = UIEdgeInsetsZero;
    if (@available(iOS 11.0, *)) {
        edgeInsets = [UIApplication sharedApplication].keyWindow.safeAreaInsets;
    }
    
    [self.view addSubview:self.backBtn];
    
    [self.backBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.mas_equalTo(self.view.mas_top).mas_equalTo(34);
        make.left.equalTo(self.view.mas_left).mas_equalTo(16);
        make.height.mas_equalTo(28);
        make.width.mas_equalTo(28);
    }];
    
    [self.view addSubview:self.titleLabel];
    _titleLabel.text = @"请输入拉流地址";
    [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(20);
        make.right.mas_equalTo(-20);
        make.height.mas_equalTo(40);
        make.top.mas_equalTo(self.backBtn.mas_bottom).mas_equalTo(55);
    }];
    
    [self.view addSubview:self.streamingUrlInputView];
    [self.streamingUrlInputView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(20);
        make.right.mas_equalTo(-20);
        make.top.mas_equalTo(self.titleLabel.mas_bottom).mas_equalTo(20);
        make.height.mas_equalTo(160);

    }];
    
    [self.view addSubview:self.copStreamingUrlBtn];
    [self.copStreamingUrlBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.right.mas_equalTo(-40);
        make.bottom.mas_equalTo(self.streamingUrlInputView.mas_bottom).mas_equalTo(-12);
        make.height.mas_equalTo(20);
        make.width.mas_equalTo(40);
    }];
    
    [self.view addSubview:self.pullStreamingBtn];
    [self.pullStreamingBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.mas_equalTo(20);
        make.right.mas_equalTo(-20);
        make.top.mas_equalTo(self.streamingUrlInputView.mas_bottom).mas_equalTo(40);
        make.height.mas_equalTo(49);

    }];
    
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

/// showToast
- (void)showToastWithMessage:(NSString *)message icon:(NSString *)icon isloading:(BOOL )isloading {
    
    UIViewController *windowVC = [DeviceInforTool topViewController];
    [[ToastComponents shareToastComponents] showWithMessage:message view:windowVC.view keep:NO isLoading:isloading icon:icon block:^(BOOL result) {
        
    }];
}

#pragma mark --UITextViewDelegate
- (void)textViewDidChange:(UITextView *)textView {
    if(textView.text.length) {
        _pullStreamingBtn.enabled = YES;
        _pullStreamingBtn.alpha = 1;
        _pullStreamingBtn.layer.backgroundColor = [UIColor colorWithRed:0.086 green:0.365 blue:1 alpha:1].CGColor;
        [_pullStreamingBtn setTitleColor:[UIColor colorWithRed:1 green:1 blue:1 alpha:1]  forState:UIControlStateNormal];
    }
}



#pragma mark --btnClick
- (void)back:(UIButton *)sender {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)copStreamingUrlBtnClick:(UIButton *)sender {
    self.streamingUrlInputView.text = [UIPasteboard generalPasteboard].string;
    if(self.streamingUrlInputView.text.length) {
        _pullStreamingBtn.enabled = YES;
        _pullStreamingBtn.alpha = 1;
        _pullStreamingBtn.layer.backgroundColor = [UIColor colorWithRed:0.086 green:0.365 blue:1 alpha:1].CGColor;
        [_pullStreamingBtn setTitleColor:[UIColor colorWithRed:1 green:1 blue:1 alpha:1]  forState:UIControlStateNormal];
    }
}

- (void)pullStreamingBtnClick:(UIButton *)sender {
    
    [self showToastWithMessage:@"正在拉流..."  icon:@"" isloading:YES];
    PullStreamingViewController *pullStreamingViewController  = [[PullStreamingViewController alloc]init];
    if(self.streamingUrlInputView.text.length) {
        pullStreamingViewController.pullStreamingUrl = self.streamingUrlInputView.text;
    } else {
        pullStreamingViewController.pullStreamingUrl = PullStreamUrl;
    }
    [self.navigationController pushViewController:pullStreamingViewController animated:YES];

}

#pragma mark - Getter
- (UIButton *)backBtn{
    if(!_backBtn){
        _backBtn = [[UIButton alloc] init];
        _backBtn.imageView.contentMode = UIViewContentModeScaleAspectFit;
        [_backBtn setImage:[UIImage imageNamed:@"back_icon"] forState:UIControlStateNormal];
        [_backBtn addTarget:self action:@selector(back:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _backBtn;
}

- (UILabel *)titleLabel{
    if(!_titleLabel){
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.textColor =[UIColor whiteColor];
        _titleLabel.font = [UIFont systemFontOfSize:18];
    }
    return _titleLabel;
}

- (UITextView *)streamingUrlInputView{
    if(!_streamingUrlInputView){
        _streamingUrlInputView = [[UITextView alloc] init];
        _streamingUrlInputView.delegate = self;
        _streamingUrlInputView.textColor = [UIColor whiteColor];
        _streamingUrlInputView.backgroundColor = [UIColor whiteColor];
        _streamingUrlInputView.layer.backgroundColor = [UIColor colorWithRed:0.137 green:0.165 blue:0.231 alpha:1].CGColor;
        _streamingUrlInputView.layer.cornerRadius = 12;
        _streamingUrlInputView.layer.masksToBounds = YES;
    }
    return _streamingUrlInputView;
}

- (UIButton *)copStreamingUrlBtn{
    if(!_copStreamingUrlBtn){
        _copStreamingUrlBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        [_copStreamingUrlBtn setTitle:@"粘贴" forState:UIControlStateNormal];
        [_copStreamingUrlBtn setTitleColor:[UIColor colorWithRed:0.251 green:0.502 blue:1 alpha:1] forState:UIControlStateNormal];
        [_copStreamingUrlBtn addTarget:self action:@selector(copStreamingUrlBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _copStreamingUrlBtn;
}


- (UIButton *)pullStreamingBtn{
    if(!_pullStreamingBtn){
        _pullStreamingBtn = [UIButton buttonWithType:UIButtonTypeCustom];
        _pullStreamingBtn.backgroundColor = [UIColor whiteColor];
        _pullStreamingBtn.alpha = 0.3;
        _pullStreamingBtn.layer.backgroundColor = [UIColor colorWithRed:0.251 green:0.502 blue:1 alpha:1].CGColor;
        [_pullStreamingBtn setTitle:@"拉流" forState:UIControlStateNormal];
        [_pullStreamingBtn setTitleColor:[UIColor colorWithRed:1 green:1 blue:1 alpha:0.3]  forState:UIControlStateNormal];
        _pullStreamingBtn.layer.cornerRadius = 25;
        _pullStreamingBtn.layer.masksToBounds = YES;
        _pullStreamingBtn.enabled = NO;
        [_pullStreamingBtn addTarget:self action:@selector(pullStreamingBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _pullStreamingBtn;
}
@end

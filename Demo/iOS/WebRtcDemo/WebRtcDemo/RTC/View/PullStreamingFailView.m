//
//  PullStreamingFailView.m
//  WebRtcDemo
//
//  Created by ByteDance on 2022/6/15.
//

#import "PullStreamingFailView.h"

@interface PullStreamingFailView ()

@property (nonatomic, strong) UIImageView *failImageView;

@property (nonatomic, strong) UIImageView *btnIconImageView;

@property (nonatomic, strong) UIButton *retryBtn;


@end

@implementation PullStreamingFailView

- (instancetype)initWithFrame:(CGRect)frame {
    
    if (self = [super initWithFrame:frame]) {
        
        [self addSubview:self.failImageView];
        [self.failImageView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self);
            make.centerX.equalTo(self.mas_centerX);
            make.width.mas_equalTo(150);
            make.height.mas_equalTo(150);
        }];
        
        [self addSubview:self.retryBtn];
        [self.retryBtn mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self.failImageView.mas_bottom).mas_equalTo(30);
            make.centerX.equalTo(self.mas_centerX);
            make.height.mas_equalTo(40);
            make.width.mas_equalTo(150);
        }];
        
        [self.retryBtn addSubview:self.btnIconImageView];
        [self.btnIconImageView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.mas_equalTo(15);
            make.right.mas_equalTo(-15);
            make.top.mas_equalTo(10);
            make.bottom.mas_equalTo(-10);
        }];
        
    }
    
    return self;
}

#pragma mark -btnclick
- (void)retryBtnClick:(UIButton *)sender {
    if (self.retryBlock) {
        self.retryBlock();
    }
}

#pragma mark - Getter
- (UIImageView *)failImageView{
    if(!_failImageView){
        _failImageView = [[UIImageView alloc] init];
        _failImageView.image = [UIImage imageNamed:@"pullStreamingFail_icon"];
        
    }
    return _failImageView;
}

- (UIImageView *)btnIconImageView{
    if(!_btnIconImageView){
        _btnIconImageView = [[UIImageView alloc] init];
        _btnIconImageView.contentMode = UIViewContentModeScaleAspectFit;
        _btnIconImageView.image = [UIImage imageNamed:@"retryBtn_icon"];
    }
    return _btnIconImageView;
}



- (UIButton *)retryBtn{
    if(!_retryBtn){
        _retryBtn = [[UIButton alloc] init];
        _retryBtn.backgroundColor = [UIColor whiteColor];
        _retryBtn.layer.backgroundColor = [UIColor colorWithRed:0.086 green:0.365 blue:1 alpha:1].CGColor;
        _retryBtn.layer.cornerRadius = 20;
        _retryBtn.layer.masksToBounds = YES;
        [_retryBtn addTarget:self action:@selector(retryBtnClick:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _retryBtn;
}

@end

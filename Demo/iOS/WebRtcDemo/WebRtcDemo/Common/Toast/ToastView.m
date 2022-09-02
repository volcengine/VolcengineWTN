//
//  ToastView.m
//  veRTC_Demo
//
//  Created by bytedance on 2021/6/3.
//  Copyright © 2021 . All rights reserved.
//

#import "ToastView.h"
#import "Masonry.h"

@interface ToastView ()

@property (nonatomic, strong) UIView *bgView;

@end


@implementation ToastView

- (instancetype)initWithMeeage:(NSString *)message icon:(NSString *)icon isLoading:(BOOL )isLoading isHaveIcon:(BOOL )isHaveIcon {
    self = [super init];
    if (self) {
        CGFloat minScreen = MIN([UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
        CGFloat scale = (minScreen / 375);
        self.bgView = [[UIView alloc] init];
        self.bgView.backgroundColor = [UIColor whiteColor];
        self.bgView.layer.backgroundColor = [UIColor colorWithRed:0.188 green:0.226 blue:0.312 alpha:0.9].CGColor;
        self.bgView.layer.cornerRadius = 4;
        self.bgView.layer.masksToBounds = YES;
        
        UILabel *titleLabel = [[UILabel alloc] init];
        titleLabel.numberOfLines = 0;
        titleLabel.textColor = [UIColor whiteColor];
        titleLabel.textAlignment = NSTextAlignmentCenter;
        titleLabel.font = [UIFont systemFontOfSize:16.0 * scale weight:UIFontWeightRegular];
        titleLabel.text = message;
        
        
        [self addSubview:self.bgView];
        [self.bgView addSubview:titleLabel];
        
        if (isHaveIcon || isLoading) {
            
            if (isLoading) {
                UIActivityIndicatorView * activityIndicator = [[UIActivityIndicatorView alloc]initWithActivityIndicatorStyle:(UIActivityIndicatorViewStyleGray)];
                [self.bgView addSubview:activityIndicator];
                activityIndicator.color = [UIColor whiteColor];
                activityIndicator.hidesWhenStopped = NO;
                [activityIndicator startAnimating];
                
                [activityIndicator mas_makeConstraints:^(MASConstraintMaker *make) {
                    make.left.mas_equalTo(16);
                    make.size.mas_equalTo(16);
                    make.centerY.equalTo(self.bgView.mas_centerY);
                }];
            } else {
                UIImageView *iconImageView = [[UIImageView alloc] init];
                iconImageView.image = [UIImage imageNamed:icon];
                iconImageView.contentMode = UIViewContentModeScaleAspectFit;
                [self.bgView addSubview:iconImageView];
                
                [iconImageView mas_makeConstraints:^(MASConstraintMaker *make) {
                    make.left.mas_equalTo(16);
                    make.size.mas_equalTo(16);
                    make.centerY.equalTo(self.bgView.mas_centerY);
                }];
            }
            
            [titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
                make.left.mas_equalTo(38);
                make.right.mas_equalTo(-13);
                make.height.mas_equalTo(26);
                make.width.mas_lessThanOrEqualTo(minScreen - 38 - 13);
            }];
        } else {
            [titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
                make.center.equalTo(self.bgView);
                make.width.mas_lessThanOrEqualTo(minScreen - 24 * 2);
            }];
        }
        
        [self.bgView mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(titleLabel).offset(-12);
            make.right.equalTo(titleLabel).offset(12);
            make.top.equalTo(titleLabel).offset(-12);;
            make.bottom.equalTo(titleLabel).offset(12);;
        }];
        
        [self mas_updateConstraints:^(MASConstraintMaker *make) {
            make.size.equalTo(self.bgView);
        }];
    }
    return self;
}

@end

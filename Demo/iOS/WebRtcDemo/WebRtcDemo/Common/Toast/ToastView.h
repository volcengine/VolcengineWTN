//
//  ToastView.h
//  veRTC_Demo
//
//  Created by bytedance on 2021/6/3.
//  Copyright © 2021 . All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface ToastView : UIView

- (instancetype)initWithMeeage:(NSString *)message icon:(NSString *)icon isLoading:(BOOL )isLoading isHaveIcon:(BOOL )isHaveIcon;

@end

NS_ASSUME_NONNULL_END

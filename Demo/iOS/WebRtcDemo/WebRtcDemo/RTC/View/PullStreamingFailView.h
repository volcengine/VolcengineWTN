//
//  PullStreamingFailView.h
//  WebRtcDemo
//
//  Created by ByteDance on 2022/6/15.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^RetryBlock)(void);

@interface PullStreamingFailView : UIView

@property (nonatomic, copy) RetryBlock retryBlock;

@end

NS_ASSUME_NONNULL_END

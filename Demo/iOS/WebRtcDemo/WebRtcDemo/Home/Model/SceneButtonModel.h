//
//  SceneCellModel.h
//  veRTC_Demo
//
//  Created by bytedance on 2021/7/26.
//  Copyright © 2021 . All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface SceneButtonModel : NSObject
@property (nonatomic, copy) NSString *iconName;
@property (nonatomic, copy) NSString *bgName;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *descTitle;
@property (nonatomic, strong) NSObject *scenes;

@end

NS_ASSUME_NONNULL_END

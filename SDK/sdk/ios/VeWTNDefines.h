/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#define VEWTN_EXPORT __attribute__((visibility("default")))
/** {zh}
 * @type keytype
 * @brief 音视频流发布状态
 */
/** {en}
 * @type keytype
 * @brief Publish state
 */
typedef NS_ENUM(NSInteger, PublishState) {
   /** {zh}
    * @brief 开始发布成功
    */
   /** {en}
    * @brief Succeed to start publishing the stream
    */
    StartPublishSuccess = 0,
   /** {zh}
    * @brief 开始发布失败
    */
   /** {en}
    * @brief Fail to start publishing the stream
    */
    StartPublishFailed = 1,
   /** {zh}
    * @brief 停止发布成功
    */
   /** {en}
    * @brief Succeed to start publishing the stream
    */
    StopPublishSuccess = 2,
   /** {zh}
    * @brief 停止发布失败
    */
   /** {en}
    * @brief Fail to stop publishing the stream
    */
    StopPublishFailed = 3,
};
/** {zh}
 * @type keytype
 * @brief 音视频流订阅状态
 */
/** {en}
 * @type keytype
 * @brief Subscribe state
 */
typedef NS_ENUM(NSInteger, SubscribeState) {
   /** {zh}
    * @brief 开始订阅成功
    */
   /** {en}
    * @brief Succeed to start subscribing the stream
    */
    StartSubscribeSuccess = 0,
   /** {zh}
    * @brief 开始订阅失败
    */
   /** {en}
    * @brief Fail to start subscribing the stream
    */
    StartSubscribeFailed = 1,
   /** {zh}
    * @brief 停止订阅成功
    */
   /** {en}
    * @brief Succeed to stop subscribing the stream
    */
    StopSubscribeSuccess = 2,
   /** {zh}
    * @brief 停止订阅失败
    */
   /** {en}
    * @brief Fail to stop subscribing the stream
    */
    StopSubscribeFailed = 3,
};
/** {zh}
 * @type keytype
 * @brief 订阅/发布成功或失败的原因
 */
/** {en}
 * @type keytype
 * @brief reason
 */
typedef NS_ENUM(NSInteger, Reason) {
   /** {zh}
    * @brief 成功
    */
   /** {en}
    * @brief Success
    */
    Success = 0,
   /** {zh}
    * @brief 失败。参数错误。
    */
   /** {en}
    * @brief Failure. Parameter error.
    */
    ParameterError = 1,
   /** {zh}
    * @brief 失败。Token 错误。包括 Token 错误、Token 过期、Token 鉴权失败等。
    */
   /** {en}
    * @brief Failure. Token error, including wrong token, expired token and token authentication failed.
    */ 
    TokenError = 2,
   /** {zh}
    * @brief 失败。未找到资源。
    */
   /** {en}
    * @brief Failure. Resources not found.
    */
    ResourcesNotFound = 3,
   /** {zh}
    * @brief 失败。服务器内部错误。请重试。
    */
   /** {en}
    * @brief Failure. Server internal error. Please try again.
    */
    ServerInternalError = 4,
   /** {zh}
    * @brief 失败。SDK 内部错误。请重试。
    */
   /** {en}
    * @brief Failure. SDK internal error. Please try again.
    */
    SDKInternalError = 5,
   /** {zh}
    * @brief 失败。网络错误。
    */
   /** {en}
    * @brief Failure. Network error.
    */
    NetworkError = 6,
   /** {zh}
    * @brief 失败。未知错误。请重试。
    */
   /** {en}
    * @brief Failure. Unknown error. Please try again.
    */
    UnknowError = 7,
};
/** {zh}
 * @type keytype
 * @brief 视频配置
 */
/** {en}
 * @type keytype
 * @brief Video config
 */
VEWTN_EXPORT @interface VideoConfig : NSObject
/** {zh}
 * @brief 宽度（px）。默认值是 640。
 */
/** {en}
 * @brief Width in px. 640 by default.
 */
@property (nonatomic, assign) int width;
/** {zh}
 * @brief 高度（px）。默认值是 960。
 */
/** {en}
 * @brief Height in px. 960 by default.
 */
@property (nonatomic, assign) int height;
/** {zh}
 * @brief 最大帧率（fps）。默认值是 15。
 */
/** {en}
 * @brief Max fps. 15 by default.
 */
@property (nonatomic, assign) int fps;
@end


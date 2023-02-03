/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/**
 * {zh}
 * @type callback
 * @brief 事件回调类
 */
/** {en}
 * @type callback
 * @brief Event handler for RtcEngine
 */
public class RtcEventHandler {
    /** {zh}
     * @type callback
     * @brief 音视频流的发布状态发生改变时，收到此回调。
     * @param streamId 流 ID
     * @param state 当前发布状态，参看 State{@link #State}。
     * @param reason 发布状态发生改变的原因，参看 Reason{@link #Reason}。
     */
    /** {en}
     * @type callback
     * @brief Receive this event when the publish state of the media stream changes.
     * @param streamId stream id
     * @param state The current publishing state. See State{@link #State}.
     * @param reason Error code for the change. See Reason{@link #Reason}.
     */
    public void onPublishStateChanged(String streamId, RtcDefines.State state,
                                      RtcDefines.Reason reason) {
    }
    /** {zh}
     * @type callback
     * @brief 音视频流的订阅状态发生改变时，收到此回调。
     * @param streamId 流 ID
     * @param state 当前订阅状态，参看 State{@link #State}。
     * @param reason 订阅状态发生改变的原因，参看 Reason{@link #Reason}。
     */
    /** {en}
     * @type callback
     * @brief Receive this event when the subscribing state changes.
     * @param streamId stream id
     * @param state The current subscribing state. See State{@link #State}.
     * @param reason Error code when subscribing state changes. See Reason{@link #Reason}
     */
    public void onSubscribeStateChanged(String streamId, RtcDefines.State state,
                                        RtcDefines.Reason reason) {
    }
    /** {zh}
     * @type callback
     * @brief 发布流状态周期回调，周期1秒。
     * @param streamId 流 ID
     * @param audioStats 音频流信息，参看 RTCAudioStats{@link #RTCAudioStats}。
     * @param videoStats 视频流信息，参看 RTCVideoStats{@link #RTCVideoStats}。
     */
    /** {en}
     * @type callback
     * @brief Release flow state cycle callback with 1 second period..
     * @param streamId Stream ID
     * @param audioStats Audio stream information, see RTCAudioStats{@link #RTCAudioStats}.
     * @param videoStats Video stream information, see RTCVideoStats{@link #RTCVideoStats}.
     */
    public void onLocalStats(String streamId, RTCAudioStats audioStats, RTCVideoStats videoStats) {
    }
    /** {zh}
     * @type callback
     * @brief Mute音频成功回调。
     * @param streamId 流 ID
     */
    /** {en}
     * @type callback
     * @brief Mute audio success callback.
     * @param streamId Stream ID
     */
    public void onMuteAudioSuccess(String streamId) {
    }
    /** {zh}
     * @type callback
     * @brief Mute音频失败回调。
     * @param streamId 流 ID
     */
    /** {en}
     * @type callback
     * @brief Mute audio failed callback.
     * @param streamId Stream ID
     */
    public void onMuteAudioFailed(String streamId, RtcDefines.Reason reason) {
    }
    /** {zh}
     * @type callback
     * @brief Mute视频成功回调。
     * @param streamId 流 ID
     */
    /** {en}
     * @type callback
     * @brief Mute video success callback.
     * @param streamId Stream ID
     */
    public void onMuteVideoSuccess(String streamId) {
    }
    /** {zh}
     * @type callback
     * @brief Mute视频失败回调。
     * @param streamId 流 ID
     */
    /** {en}
     * @type callback
     * @brief Mute video failed callback.
     * @param streamId Stream ID
     */
    public void onMuteVideoFailed(String streamId, RtcDefines.Reason reason) {
    }
    /** {zh}
     * @type callback
     * @brief 订阅流状态周期回调，周期1秒。
     * @param streamId 流 ID
     * @param audioStats 音频流信息，参看 RTCAudioStats{@link #RTCAudioStats}。
     * @param videoStats 视频流信息，参看 RTCVideoStats{@link #RTCVideoStats}。
     */
    /** {en}
     * @type callback
     * @brief Release flow state cycle callback with 1 second period..
     * @param streamId Stream ID
     * @param audioStats Audio stream information, see RTCAudioStats{@link #RTCAudioStats}.
     * @param videoStats Video stream information, see RTCVideoStats{@link #RTCVideoStats}.
     */
    public void onRemoteStats(String streamId, RTCAudioStats audioStats, RTCVideoStats videoStats) {
    }
    /** {zh}
     * @type callback
     * @brief 远端首帧回调（订阅拉流）
     * @param streamId 流 ID
     */
    /** {en}
     * @type callback
     * @brief Release flow state cycle callback with 1 second period..
     * @param streamId Stream ID
     */
    public void onFirstRemoteVideoFrameRendered(String streamId) {
    }
    /** {zh}
     * @type callback
     * @brief 卡顿信息回调，周期1秒。
     * @param streamId 流 ID
     * @param stallInfo 卡顿信息，参看 StallInfo{@link #StallInfo}。
     */
    /** {en}
     * @type callback
     * @brief Stuck message callback, period 1 sec.
     * @param streamId Stream ID
     * @param stallInfo The stall information, see StallInfo{@link #StallInfo}.
     */
    public void onStallInfo(String streamId, StallInfo stallInfo) {
    }
    /** {zh}
     * @type callback
     * @brief 编码视频帧回调，周期由帧率决定(1000/fps ms)。
     * @param streamId 流 ID
     * @param encodedFrame 视频帧信息，参看 EncodedVideoFrame{@link #EncodedVideoFrame}。
     */
    /** {en}
     * @type callback
     * @brief Video frame callback with period determined by frame rate (1000/fps ms).
     * @param streamId Stream ID
     * @param encodedFrame Video frame information, see EncodedVideoFrame{@link #EncodedVideoFrame}.
     */
    public void onEncodedVideoFrame(String streamId, EncodedVideoFrame encodedFrame) {
    }
    /** {zh}
     * @type callback
     * @brief 视频帧回调，周期由帧率决定(1000/fps ms)。
     * @param streamId 流 ID
     * @param videoFrame 视频帧信息，参看 VideoFrame{@link #VideoFrame}。
     */
    /** {en}
     * @type callback
     * @brief Video frame callback with period determined by frame rate (1000/fps ms).
     * @param streamId Stream ID
     * @param videoFrame Video frame information, see VideoFrame{@link #VideoFrame}.
     */
    public void onVideoFrame(String streamId, VideoFrame videoFrame) {
    }
    /** {zh}
     * @type callback
     * @brief 音频帧回调，周期10ms。
     * @param streamId 流 ID
     * @param audioFrame 音频帧信息，参看 AudioFrame{@link #AudioFrame}。
     */
    /** {en}
     * @type callback
     * @brief Audio frame callback with 10ms period.
     * @param streamId Stream ID
     * @param audioFrame Audio frame information, see AudioFrame{@link #AudioFrame}.
     */
    public void onAudioFrame(String streamId, AudioFrame audioFrame) {
    }
}

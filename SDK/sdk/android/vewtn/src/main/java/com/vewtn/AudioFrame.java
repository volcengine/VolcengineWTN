/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 音频帧
 */
/** {en}
 * @type keytype
 * @brief Audio frame
 */
public class AudioFrame {
    /** {zh}
     * @brief 帧类型。目前仅支持 PCM16。
     */
    /** {en}
     * @brief Frame type. Only PCM16 support.
     */
    public RtcDefines.AudioFrameType type = RtcDefines.AudioFrameType.Pcm16;
    /** {zh}
     * @brief 数据缓存
     */
    /** {en}
     * @brief data buffer
     */
    public byte[] data;
    /** {zh}
     * @brief 数据缓存大小
     */
    /** {en}
     * @brief data buffer size
     */
    public int sampleLength;
    /** {zh}
     * @brief 时间戳，单位：毫秒。
     */
    /** {en}
     * @brief Timestamp in ms
     */
    public long timestampMs;
    /** {zh}
     * @brief 采样率
     */
    /** {en}
     * @brief SampleRate
     */
    public int sampleRate;
    /** {zh}
     * @brief 通道数
     */
    /** {en}
     * @brief Number of channels
     */
    public int channels;
}

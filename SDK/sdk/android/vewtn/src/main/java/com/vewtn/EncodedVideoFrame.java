/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 编码后的视频帧
 */
/** {en}
 * @type keytype
 * @brief Encoded video frame struct
 */
public class EncodedVideoFrame {
    /** {zh}
     * @brief 帧类型。目前仅支持 H.264，且必须是 Annex B 格式。
     */
    /** {en}
     * @brief Frame type. Only H264 - Annex B supported.
     */
    public RtcDefines.EncodedVideoFrameType codecType = RtcDefines.EncodedVideoFrameType.CodecH264;
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
    public int size;
    /** {zh}
     * @brief 宽度（px）
     */
    /** {en}
     * @brief Width in px
     */
    public int width;
    /** {zh}
     * @brief 高度（px）
     */
    /** {en}
     * @brief Height in px
     */
    public int height;
    /** {zh}
     * @brief 时间戳，单位毫秒
     */
    /** {en}
     * @brief Timestamp in ms
     */
    public long timestampMs;
    /** {zh}
     * @brief 逆时针旋转角度，单位为度。可选值为 0，90，180，270。
     */
    /** {en}
     * @brief Counterclockwise rotation in degrees. Options are 0, 90, 180, 270。
     */
    public int rotation;
    /** {zh}
     * @brief 是否为关键帧。如果是关键帧，那么，必须包含 SPS 和 PPS 信息。
     */
    /** {en}
     * @brief key frame or not. If it is a key frame, it must contain SPS and PPS information.
     */
    public boolean isKeyFrame;
}

/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 视频帧
 */
/** {en}
 * @type keytype
 * @brief Video frame struct
 */
public class VideoFrame {
    /** {zh}
     * @brief 帧类型。目前仅支持YUV I420。
     */
    /** {en}
     * @brief Frame type. Only YUV I420 supported.
     */
    public RtcDefines.VideoFrameType type = RtcDefines.VideoFrameType.YuvI420;
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
     * @brief Y 分量数据缓存
     */
    /** {en}
     * @brief Y buffer
     */
    public byte[] yBuffer;
    /** {zh}
     * @brief U 分量数据缓存
     */
    /** {en}
     * @brief U buffer
     */
    public byte[] uBuffer;
    /** {zh}
     * @brief V 分量数据缓存
     */
    /** {en}
     * @brief V buffer
     */
    public byte[] vBuffer;
    /** {zh}
     * @brief Y 分量 stride
     */
    /** {en}
     * @brief Y stride
     */
    public int yStride;
    /** {zh}
     * @brief U 分量 stride
     */
    /** {en}
     * @brief U stride
     */
    public int uStride;
    /** {zh}
     * @brief V 分量 stride
     */
    /** {en}
     * @brief V stride
     */
    public int vStride;
    /** {zh}
     * @brief 时间戳，单位毫秒。
     */
    /** {en}
     * @brief Timestamp in ms
     */
    public long timestampMs;
    /** {zh}
     * @brief 逆时针旋转角度，可选值为 0，90，180，270
     */
    /** {en}
     * @brief Counterclockwise rotation. The options are 0, 90, 180, and 270.
     */
    public int rotation;
}

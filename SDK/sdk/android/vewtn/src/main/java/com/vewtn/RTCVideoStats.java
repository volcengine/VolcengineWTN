/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 视频流数据
 */
/** {en}
 * @type keytype
 * @brief Video stats
 */
public class RTCVideoStats {
    /** {zh}
     * @brief 发送字节数，单位字节
     */
    /** {en}
     * @brief Number of bytes sent, in bytes
     */
    public long bytesSent;
    /** {zh}
     * @brief 接收字节数，单位字节
     */
    /** {en}
     * @brief Number of bytes received, in bytes
     */
    public long bytesReceived;
    /** {zh}
     * @brief 每秒发送字节数，单位字节
     */
    /** {en}
     * @brief Number of bytes sent per second, in bytes
     */
    public double bytesSentPerSecond;
    /** {zh}
     * @brief 每秒接收字节数，单位字节
     */
    /** {en}
     * @brief Number of bytes received per second, in bytes
     */
    public double bytesReceivedPerSecond;
    /** {zh}
     * @brief 视频帧宽度。单位：px。
     */
    /** {en}
     * @brief Video frame width in px.
     */
    public long frameWidth;
    /** {zh}
     * @brief 视频帧高度。单位：px。
     */
    /** {en}
     * @brief Video frame height in px.
     */
    public long frameHeight;
    /** {zh}
     * @brief 帧率
     */
    /** {en}
     * @brief frames per second
     */
    public long framesPerSecond;
    /** {zh}
     * @brief 收到的 RTCP RR 包的总个数。
     */
    /** {en}
     * @brief The total number of the received RTCP RR packets.
     */
    public long roundTripTimeMeasurements;
    /** {zh}
     * @brief 总 rtt 时间。单位：秒。除以 `roundTripTimeMeasurements` 可以获得平均时间。
     */
    /** {en}
     * @brief Total rtt time in seconds. You can get the average rtt time by dividing `totalRoundTripTime` by `roundTripTimeMeasurements`.
     */
    public double totalRoundTripTime;
    /** {zh}
     * @brief 发送包个数
     */
    /** {en}
     * @brief packets sent
     */
    public long packetsSent;
    /** {zh}
     * @brief 接收包个数
     */
    /** {en}
     * @brief packets received
     */
    public long packetsReceived;
    /** {zh}
     * @brief 发布过程中的丢包数
     */
    /** {en}
     * @brief Packets lost during publishing.
     */
    public long packetsLost;
    /** {zh}
     * @brief 丢包率。取值范围：`[0, 1.0]`。
     */
    /** {en}
     * @brief Packet lost rate during publishing. The range is `[0, 1.0]`.
     */
    public double franctionLoss;
    /** {zh}
     * @brief jitter 抖动。单位：秒。
     */
    /** {en}
     * @brief jitter in seconds.
     */
    public double jitter;
}

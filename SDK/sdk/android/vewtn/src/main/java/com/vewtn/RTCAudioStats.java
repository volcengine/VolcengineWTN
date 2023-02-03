/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 音频流数据
 */
/** {en}
 * @type keytype
 * @brief Audio stats
 */
public class RTCAudioStats {
    /** {zh}
     * @brief 发送字节数，单位：字节。
     */
    /** {en}
     * @brief Number of bytes sent.
     */
    public long bytesSent;
    /** {zh}
     * @brief 接收字节数，单位：字节。
     */
    /** {en}
     * @brief Number of bytes received.
     */
    public long bytesReceived;
    /** {zh}
     * @brief 每秒发送字节数，单位：字节。
     */
    /** {en}
     * @brief Number of bytes sent per second.
     */
    public double bytesSentPerSecond;
    /** {zh}
     * @brief 每秒接收字节数，单位：字节。
     */
    /** {en}
     * @brief Number of bytes received per second.
     */
    public double bytesReceivedPerSecond;
    /** {zh}
     * @brief 音量强度。<br>
     *        取值范围：`[0, 1.0]`。值为 `1.0` 时，表示 0dBov；值为 `0.5` 时，表示 6dB SPL。
     */
    /** {en}
     * @brief audio level. <br>
     *        The range is `[0, 1.0]`. When the value is `1.0`, the sound pressure level is 0dBov; when the value is `0.5`, the sound pressure level is 6dB SPL.
     */
    public double audioLevel;
    /** {zh}
     * @brief 总音量强度。值越大，强度越高。
     */
    /** {en}
     * @brief Total audio energy. Higher the value, bigger the total energy.
     */
    public double totalAudioEnergy;
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

/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 视频卡顿信息
 */
/** {en}
 * @type keytype
 * @brief Video stall info
 */
public class StallInfo {
    /** {zh}
     * @brief 500 毫秒卡顿次数
     */
    /** {en}
     * @brief Stall count in 500ms
     */
    public int stallCount500;
    /** {zh}
     * @brief 500 毫秒卡顿时长
     */
    /** {en}
     * @brief Stall duration in 500ms
     */
    public int stallDuration500;
    /** {zh}
     * @brief 200 毫秒卡顿次数
     */
    /** {en}
     * @brief Stall count in 200ms
     */
    public int stallCount200;
    /** {zh}
     * @brief 200 毫秒卡顿时长
     */
    /** {en}
     * @brief Stall duration in 200ms
     */
    public int stallDuration200;
    /** {zh}
     * @brief 帧率
     */
    /** {en}
     * @brief Frame rate
     */
    public int frameRate;
}

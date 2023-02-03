/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 发布参数
 */
/** {en}
 * @type keytype
 * @brief publish param
 */
public class PubExtraParam {
    /** {zh}
     * @brief 音频源发布
     */
    /** {en}
     * @brief Publish audio source
     */
    public RtcDefines.PubAudioSource pubAudioSource = RtcDefines.PubAudioSource.InternalRecord;
    /** {zh}
     * @brief 视频源发布
     */
    /** {en}
     * @brief Publish video source
     */
    public RtcDefines.PubVideoSource pubVideoSource = RtcDefines.PubVideoSource.InternalCamera;
}

/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;

/** {zh}
 * @type keytype
 * @brief 订阅额外参数
 */
/** {en}
 * @type keytype
 * @brief Extra param for subscribing
 */
public class SubExtraParam {
    /** {zh}
     * @brief 音频源订阅。参看 SubAudioSource{@link #SubAudioSource}。
     */
    /** {en}
     * @brief Subscribe audio source. See SubAudioSource{@link #SubAudioSource}.
     */
    public RtcDefines.SubAudioSource subAudioSource = RtcDefines.SubAudioSource.InternalPlay;
    /** {zh}
     * @brief 视频源订阅。参看 SubVideoSource{@link #SubVideoSource}。
     */
    /** {en}
     * @brief Subscribe video source. See SubVideoSource{@link #SubVideoSource}.
     */
    public RtcDefines.SubVideoSource subVideoSource = RtcDefines.SubVideoSource.InternalRender;
}

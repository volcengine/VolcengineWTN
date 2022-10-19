/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

package com.vewtn;
/** {zh}
 * @type keytype
 * @brief 视频配置
 */
/** {en}
 * @type keytype
 * @brief Video config
 */
public class VideoConfig {
   /** {zh}
    * @brief 宽度（px）。默认值是 640。
    */
   /** {en}
    * @brief Width in px. 640 by default.
    */
    public int width = 960;
   /** {zh}
    * @brief 高度（px）。默认值是 960。
    */
   /** {en}
    * @brief Height in px. 960 by default.
    */
    public int height = 640;
   /** {zh}
    * @brief 最大帧率（fps）。默认值是 15。
    */
   /** {en}
    * @brief Maximum fps. 15 by default.
    */
    public int maxFPS = 15;
}

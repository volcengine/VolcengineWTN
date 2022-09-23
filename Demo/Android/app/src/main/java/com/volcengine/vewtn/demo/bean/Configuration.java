/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.bean;

import com.volcengine.vewtn.demo.Constants;

/**
 * 保存服务器配置信息
 */
public class Configuration {

    /**
     * 服务器配置的推流地址
     */
    public final String pushUrl;
    /**
     * 服务器配置的推流限制分钟数
     */
    public final int maxPushDurationInMinutes;

    public Configuration() {
        this(Constants.PUSH_URL, 20);
    }

    public Configuration(String pushUrl, int maxPushDurationInMinutes) {
        this.pushUrl = pushUrl;
        this.maxPushDurationInMinutes = maxPushDurationInMinutes;
    }

    public String getPublishUrl() {
        return pushUrl;
    }

    public int getMaxPushDurationInMinutes() {
        return maxPushDurationInMinutes;
    }

    public long getMaxPushDurationInMillis() {
        return maxPushDurationInMinutes * 60 * 1000L;
    }
}

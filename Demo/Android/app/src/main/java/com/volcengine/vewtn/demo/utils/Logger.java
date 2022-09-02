/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.utils;

import android.util.Log;

public final class Logger {
    private static final String TAG = "veWTN";

    public static void d(String tag, String message) {
        Log.d(TAG, "{" + Thread.currentThread().getName() + "}[" + tag + "] " + message);
    }

    public static void d(String tag, String message, Throwable e) {
        Log.d(TAG, "{" + Thread.currentThread().getName() + "}[" + tag + "] " + message, e);
    }
}

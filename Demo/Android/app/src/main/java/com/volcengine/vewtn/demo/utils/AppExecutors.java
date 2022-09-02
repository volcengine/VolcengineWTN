/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.utils;

import android.os.Handler;
import android.os.Looper;

import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AppExecutors {
    private static AppExecutors sInstance;

    private final ExecutorService mNetworkIO;
    private final Executor mMainThread;

    private AppExecutors() {
        this.mNetworkIO = Executors.newFixedThreadPool(Runtime.getRuntime().availableProcessors());
        this.mMainThread = new MainThreadExecutor();
    }

    public static AppExecutors getInstance() {
        if (sInstance == null) {
            synchronized (AppExecutors.class) {
                sInstance = new AppExecutors();
            }
        }

        return sInstance;
    }

    public static ExecutorService networkIO() {
        return getInstance().mNetworkIO;
    }

    public static Executor mainThread() {
        return getInstance().mMainThread;
    }

    private static class MainThreadExecutor implements Executor {
        private final Handler mainThreadHandler;

        private MainThreadExecutor() {
            this.mainThreadHandler = new Handler(Looper.getMainLooper());
        }

        public void execute(Runnable command) {
            this.mainThreadHandler.post(command);
        }
    }
}

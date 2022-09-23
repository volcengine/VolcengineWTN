/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.ui;

import android.os.Handler;
import android.view.View;
import android.widget.TextView;

import androidx.annotation.DrawableRes;
import androidx.annotation.MainThread;
import androidx.annotation.StringRes;
import androidx.core.content.res.ResourcesCompat;

/**
 * 通过页面内的 {@link TextView} 来展现一个 Toast
 */
@MainThread
public class InnerToast {
    private static final long DURATION = 2000L;

    private final TextView view;
    private final Handler handler;

    private final Runnable hideCommand;

    public InnerToast(TextView view) {
        this.view = view;
        handler = new Handler();
        hideCommand = () -> view.setVisibility(View.GONE);
    }

    public void show(@StringRes int text) {
        show(ResourcesCompat.ID_NULL, text);
    }

    public void show(
            @DrawableRes int icon,
            @StringRes int text) {
        view.setCompoundDrawablesRelativeWithIntrinsicBounds(
                icon,
                ResourcesCompat.ID_NULL,
                ResourcesCompat.ID_NULL,
                ResourcesCompat.ID_NULL
        );
        view.setText(text);
        view.setVisibility(View.VISIBLE);
        handler.removeCallbacks(hideCommand);

        handler.postDelayed(hideCommand, DURATION);
    }
}

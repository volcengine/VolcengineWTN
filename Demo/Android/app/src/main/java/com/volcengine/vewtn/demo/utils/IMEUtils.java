/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.utils;

import android.content.Context;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

public final class IMEUtils {
    private IMEUtils() {
    }

    public static void closeIME(View v) {
        InputMethodManager mgr = (InputMethodManager) v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.hideSoftInputFromWindow(v.getWindowToken(), 0);
        v.clearFocus();
    }

    public static void openIME(final EditText v) {
        v.requestFocus();
        if (v.hasFocus()) {
            AppExecutors.mainThread().execute(() -> {
                InputMethodManager mgr = (InputMethodManager) v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                mgr.showSoftInput(v, InputMethodManager.SHOW_FORCED);
            });
        }
    }
}

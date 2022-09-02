/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.ui;

import android.content.DialogInterface;
import android.os.Bundle;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.core.content.res.ResourcesCompat;
import androidx.fragment.app.DialogFragment;

import com.volcengine.vewtn.demo.databinding.DialogSingleBinding;
import com.volcengine.vewtn.demo.databinding.DialogTwoBinding;

public class CustomDialog extends DialogFragment {
    public static final String EXTRA_BUTTON = "extra_button";

    private static final String EXTRA_STYLE = "extra_style";

    public static final int STYLE_SINGLE = 1;
    public static final int STYLE_TWO = 2;

    private static final String EXTRA_REQUEST_KEY = "extra_request_key";

    private static final String EXTRA_TITLE = "extra_title";
    private static final String EXTRA_TITLE_STR = "extra_title_str";
    private static final String EXTRA_MESSAGE = "extra_message";
    private static final String EXTRA_MESSAGE_STR = "extra_message_str";
    private static final String EXTRA_BUTTON_POSITIVE = "extra_button_positive";
    private static final String EXTRA_BUTTON_NEGATIVE = "extra_button_negative";

    public static CustomDialog create2TitleOnly(@NonNull String requestKey, @StringRes int title) {
        return create2(requestKey,
                title,
                ResourcesCompat.ID_NULL,
                android.R.string.ok,
                android.R.string.cancel);
    }

    public static CustomDialog create2TitleMessage(@NonNull String requestKey, @StringRes int title, @StringRes int message) {
        return create2(requestKey,
                title,
                message,
                android.R.string.ok,
                android.R.string.cancel);
    }

    public static CustomDialog create2(String requestKey,
                                       @StringRes int title,
                                       @StringRes int message,
                                       @StringRes int positive,
                                       @StringRes int negative) {
        Bundle args = new Bundle();
        args.putString(EXTRA_REQUEST_KEY, requestKey);

        args.putInt(EXTRA_STYLE, STYLE_TWO);
        args.putInt(EXTRA_TITLE, title);
        args.putInt(EXTRA_MESSAGE, message);
        args.putInt(EXTRA_BUTTON_POSITIVE, positive);
        args.putInt(EXTRA_BUTTON_NEGATIVE, negative);

        CustomDialog dialog = new CustomDialog();
        dialog.setArguments(args);
        return dialog;
    }

    public static CustomDialog create1TitleOnly(String requestKey, @Nullable String title, @StringRes int positive) {
        return create1(requestKey,
                title,
                ResourcesCompat.ID_NULL,
                positive);
    }

    public static CustomDialog create1(String requestKey, @Nullable String title, @StringRes int message, @StringRes int positive) {
        Bundle args = new Bundle();
        args.putString(EXTRA_REQUEST_KEY, requestKey);

        args.putInt(EXTRA_STYLE, STYLE_SINGLE);
        args.putString(EXTRA_TITLE_STR, title);
        args.putInt(EXTRA_MESSAGE, message);
        args.putInt(EXTRA_BUTTON_POSITIVE, positive);

        CustomDialog dialog = new CustomDialog();
        dialog.setArguments(args);
        return dialog;
    }

    public static CustomDialog create1(String requestKey, @StringRes int title, @Nullable String message, @StringRes int positive) {
        Bundle args = new Bundle();
        args.putString(EXTRA_REQUEST_KEY, requestKey);

        args.putInt(EXTRA_STYLE, STYLE_SINGLE);
        args.putInt(EXTRA_TITLE, title);
        args.putString(EXTRA_MESSAGE_STR, message);
        args.putInt(EXTRA_BUTTON_POSITIVE, positive);

        CustomDialog dialog = new CustomDialog();
        dialog.setArguments(args);
        return dialog;
    }

    public static CustomDialog create1(String requestKey, @StringRes int title, @StringRes int message, @StringRes int positive) {
        Bundle args = new Bundle();
        args.putString(EXTRA_REQUEST_KEY, requestKey);

        args.putInt(EXTRA_STYLE, STYLE_SINGLE);
        args.putInt(EXTRA_TITLE, title);
        args.putInt(EXTRA_MESSAGE, message);
        args.putInt(EXTRA_BUTTON_POSITIVE, positive);

        CustomDialog dialog = new CustomDialog();
        dialog.setArguments(args);
        return dialog;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        android.app.Dialog dialog = getDialog();
        if (dialog != null) {
            dialog.setCanceledOnTouchOutside(false);
            dialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
        }


        final Bundle arguments = requireArguments();
        final int style = arguments.getInt(EXTRA_STYLE, STYLE_SINGLE);
        if (style == STYLE_TWO) {
            DialogTwoBinding binding = DialogTwoBinding.inflate(inflater, container, false);
            render2(binding, savedInstanceState);
            return binding.getRoot();
        } else {
            DialogSingleBinding binding = DialogSingleBinding.inflate(inflater, container, false);
            render1(binding, savedInstanceState);
            return binding.getRoot();
        }
    }

    private void render1(@NonNull DialogSingleBinding binding, @Nullable Bundle savedInstanceState) {
        final Bundle arguments = requireArguments();
        @NonNull final String requestKey = arguments.getString(EXTRA_REQUEST_KEY, "");
        final int title = arguments.getInt(EXTRA_TITLE, ResourcesCompat.ID_NULL);
        if (title != ResourcesCompat.ID_NULL) {
            binding.title.setText(title);
        } else {
            String strTitle = arguments.getString(EXTRA_TITLE_STR);
            binding.title.setText(strTitle);
        }

        final int message = arguments.getInt(EXTRA_MESSAGE);
        if (message != ResourcesCompat.ID_NULL) {
            binding.message.setText(message);
        } else {
            String strMessage = arguments.getString(EXTRA_MESSAGE_STR);
            if (TextUtils.isEmpty(strMessage)) {
                binding.message.setVisibility(View.GONE);
            } else {
                binding.message.setText(strMessage);
            }
        }

        final int positive = arguments.getInt(EXTRA_BUTTON_POSITIVE);
        binding.button2.setText(positive);
        binding.button2.setOnClickListener(v -> {
            dismiss();
            if (!TextUtils.isEmpty(requestKey)) {
                final Bundle result = new Bundle();
                result.putInt(EXTRA_BUTTON, DialogInterface.BUTTON_NEGATIVE);
                getParentFragmentManager()
                        .setFragmentResult(requestKey, result);
            }
        });
    }

    private void render2(@NonNull DialogTwoBinding binding, @Nullable Bundle savedInstanceState) {
        final Bundle arguments = requireArguments();
        @NonNull final String requestKey = arguments.getString(EXTRA_REQUEST_KEY, "");
        final int title = arguments.getInt(EXTRA_TITLE);
        binding.title.setText(title);

        final int message = arguments.getInt(EXTRA_MESSAGE);
        if (message != ResourcesCompat.ID_NULL) {
            binding.message.setText(message);
        } else {
            binding.message.setVisibility(View.GONE);
        }

        final int negative = arguments.getInt(EXTRA_BUTTON_NEGATIVE);
        binding.button1.setText(negative);
        binding.button1.setOnClickListener(v -> {
            dismiss();
            if (!TextUtils.isEmpty(requestKey)) {
                final Bundle result = new Bundle();
                result.putInt(EXTRA_BUTTON, DialogInterface.BUTTON_NEGATIVE);
                getParentFragmentManager()
                        .setFragmentResult(requestKey, result);
            }
        });

        final int positive = arguments.getInt(EXTRA_BUTTON_POSITIVE);
        binding.button2.setText(positive);
        binding.button2.setOnClickListener(v -> {
            dismiss();
            if (!TextUtils.isEmpty(requestKey)) {
                final Bundle result = new Bundle();
                result.putInt(EXTRA_BUTTON, DialogInterface.BUTTON_POSITIVE);
                getParentFragmentManager()
                        .setFragmentResult(requestKey, result);
            }
        });
    }
}

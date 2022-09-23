/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.subscribe;

import static com.volcengine.vewtn.demo.ui.CustomDialog.EXTRA_BUTTON;

import android.Manifest;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.view.View;

import androidx.activity.OnBackPressedCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.DrawableRes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.core.content.PermissionChecker;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.Navigation;

import com.vewtn.SubscriberEventHandler;
import com.volcengine.vewtn.demo.Constants;
import com.volcengine.vewtn.demo.R;
import com.volcengine.vewtn.demo.databinding.FragmentSubscribeBinding;
import com.volcengine.vewtn.demo.ui.CustomDialog;
import com.volcengine.vewtn.demo.ui.InnerToast;
import com.volcengine.vewtn.demo.utils.IMEUtils;

public class SubscribeFragment extends Fragment {
    public static final String RK_CONFIRM_STOP_SUBSCRIBE = "confirm_stop_subscribe";

    private SubscribeViewModel viewModel;

    private boolean micPermission = false;

    @Nullable
    private InnerToast toast;

    public SubscribeFragment() {
        super(R.layout.fragment_subscribe);
    }

    @Nullable
    private View permissionDescription;

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);

        FragmentSubscribeBinding binding = FragmentSubscribeBinding.bind(view);

        // region 解析拉流地址
        binding.back.setOnClickListener(v -> Navigation.findNavController(v).popBackStack());

        binding.urlInput.addTextChangedListener(new TextWatcherAdapter() {
            @Override
            public void afterTextChanged(Editable s) {
                binding.startSubscribe.setEnabled(s.length() > 0);
            }
        });

        binding.urlInputPaste.setOnClickListener(v -> {
            final CharSequence text = getCopiedUrl();
            if (!TextUtils.isEmpty(text)) {
                binding.urlInput.setText(text);
            }
        });

        if (!TextUtils.isEmpty(Constants.PULL_URL)) {
            binding.urlInput.setText(Constants.PULL_URL);
            binding.startSubscribe.setEnabled(true);
        } else {
            binding.startSubscribe.setEnabled(false);
        }

        binding.startSubscribe.setOnClickListener(v -> {
            if (!micPermission) {
                showToast(R.drawable.ic_fail_cross, R.string.subscribe_need_permission_record_audio);
                return;
            }
            String text = binding.urlInput.getText().toString().trim();

            if (TextUtils.isEmpty(text)) {
                showToast(R.drawable.ic_fail_cross, R.string.please_input_correct_url);
                return;
            }

            String url = text;

            IMEUtils.closeIME(binding.urlInput);

            binding.resolveLinkProgress.setVisibility(View.VISIBLE);

            viewModel.startSubscribe(url);
        });
        // endregion

        binding.close.setOnClickListener(this::showCloseConfirmIfNeed);

        binding.resumeSubscribe.setOnClickListener(v -> viewModel.resumeSubscribe());

        toast = new InnerToast(binding.toast);

        viewModel = new ViewModelProvider(this).get(SubscribeViewModel.class);
        viewModel.initSubscribe(requireContext(), binding.preview);

        viewModel.viewMode.observe(getViewLifecycleOwner(), style -> {
            switch (style) {
                case SubscribeViewModel.MODE_SUBSCRIBE:
                    binding.layoutError.setVisibility(View.GONE);
                    binding.layoutResolveLink.setVisibility(View.GONE);

                    binding.layoutSubscribe.setVisibility(View.VISIBLE);
                    break;
                case SubscribeViewModel.MODE_ERROR:
                    binding.layoutSubscribe.setVisibility(View.GONE);
                    binding.layoutResolveLink.setVisibility(View.GONE);

                    binding.layoutError.setVisibility(View.VISIBLE);
                    break;
                default:
                    binding.layoutError.setVisibility(View.GONE);
                    binding.layoutSubscribe.setVisibility(View.GONE);

                    binding.layoutResolveLink.setVisibility(View.VISIBLE);
                    break;
            }
        });

        viewModel.events.observe(getViewLifecycleOwner(), event -> {
            if (event.state == SubscriberEventHandler.SubscribeState.StartSubscribeFailed) {
                viewModel.stopSubscribe();
                final Integer mode = viewModel.viewMode.getValue();
                if (mode == null || mode == SubscribeViewModel.MODE_INPUT) {
                    // 首次解析时，会显示一个 Loading，故此处停止 Loading 停留在输入页面
                    binding.resolveLinkProgress.setVisibility(View.GONE);
                    showToast(R.drawable.ic_fail_cross, R.string.subscribe_url_timeout);
                } else {
                    // 显示错误页面
                    viewModel.viewMode.postValue(SubscribeViewModel.MODE_ERROR);
                }
            } else if (event.state == SubscriberEventHandler.SubscribeState.StartSubscribeSuccess) {
                binding.resolveLinkProgress.setVisibility(View.GONE);

                viewModel.viewMode.postValue(SubscribeViewModel.MODE_SUBSCRIBE);
            }
        });

        setFragmentResultListeners();

        requireActivity().getOnBackPressedDispatcher().addCallback(getViewLifecycleOwner(), new OnBackPressedCallback(true) {
            @Override
            public void handleOnBackPressed() {
                showCloseConfirmIfNeed(requireView());
            }
        });

        if (PermissionChecker.checkSelfPermission(requireContext(), Manifest.permission.RECORD_AUDIO)
                != PermissionChecker.PERMISSION_GRANTED) {
            permissionDescription = binding.permissionDescription;
            binding.permissionDescription.setVisibility(View.VISIBLE);
        }
        // webrtc SDK 需要音频权限，否则会崩溃
        requestPermission.launch(Manifest.permission.RECORD_AUDIO);
    }

    /**
     * 获取剪贴板中复制的网址
     *
     * @return
     */
    @Nullable
    CharSequence getCopiedUrl() {
        Context context = requireContext();
        ClipboardManager clipboardManager = (ClipboardManager) context.getSystemService(Context.CLIPBOARD_SERVICE);
        ClipData primaryClip = clipboardManager.getPrimaryClip();
        int itemCount = primaryClip == null ? 0 : primaryClip.getItemCount();
        if (itemCount > 0) {
            ClipData.Item itemAt = primaryClip.getItemAt(0);
            return itemAt.getText();
        }

        return null;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        toast = null;
        permissionDescription = null;
    }

    void showToast(@DrawableRes int icon, @StringRes int text) {
        if (toast != null) {
            toast.show(icon, text);
        }
    }

    /**
     * 退出页面
     * 检查是否进行推流，如果正在推流就弹窗请求用户确认，否则直接退出页面¬
     *
     * @param view
     * @see #setFragmentResultListeners()
     */
    void showCloseConfirmIfNeed(View view) {
        if (viewModel.isSubscribing()) {
            CustomDialog.create2TitleOnly(RK_CONFIRM_STOP_SUBSCRIBE, R.string.confirm_stop_subscribe).show(getChildFragmentManager(), RK_CONFIRM_STOP_SUBSCRIBE);
        } else {
            Navigation.findNavController(view).popBackStack();
        }
    }

    /**
     * 设置对话框响应回调
     *
     * @see #showCloseConfirmIfNeed
     */
    private void setFragmentResultListeners() {
        FragmentManager childFragmentManager = getChildFragmentManager();
        childFragmentManager.setFragmentResultListener(RK_CONFIRM_STOP_SUBSCRIBE, getViewLifecycleOwner(), (requestKey, result) -> {
            int button = result.getInt(EXTRA_BUTTON, DialogInterface.BUTTON_NEGATIVE);
            if (button == DialogInterface.BUTTON_POSITIVE) {
                Navigation.findNavController(requireView()).popBackStack();
            }
        });
    }

    private final ActivityResultLauncher<String> requestPermission =
            registerForActivityResult(new ActivityResultContracts.RequestPermission(), result -> {
                if (permissionDescription != null) permissionDescription.setVisibility(View.GONE);
                micPermission = result != null && result;
            });

}

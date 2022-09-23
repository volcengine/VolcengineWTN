/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.publish;

import static com.volcengine.vewtn.demo.ui.CustomDialog.EXTRA_BUTTON;

import android.Manifest;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.text.TextUtils;
import android.view.View;

import androidx.activity.OnBackPressedCallback;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.DrawableRes;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.lifecycle.ViewModelProvider;
import androidx.navigation.Navigation;

import com.vewtn.PublisherEventHandler;
import com.volcengine.vewtn.demo.R;
import com.volcengine.vewtn.demo.bean.Configuration;
import com.volcengine.vewtn.demo.databinding.FragmentPublishBinding;
import com.volcengine.vewtn.demo.ui.CustomDialog;
import com.volcengine.vewtn.demo.ui.InnerToast;
import com.volcengine.vewtn.demo.utils.Logger;


public class PublishFragment extends Fragment {
    private static final String TAG = "Publisher";

    public static final String RK_CONFIRM_STOP_PUBLISH = "confirm_stop_publish";
    public static final String RK_RESTRICT_TIME = "confirm_restrict_time";
    public static final String RK_RECORD_AUDIO_RATIONALE = "request_record_audio_rationale";

    private final Handler handler = new Handler(Looper.getMainLooper());

    private PublishViewModel viewModel;

    @Nullable
    private InnerToast toast;

    public PublishFragment() {
        super(R.layout.fragment_publish);
    }

    private final Configuration config = new Configuration();

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        final FragmentPublishBinding binding = FragmentPublishBinding.bind(view);
        final Context context = requireContext();

        toast = new InnerToast(binding.toast);

        // region Top Bar
        binding.switchCamera.setOnClickListener(v -> viewModel.switchCamera());
        binding.close.setOnClickListener(this::showCloseConfirmIfNeed);
        binding.userId.setText(getString(R.string.id_prefix, "veWTN"));
        // endregion

        // region Bottom Bar
        binding.microphone.setOnClickListener(v -> viewModel.toggleMic());
        binding.camera.setOnClickListener(v -> viewModel.toggleMicCamera());
        binding.startPublish.setOnClickListener(v -> {
            if (!viewModel.micPermission) {
                showToast(R.drawable.ic_fail_cross, R.string.publish_need_permission_record_audio);
                return;
            }
            String url = config.getPublishUrl();
            if (TextUtils.isEmpty(url)) {
                showToast(R.drawable.ic_fail_cross, R.string.publish_failed_common);
            } else {
                Logger.d(TAG, "Start Publish");
                viewModel.startPublish(url);
                v.setVisibility(View.GONE);

                startCountDown();
            }
        });
        // endregion

        // region ViewModel & Observers
        viewModel = new ViewModelProvider(this).get(PublishViewModel.class);
        viewModel.initPublisher(context, binding.preview);

        viewModel.micStatus.observe(getViewLifecycleOwner(), binding.microphone::setSelected);
        viewModel.cameraStatus.observe(getViewLifecycleOwner(), binding.camera::setSelected);

        viewModel.events.observe(getViewLifecycleOwner(), publishEvent -> {
            if (publishEvent.state == PublisherEventHandler.PublishState.StartPublishSuccess) {
                binding.preview.setKeepScreenOn(true);
            } else {
                binding.preview.setKeepScreenOn(false);

                viewModel.stopPublish();

                if (publishEvent.reason == PublisherEventHandler.Reason.TokenError) {
                    // Token 过期
                }

                showToast(R.drawable.ic_fail_cross, R.string.publish_failed_common);

                binding.startPublish.setVisibility(View.VISIBLE);
            }
        });

        viewModel.isPublishSuccess.observe(getViewLifecycleOwner(), isSuccess -> {
            if (isSuccess == Boolean.TRUE) {
                binding.startPublish.setVisibility(View.GONE);
            } else {
                binding.startPublish.setVisibility(View.VISIBLE);
            }
        });
        // endregion

        // 申请权限
        requestPermissions.launch(new String[]{
                Manifest.permission.CAMERA,
                Manifest.permission.RECORD_AUDIO,
        });

        setFragmentResultListeners();

        requireActivity()
                .getOnBackPressedDispatcher()
                .addCallback(getViewLifecycleOwner(), new OnBackPressedCallback(true) {
                    @Override
                    public void handleOnBackPressed() {
                        showCloseConfirmIfNeed(requireView());
                    }
                });
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        toast = null;
        handler.removeCallbacks(restrictPublishTimeCommand);
    }

    /**
     * 限制 Demo 的体验时间
     */
    private void startCountDown() {
        final long duration = config.getMaxPushDurationInMillis();
        long uptimeMillis = viewModel.publishStartTime + duration;
        handler.postAtTime(restrictPublishTimeCommand, uptimeMillis);
    }

    private final Runnable restrictPublishTimeCommand = new Runnable() {
        @Override
        public void run() {
            viewModel.stopPublish();
            if (isAdded()) {
                final int maxPushDurationInMinutes = config.getMaxPushDurationInMinutes();
                CustomDialog.create1TitleOnly(RK_RESTRICT_TIME,
                                getString(R.string.confirm_stop_publish_time_limit, maxPushDurationInMinutes),
                                R.string.i_know)
                        .show(getChildFragmentManager(), RK_RESTRICT_TIME);
            }
        }
    };

    /**
     * 退出页面
     * 检查是否进行推流，如果正在推流就弹窗请求用户确认，否则直接退出页面¬
     *
     * @param view
     * @see #setFragmentResultListeners()
     */
    void showCloseConfirmIfNeed(View view) {
        if (viewModel.isPublishing()) {
            CustomDialog.create2TitleOnly(RK_CONFIRM_STOP_PUBLISH,
                            R.string.confirm_stop_publish)
                    .show(getChildFragmentManager(), RK_CONFIRM_STOP_PUBLISH);
        } else {
            Navigation.findNavController(view).popBackStack();
        }
    }

    void showToast(@DrawableRes int icon, @StringRes int text) {
        if (toast != null) {
            toast.show(icon, text);
        }
    }

    /**
     * 设置对话框响应回调
     *
     * @see #showCloseConfirmIfNeed
     * @see #restrictPublishTimeCommand
     */
    private void setFragmentResultListeners() {
        FragmentManager childFragmentManager = getChildFragmentManager();
        childFragmentManager.setFragmentResultListener(RK_CONFIRM_STOP_PUBLISH,
                getViewLifecycleOwner(),
                (requestKey, result) -> {
                    int button = result.getInt(EXTRA_BUTTON, DialogInterface.BUTTON_NEGATIVE);
                    if (button == DialogInterface.BUTTON_POSITIVE) {
                        Navigation.findNavController(requireView())
                                .popBackStack();
                    }
                });

        childFragmentManager.setFragmentResultListener(RK_RESTRICT_TIME,
                getViewLifecycleOwner(),
                (requestKey, result) -> Navigation.findNavController(requireView())
                        .popBackStack());

        childFragmentManager.setFragmentResultListener(RK_RECORD_AUDIO_RATIONALE,
                getViewLifecycleOwner(),
                (requestKey, result) -> {
                    int button = result.getInt(EXTRA_BUTTON, DialogInterface.BUTTON_NEGATIVE);
                    if (button == DialogInterface.BUTTON_POSITIVE) {
                        requestPermissions.launch(new String[]{
                                Manifest.permission.CAMERA,
                                Manifest.permission.RECORD_AUDIO
                        });
                    }
                });
    }

    private final ActivityResultLauncher<String[]> requestPermissions = registerForActivityResult(
            new ActivityResultContracts.RequestMultiplePermissions(),
            result -> {
                viewModel.updatePermission(
                        result.get(Manifest.permission.CAMERA) == Boolean.TRUE,
                        result.get(Manifest.permission.RECORD_AUDIO) == Boolean.TRUE);

                if (result.get(Manifest.permission.RECORD_AUDIO) != Boolean.TRUE
                        && shouldShowRequestPermissionRationale(Manifest.permission.RECORD_AUDIO)) {
                    showRequestPermissionRationale();
                }
            }
    );

    /**
     * 向用户解释说明需要麦克风权限
     */
    void showRequestPermissionRationale() {
        Logger.d(TAG, "showRequestPermissionRationale");
        CustomDialog.create2TitleMessage(RK_RECORD_AUDIO_RATIONALE,
                        R.string.record_audio_permission_description_title,
                        R.string.record_audio_permission_description_content)
                .show(getChildFragmentManager(), RK_RECORD_AUDIO_RATIONALE);
    }
}

/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.publish;

import android.content.Context;
import android.os.SystemClock;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.vewtn.Publisher;
import com.vewtn.PublisherEventHandler;
import com.volcengine.vewtn.demo.utils.Logger;

public class PublishViewModel extends ViewModel {
    private static final String TAG = "PushViewModel";

    final MutableLiveData<Boolean> micStatus = new MutableLiveData<>(true);
    final MutableLiveData<Boolean> cameraStatus = new MutableLiveData<>(true);

    /**
     * 记录开始推送的时间，限制 Demo 体验时间使用
     */
    long publishStartTime = 0;

    /**
     * 是否发布成功
     * 基于目前的交互逻辑；已经设置的情况下，不会主动设置为 False
     */
    final MutableLiveData<Boolean> isPublishSuccess = new MutableLiveData<>(false);

    final MutableLiveData<PublishEvent> events = new MutableLiveData<>();

    @Nullable
    private Publisher publisher;

    public boolean isPublishing() {
        return publisher != null && publisher.isPublishing();
    }

    boolean micPermission = false;
    boolean cameraPermission = false;

    public void toggleMic() {
        if (!micPermission) {
            return;
        }
        if (micStatus.getValue() == Boolean.TRUE) {
            // Change to False
            micStatus.postValue(false);

            if (publisher != null) publisher.stopAudioCapture();
        } else {
            // Change to True
            micStatus.postValue(true);

            if (publisher != null) publisher.startAudioCapture();
        }
    }

    public void toggleMicCamera() {
        if (!cameraPermission) {
            return;
        }
        if (cameraStatus.getValue() == Boolean.TRUE) {
            // Change to False
            cameraStatus.postValue(false);

            if (publisher != null) publisher.stopVideoCapture();
        } else {
            // Change to True
            cameraStatus.postValue(true);

            if (publisher != null) publisher.startVideoCapture();
        }
    }

    public void updatePermission(boolean cameraPermission, boolean micPermission) {
        this.cameraPermission = cameraPermission;
        this.micPermission = micPermission;

        Logger.d(TAG, "updatePermission: camera=" + cameraPermission + "; micPermission=" + micPermission);
        if (cameraPermission) {
            if (cameraStatus.getValue() == Boolean.TRUE && publisher != null) {
                Logger.d(TAG, "updatePermission: startVideoCapture");
                publisher.startVideoCapture();
            }
        } else {
            cameraStatus.postValue(false);
        }

        if (micPermission) {
            if (micStatus.getValue() == Boolean.TRUE && publisher != null) {
                publisher.startAudioCapture();
            }
        } else {
            micStatus.postValue(false);
        }
    }

    public void initPublisher(Context context, View remoteView) {
        if (publisher == null) {
            publisher = Publisher.create(context, publisherEventHandler);
        }
        publisher.setLocalView(remoteView);
    }

    public void startPublish(String publishUrl) {
        if (publisher != null && !publisher.isPublishing()) {
            publisher.startPublish(publishUrl);

            publishStartTime = SystemClock.uptimeMillis();
        }
    }

    public void switchCamera() {
        if (publisher != null) {
            publisher.switchCamera();
        }
    }

    public void stopPublish() {
        if (publisher != null) {
            publisher.stopPublish();
        }
    }

    @Override
    protected void onCleared() {
        super.onCleared();
        stopPublish();

        if (publisher != null) {
            publisher.destroy();
        }
    }

    private final PublisherEventHandler publisherEventHandler = new PublisherEventHandler() {
        @Override
        public void onPublishStateChanged(PublishState state, Reason reason) {
            Logger.d(TAG, "onPublishStateChanged: state=" + state + "; reason=" + reason);
            events.postValue(new PublishEvent(state, reason));
            if (state == PublishState.StartPublishSuccess) {
                isPublishSuccess.postValue(true);
            }
        }
    };
}

/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo.subscribe;

import android.content.Context;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.lifecycle.MutableLiveData;
import androidx.lifecycle.ViewModel;

import com.vewtn.Subscriber;
import com.vewtn.SubscriberEventHandler;
import com.volcengine.vewtn.demo.utils.Logger;

public class SubscribeViewModel extends ViewModel {
    private static final String TAG = "SubscriberVM";

    /**
     * 输入 URL 模式
     */
    public static final int MODE_INPUT = 0;
    /**
     * 订阅成功，显示远端视频流页面
     */
    public static final int MODE_SUBSCRIBE = 2;
    /**
     * 订阅错误，显示重试页面
     */
    public static final int MODE_ERROR = 4;

    final MutableLiveData<Integer> viewMode = new MutableLiveData<>(MODE_INPUT);

    final MutableLiveData<SubscribeEvent> events = new MutableLiveData<>();

    @Nullable
    private Subscriber subscriber;

    @Nullable
    private String subscribeUrl;

    public void initSubscribe(Context context, View remoteView) {
        subscriber = Subscriber.create(context, eventHandler);
        subscriber.setRemoteView(remoteView);
    }

    public void startSubscribe(String url) {
        subscribeUrl = url;
        if (subscriber != null) {
            subscriber.startSubscribe(url);
        }
    }

    public void resumeSubscribe() {
        if (subscriber != null && subscribeUrl != null) {
            subscriber.startSubscribe(subscribeUrl);
        }
    }

    @Override
    protected void onCleared() {
        super.onCleared();
        if (subscriber != null) {
            subscriber.stopSubscribe();
            subscriber.destroy();
        }
    }

    final SubscriberEventHandler eventHandler = new SubscriberEventHandler() {

        @Override
        public void onSubscribeStateChanged(SubscribeState state, Reason reason) {
            Logger.d(TAG, "onSubscribeStateChanged: state=" + state + "; reason=" + reason);
            events.postValue(new SubscribeEvent(state, reason));
        }
    };

    public boolean isSubscribing() {
        return subscriber != null && subscriber.isSubscribed();
    }

    /**
     * 停止拉流，不在接受后续的消息
     */
    public void stopSubscribe() {
        if (subscriber != null) {
            subscriber.stopSubscribe();
        }
    }
}

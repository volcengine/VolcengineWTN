/*
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */
package com.volcengine.vewtn.demo;

import android.os.Bundle;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;
import androidx.navigation.Navigation;

import com.volcengine.vewtn.demo.databinding.FragmentMainBinding;

public class MainFragment extends Fragment {

    public MainFragment() {
        super(R.layout.fragment_main);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        FragmentMainBinding binding = FragmentMainBinding.bind(view);

        // region "主页" 页面响应事件
        binding.tabMain.publish.setOnClickListener(v ->
                Navigation.findNavController(v)
                        .navigate(R.id.open_publish));
        binding.tabMain.subscribe.setOnClickListener(v ->
                Navigation.findNavController(v)
                        .navigate(R.id.open_subscribe));
        // endregion
    }
}

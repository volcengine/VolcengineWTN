/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { createSlice } from '@reduxjs/toolkit';
import { DEFAULTCONFIG } from '@/config';

const initialState: MediaStreamConstraints = {
  audio: true,
  video: {
    ...DEFAULTCONFIG.resolution,
    frameRate: DEFAULTCONFIG.frameRate,
  },
};

export const streamSlice = createSlice({
  name: 'stream',
  initialState,
  reducers: {
    updateAudio: (state, { payload }) => {
      state.audio = payload;
    },

    updateVideo: (state, { payload }) => {
      state.video = {
        ...(state.video as MediaTrackConstraints),
        ...(payload as MediaTrackConstraints),
      };
    },
  },
});
export const { updateAudio, updateVideo } = streamSlice.actions;

export default streamSlice.reducer;

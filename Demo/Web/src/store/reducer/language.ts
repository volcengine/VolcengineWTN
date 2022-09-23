/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { createSlice } from '@reduxjs/toolkit';

export enum LanguageState {
  CN = 'cn',
  EN = 'en',
}

export const languageSlice = createSlice({
  name: 'room',
  initialState: LanguageState.CN,
  reducers: {
    selectLanguage: (state, { payload }) => {
      return payload;
    },
  },
});
export const { selectLanguage } = languageSlice.actions;

export default languageSlice.reducer;

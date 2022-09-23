/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { configureStore } from '@reduxjs/toolkit';
import languageReducer from './reducer/language';
import constraintsReducer from './reducer/constraints';

const store = configureStore({
  reducer: {
    language: languageReducer,
    constraints: constraintsReducer,
  },
});

export default store;

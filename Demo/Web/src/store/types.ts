/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import type { LanguageState } from './reducer/language';

export type RootState = {
  language: LanguageState;
  constraints: MediaStreamConstraints;
};

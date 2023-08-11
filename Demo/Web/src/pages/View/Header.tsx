/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { Select } from 'antd';
import { useSelector, useDispatch } from 'react-redux';
import { useTranslation } from 'react-i18next';
import i18n from 'i18next';
import styles from './index.module.less';
import { RootState } from '@/store/types';

import { DEMO_VERSION } from '@/config';
import { selectLanguage } from '@/store/reducer/language';

function Push() {
  const language = useSelector((state: RootState) => state.language);
  const { t } = useTranslation();
  const dispatch = useDispatch();

  const handleLanguageChange = (value: RootState['language']) => {
    dispatch(selectLanguage(value));
    i18n.changeLanguage(value);
  };

  return (
    <header className={styles.header}>
      <div>
        <span className="version">
          {t('DemoVersion')}: {DEMO_VERSION}
        </span>
        <Select value={language} onChange={handleLanguageChange}>
          <Select.Option value="cn">中文</Select.Option>
          <Select.Option value="en">English</Select.Option>
        </Select>
      </div>
    </header>
  );
}

export default Push;

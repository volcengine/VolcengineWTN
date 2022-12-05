/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import styles from './index.module.less';
import Copy from 'react-copy-to-clipboard';
import { message } from 'antd';
import { useTranslation } from 'react-i18next';
import React from 'react';

import linkImg from '/assets/images/link2x.png';

interface ViewBtnProps {
  shareUrl: string;
}

function ViewCopyBtn(props: ViewBtnProps) {
  const { shareUrl } = props;
  const { t } = useTranslation();
  return (
    <div className={styles['view-btn']}>
      <Copy
        onCopy={() => {
          message.success({
            className: styles.message,
            content: (
              <div className="content-wrapper">
                <span className="title">{t('copyTitle')}</span>
                <div className="content">{t('copyContent')}</div>
              </div>
            ),
          });
        }}
        text={shareUrl}
      >
        <div className={styles['view-link']}>
          <img src={linkImg} alt="link" />
        </div>
      </Copy>

      <span className={styles['view-line']} />
    </div>
  );
}

export default React.memo(ViewCopyBtn);

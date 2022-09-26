/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { useState } from 'react';

import settingPubImg from '/assets/images/settingPub2x.png';
import settingUnPubImg from '/assets/images/settingUnPub2x.png';

import { Modal, Select } from 'antd';
import { useTranslation } from 'react-i18next';
import styles from './index.module.less';

import { DEFAULTCONFIG, Frame, RESOLUTIOIN_LIST } from '@/config';

interface Props {
  published: boolean;
  onSettingChange: (videoConstraints: MediaTrackConstraints) => void;
}

/**
 * 设置按钮icon
 * @param props
 * @returns
 */
const getIcon = (
  props: Props
): {
  src: string;
  alt: string;
} => {
  const { published } = props;

  if (published) {
    return {
      src: settingPubImg,
      alt: 'setting',
    };
  }
  return {
    src: settingUnPubImg,
    alt: 'setting',
  };
};

function SettingButton(props: Props) {
  const { published, onSettingChange } = props;
  const { t } = useTranslation();
  const imgIcon = getIcon(props);
  const [visible, setVisible] = useState(false);

  const [resolution, setResolution] = useState(DEFAULTCONFIG.resolutionsText);
  const [frameRate, setFrameRate] = useState(DEFAULTCONFIG.frameRate);

  const handleOk = () => {
    setVisible(false);

    const resolutions = RESOLUTIOIN_LIST.find((r) => r.text === resolution);

    onSettingChange({ ...resolutions?.val, frameRate });
  };

  return (
    <>
      <div
        className={styles['setting-button']}
        onClick={() => {
          setVisible(true);
        }}
        data-published={published}
      >
        <img {...imgIcon} />
      </div>

      <Modal
        title={<div style={{ textAlign: 'center' }}>{t('setting')}</div>}
        visible={visible}
        cancelText={t('cancelText')}
        okText={t('okText')}
        cancelButtonProps={{
          style: {
            background: '#F2F3F8',
            borderRadius: 2,
            border: 'none',
          },
        }}
        okButtonProps={{
          style: {
            backgroundColor: '#165dff',
            borderRadius: 2,
            border: 'none',
          },
        }}
        onOk={handleOk}
        onCancel={() => {
          setVisible(false);
        }}
        transitionName=""
        maskTransitionName=""
      >
        <div className={styles['setting-content']}>
          <div className="config">
            <label>{t('Resolution')}</label>
            <Select value={resolution} onChange={setResolution}>
              {RESOLUTIOIN_LIST.map((f) => {
                return (
                  <Select.Option value={f.text} key={f.text}>
                    {f.text}
                  </Select.Option>
                );
              })}
            </Select>
          </div>
          <div className="config">
            <label>{t('frameRate')}</label>
            <Select value={frameRate} onChange={setFrameRate}>
              {Frame.map((f) => {
                return (
                  <Select.Option value={f} key={f}>
                    {f}
                  </Select.Option>
                );
              })}
            </Select>
          </div>
        </div>
      </Modal>
    </>
  );
}

export default SettingButton;

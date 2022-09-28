/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { Button, message } from 'antd';
import { useEffect, useRef, useState } from 'react';
import { PoweroffOutlined, ReloadOutlined } from '@ant-design/icons';
import { useTranslation } from 'react-i18next';
import Header from './Header';
import styles from './index.module.less';

import rtcClient from '@/lib/RtcClient';

function View() {
  const player = useRef<HTMLVideoElement>(null);
  const [stopping, setStopping] = useState(false);

  const [pulling, setPulling] = useState(true);
  const { t } = useTranslation();

  const [pullUrl, setPullUrl] = useState('');

  useEffect(() => {
    const urlObj = new URL(location.href);
    const appid = urlObj.searchParams.get('appid');
    const streamid = urlObj.searchParams.get('streamid');
    const domain = urlObj.searchParams.get('domain');
    const token = urlObj.searchParams.get('token');

    // Your Pull URL
    const url = `https://${domain}/pull/${appid}/${streamid}`;
    setPullUrl(url);
  }, []);

  const startSubscribe = async () => {
    try {
      const stream = await rtcClient.subscribe(pullUrl);
      if (player.current) {
        player.current.onloadeddata = () => {
          player.current?.play();
        };
        player.current.srcObject = stream;
      }
    } catch (e) {
      message.error(t('noStream'));
    }
  };

  useEffect(() => {
    if (pullUrl && player.current) {
      startSubscribe();
    }
  }, [pullUrl, player]);

  const stopSubscribe = async () => {
    return await rtcClient.stopSubscribe();
  };

  const handlePull = async () => {
    setPulling(!pulling);
    if (pulling) {
      setStopping(true);
      const res = await stopSubscribe();
      setStopping(false);
    } else {
      startSubscribe();
    }
  };

  return (
    <div className={styles.container}>
      <video
        id="local-stream"
        ref={player}
        controls
        autoPlay
        muted
        className={styles['stream-player']}
      />
      <Header />
      <div className={styles.pull}>
        <Button
          type="primary"
          danger={pulling}
          onClick={handlePull}
          loading={stopping}
          disabled={stopping}
        >
          {pulling ? (
            <>
              <PoweroffOutlined />
              {t('stopPull')}
            </>
          ) : (
            <>
              <ReloadOutlined />
              {t('pull')}
            </>
          )}
        </Button>
      </div>
    </div>
  );
}

export default View;

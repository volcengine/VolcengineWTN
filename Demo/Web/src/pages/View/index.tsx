/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { Button, message, Modal } from 'antd';
import { useEffect, useRef, useState } from 'react';
import { PoweroffOutlined, ReloadOutlined } from '@ant-design/icons';
import { useTranslation } from 'react-i18next';
import rtcClient from '@/lib/RtcClient';
import { ERRORTYPE, PEEREVENT } from '@/lib/interface';
import { subUrl } from '@/config';

import Header from './Header';
import styles from './index.module.less';

function View() {
  const player = useRef<HTMLVideoElement>(null);
  const [stopping, setStopping] = useState(false);

  const [pulling, setPulling] = useState(true);
  const { t } = useTranslation();

  const startSubscribe = async () => {
    try {
      const stream = await rtcClient.subscribe(subUrl);
      if (player.current) {
        player.current.onloadeddata = () => {
          player.current?.play();
        };
        player.current.srcObject = stream;
      }
      setPulling(true);
    } catch (e) {
      //   message.error(t('noStream'));
      console.error('startSubscribe', e);
      message.error(t('pullHttpError'));
    }
  };

  useEffect(() => {
    const handelDisconnect = (err: ERRORTYPE) => {
      setPulling(false);
      if (err === ERRORTYPE.MEDIA) {
        message.error(t('pullMediaError'));
      } else if (err === ERRORTYPE.PEER) {
        Modal.confirm({
          content: t('pullP2pError'),
          okText: t('retry'),
          cancelText: t('close'),
          onOk: () => {
            rtcClient.beforeReSubscribe();
            startSubscribe();
          },
        });
      }
    };

    rtcClient.on(PEEREVENT.Disconnect, handelDisconnect);

    return () => {
      rtcClient.removeListener(PEEREVENT.Disconnect, handelDisconnect);
    };
  }, [setPulling]);

  useEffect(() => {
    if (player.current) {
      startSubscribe();
    }
  }, [player]);

  const handlePull = async () => {
    setStopping(true);
    try {
      if (pulling) {
        const res = (await rtcClient.stopSubscribe()) as {
          code: number;
          status: number;
        };

        if (res.code === ERRORTYPE.SUCCESS) {
          setPulling(!pulling);
        }

        if (res.code !== ERRORTYPE.SUCCESS) {
          console.error('停止订阅失败', res);
        }

        if (res.code === ERRORTYPE.HTTP) {
          message.error(t('stopPullError'));
        }
      } else {
        await startSubscribe();
      }
    } finally {
      setStopping(false);
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

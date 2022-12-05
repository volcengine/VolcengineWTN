/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { useEffect, useRef, useState } from 'react';
import { useTranslation } from 'react-i18next';
import { message, Modal } from 'antd';
import rtcClient from '@/lib/RtcClient';
import { DEFAULTCONFIG, isDev } from '@/config';
import { ERRORTYPE, PEEREVENT } from '@/lib/interface';

import Header from './Header';
import StreamButtons from './StreamButtons';
import styles from './index.module.less';

function Push() {
  const { t } = useTranslation();
  const [published, setPublished] = useState(false);
  const [publishing, setPublishing] = useState(false);

  const [pushAndPullUrl, setPushAndPullUrl] = useState({
    pushUrl: '',
    pullUrl: '',
  });
  const player = useRef<HTMLVideoElement>(null);

  useEffect(() => {
    const urlObj = new URL(location.href);
    const appid = urlObj.searchParams.get('appid');
    const streamid = urlObj.searchParams.get('streamid');
    const domain = urlObj.searchParams.get('domain');
    const token = urlObj.searchParams.get('token');

    // Your Push URL and Pull URL
    const pushUrl = `https://${domain}/push/${appid}/${streamid}?TokenType=Bearer&Token=${token}`;
    const pullUrl = isDev
      ? `http://${location.host}/view?appid=${appid}&streamid=${streamid}&domain=${domain}`
      : `https://${location.host}/wtn/streamingwithurl/view?appid=${appid}&streamid=${streamid}&domain=${domain}`;
    setPushAndPullUrl({
      pushUrl,
      pullUrl,
    });
  }, []);

  const handlePublish = async (published: boolean) => {
    if (publishing) {
      return;
    }
    setPublishing(true);

    if (published) {
      const res = await rtcClient.publish(pushAndPullUrl.pushUrl);

      if (res.code === ERRORTYPE.SUCCESS) {
        setPublished(published);
        message.success({
          className: styles.message,
          content: <span className="title">{t('pushSuccMsg')}</span>,
        });
      } else if (res.status === 401 || res.status === 403) {
        message.error(t('pushTokenExpired'));
      } else {
        message.error(t('pushHttpError'));
      }
    } else {
      const res = (await rtcClient.unPublish()) as {
        code: number;
        status: number;
      };

      if (res.code === ERRORTYPE.SUCCESS) {
        setPublished(published);
      } else {
        message.error(t('stopPushError'));
        console.error(`停止推流失败`, res);
      }
    }

    setPublishing(false);
  };

  useEffect(() => {
    const handelDisconnect = (err: ERRORTYPE) => {
      setPublished(false);
      if (err === ERRORTYPE.MEDIA) {
        message.error(t('pushMediaError'));
        console.error('推流失败，原因：媒体建连异常');
      } else if (err === ERRORTYPE.PEER) {
        Modal.confirm({
          content: t('pushP2pError'),
          okText: t('retry'),
          cancelText: t('关闭'),
          onOk: () => {
            rtcClient.beforeRePublish();
            handlePublish(true);
          },
          onCancel: () => {
            rtcClient.beforeRePublish();
          },
        });
      }
    };

    rtcClient.on(PEEREVENT.Disconnect, handelDisconnect);

    return () => {
      rtcClient.removeListener(PEEREVENT.Disconnect, handelDisconnect);
    };
  }, [setPublished]);

  useEffect(() => {
    const play = async () => {
      // 1. 设置视频参数
      await rtcClient.setVideoCaptureConfig({
        ...DEFAULTCONFIG.resolution,
        frameRate: DEFAULTCONFIG.frameRate,
      });
      // 2. 创建本地流
      await rtcClient.startAudioCapture();
      await rtcClient.startVideoCapture();
      // 3. 播放本地流
      rtcClient.setLocalVideoPlayer('local-stream');
    };
    play();
  }, []);

  return (
    <div className={styles.container}>
      <video id="local-stream" muted ref={player} className={styles['stream-player']} />
      <Header />
      <StreamButtons
        shareUrl={pushAndPullUrl.pullUrl}
        onPublish={handlePublish}
        // onMute={handleMute}
        published={published}
      />
    </div>
  );
}

export default Push;

/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { useEffect, useRef, useState } from 'react';
import { useTranslation } from 'react-i18next';
import Header from './Header';
import StreamButtons from './StreamButtons';
import styles from './index.module.less';
import rtcClient from '@/lib/RtcClient';
import { DEFAULTCONFIG } from '@/config';

function Push() {
  const { t } = useTranslation();
  const [published, setPublished] = useState(false);
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
    const pullUrl = `https://${location.host}/wtn/streamingwithurl/view?appid=${appid}&streamid=${streamid}&domain=${domain}`;
    setPushAndPullUrl({
      pushUrl,
      pullUrl,
    });
  }, []);

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

  // 推流/结束推流
  const handlePublish = (published: boolean) => {
    setPublished(published);

    if (published) {
      rtcClient.publish(pushAndPullUrl.pushUrl);
    } else {
      rtcClient.unPublish();
    }
  };

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

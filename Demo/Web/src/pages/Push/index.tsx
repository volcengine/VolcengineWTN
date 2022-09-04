/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { useEffect, useRef, useState } from 'react';
import { useSelector } from 'react-redux';
import { message } from 'antd';
import { useTranslation } from 'react-i18next';
import Header from './Header';
import StreamButtons from './StreamButtons';
import styles from './index.module.less';
import { RootState } from '@/store/types';
import rtcClient from '@/lib/RtcClient';

function Push() {
  const { t } = useTranslation();
  const constraints = useSelector((state: RootState) => state.constraints);
  const [published, setPublished] = useState(false);
  const [pushAndPullUrl, setPushAndPullUrl] = useState({
    pushUrl: '',
    pullUrl: '',
  });

  const player = useRef<HTMLVideoElement>(null);
  const [audioMute, setAudioMute] = useState(false);
  const [videoMute, setVideoMute] = useState(false);

  const streamRef = useRef<MediaStream | null>(null);

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

  const play = (options: MediaStreamConstraints) => {
    return rtcClient
      .getStream(options)
      .then((stream) => {
        streamRef.current = stream;

        if (audioMute) {
          streamRef.current.getAudioTracks().forEach((track: MediaStreamTrack) => {
            track.enabled = false;
          });
        }
        if (videoMute) {
          streamRef.current.getVideoTracks().forEach((track: MediaStreamTrack) => {
            track.enabled = false;
          });
        }

        if (player.current) {
          player.current.onloadeddata = () => {
            player.current?.play();
          };
          player.current.srcObject = stream;
        }

        return true;
      })
      .catch((error) => {
        message.error(t('getDeviceFailed'));
        return false;
      });
  };

  const handlePublish = async (published: boolean) => {
    // 开始推流
    if (published) {
      const tracks: MediaStreamTrack[] = [];
      streamRef.current?.getTracks().forEach((track: MediaStreamTrack) => {
        tracks.push(track);
      });
      try {
        const res = await rtcClient.publish(pushAndPullUrl.pushUrl, tracks);
        if (res?.status === 200) {
          setPublished(published);
          message.success({
            className: styles.message,
            content: <span className="title">{t('pushSuccMsg')}</span>,
          });
        }
      } catch (err) {
        message.error({
          className: styles.message,
          content: <span className="title">{t('pushFailMsg')}</span>,
        });
      }
    } else {
      rtcClient.stopPublish();
      setPublished(published);
    }
  };

  useEffect(() => {
    play(constraints);
  }, []);

  useEffect(() => {
    const updateStream = async () => {
      if (streamRef.current) {
        streamRef.current.getTracks().forEach((track: MediaStreamTrack) => {
          track.stop();
        });

        await play(constraints);
        const tracks: MediaStreamTrack[] = [];
        streamRef.current.getTracks().forEach((track: MediaStreamTrack) => {
          tracks.push(track);
        });

        rtcClient.updateStream(tracks);
      }
    };

    updateStream();
  }, [constraints]);

  const handleMute = (type: 'audio' | 'video', muted: boolean) => {
    if (type === 'audio') {
      setAudioMute(muted);
    } else if (type === 'video') {
      setVideoMute(muted);
    }
    if (type === 'audio') {
      streamRef.current?.getAudioTracks().forEach((track: MediaStreamTrack) => {
        track.enabled = !muted;
      });
    } else {
      streamRef.current?.getVideoTracks().forEach((track: MediaStreamTrack) => {
        track.enabled = !muted;
      });
    }
  };

  return (
    <div className={styles.container}>
      <video id="local-stream" muted ref={player} className={styles['stream-player']} />
      <Header />
      <StreamButtons
        shareUrl={pushAndPullUrl.pullUrl}
        onPublish={handlePublish}
        onMute={handleMute}
        published={published}
      />
    </div>
  );
}

export default Push;

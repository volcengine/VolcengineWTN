/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { Button, Modal } from 'antd';
import stopImg from '/assets/images/stop2x.png';
import { useDispatch } from 'react-redux';
import { useEffect, useState } from 'react';
import { ExclamationOutlined } from '@ant-design/icons';
import { useTranslation } from 'react-i18next';
import styles from './index.module.less';
import MediaButton from '@/components/MediaButton';
import SettingButton from '@/components/SettingButton';

import rtcClient from '@/lib/RtcClient';
import { Devices } from './interface';
import ViewBtn from '@/components/ViewBtn';
import { updateAudio, updateVideo } from '@/store/reducer/constraints';

interface Props {
  onPublish: (status: boolean) => void;
  published: boolean;
  onMute: (type: 'audio' | 'video', muted: boolean) => void;
  shareUrl: string;
}

function StreamButtons(props: Props) {
  const dispatch = useDispatch();
  const { t } = useTranslation();
  const [stopModalVisible, setStopModalVisible] = useState(false);

  const { onPublish, published, onMute, shareUrl } = props;

  const [deviceSelectPop, setDeviceSelectPop] = useState('');

  const [audioMute, setAudioMute] = useState(false);
  const [videoMute, setVideoMute] = useState(false);

  const [devices, setDevices] = useState<Devices>({
    audioin: [],
    videoin: [],
  });

  useEffect(() => {
    rtcClient.getMediaDevices().then((curDevices) => setDevices(curDevices));
  }, []);

  const handleMicChange = (device: string | null | true) => {
    if (device === null) {
      onMute('audio', true);
      setAudioMute(true);
    } else if (device === true) {
      onMute('audio', false);
      setAudioMute(false);
    } else {
      dispatch(
        updateAudio({
          deviceId: {
            exact: device,
          },
        })
      );
    }
  };

  const handleCameraChange = (device: string | null | true) => {
    if (device === null) {
      onMute('video', true);
      setVideoMute(true);
    } else if (device === true) {
      onMute('video', false);
      setVideoMute(false);
    } else {
      dispatch(
        updateVideo(
          device
            ? {
                deviceId: {
                  exact: device,
                },
              }
            : false
        )
      );
    }
  };

  const handlePublish = () => {
    if (!published) {
      onPublish(!published);
    } else {
      setStopModalVisible(true);
    }
  };

  return (
    <div className={published ? styles['stream-btns-published'] : styles['stream-btns']}>
      <MediaButton
        onDeviceChange={handleMicChange}
        type="mic"
        devices={devices.audioin}
        published={published}
        opened={!audioMute}
        onDeviceSelect={setDeviceSelectPop}
        deviceSelectPop={deviceSelectPop}
      />

      <MediaButton
        type="cam"
        devices={devices.videoin}
        published={published}
        onDeviceChange={handleCameraChange}
        onDeviceSelect={setDeviceSelectPop}
        deviceSelectPop={deviceSelectPop}
        opened={!videoMute}
      />

      <SettingButton published={published} />
      {published && <ViewBtn shareUrl={shareUrl} />}

      {published ? (
        <div onClick={handlePublish}>
          <img
            src={stopImg}
            alt="stop"
            style={{
              transform: 'scale(0.5)',
            }}
          />
        </div>
      ) : (
        <Button type="primary" onClick={handlePublish}>
          {t('startPublish')}
        </Button>
      )}

      <Modal
        title={null}
        visible={stopModalVisible}
        cancelText={t('cancelText')}
        okText={t('cancelText')}
        footer={null}
        closable={false}
        onCancel={() => {
          setStopModalVisible(false);
        }}
        transitionName=""
        maskTransitionName=""
      >
        <div className={styles.stopModal}>
          <div className="header">
            <ExclamationOutlined
              style={{
                color: '#fff',
                backgroundColor: '#FF7D00',
                borderRadius: '50%',
              }}
            />
            <span className="stopText">{t('confirmStop')}</span>
          </div>
          <Button
            type="primary"
            danger
            onClick={() => {
              setStopModalVisible(false);
              onPublish(!published);
            }}
          >
            {t('sureText')}
          </Button>
          <Button
            onClick={() => {
              setStopModalVisible(false);
            }}
          >
            {t('cancelText')}
          </Button>
        </div>
      </Modal>
    </div>
  );
}
export default StreamButtons;

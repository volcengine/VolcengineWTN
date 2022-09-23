/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import micOnUnPubImg from '/assets/images/micOnUnPub2x.png';
import micOnPubImg from '/assets/images/micOnPub2x.png';
import micOffImg from '/assets/images/micOff2x.png';

import camOnUnPubImg from '/assets/images/camOnUnPub2x.png';
import camOnPubImg from '/assets/images/camOnPub2x.png';
import camOffImg from '/assets/images/camOff2x.png';

import downLightImg from '/assets/images/down_light_2x.png';
import downDarkImg from '/assets/images/down_dark_2x.png';
import upLightImg from '/assets/images/up_light_2x.png';
import upDarkImg from '/assets/images/up_dark_2x.png';

import styles from './index.module.less';
import { useEffect, useState } from 'react';
import { Radio, RadioChangeEvent } from 'antd';

interface Props {
  type: 'mic' | 'cam';
  published: boolean;
  opened: boolean;
  devices: MediaDeviceInfo[];
  onDeviceChange: (p: string | null | true) => void;
  onDeviceSelect: (p: string) => void;
  deviceSelectPop: string;
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
  const { type, published, opened } = props;

  if (type === 'mic') {
    if (published) {
      if (opened) {
        return {
          src: micOnPubImg,
          alt: 'mic on',
        };
      }
      return {
        src: micOffImg,
        alt: 'mic mute',
      };
    }
    if (opened) {
      return {
        src: micOnUnPubImg,
        alt: 'mic on',
      };
    }
    return {
      src: micOffImg,
      alt: 'mic mute',
    };
  }
  // cam
  if (published) {
    if (opened) {
      return {
        src: camOnPubImg,
        alt: 'camera on',
      };
    }
    return {
      src: camOffImg,
      alt: 'camera mute',
    };
  }
  if (opened) {
    return {
      src: camOnUnPubImg,
      alt: 'camera on',
    };
  }
  return {
    src: camOffImg,
    alt: 'camera mute',
  };
};

const getArrowIcon = (
  showDeviceMenu: boolean,
  published: boolean
): {
  src: string;
  alt: string;
} => {
  if (showDeviceMenu) {
    if (published) {
      return {
        src: upLightImg,
        alt: 'up-arrow',
      };
    }
    return {
      src: upDarkImg,
      alt: 'up-arrow',
    };
  }
  if (published) {
    return {
      src: downLightImg,
      alt: 'down-arrow',
    };
  }
  return {
    src: downDarkImg,
    alt: 'down-arrow',
  };
};

function MediaButton(props: Props) {
  const { type, devices, opened, onDeviceChange, published, onDeviceSelect, deviceSelectPop } =
    props;
  const imgIcon = getIcon(props);

  const [showDeviceMenu, setShowDeviceMenu] = useState(false);

  const handleSetDevices = () => {
    setShowDeviceMenu(!showDeviceMenu);
    onDeviceSelect(showDeviceMenu ? '' : type);
  };

  const [lastValue, setLastValue] = useState<string | null>(null);

  useEffect(() => {
    setLastValue(devices[0]?.deviceId);
  }, [devices]);

  useEffect(() => {
    setLastValue(devices[0]?.deviceId);
    if (deviceSelectPop && deviceSelectPop !== type) {
      setShowDeviceMenu(false);
    }
  }, [deviceSelectPop]);

  const handleDeviceChange = (e: RadioChangeEvent) => {
    setLastValue(e.target.value);
    setShowDeviceMenu(false);
    onDeviceChange(e.target.value);
  };

  const handleDeviceMute = () => {
    if (opened) {
      onDeviceChange(null);
    } else {
      onDeviceChange(true);
    }
  };

  return (
    <div className={styles['media-button']} data-type={type} data-published={published}>
      <div
        className={styles['media-button-icon']}
        data-type={type}
        data-published={published}
        onClick={handleDeviceMute}
      >
        <img {...imgIcon} />
      </div>
      <div
        className={styles['media-select-btn']}
        data-type={type}
        data-published={published}
        onClick={handleSetDevices}
      >
        <img {...getArrowIcon(showDeviceMenu, published)} />
      </div>

      <div
        className={styles['media-devices-list']}
        onMouseLeave={() => {
          setShowDeviceMenu(false);
        }}
        style={{
          display: showDeviceMenu ? 'block' : 'none',
        }}
        data-published={published}
      >
        <div className="media-devices-list-content">
          <Radio.Group onChange={handleDeviceChange} value={lastValue}>
            {devices.map((device) => {
              return (
                <Radio value={device.deviceId} key={device.deviceId}>
                  {device.label}
                </Radio>
              );
            })}
          </Radio.Group>
        </div>
      </div>
    </div>
  );
}

export default MediaButton;

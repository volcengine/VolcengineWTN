/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { Devices } from '@/pages/Push/interface';
import WhipClient from './WhipClient';

class RtcClient {
  whip: WhipClient | null;

  peer: RTCPeerConnection | undefined;

  published: boolean;

  constructor() {
    this.whip = null;
    this.published = false;
  }

  /**
   * 获取摄像头/麦克风
   * @returns Promise<Devices>
   */
  getMediaDevices = (): Promise<Devices> => {
    if (!navigator.mediaDevices || !navigator.mediaDevices.enumerateDevices) {
      return Promise.reject(
        new Error(
          'Could not get list of media devices!  This might not be supported by this browser.'
        )
      );
    }

    return new Promise((resolve, reject) => {
      navigator.mediaDevices
        .enumerateDevices()
        .then((list) => {
          const items = { audioin: [] as MediaDeviceInfo[], videoin: [] as MediaDeviceInfo[] }; // ,active:{audio:null,video:null}};
          list.forEach((device) => {
            switch (device.kind) {
              case 'audioinput':
                if (device.deviceId !== 'default') {
                  items.audioin.push(device);
                }
                break;
              case 'videoinput':
                if (device.deviceId !== 'default') {
                  items.videoin.push(device);
                }
                break;

              default:
                break;
            }
          });

          resolve(items);
        })
        .catch((error) => {
          resolve({ audioin: [], videoin: [] });
        });
    });
  };

  /**
   * 创建视频流
   * @param options MediaStreamConstraints
   * @returns
   */
  getStream = (options: MediaStreamConstraints): Promise<MediaStream> => {
    return new Promise((resolve, reject) => {
      navigator.mediaDevices
        .getUserMedia(options)
        .then((stream) => {
          resolve(stream);
        })
        .catch((err) => {
          reject(err);
        });
    });
  };

  /**
   * 发布流
   * @param publishUrl 发布地址
   * @param tracks 需要发布的流
   */
  publish = async (publishUrl: string, tracks: MediaStreamTrack[]) => {
    if (!this.peer) {
      this.peer = new RTCPeerConnection({});
    }
    tracks.forEach((track) => {
      this.peer?.addTrack(track);
    });
    this.whip = new WhipClient();

    try {
      await this.whip.start(this.peer, publishUrl);
      this.published = true;
      return {
        status: 200,
        statusText: 'success',
      };
    } catch (err) {
      throw new Error(err as any);
    }
  };

  /**
   * 停止发布流
   */
  stopPublish = async () => {
    try {
      await this.whip?.stop();
      this.published = false;
      this.peer = undefined;
    } catch (err) {
      throw new Error('stop publish error');
    }
  };

  /**
   * 更新流，发生在设备切换/设置分辨率等
   * @param publishUrl
   * @param tracks
   */
  updateStream = async (tracks: MediaStreamTrack[]) => {
    if (this.published) {
      // 移除原有的流

      const senders = this.peer?.getSenders();
      tracks.forEach((track) => {
        senders?.forEach((sender) => {
          if (track.kind === sender.track?.kind) {
            sender.replaceTrack(track);
          }
        });
      });
    }
  };

  /**
   * 订阅流
   * @param url 订阅地址
   * @returns
   */
  subscribe = async (url: string): Promise<MediaStream> => {
    return new Promise((resolve, reject) => {
      if (!this.peer) {
        this.peer = new RTCPeerConnection({});
      }

      this.peer.ontrack = function (event) {
        if (event && event.streams) {
          if (event.track.kind === 'video') {
            resolve(event.streams[0]);
          }
        }
      };

      this.whip = new WhipClient();
      this.whip.start(this.peer, url).catch((err) => {
        reject(err);
      });
    });
  };

  /**
   * 停止订阅
   */
  stopSubscribe = async () => {
    try {
      await this.whip?.stop();
      this.peer = undefined;
    } catch (err) {}
  };
}

export default new RtcClient();

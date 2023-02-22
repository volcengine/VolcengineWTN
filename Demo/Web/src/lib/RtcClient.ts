/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import Eventemitter from 'eventemitter3';
import { Devices } from '@/pages/Push/interface';
import WhipClient from './WhipClient';
import { ERRORTYPE, PEEREVENT } from './interface';

class RtcClient extends Eventemitter {
  whip?: WhipClient;

  peer?: RTCPeerConnection;

  private _audioStreamConstraints: MediaTrackConstraints = {};

  private _localAudioDeviceId?: string;

  private _localAudioTrack?: MediaStreamTrack;

  private _videoStreamConstraints: MediaTrackConstraints = {};

  private _localVideoDeviceId?: string;

  private _localVideoTrack?: MediaStreamTrack;

  private _published: boolean = false;

  private _subscribed: boolean = false;

  private _videoPlayer?: HTMLVideoElement;

  private _audioMute: boolean = false;

  private _videoMute: boolean = false;

  private devices: Devices | undefined;

  /**
   * 销毁状态
   */
  destory = () => {
    this.whip = undefined;
    this.peer = undefined;
    this._audioStreamConstraints = {};
    this._localAudioDeviceId = undefined;
    this._localAudioTrack = undefined;
    this._videoStreamConstraints = {};
    this._localVideoDeviceId = undefined;
    this._localVideoTrack = undefined;
    this._published = false;
    this._videoPlayer = undefined;
    this._audioMute = false;
    this._videoMute = false;
  };

  /**
   * 开始捕获音频
   * @param deviceId
   */
  startAudioCapture = async (deviceId?: string) => {
    deviceId = deviceId || this._localAudioDeviceId;
    if (deviceId) {
      this._audioStreamConstraints.deviceId = { exact: deviceId };
    }

    let mediaStream;

    try {
      mediaStream = await navigator.mediaDevices.getUserMedia({
        audio: this._audioStreamConstraints,
      });
      const track = mediaStream.getAudioTracks()[0];
      track.enabled = !this._audioMute;
      this._localAudioTrack = track;

      if (this._published) {
        this._updatePublish();
      }
    } catch (e) {
      console.error(e);
    }
  };

  /**
   * 停止捕获音频
   */
  stopAudioCapture = () => {
    this._localAudioTrack?.stop();
    if (this._published) {
      this._updatePublish();
    }
  };

  /**
   * 开始捕获视频
   * @param deviceId
   */
  startVideoCapture = async (deviceId?: string) => {
    deviceId = deviceId || this._localVideoDeviceId;
    if (deviceId) {
      this._videoStreamConstraints.deviceId = { exact: deviceId };
    }

    let mediaStream;

    try {
      mediaStream = await navigator.mediaDevices.getUserMedia({
        video: this._videoStreamConstraints,
      });
      const track = mediaStream.getVideoTracks()[0];
      track.enabled = !this._videoMute;
      this._localVideoTrack = track;

      this._play();

      if (this._published) {
        this._updatePublish();
      }
    } catch (e) {
      console.error(e);
    }
  };

  /**
   * 停止捕获视频
   */
  stopVideoCapture = () => {
    this._localVideoTrack?.stop();
    if (this._published) {
      this._updatePublish();
    }
  };

  /**
   * 设置音频采集设备
   * @param device
   */
  setAudioCaptureDevice = (device: string) => {
    this._localAudioTrack?.stop();
    this._localAudioDeviceId = device;
    this.startAudioCapture(device);
  };

  /**
   * 设置视频采集设备
   * @param device
   */
  setVideoCaptureDevice = (device: string) => {
    this._localVideoTrack?.stop();
    this._localVideoDeviceId = device;
    this.startVideoCapture(device);
  };

  /**
   * 设置视频播放器
   * @param renderDom
   */
  setLocalVideoPlayer = (renderDom: string | HTMLVideoElement) => {
    const _videoDom: HTMLVideoElement | null =
      typeof renderDom === 'string'
        ? (document.getElementById(renderDom) as HTMLVideoElement)
        : renderDom;
    if (!_videoDom) {
      throw new Error(`can't find video player dom`);
    }

    this._videoPlayer = _videoDom;

    this._play();
  };

  /**
   * 设置视频采集属性
   * @param config
   */
  setVideoCaptureConfig = async (config: MediaTrackConstraints) => {
    const deviceId = this._videoStreamConstraints.deviceId;

    this._videoStreamConstraints = config;
    if (deviceId) {
      this._videoStreamConstraints.deviceId = deviceId;
    }

    if (this._localVideoTrack) {
      await this._localVideoTrack.applyConstraints(this._videoStreamConstraints);
    }

    if (this._published) {
      this._updatePublish();
    }
  };

  /**
   * 禁用音频
   * @param mute
   */
  muteAudio = (mute: boolean) => {
    this._audioMute = mute;
    if (this._localAudioTrack) {
      this._localAudioTrack.enabled = !mute;
    } else {
      this.startAudioCapture();
    }
  };

  /**
   * 禁用视频
   * @param mute
   */
  muteVideo = (mute: boolean) => {
    this._videoMute = mute;
    if (this._localVideoTrack) {
      this._localVideoTrack.enabled = !mute;
    } else {
      this.startVideoCapture();
    }
  };

  private _updatePublish = () => {
    if (!this.whip) {
      return;
    }
    this.whip.updateTrack(this._localAudioTrack, this._localVideoTrack);
  };

  private _play = () => {
    if (this._videoPlayer && this._localVideoTrack) {
      const ms = new MediaStream();
      ms.addTrack(this._localVideoTrack);
      this._videoPlayer.onloadeddata = () => {
        this._videoPlayer?.play();
      };
      this._videoPlayer.srcObject = ms;
    }
  };

  /**
   * 获取摄像头/麦克风
   * @returns Promise<Devices>
   */
  getMediaDevices = (): Promise<Devices> => {
    if (this.devices) {
      return Promise.resolve(this.devices);
    }
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

          this._localVideoDeviceId = items.videoin[0]?.deviceId;
          this._localAudioDeviceId = items.audioin[0]?.deviceId;

          this.devices = items;
          resolve(items);
        })
        .catch((error) => {
          resolve({ audioin: [], videoin: [] });
        });
    });
  };

  /**
   * 发布流
   * @param publishUrl 发布地址
   * @param tracks 需要发布的流
   */
  publish = async (publishUrl: string) => {
    this.initPeerConnection();

    this.whip = new WhipClient(this.peer!);
    this.whip.setPublishUrl(publishUrl);

    const res = await this.whip.startPublish(this._localAudioTrack, this._localVideoTrack);

    if (res.code === ERRORTYPE.SUCCESS) {
      this._published = true;
    }

    return res;
  };

  beforeRePublish = () => {
    this.destoryPeer();
    this._published = false;
  };

  /**
   * 停止发布流
   */
  unPublish = async () => {
    if (!this.whip) {
      return {
        code: ERRORTYPE.CLIENT,
        message: 'no WhipClient',
      };
    }

    const res = await this.whip.stop();

    if (res?.code === ERRORTYPE.SUCCESS) {
      this.destoryPeer();
      this._published = false;
    }

    return res;
  };

  /**
   * 订阅流
   * @param url 订阅地址
   * @returns
   */
  subscribe = async (url: string): Promise<MediaStream> => {
    return new Promise((resolve, reject) => {
      const sub = async () => {
        this.initPeerConnection();
        this.peer!.ontrack = function (event) {
          if (event && event.streams) {
            if (event.track.kind === 'video') {
              resolve(event.streams[0]);
            }
          }
        };

        this.whip = new WhipClient(this.peer!);
        this.whip.setSubscribeUrl(url);
        const res = await this.whip.startSubscribe();
        if (res.code !== ERRORTYPE.SUCCESS) {
          reject(res);
        } else {
          this._subscribed = true;
        }
      };
      sub();
    });
  };

  beforeReSubscribe = () => {
    this.destoryPeer();
    this._subscribed = false;
  };

  /**
   * 停止订阅
   */
  stopSubscribe = async () => {
    if (!this.whip) {
      return {
        code: ERRORTYPE.CLIENT,
        message: 'no WhipClient',
      };
    }

    const res = await this.whip.stop();

    if (res?.code === ERRORTYPE.SUCCESS) {
      this._subscribed = false;
      this.destoryPeer();
    }

    return res;
  };

  private destoryPeer = () => {
    if (this.peer) {
      this.peer.close();
      this.peer.onconnectionstatechange = null;
    }
    this.peer = undefined;
    this.whip = undefined;
  };

  private initPeerConnection = () => {
    if (!this.peer) {
      this.peer = new RTCPeerConnection({});

      this.peer.onconnectionstatechange = () => {
        if (
          this.peer?.iceConnectionState === 'failed' ||
          this.peer?.iceConnectionState === 'closed' ||
          this.peer?.connectionState === 'failed'
        ) {
          if (this._subscribed || this._published) {
            this.emit(PEEREVENT.Disconnect, ERRORTYPE.PEER);
          } else {
            this.emit(PEEREVENT.Disconnect, ERRORTYPE.MEDIA);
          }
        }
      };
    }
  };
}

export default new RtcClient();

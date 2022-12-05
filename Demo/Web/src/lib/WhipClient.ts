/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

import { ERRORTYPE } from './interface';

/**
 * 使用了whip协议，https://datatracker.ietf.org/doc/html/draft-ietf-wish-whip-01
 * 现在只支持start / stop 进行p2p连接
 */
class WhipClient {
  peerConn?: RTCPeerConnection;

  resourceURL: URL | undefined;

  publishUrl: string | undefined;

  subscribeUrl: string | undefined;

  private _audioTransceiver?: RTCRtpTransceiver;

  private _videoTransceiver?: RTCRtpTransceiver;

  constructor(peerConn: RTCPeerConnection) {
    this.peerConn = peerConn;
  }

  setPublishUrl = (publishUrl: string) => {
    this.publishUrl = publishUrl;
  };

  setSubscribeUrl = (subscribeUrl: string) => {
    this.subscribeUrl = subscribeUrl;
  };

  /**
   * p2p建联，offer交换
   * @param peerConn
   * @param url
   * @returns
   */
  async startPublish(audioTrack?: MediaStreamTrack, videoTrack?: MediaStreamTrack) {
    if (!this.peerConn) {
      return {
        code: ERRORTYPE.CLIENT,
        message: 'Create PeerConnection First',
      };
    }

    const ms = new MediaStream();
    if (audioTrack) ms.addTrack(audioTrack);
    if (videoTrack) ms.addTrack(videoTrack);

    this._audioTransceiver = this.peerConn.addTransceiver(audioTrack || 'audio', {
      streams: [ms],
      direction: 'sendonly',
    });

    this._videoTransceiver = this.peerConn.addTransceiver(videoTrack || 'video', {
      streams: [ms],
      direction: 'sendonly',
    });

    const offer = await this.peerConn.createOffer();
    const setLocalOk = this.peerConn.setLocalDescription(offer);

    const headers: Record<string, string> = {
      'Content-Type': 'application/sdp',
    };

    try {
      const res = await fetch(this.publishUrl!, { method: 'POST', body: offer.sdp, headers });

      if (!res.ok) {
        return {
          code: ERRORTYPE.HTTP,
          status: res.status,
          message: 'Publish err',
        };
      }
      if (!res.headers.get('location')) {
        console.error('no http header location', res);
        return {
          code: ERRORTYPE.HTTP,
          status: 400,
          message: 'Resource url unknow',
        };
      }

      this.resourceURL = new URL(res.headers.get('location') || '', this.publishUrl);

      const sdp = await res.text();
      await setLocalOk;
      const answer = new RTCSessionDescription({ type: 'answer', sdp });
      await this.peerConn.setRemoteDescription(answer);
      return {
        code: ERRORTYPE.SUCCESS,
        message: 'success',
      };
    } catch (e) {
      console.error('startPublish err', e);
      return {
        code: ERRORTYPE.CLIENT,
        message: 'something err',
      };
    }
  }

  /**
   * p2p建联，offer交换
   * @param peerConn
   * @param url
   * @returns
   */
  async startSubscribe() {
    if (!this.peerConn) {
      return {
        code: ERRORTYPE.CLIENT,
        message: 'Create PeerConnection First',
      };
    }

    this._audioTransceiver = this.peerConn.addTransceiver('audio', {
      direction: 'recvonly',
    });

    this._videoTransceiver = this.peerConn.addTransceiver('video', {
      direction: 'recvonly',
    });

    const offer = await this.peerConn.createOffer();
    const setLocalOk = this.peerConn.setLocalDescription(offer);

    const headers: Record<string, string> = {
      'Content-Type': 'application/sdp',
    };

    try {
      const res = await fetch(this.subscribeUrl!, { method: 'POST', body: offer.sdp, headers });

      if (!res.ok) {
        return {
          code: ERRORTYPE.HTTP,
          status: res.status,
          message: 'Subscribe err',
        };
      }
      if (!res.headers.get('location')) {
        console.error('no http header location', res);
        return {
          code: ERRORTYPE.HTTP,
          status: 400,
          message: 'Resource url unknow',
        };
      }

      this.resourceURL = new URL(res.headers.get('location') || '', this.subscribeUrl);

      const sdp = await res.text();
      await setLocalOk;
      const answer = new RTCSessionDescription({ type: 'answer', sdp });
      await this.peerConn.setRemoteDescription(answer);
      return {
        code: ERRORTYPE.SUCCESS,
        message: 'success',
      };
    } catch (e) {
      console.error('start subscribe err', e);
      return {
        code: ERRORTYPE.CLIENT,
        message: 'something err',
      };
    }
  }

  updateTrack = async (audioTrack?: MediaStreamTrack, videoTrack?: MediaStreamTrack) => {
    if (audioTrack) {
      this._audioTransceiver?.sender.replaceTrack(audioTrack);
    } else {
      this._audioTransceiver?.sender.replaceTrack(null);
    }

    if (videoTrack) {
      this._videoTransceiver?.sender.replaceTrack(videoTrack);
    } else {
      this._videoTransceiver?.sender.replaceTrack(null);
    }
  };

  /**
   * whip断开连接
   * @returns
   */
  async stop() {
    if (!this.peerConn) {
      return {
        code: ERRORTYPE.CLIENT,
        message: 'Create PeerConnection First',
      };
    }

    if (!this.resourceURL) {
      return {
        code: ERRORTYPE.CLIENT,
        message: 'Resource url unknow',
      };
    }
    const headers: Record<string, string> = {};
    try {
      const res = await fetch(this.resourceURL.href, { method: 'DELETE', headers });
      if (!res.ok) {
        return {
          code: ERRORTYPE.HTTP,
          status: res.status,
          message: `Fetch rejected with status ${res.status}`,
        };
      }

      return {
        code: ERRORTYPE.SUCCESS,
        message: 'success',
      };
    } catch (err) {
      console.error('stop  err', err);
      return {
        code: ERRORTYPE.HTTP,
        message: 'something err',
      };
    }
  }
}

export default WhipClient;

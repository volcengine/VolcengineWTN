/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

/**
 * 使用了whip协议，https://datatracker.ietf.org/doc/html/draft-ietf-wish-whip-01
 * 现在只支持start / stop 进行p2p连接
 */
class WhipClient {
  peerConn?: RTCPeerConnection;

  resourceURL: URL | undefined;

  url: string;

  private _audioTransceiver?: RTCRtpTransceiver;

  private _videoTransceiver?: RTCRtpTransceiver;

  constructor(peerConn: RTCPeerConnection, url: string) {
    this.peerConn = peerConn;
    this.url = url;
  }

  /**
   * p2p建联，offer交换
   * @param peerConn
   * @param url
   * @returns
   */
  async start(audioTrack?: MediaStreamTrack, videoTrack?: MediaStreamTrack) {
    if (!this.peerConn) {
      return;
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

    const res = await fetch(this.url, { method: 'POST', body: offer.sdp, headers });

    if (!res.ok) {
      return res;
    }
    if (!res.headers.get('location')) {
      throw new Error('Resource url unknow');
    }

    this.resourceURL = new URL(res.headers.get('location') || '', this.url);

    const sdp = await res.text();
    await setLocalOk;
    const answer = new RTCSessionDescription({ type: 'answer', sdp });
    await this.peerConn.setRemoteDescription(answer);
  }

  /**
   * p2p建联，offer交换
   * @param peerConn
   * @param url
   * @returns
   */
  async startSubscribe() {
    if (!this.peerConn) {
      return;
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

    const res = await fetch(this.url, { method: 'POST', body: offer.sdp, headers });

    if (!res.ok) {
      return res;
    }
    if (!res.headers.get('location')) {
      throw new Error('Resource url unknow');
    }

    this.resourceURL = new URL(res.headers.get('location') || '', this.url);

    const sdp = await res.text();
    await setLocalOk;
    const answer = new RTCSessionDescription({ type: 'answer', sdp });
    await this.peerConn.setRemoteDescription(answer);
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
      return;
    }
    this.peerConn.close();
    this.peerConn = undefined;

    if (!this.resourceURL) {
      throw new Error('Resource url unknow');
    }
    const headers: Record<string, string> = {};

    const res = await fetch(this.resourceURL.href, { method: 'DELETE', headers });
    if (!res.ok) {
      throw new Error(`Fetch rejected with status ${res.status}`);
    }
  }
}

export default WhipClient;

/**
 * Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 * SPDX-license-identifier: BSD-3-Clause
 */

/**
 * 使用了whip协议，https://datatracker.ietf.org/doc/html/draft-ietf-wish-whip-01
 * 现在只支持start / stop 进行p2p连接
 */
class WhipClient {
  peerConn: RTCPeerConnection | undefined;

  resourceURL: URL | undefined;

  /**
   * p2p建联，offer交换
   * @param peerConn
   * @param url
   * @returns
   */
  async start(peerConn: RTCPeerConnection, url: string) {
    if (this.peerConn) {
      throw new Error('Already started');
    }

    this.peerConn = peerConn;

    const offer = await peerConn.createOffer({
      offerToReceiveVideo: true,
      offerToReceiveAudio: true,
    });
    const setLocalOk = peerConn.setLocalDescription(offer);

    const headers: Record<string, string> = {
      'Content-Type': 'application/sdp',
    };

    const res = await fetch(url, { method: 'POST', body: offer.sdp, headers });

    if (!res.headers.get('location')) {
      throw new Error('Resource url unknow');
    }

    try {
      this.resourceURL = new URL(res.headers.get('location') || '', url);

      const sdp = await res.text();
      await setLocalOk;
      const answer = new RTCSessionDescription({ type: 'answer', sdp });
      await peerConn.setRemoteDescription(answer);
      return {
        status: 200,
        statusText: 'success',
      };
    } catch (err) {
      throw new Error((err as any)?.message);
    }
  }

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

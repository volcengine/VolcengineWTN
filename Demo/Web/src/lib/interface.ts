export enum ERRORTYPE {
  CLIENT, // 客户端业务逻辑问题
  HTTP, // 发布/订阅/停止发布or订阅 的http请求出错
  MEDIA, // offer交换成功，设置失败
  PEER, //  建联后 p2p 连接状态断开
  SUCCESS,
}

export enum PEEREVENT {
  Disconnect = 'Disconnect',
}

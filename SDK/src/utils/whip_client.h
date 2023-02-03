/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include <memory>
#include "wtn_interface.h"
#include "http_client.h"

namespace vewtn {

class WhipClientObserver {
public:
    virtual void OnStartSuccess(std::string sdp) = 0;
    
    virtual void OnStartFailed(Reason code) = 0;
    
    virtual void OnStopSuccess() = 0;
    
    virtual void OnStopFailed(Reason code) = 0;
    
    virtual void OnMuteAudioSuccess() = 0;
    
    virtual void OnMuteAudioFailed(Reason code) = 0;
    
    virtual void OnMuteVideoSuccess() = 0;
    
    virtual void OnMuteVideoFailed(Reason code) = 0;
};

class WhipClient {
public:
    WhipClient(WhipClientObserver *observer);
    
    ~WhipClient();
    
    void Start(const std::string &url, const std::string &sdp);
    
    void Stop();
    
    int SetAudioMute(bool mute);
    int SetVideoMute(bool mute);
    
    void OnStartResponse(int code,
                         std::map<std::string, std::string> headers,
                         std::string response);
    
    void OnStopResponse(int code,
                        std::map<std::string, std::string> headers,
                        std::string response);
    
    void OnMuteAudioResponse(int code,
                             std::map<std::string, std::string> headers,
                             std::string response);
    
    void OnMuteVideoResponse(int code,
                             std::map<std::string, std::string> headers,
                             std::string response);
    
private:
    Reason GetReasonByCode(int code);
    std::vector<std::string> Split(std::string s, std::string delimiter);

private:
    std::unique_ptr<AsyncHttpClient> http_client_;
    std::unique_ptr<AsyncHttpClient> redirect_http_client_;
    std::map<std::string, std::string> headers_;
    WhipClientObserver *observer_;
    std::string resource_url_;
    std::string sdp_;
};

}

/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once

#include "http_client.h"
#include <memory>
#include "vewtn_interface.h"

namespace vewtn {

class WhipClientObserver {
public:
    virtual void OnStartSuccess(std::string sdp) = 0;
    
    virtual void OnStartFailed(Reason code) = 0;
    
    virtual void OnStopSuccess() = 0;
    
    virtual void OnStopFailed(Reason code) = 0;
};

class WhipClient {
public:
    WhipClient(WhipClientObserver *observer);
    
    ~WhipClient();
    
    void Start(const std::string &url, const std::string &sdp);
    
    void Stop();
    
    void OnStartResponse(int code,
                         std::map<std::string, std::string> headers,
                         std::string response);
    
    void OnStopResponse(int code,
                        std::map<std::string, std::string> headers,
                        std::string response);
    
private:
    std::unique_ptr<AsyncHttpClient> http_client_;
    WhipClientObserver *observer_;
    std::string stop_url_;
};

}

/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "whip_client.h"
#include <map>

namespace vewtn {

WhipClient::WhipClient(WhipClientObserver *observer)
: observer_(observer) {
}

WhipClient::~WhipClient() {
    if (http_client_) {
        http_client_->CancelAsyncTasks();
    }
}

void WhipClient::Start(const std::string &url, const std::string &sdp) {
    http_client_ = std::unique_ptr<AsyncHttpClient>(new AsyncHttpClient());
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/sdp";
    for (auto kv : headers) {
        http_client_->AddHeader(kv.first, kv.second);
    }
    http_client_->Post(url, sdp,
                       std::bind(&WhipClient::OnStartResponse, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3), 10000);
}

void WhipClient::Stop() {
    if (stop_url_.empty()) {
        observer_->OnStopFailed(Reason::ParameterError);
        return;
    }
    http_client_.reset(new AsyncHttpClient());
    std::map<std::string, std::string> headers;
    headers["Content-Type"] = "application/sdp";
    for (auto kv : headers) {
        http_client_->AddHeader(kv.first, kv.second);
    }
    http_client_->Delete(stop_url_,
                         std::bind(&WhipClient::OnStopResponse, this, std::placeholders::_1,
                                   std::placeholders::_2, std::placeholders::_3), 10000);
}

void WhipClient::OnStartResponse(int code,
                                 std::map<std::string, std::string> headers,
                                 std::string response) {
    if (!observer_) {
        return;
    }
    
    if (code == 201) {
        auto it = headers.find("Location");
        if (it != headers.end()) {
            stop_url_ = it->second;
        }
        observer_->OnStartSuccess(response);
    } else {
        Reason reason;
        switch (code) {
            case 400:  // param error
                reason = Reason::ParameterError;
                break;
            case 401:  // token error
            case 403:  // token has expired or no push permission
                reason = Reason::TokenError;
                break;
            case 500:  // Internal server error
                reason = Reason::ServerInternalError;
                break;
            case -10011:  // network error
                reason = Reason::NetworkError;
                break;
            default:
                reason = Reason::UnknowError;
                break;
        }
        observer_->OnStartFailed(reason);
    }
}

void WhipClient::OnStopResponse(int code,
                                std::map<std::string, std::string> headers,
                                std::string response) {
    if (!observer_) {
        return;
    }
    
    if (code == 200) {
        observer_->OnStopSuccess();
    } else {
        Reason reason;
        switch (code) {
            case 400:  // param error
                reason = Reason::ParameterError;
                break;
            case 404:  // Resources do not exist
                reason = Reason::ResourcesNotFound;
                break;
            case 500:  // Internal server error
                reason = Reason::ServerInternalError;
                break;
            case -10011:  // network error
                reason = Reason::NetworkError;
                break;
            default:
                reason = Reason::UnknowError;
                break;
        }
        observer_->OnStopFailed(reason);
    }
}

}

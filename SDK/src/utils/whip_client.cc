/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "whip_client.h"
#include <map>
#include "rtc_base/logging.h"

namespace vewtn {

WhipClient::WhipClient(WhipClientObserver *observer)
    : observer_(observer) {
}

WhipClient::~WhipClient() {
    if (http_client_) {
        http_client_->CancelAsyncTasks();
    }
    if (redirect_http_client_) {
        redirect_http_client_->CancelAsyncTasks();
    }
}

void WhipClient::Start(const std::string &url, const std::string &sdp) {
    http_client_.reset(new AsyncHttpClient());
    headers_.clear();
    headers_["Content-Type"] = "application/sdp";

    std::string token;
    std::string token_type;
    std::vector<std::string> split_url = Split(url, "?");
    if (split_url.size() == 2) {
        std::string param = split_url[1];
        std::vector<std::string> split_param = Split(param, "&");
        for (auto p : split_param) {
            std::vector<std::string> key_value = Split(p, "=");
            if (key_value.size() == 2) {
                if (key_value[0] == "Token") {
                    token = key_value[1];
                } else if (key_value[0] == "TokenType") {
                    token_type = key_value[1];
                }
            }
        }
    }
    if (!token.empty() && !token_type.empty()) {
        headers_["Authorization"] = token_type + " " + token;
    } else if (!token.empty() && token_type.empty()) {
        headers_["Authorization"] = token;
    }
    for (auto kv : headers_) {
        http_client_->AddHeader(kv.first, kv.second);
    }
    sdp_ = sdp;
    http_client_->Post(url, sdp,
                       std::bind(&WhipClient::OnStartResponse, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3), 10000);
}

void WhipClient::Stop() {
    if (resource_url_.empty()) {
        observer_->OnStopFailed(Reason::ParameterError);
        return;
    }
    http_client_.reset(new AsyncHttpClient());
    headers_["Content-Type"] = "application/sdp";
    for (auto kv : headers_) {
        http_client_->AddHeader(kv.first, kv.second);
    }
    http_client_->Delete(resource_url_,
                         std::bind(&WhipClient::OnStopResponse, this, std::placeholders::_1,
                                   std::placeholders::_2, std::placeholders::_3), 10000);
}

int WhipClient::SetAudioMute(bool mute) {
    if (resource_url_.empty()) {
        observer_->OnMuteAudioFailed(Reason::ParameterError);
        return -1;
    }
    http_client_.reset(new AsyncHttpClient());
    headers_["Content-Type"] = "application/json";
    for (auto kv : headers_) {
        http_client_->AddHeader(kv.first, kv.second);
    }
    std::string body = mute ? "{\"MuteAudio\":true}" : "{\"MuteAudio\":false}";
    http_client_->Post(resource_url_, body,
                       std::bind(&WhipClient::OnMuteAudioResponse, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3), 10000);
    return 0;
}

int WhipClient::SetVideoMute(bool mute) {
    if (resource_url_.empty()) {
        observer_->OnMuteVideoFailed(Reason::ParameterError);
        return -1;
    }
    http_client_.reset(new AsyncHttpClient());
    headers_["Content-Type"] = "application/json";
    for (auto kv : headers_) {
        http_client_->AddHeader(kv.first, kv.second);
    }
    std::string body = mute ? "{\"MuteVideo\":true}" : "{\"MuteVideo\":false}";
    http_client_->Post(resource_url_, body,
                       std::bind(&WhipClient::OnMuteVideoResponse, this, std::placeholders::_1,
                                 std::placeholders::_2, std::placeholders::_3), 10000);
    return 0;
}

void WhipClient::OnStartResponse(int code,
                                 std::map<std::string, std::string> headers,
                                 std::string response) {
    if (!observer_) {
        return;
    }
    
    RTC_LOG(LS_INFO) << "OnStartResponse, code:" << code;
    if (code == 201) {
        auto it = headers.find("Location");
        if (it != headers.end()) {
            resource_url_ = it->second;
            RTC_LOG(LS_INFO) << "resource_url: " << resource_url_;
        }
        observer_->OnStartSuccess(response);
    } else if (code == 307) {
        std::string url = headers["Location"];
        redirect_http_client_ = std::unique_ptr<AsyncHttpClient>(new AsyncHttpClient());
        for (auto kv : headers_) {
            redirect_http_client_->AddHeader(kv.first, kv.second);
        }
        redirect_http_client_->Post(url, sdp_,
                                    std::bind(&WhipClient::OnStartResponse, this, std::placeholders::_1,
                                              std::placeholders::_2, std::placeholders::_3), 10000);
    } else {
        RTC_LOG(LS_INFO) << "OnStartResponse, response:" << response;
        if (code == -10012) {
            return;
        }
        Reason reason = GetReasonByCode(code);
        observer_->OnStartFailed(reason);
    }
}

void WhipClient::OnStopResponse(int code,
                                std::map<std::string, std::string> headers,
                                std::string response) {
    if (!observer_) {
        return;
    }
    
    RTC_LOG(LS_INFO) << "OnStopResponse, code:" << code;
    if (code == 200) {
        observer_->OnStopSuccess();
    } else {
        if (code == -10012) {
            return;
        }
        Reason reason = GetReasonByCode(code);
        observer_->OnStopFailed(reason);
    }
}

void WhipClient::OnMuteAudioResponse(int code,
                                     std::map<std::string, std::string> headers,
                                     std::string response) {
    if (!observer_) {
        return;
    }
    
    RTC_LOG(LS_INFO) << "OnMuteAudioResponse, code:" << code;
    if (code == 200) {
        observer_->OnMuteAudioSuccess();
    } else {
        Reason reason = GetReasonByCode(code);
        observer_->OnMuteAudioFailed(reason);
    }
}

void WhipClient::OnMuteVideoResponse(int code,
                                     std::map<std::string, std::string> headers,
                                     std::string response) {
    if (!observer_) {
        return;
    }
    
    RTC_LOG(LS_INFO) << "OnMuteVideoResponse, code:" << code;
    if (code == 200) {
        observer_->OnMuteVideoSuccess();
    } else {
        Reason reason = GetReasonByCode(code);
        observer_->OnMuteVideoFailed(reason);
    }
}

Reason WhipClient::GetReasonByCode(int code) {
    Reason reason;
    switch (code) {
        case 400:  // param error
            reason = Reason::ParameterError;
            break;
        case 401:  // token error
        case 403:  // token has expired or no push permission
            reason = Reason::TokenError;
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
            reason = Reason::UnknownError;
            break;
    }
    return reason;
}

std::vector<std::string> WhipClient::Split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;
    
    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }
    
    res.push_back (s.substr (pos_start));
    return res;
}

}

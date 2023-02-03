/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "http_client.h"
#include <iostream>
#include <boost/bind.hpp>
#include "rtc_base/logging.h"

namespace vewtn {

AsyncHttpClient::AsyncHttpClient() {
    headers_["Accept"] = "*/*";
    headers_["Connection"] = "close";
}

AsyncHttpClient::~AsyncHttpClient() {
    StopHttpRequest();
    UninitHttpRequest();
}

void AsyncHttpClient::TimeoutHandler(const boost::system::error_code &error) {
    if (!error) {
        callback_(IHttpAsyncClient::HttpRequestError::kTimeout, response_headers_,
                  "Async http client timeout!\n");
        StopHttpRequest();
    }
}

IHttpAsyncClient::HttpClientTypes AsyncHttpClient::GetCurrentType() {
    return IHttpAsyncClient::HttpClientTypes::kOriginalHttpClient;
}

void AsyncHttpClient::CancelAsyncTasks() {
    callback_(IHttpAsyncClient::HttpRequestError::kCanceled, response_headers_,
              "Async http client canceled!\n");
    StopHttpRequest();
}

void AsyncHttpClient::AddHeader(const std::string &key, const std::string &value) {
    headers_[key] = value;
}

void AsyncHttpClient::Get(
                          const std::string &url, HttpCallBack callback, int timeoutMillisecond) {
    if (started_) {
        return;
    }
    callback_ = callback;
    std::string server, port, path;
    UrlParse(url, &server, &port, &path);
    GetImpl(server, server, path, port, timeoutMillisecond);
}

void AsyncHttpClient::Get(
                          const std::string &urlWithIP, const std::string &host, HttpCallBack callback,
                          int timeoutMillisecond) {
    if (started_) {
        return;
    }
    callback_ = callback;
    std::string server, port, path;
    UrlParse(urlWithIP, &server, &port, &path);
    GetImpl(server, host, path, port, timeoutMillisecond);
}

void AsyncHttpClient::GetImpl(const std::string &server, const std::string &host,
                              const std::string &path,
                              const std::string &port, int timeoutMillisecond) {
    headers_["Host"] = host;
    std::ostream request_stream(&request_);
    request_stream << "GET " << path << " HTTP/1.0\r\n";
    for (auto &item : headers_) {
        request_stream << item.first << ": " << item.second << "\r\n";
    }
    request_stream << "\r\n";
    InitHttpRequest();
    StartHttpRequest(server, port, timeoutMillisecond);
}

void AsyncHttpClient::Delete(
                             const std::string &url, HttpCallBack callback, int timeoutMillisecond) {
    if (started_) {
        return;
    }
    callback_ = callback;
    std::string server, port, path;
    UrlParse(url, &server, &port, &path);
    DeleteImpl(server, server, path, port, timeoutMillisecond);
}

void AsyncHttpClient::DeleteImpl(const std::string &server, const std::string &host,
                                 const std::string &path,
                                 const std::string &port, int timeoutMillisecond) {
    headers_["Host"] = host;
    std::ostream request_stream(&request_);
    request_stream << "DELETE " << path << " HTTP/1.0\r\n";
    for (auto &item : headers_) {
        request_stream << item.first << ": " << item.second << "\r\n";
    }
    request_stream << "\r\n";
    InitHttpRequest();
    StartHttpRequest(server, port, timeoutMillisecond);
}

void AsyncHttpClient::Post(
                           const std::string &url, const std::string &content, HttpCallBack callback,
                           int timeoutMillisecond) {
    if (started_) {
        return;
    }
    callback_ = callback;
    std::string server, port, path;
    UrlParse(url, &server, &port, &path);
    PostImpl(server, server, path, port, content, timeoutMillisecond);
}

void AsyncHttpClient::Post(const std::string &urlWithIP, const std::string &host,
                           const std::string &content,
                           HttpCallBack callback, int timeoutMillisecond) {
    if (started_) {
        return;
    }
    callback_ = callback;
    std::string server, port, path;
    UrlParse(urlWithIP, &server, &port, &path);
    PostImpl(server, host, path, port, content, timeoutMillisecond);
}

void AsyncHttpClient::PostImpl(const std::string &server, const std::string &host,
                               const std::string &path,
                               const std::string &port, const std::string &content,
                               int timeoutMillisecond) {
    headers_["Host"] = host;
    headers_["Content-Length"] = std::to_string(content.length());
    if (headers_.find("Content-Type") == headers_.end()) {
        headers_["Content-Type"] = "application/json";
    }
    std::ostream request_stream(&request_);
    request_stream << "POST " << path << " HTTP/1.0\r\n";
    for (auto &item : headers_) {
        request_stream << item.first << ": " << item.second << "\r\n";
    }
    request_stream << "\r\n";
    request_stream << content;
    
    InitHttpRequest();
    StartHttpRequest(server, port, timeoutMillisecond);
}

void AsyncHttpClient::InitHttpRequest() {
    io_service_.reset(new boost::asio::io_service());
    if (is_https_) {
        ssl_context_.reset(
                           new boost::asio::ssl::context(boost::asio::ssl::context::tlsv13_compatible));
        boost::system::error_code ec;
        ssl_context_->set_options(boost::asio::ssl::context::default_workarounds |
                                  boost::asio::ssl::context::no_sslv2 |
                                  boost::asio::ssl::context::single_dh_use,
                                  ec);
        ssl_socket_.reset(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(
                                                                                     boost::ref(*io_service_), boost::ref(*ssl_context_)));
    } else {
        socket_.reset(new boost::asio::ip::tcp::socket(*io_service_));
    }
    resolver_.reset(new boost::asio::ip::tcp::resolver(*io_service_));
    timer_.reset(new boost::asio::deadline_timer(*io_service_));
    response_headers_.clear();
}

void AsyncHttpClient::UninitHttpRequest() {
    if (thread_ && thread_->joinable()) {
        thread_->join();
    }
    timer_.reset();
    resolver_.reset();
    ssl_socket_.reset();
    socket_.reset();
    ssl_context_.reset();
    io_service_.reset();
    thread_.reset();
    response_headers_.clear();
}

void AsyncHttpClient::StartHttpRequest(const std::string &server, const std::string &port,
                                       int timeoutMill) {
    std::lock_guard<std::mutex> guard(mutext_);
    boost::asio::ip::tcp::resolver::query query(server, port);
    resolver_->async_resolve(query, boost::bind(&AsyncHttpClient::HandleResolve, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::iterator));
    thread_.reset(new std::thread([this]() { io_service_->run(); }));
    // Set timeout cancel
    timer_->expires_from_now(boost::posix_time::milliseconds(timeoutMill));
    timer_->async_wait(
                       std::bind(&AsyncHttpClient::TimeoutHandler, this, std::placeholders::_1));
    started_ = true;
}

void AsyncHttpClient::StopHttpRequest() {
    std::lock_guard<std::mutex> guard(mutext_);
    if (started_) {
        io_service_->stop();
        timer_->cancel();
        resolver_->cancel();
        boost::system::error_code ec;
        if (is_https_) {
            if (ssl_socket_ && ssl_socket_->lowest_layer().is_open()) {
                ssl_socket_->lowest_layer().close(ec);
            }
        } else {
            socket_->close(ec);
        }
        started_ = false;
    }
}

void AsyncHttpClient::UrlParse(const std::string &url, std::string *server, std::string *port,
                               std::string *path) {
    std::string tmp;
    is_https_ = false;
    if (url.find("http://") == 0) {
        tmp = url.substr(7);
        *port = "80";
    } else if (url.find("https://") == 0) {
        tmp = url.substr(8);
        *port = "443";
        is_https_ = true;
    } else {
        tmp = url;
        *port = "80";
    }
    
    size_t idx = tmp.find('/');
    if (idx == std::string::npos) {
        idx = tmp.length();
        *path = "/";
    } else {
        *path = tmp.substr(idx);
    }
    *server = tmp.substr(0, idx);
    idx = (*server).find(':');
    if (idx != std::string::npos) {
        *port = (*server).substr(idx + 1);
        *server = (*server).substr(0, idx);
    }
}

void AsyncHttpClient::HandleResolve(
                                    const boost::system::error_code &err,
                                    boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleResolve";
    if (!err) {
        if (is_https_) {
            std::lock_guard<std::mutex> guard(mutext_);
            boost::asio::async_connect(ssl_socket_->lowest_layer(), endpoint_iterator,
                                       boost::bind(&AsyncHttpClient::HandleConnect, this,
                                                   boost::asio::placeholders::error));
        } else {
            boost::asio::async_connect(*socket_, endpoint_iterator,
                                       boost::bind(&AsyncHttpClient::HandleConnect, this,
                                                   boost::asio::placeholders::error));
        }
    } else {
        InvokeCallback(err.value(), err.message());
    }
}

void AsyncHttpClient::HandleConnect(const boost::system::error_code &err) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleConnect";
    if (!err) {
        if (is_https_) {
            std::lock_guard<std::mutex> guard(mutext_);
            if (!ssl_socket_->lowest_layer().is_open()) {
                return;
            }
            // set server host to enable tls1.3
            SSL_set_tlsext_host_name(ssl_socket_->native_handle(), headers_["Host"].c_str());
            boost::asio::ip::tcp::no_delay option(true);
            ssl_socket_->lowest_layer().set_option(option);
            ssl_socket_->async_handshake(boost::asio::ssl::stream_base::client,
                                         boost::bind(&AsyncHttpClient::HandleHandshake, this,
                                                     boost::asio::placeholders::error));
        } else {
            HandleHandshake(err);
        }
    } else {
        InvokeCallback(err.value(), err.message());
    }
}

void AsyncHttpClient::HandleHandshake(const boost::system::error_code &err) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleHandshake";
    if (!err) {
        if (is_https_) {
            boost::asio::async_write(*ssl_socket_, request_,
                                     boost::bind(&AsyncHttpClient::HandleWriteRequest, this,
                                                 boost::asio::placeholders::error));
        } else {
            boost::asio::async_write(*socket_, request_,
                                     boost::bind(&AsyncHttpClient::HandleWriteRequest, this,
                                                 boost::asio::placeholders::error));
        }
    } else {
        InvokeCallback(err.value(), err.message());
    }
}

void AsyncHttpClient::HandleWriteRequest(const boost::system::error_code &err) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleWriteRequest";
    if (!err) {
        if (is_https_) {
            boost::asio::async_read_until(*ssl_socket_, response_, "\r\n",
                                          boost::bind(&AsyncHttpClient::HandleReadStatusLine,
                                                      this, boost::asio::placeholders::error));
        } else {
            boost::asio::async_read_until(*socket_, response_, "\r\n",
                                          boost::bind(&AsyncHttpClient::HandleReadStatusLine,
                                                      this, boost::asio::placeholders::error));
        }
        
    } else {
        InvokeCallback(err.value(), err.message());
    }
}

void AsyncHttpClient::HandleReadStatusLine(const boost::system::error_code &err) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleReadStatusLine";
    if (!err) {
        std::istream response_stream(&response_);
        std::string http_version;
        response_stream >> http_version;
        response_stream >> code_;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            // std::cout << "Invalid response\n";
            return;
        }
        if (is_https_) {
            boost::asio::async_read_until(*ssl_socket_, response_, "\r\n\r\n",
                                          boost::bind(&AsyncHttpClient::HandleReadHeaders, this,
                                                      boost::asio::placeholders::error));
        } else {
            boost::asio::async_read_until(*socket_, response_, "\r\n\r\n",
                                          boost::bind(&AsyncHttpClient::HandleReadHeaders, this,
                                                      boost::asio::placeholders::error));
        }
    } else {
        InvokeCallback(err.value(), err.message());
    }
}

void AsyncHttpClient::HandleReadHeaders(const boost::system::error_code &err) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleReadHeaders";
    if (!err) {
        std::istream response_stream(&response_);
        std::string tmp;
        response_headers_.clear();
        while (std::getline(response_stream, tmp) && tmp != "\r") {
            size_t idx = tmp.find(':');
            if (idx != std::string::npos) {
                std::string key = tmp.substr(0, idx);
                std::string value = tmp.substr(idx + 2, tmp.size() - idx - 3);
                response_headers_[key] = value;
                if (key == "Content-Length") {
                    content_length_ = atoi(value.c_str());
                }
            }
        }
        std::size_t size = response_.data().size();
        if (size != 0) {
            curr_content_length_ = size;
        }
        if (curr_content_length_ == content_length_ && curr_content_length_ > 0) {
            std::string content(boost::asio::buffers_begin(response_.data()),
                                boost::asio::buffers_end(response_.data()));
            InvokeCallback(code_, std::move(content));
        } else {
            if (is_https_) {
                boost::asio::async_read(*ssl_socket_, response_, boost::asio::transfer_at_least(1),
                                        boost::bind(&AsyncHttpClient::HandleReadContent, this,
                                                    boost::asio::placeholders::error));
            } else {
                boost::asio::async_read(*socket_, response_, boost::asio::transfer_at_least(1),
                                        boost::bind(&AsyncHttpClient::HandleReadContent, this,
                                                    boost::asio::placeholders::error));
            }
        }
    } else {
        InvokeCallback(err.value(), err.message());
    }
}

void AsyncHttpClient::HandleReadContent(const boost::system::error_code &err) {
    RTC_LOG(LS_INFO) << "AsyncHttpClient::HandleReadContent";
    if (!err) {
        std::size_t size = response_.data().size();
        if (size != 0) {
            curr_content_length_ = size;
        }
        if (curr_content_length_ == content_length_ && curr_content_length_ > 0) {
            std::string content(boost::asio::buffers_begin(response_.data()),
                                boost::asio::buffers_end(response_.data()));
            InvokeCallback(code_, std::move(content));
        } else {
            if (is_https_) {
                boost::asio::async_read(*ssl_socket_, response_, boost::asio::transfer_at_least(1),
                                        boost::bind(&AsyncHttpClient::HandleReadContent, this,
                                                    boost::asio::placeholders::error));
            } else {
                boost::asio::async_read(*socket_, response_, boost::asio::transfer_at_least(1),
                                        boost::bind(&AsyncHttpClient::HandleReadContent, this,
                                                    boost::asio::placeholders::error));
            }
        }
    } else if (err != boost::asio::error::eof &&
               err != boost::asio::ssl::error::stream_truncated) {
        InvokeCallback(err.value(), err.message());
    } else {
        std::string content(boost::asio::buffers_begin(response_.data()),
                            boost::asio::buffers_end(response_.data()));
        InvokeCallback(code_, std::move(content));
    }
}

void AsyncHttpClient::InvokeCallback(int code, std::string data) {
    if (started_) {
        callback_(code, response_headers_, data);
    }
    StopHttpRequest();
}

}  // namespace vewtn

/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <thread>

namespace vewtn {

class IHttpAsyncClient {
public:
    // 用户需要实现该回调方法，用于异步请求之后的回调
    using HttpCallBack = std::function<void(int code,
                                            std::map<std::string, std::string> headers,
                                            std::string data)>;
    
    // 当前支持的HttpClient实现
    enum class HttpClientTypes {
        kDefault = 0, kOriginalHttpClient = 1, kTTNetHttpClient = 2, kUpperHttpClient = 3
    };
    
    // Http Client返回的错误码，会返回在Http Code里面
    enum HttpRequestError {
        kTimeout = -10011, kCanceled = -10012, kDestroyed = -10013, kFailed = -10014
    };
    
    /** 析构方法，释放HttpClient申请的资源，并取消所有未完成的异步请求
     * 注意：不要在同步请求未完成的情况下进行析构，否则会有未知风险
     **/
    virtual ~IHttpAsyncClient() = default;
    
    // 异步的Get方法
    virtual void Get(
                     const std::string &url,
                     HttpCallBack callback = [](int code, std::map<std::string, std::string> headers,
                                                std::string data) {},
                     int timeoutMillisecond = 5000) = 0;
    
    // 异步的Post方法
    virtual void Post(
                      const std::string &url, const std::string &content,
                      HttpCallBack callback = [](int code, std::map<std::string, std::string> headers,
                                                 std::string data) {}, int timeoutMillisecond = 5000) = 0;
    
    // 异步的Delete方法
    virtual void Delete(
                        const std::string &url,
                        HttpCallBack callback = [](int code, std::map<std::string, std::string> headers,
                                                   std::string data) {},
                        int timeoutMillisecond = 5000) = 0;
    
    // 取消当前正在进行的异步调用
    virtual void CancelAsyncTasks() = 0;
    
    // 获取当前的HttpClient实现
    virtual IHttpAsyncClient::HttpClientTypes GetCurrentType() = 0;
};

class AsyncHttpClient : public IHttpAsyncClient {
public:
    AsyncHttpClient();
    
    virtual ~AsyncHttpClient();
    
    void AddHeader(const std::string &key, const std::string &value);
    
    void
    Get(const std::string &url, HttpCallBack callback, int timeoutMillisecond = 5000) override;
    
    void Post(const std::string &url, const std::string &content, HttpCallBack callback,
              int timeoutMillisecond = 5000) override;
    
    void Delete(const std::string &url, HttpCallBack callback,
                int timeoutMillisecond = 5000) override;
    
    void Get(const std::string &urlWithIP, const std::string &host, HttpCallBack callback,
             int timeoutMillisecond = 5000);
    
    void Post(const std::string &urlWithIP, const std::string &host, const std::string &content,
              HttpCallBack callback,
              int timeoutMillisecond = 5000);
    
    void CancelAsyncTasks() override;
    
    IHttpAsyncClient::HttpClientTypes GetCurrentType() override;
    
private:
    void InitHttpRequest();
    
    void UninitHttpRequest();
    
    void StartHttpRequest(const std::string &server, const std::string &port, int timeoutMill);
    
    void StopHttpRequest();
    
    void GetImpl(const std::string &server, const std::string &host, const std::string &path,
                 const std::string &port,
                 int timeoutMillisecond);
    
    void DeleteImpl(const std::string &server, const std::string &host, const std::string &path,
                    const std::string &port, int timeoutMillisecond);
    
    void PostImpl(const std::string &server, const std::string &host, const std::string &path,
                  const std::string &port,
                  const std::string &content, int timeoutMillisecond);
    
    void
    UrlParse(const std::string &url, std::string *server, std::string *port, std::string *path);
    
    void HandleResolve(
                       const boost::system::error_code &err,
                       boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    
    void HandleConnect(const boost::system::error_code &err);
    
    void HandleHandshake(const boost::system::error_code &e);
    
    void HandleWriteRequest(const boost::system::error_code &err);
    
    void HandleReadStatusLine(const boost::system::error_code &err);
    
    void HandleReadHeaders(const boost::system::error_code &err);
    
    void HandleReadContent(const boost::system::error_code &err);
    
    void TimeoutHandler(const boost::system::error_code &error);
    
    void InvokeCallback(int code, std::string data);
    
private:
    bool started_ = false;
    std::mutex mutext_;
    HttpCallBack callback_;
    std::map<std::string, std::string> headers_;
    std::map<std::string, std::string> response_headers_;
    int code_ = 0;
    bool is_https_ = false;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
    std::unique_ptr<std::thread> thread_;
    std::unique_ptr<boost::asio::io_service> io_service_;
    std::unique_ptr<boost::asio::ip::tcp::socket> socket_;
    std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> ssl_socket_;
    std::unique_ptr<boost::asio::ip::tcp::resolver> resolver_;
    std::unique_ptr<boost::asio::ssl::context> ssl_context_;
    std::unique_ptr<boost::asio::deadline_timer> timer_;
};

}  // namespace vewtn

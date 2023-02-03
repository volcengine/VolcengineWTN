/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include <stddef.h>
#include <stdint.h>

#include <memory>
#include <vector>
#include <queue>

#include "wtn_interface.h"

#include "api/fec_controller_override.h"
#include "api/sequence_checker.h"
#include "api/task_queue/task_queue_factory.h"
#include "api/video/encoded_image.h"
#include "api/video/video_bitrate_allocation.h"
#include "api/video/video_frame.h"
#include "api/video_codecs/video_codec.h"
#include "api/video_codecs/video_encoder.h"
#include "modules/video_coding/include/video_codec_interface.h"
#include "rtc_base/synchronization/mutex.h"
#include "rtc_base/thread_annotations.h"
#include "system_wrappers/include/clock.h"
#include "modules/video_coding/codecs/h264/include/h264.h"
#include "api/video_codecs/video_encoder_factory.h"
#include "rtc_base/timestamp_aligner.h"

namespace vewtn {

class FunctionVideoEncoderFactory final : public webrtc::VideoEncoderFactory {
public:
    explicit FunctionVideoEncoderFactory(std::function<std::unique_ptr<webrtc::VideoEncoder>()> create)
        : create_([create = std::move(create)](const webrtc::SdpVideoFormat&) {
        return create();
    }) {
    }
    explicit FunctionVideoEncoderFactory(std::function<std::unique_ptr<webrtc::VideoEncoder>(const webrtc::SdpVideoFormat&)>
                                         create)
        : create_(std::move(create)) {
    }
    
    std::vector<webrtc::SdpVideoFormat> GetSupportedFormats() const override {
        std::vector<webrtc::SdpVideoFormat> supported_codecs;
        std::vector<webrtc::SdpVideoFormat> formats;
        webrtc::SdpVideoFormat h264_constrained_baseline("H264");
        h264_constrained_baseline.parameters[cricket::kH264FmtpLevelAsymmetryAllowed] = "1";
        h264_constrained_baseline.parameters[cricket::kH264FmtpPacketizationMode] = "1";
        h264_constrained_baseline.parameters[cricket::kH264FmtpProfileLevelId] =
            *webrtc::H264ProfileLevelIdToString(webrtc::H264ProfileLevelId(webrtc::H264Profile::kProfileConstrainedBaseline, webrtc::H264Level::kLevel3_1));
        webrtc::SdpVideoFormat h264_constrained_high("H264");
        h264_constrained_high.parameters[cricket::kH264FmtpLevelAsymmetryAllowed] = "1";
        h264_constrained_high.parameters[cricket::kH264FmtpPacketizationMode] = "1";
        h264_constrained_high.parameters[cricket::kH264FmtpProfileLevelId] =
            *webrtc::H264ProfileLevelIdToString(webrtc::H264ProfileLevelId(webrtc::H264Profile::kProfileConstrainedHigh, webrtc::H264Level::kLevel3_1));
        webrtc::SdpVideoFormat h264_high("H264");
        h264_high.parameters[cricket::kH264FmtpLevelAsymmetryAllowed] = "1";
        h264_high.parameters[cricket::kH264FmtpPacketizationMode] = "1";
        h264_high.parameters[cricket::kH264FmtpProfileLevelId] =
            *webrtc::H264ProfileLevelIdToString(webrtc::H264ProfileLevelId(webrtc::H264Profile::kProfileHigh, webrtc::H264Level::kLevel3_1));
        formats.push_back(h264_constrained_baseline);
        formats.push_back(h264_constrained_high);
        formats.push_back(h264_high);
        for (const webrtc::SdpVideoFormat& format : formats) {
            supported_codecs.push_back(format);
        }
        return supported_codecs;
    }
    
    std::unique_ptr<webrtc::VideoEncoder> CreateVideoEncoder(const webrtc::SdpVideoFormat& format) override {
        return create_(format);
    }
    
private:
    const std::function<std::unique_ptr<webrtc::VideoEncoder>(const webrtc::SdpVideoFormat&)>
    create_;
};

class FakeEncoder : public webrtc::VideoEncoder {
public:
    explicit FakeEncoder(webrtc::Clock* clock);
    virtual ~FakeEncoder();
    
    // Sets max bitrate. Not thread-safe, call before registering the encoder.
    void SetMaxBitrate(int max_kbps) RTC_LOCKS_EXCLUDED(mutex_);
    void SetQp(int qp) RTC_LOCKS_EXCLUDED(mutex_);
    
    void SetFecControllerOverride(
            webrtc::FecControllerOverride* fec_controller_override) override;
    
    int32_t InitEncode(const webrtc::VideoCodec* config, const Settings& settings)
    RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t Encode(const webrtc::VideoFrame& input_image,
                   const std::vector<webrtc::VideoFrameType>* frame_types)
    RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t RegisterEncodeCompleteCallback(webrtc::EncodedImageCallback* callback)
    RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t Release() override;
    void SetRates(const RateControlParameters& parameters)
    RTC_LOCKS_EXCLUDED(mutex_) override;
    EncoderInfo GetEncoderInfo() const override;
    
    int GetConfiguredInputFramerate() const RTC_LOCKS_EXCLUDED(mutex_);
    int GetNumInitializations() const RTC_LOCKS_EXCLUDED(mutex_);
    const webrtc::VideoCodec& config() const RTC_LOCKS_EXCLUDED(mutex_);
    
    bool PushEncodedVideoFrame(EncodedVideoFrame h264_encoded_frame);
    
    static const char* kImplementationName;
    
protected:
    struct FrameInfo {
        bool keyframe;
        struct SpatialLayer {
            SpatialLayer() = default;
            SpatialLayer(int size, int temporal_id)
            : size(size), temporal_id(temporal_id) {}
            // Size of a current frame in the layer.
            int size = 0;
            // Temporal index of a current frame in the layer.
            int temporal_id = 0;
        };
        std::vector<SpatialLayer> layers;
    };

    void SetRatesLocked(const RateControlParameters& parameters)
    RTC_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
    
    FrameInfo last_frame_info_ RTC_GUARDED_BY(mutex_);
    webrtc::Clock* const clock_;

    webrtc::VideoCodec config_ RTC_GUARDED_BY(mutex_);
    int num_initializations_ RTC_GUARDED_BY(mutex_);
    webrtc::EncodedImageCallback* callback_ RTC_GUARDED_BY(mutex_);
    RateControlParameters current_rate_settings_ RTC_GUARDED_BY(mutex_);
    int max_target_bitrate_kbps_ RTC_GUARDED_BY(mutex_);
    bool pending_keyframe_ RTC_GUARDED_BY(mutex_);
    uint32_t counter_ RTC_GUARDED_BY(mutex_);
    mutable webrtc::Mutex mutex_;
    bool used_layers_[webrtc::kMaxSimulcastStreams];
    absl::optional<int> qp_ RTC_GUARDED_BY(mutex_);
    
    // Current byte debt to be payed over a number of frames.
    // The debt is acquired by keyframes overshooting the bitrate target.
    size_t debt_bytes_;
    std::queue<webrtc::EncodedImage> encoded_images_;
    rtc::TimestampAligner timestamp_aligner_;
};

}  // namespace vewtn



/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "fake_encoder.h"
#include <string.h>
#include <algorithm>
#include <cstdint>
#include <memory>
#include <string>

#include "api/task_queue/queued_task.h"
#include "api/video/video_content_type.h"
#include "modules/video_coding/codecs/h264/include/h264_globals.h"
#include "modules/video_coding/include/video_codec_interface.h"
#include "modules/video_coding/include/video_error_codes.h"
#include "rtc_base/checks.h"
#include "system_wrappers/include/sleep.h"
#include "modules/video_coding/codecs/multiplex/include/augmented_video_frame_buffer.h"
#include "rtc_base/time_utils.h"
#include "rtc_base/logging.h"

namespace vewtn {
namespace {
const int kKeyframeSizeFactor = 5;

// Inverse of proportion of frames assigned to each temporal layer for all
// possible temporal layers numbers.
const int kTemporalLayerRateFactor[4][4] = {
    {1, 0, 0, 0},  // 1/1
    {2, 2, 0, 0},  // 1/2 + 1/2
    {4, 4, 2, 0},  // 1/4 + 1/4 + 1/2
    {8, 8, 4, 2},  // 1/8 + 1/8 + 1/4 + 1/2
};

void WriteCounter(unsigned char* payload, uint32_t counter) {
    payload[0] = (counter & 0x00FF);
    payload[1] = (counter & 0xFF00) >> 8;
    payload[2] = (counter & 0xFF0000) >> 16;
    payload[3] = (counter & 0xFF000000) >> 24;
}

}  // namespace

FakeEncoder::FakeEncoder(webrtc::Clock* clock)
    : clock_(clock),
    num_initializations_(0),
    callback_(nullptr),
    max_target_bitrate_kbps_(-1),
    pending_keyframe_(true),
    counter_(0),
    debt_bytes_(0) {
    for (bool& used : used_layers_) {
        used = false;
    }
}

FakeEncoder::~FakeEncoder() {
    callback_ = nullptr;
}

void FakeEncoder::SetFecControllerOverride(
        webrtc::FecControllerOverride* fec_controller_override) {
    // Ignored.
}

void FakeEncoder::SetMaxBitrate(int max_kbps) {
    RTC_DCHECK_GE(max_kbps, -1);  // max_kbps == -1 disables it.
    webrtc::MutexLock lock(&mutex_);
    max_target_bitrate_kbps_ = max_kbps;
    SetRatesLocked(current_rate_settings_);
}

void FakeEncoder::SetQp(int qp) {
    webrtc::MutexLock lock(&mutex_);
    qp_ = qp;
}

int32_t FakeEncoder::InitEncode(const webrtc::VideoCodec* config,
                                const Settings& settings) {
    webrtc::MutexLock lock(&mutex_);
    config_ = *config;
    ++num_initializations_;
    current_rate_settings_.bitrate.SetBitrate(0, 0, config_.startBitrate * 1000);
    current_rate_settings_.framerate_fps = config_.maxFramerate;
    pending_keyframe_ = true;
    last_frame_info_ = FrameInfo();
    return 0;
}

int32_t FakeEncoder::Encode(const webrtc::VideoFrame& input_image,
                            const std::vector<webrtc::VideoFrameType>* frame_types) {
    return 0;

}

bool FakeEncoder::PushEncodedVideoFrame(EncodedVideoFrame h264_encoded_frame) {
    if (!callback_) {
        return false;
    }
    auto buffer = webrtc::EncodedImageBuffer::Create(h264_encoded_frame.size);
    webrtc::EncodedImage encoded;
    encoded.SetEncodedData(buffer);

    webrtc::VideoRotation rotation_;
    switch (h264_encoded_frame.rotation) {
        case 0:
            rotation_ = webrtc::kVideoRotation_0;
            break;
        case 90:
            rotation_ = webrtc::kVideoRotation_90;
            break;
        case 180:
            rotation_ = webrtc::kVideoRotation_180;
            break;
        case 270:
            rotation_ = webrtc::kVideoRotation_270;
            break;
        default:
            rotation_ = webrtc::kVideoRotation_0;
            break;
    }

    encoded.SetTimestamp(h264_encoded_frame.timestamp_ms);
    encoded._frameType = h264_encoded_frame.is_key_frame ? webrtc::VideoFrameType::kVideoFrameKey : webrtc::VideoFrameType::kVideoFrameDelta;
    encoded._encodedWidth = h264_encoded_frame.width;
    encoded._encodedHeight = h264_encoded_frame.height;
    encoded.rotation_ = rotation_;
    encoded.SetSpatialIndex(0);

    memcpy(buffer->data(), h264_encoded_frame.data, h264_encoded_frame.size);
    encoded.set_size(h264_encoded_frame.size);

    webrtc::CodecSpecificInfo codec_specific;
    codec_specific.codecType = webrtc::kVideoCodecH264;
    codec_specific.codecSpecific.H264.packetization_mode = webrtc::H264PacketizationMode::NonInterleaved;

    if (callback_->OnEncodedImage(encoded, &codec_specific).error != webrtc::EncodedImageCallback::Result::OK) {
        return false;
    }

    return true;
}

int32_t FakeEncoder::RegisterEncodeCompleteCallback(
        webrtc::EncodedImageCallback* callback) {
    webrtc::MutexLock lock(&mutex_);
    callback_ = callback;
    return 0;
}

int32_t FakeEncoder::Release() {
    return 0;
}

void FakeEncoder::SetRates(const RateControlParameters& parameters) {
    webrtc::MutexLock lock(&mutex_);
    SetRatesLocked(parameters);
}

void FakeEncoder::SetRatesLocked(const RateControlParameters& parameters) {
    current_rate_settings_ = parameters;
    int allocated_bitrate_kbps = parameters.bitrate.get_sum_kbps();
    
    // Scale bitrate allocation to not exceed the given max target bitrate.
    if (max_target_bitrate_kbps_ > 0 &&
        allocated_bitrate_kbps > max_target_bitrate_kbps_) {
        for (uint8_t spatial_idx = 0; spatial_idx < webrtc::kMaxSpatialLayers;
             ++spatial_idx) {
            for (uint8_t temporal_idx = 0; temporal_idx < webrtc::kMaxTemporalStreams;
                 ++temporal_idx) {
                if (current_rate_settings_.bitrate.HasBitrate(spatial_idx,
                                                              temporal_idx)) {
                    uint32_t bitrate = current_rate_settings_.bitrate.GetBitrate(
                                                                                 spatial_idx, temporal_idx);
                    bitrate = static_cast<uint32_t>(
                                                    (bitrate * int64_t{max_target_bitrate_kbps_}) /
                                                    allocated_bitrate_kbps);
                    current_rate_settings_.bitrate.SetBitrate(spatial_idx, temporal_idx,
                                                              bitrate);
                }
            }
        }
    }
}

const char* FakeEncoder::kImplementationName = "fake_encoder";
webrtc::VideoEncoder::EncoderInfo FakeEncoder::GetEncoderInfo() const {
    EncoderInfo info;
    info.implementation_name = kImplementationName;
        webrtc::MutexLock lock(&mutex_);
    for (int sid = 0; sid < config_.numberOfSimulcastStreams; ++sid) {
        int number_of_temporal_layers =
        config_.simulcastStream[sid].numberOfTemporalLayers;
        info.fps_allocation[sid].clear();
        for (int tid = 0; tid < number_of_temporal_layers; ++tid) {
            // {1/4, 1/2, 1} allocation for num layers = 3.
            info.fps_allocation[sid].push_back(255 /
                                               (number_of_temporal_layers - tid));
        }
    }
    return info;
}

int FakeEncoder::GetConfiguredInputFramerate() const {
    webrtc::MutexLock lock(&mutex_);
    return static_cast<int>(current_rate_settings_.framerate_fps + 0.5);
}

int FakeEncoder::GetNumInitializations() const {
    webrtc::MutexLock lock(&mutex_);
    return num_initializations_;
}

const webrtc::VideoCodec& FakeEncoder::config() const {
    webrtc::MutexLock lock(&mutex_);
    return config_;
}

}  // namespace


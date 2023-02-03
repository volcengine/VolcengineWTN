
/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "fake_decoder.h"

namespace vewtn {
FakeDecoder::FakeDecoder(IRtcEventHandler* event_handler, const std::string& stream_id) 
    : callback_(nullptr),
      event_handler_(event_handler),
      stream_id_(stream_id) {}

bool FakeDecoder::Configure(const Settings& settings) {
  return true;
}

int32_t FakeDecoder::Decode(const webrtc::EncodedImage& input,
                bool missing_frames,
                int64_t render_time_ms) {
    if (event_handler_) {
        EncodedVideoFrame encodedFrame;
        encodedFrame.data = (uint8_t *)input.data();
        encodedFrame.size = input.size();
        if (input._encodedWidth != 0) {
            width_ = input._encodedWidth;
        }
        if (input._encodedHeight != 0) {
            height_ = input._encodedHeight;
        }
        encodedFrame.width = width_;
        encodedFrame.height = height_;
        encodedFrame.timestamp_ms = render_time_ms;
        encodedFrame.rotation = input.rotation_;
        encodedFrame.is_key_frame = input._frameType == webrtc::VideoFrameType::kVideoFrameKey ? true : false;
        event_handler_->OnEncodedVideoFrame(stream_id_.c_str(), encodedFrame);
    }
    return 0;
}

int32_t FakeDecoder::RegisterDecodeCompleteCallback(webrtc::DecodedImageCallback* callback) {
    callback_ = callback;
    return WEBRTC_VIDEO_CODEC_OK;
}

int32_t FakeDecoder::Release() {
    return WEBRTC_VIDEO_CODEC_OK;
}

webrtc::VideoDecoder::DecoderInfo FakeDecoder::GetDecoderInfo() const {
    DecoderInfo info;
    info.implementation_name = "FAKE_DECODER";
    info.is_hardware_accelerated = false;
    return info;
}

const char* FakeDecoder::ImplementationName() const {
    return "FAKE_DECODER";
}

}    // end of namespace

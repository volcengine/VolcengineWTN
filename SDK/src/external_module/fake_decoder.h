/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "api/video_codecs/video_decoder.h"
#include "api/video_codecs/video_decoder_factory.h"
#include "modules/video_coding/codecs/h264/include/h264.h"

namespace vewtn {

class FunctionVideoDecoderFactory final : public  webrtc::VideoDecoderFactory {
 public:
  explicit FunctionVideoDecoderFactory(
      std::function<std::unique_ptr<webrtc::VideoDecoder>()> create)
      : create_([create = std::move(create)](const  webrtc::SdpVideoFormat&) {
          return create();
        }) {}
  explicit FunctionVideoDecoderFactory(
      std::function<std::unique_ptr< webrtc::VideoDecoder>(const  webrtc::SdpVideoFormat&)>
          create)
      : create_(std::move(create)) {}
  FunctionVideoDecoderFactory(
      std::function<std::unique_ptr<webrtc::VideoDecoder>()> create,
      std::vector<webrtc::SdpVideoFormat> sdp_video_formats)
      : create_([create = std::move(create)](const  webrtc::SdpVideoFormat&) {
          return create();
        }),
        sdp_video_formats_(std::move(sdp_video_formats)) {}

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

  std::unique_ptr<webrtc::VideoDecoder> CreateVideoDecoder(const webrtc::SdpVideoFormat& format) override {
    return create_(format);
  }

 private:
  const std::function<std::unique_ptr<webrtc::VideoDecoder>(const webrtc::SdpVideoFormat&)> create_;
  const std::vector<webrtc::SdpVideoFormat> sdp_video_formats_;
};

class FakeDecoder : public webrtc::VideoDecoder {
public:
    FakeDecoder(IRtcEventHandler* event_handler, const std::string& stream_id);
    virtual ~FakeDecoder() {}

    bool Configure(const Settings& settings) override;

    int32_t Decode(const webrtc::EncodedImage& input,
                    bool missing_frames,
                    int64_t render_time_ms) override;

    int32_t RegisterDecodeCompleteCallback(webrtc::DecodedImageCallback* callback) override;

    int32_t Release() override;

    webrtc::VideoDecoder::DecoderInfo GetDecoderInfo() const override;
    const char* ImplementationName() const override;

private:
    webrtc::DecodedImageCallback* callback_;
    IRtcEventHandler* event_handler_;
    std::string stream_id_;
    int width_ = 0;
    int height_ = 0;
};

}    // end of namespace

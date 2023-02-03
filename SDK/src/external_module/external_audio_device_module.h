/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#pragma once
#include "wtn_interface.h"
#include "modules/audio_device/include/audio_device.h"
#include "rtc_base/message_handler.h"
#include "rtc_base/thread.h"
#include "rtc_base/timestamp_aligner.h"
#include "rtc_base/buffer.h"

namespace vewtn {

class ExternalAudioDeviceModule : public webrtc::AudioDeviceModule,
                                  public rtc::MessageHandlerAutoCleanup {
public:
    typedef uint16_t Sample;
    static const size_t kNumberBytesPerSample = sizeof(Sample);

    static rtc::scoped_refptr<ExternalAudioDeviceModule> Create(IRtcEventHandler* event_handler, const std::string& stream_id);

    int32_t ActiveAudioLayer(AudioLayer* audio_layer) const override;
    
    // Note: Calling this method from a callback may result in deadlock.
    int32_t RegisterAudioCallback(webrtc::AudioTransport* audio_callback) override RTC_LOCKS_EXCLUDED(mutex_);
    
    int32_t Init() override;
    int32_t Terminate() override;
    bool Initialized() const override;
    
    int16_t PlayoutDevices() override;
    int16_t RecordingDevices() override;
    int32_t PlayoutDeviceName(uint16_t index,
                              char name[webrtc::kAdmMaxDeviceNameSize],
                              char guid[webrtc::kAdmMaxGuidSize]) override;
    int32_t RecordingDeviceName(uint16_t index,
                                char name[webrtc::kAdmMaxDeviceNameSize],
                                char guid[webrtc::kAdmMaxGuidSize]) override;
    
    int32_t SetPlayoutDevice(uint16_t index) override;
    int32_t SetPlayoutDevice(WindowsDeviceType device) override;
    int32_t SetRecordingDevice(uint16_t index) override;
    int32_t SetRecordingDevice(WindowsDeviceType device) override;
    
    int32_t PlayoutIsAvailable(bool* available) override;
    int32_t InitPlayout() override;
    bool PlayoutIsInitialized() const override;
    int32_t RecordingIsAvailable(bool* available) override;
    int32_t InitRecording() override;
    bool RecordingIsInitialized() const override;
    
    int32_t StartPlayout() RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t StopPlayout() RTC_LOCKS_EXCLUDED(mutex_) override;
    bool Playing() const RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t StartRecording() RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t StopRecording() RTC_LOCKS_EXCLUDED(mutex_) override;
    bool Recording() const RTC_LOCKS_EXCLUDED(mutex_) override;
    
    int32_t InitSpeaker() override;
    bool SpeakerIsInitialized() const override;
    int32_t InitMicrophone() override;
    bool MicrophoneIsInitialized() const override;
    
    int32_t SpeakerVolumeIsAvailable(bool* available) override;
    int32_t SetSpeakerVolume(uint32_t volume) override;
    int32_t SpeakerVolume(uint32_t* volume) const override;
    int32_t MaxSpeakerVolume(uint32_t* max_volume) const override;
    int32_t MinSpeakerVolume(uint32_t* min_volume) const override;
    
    int32_t MicrophoneVolumeIsAvailable(bool* available) override;
    int32_t SetMicrophoneVolume(uint32_t volume)
    RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t MicrophoneVolume(uint32_t* volume) const
    RTC_LOCKS_EXCLUDED(mutex_) override;
    int32_t MaxMicrophoneVolume(uint32_t* max_volume) const override;
    
    int32_t MinMicrophoneVolume(uint32_t* min_volume) const override;
    
    int32_t SpeakerMuteIsAvailable(bool* available) override;
    int32_t SetSpeakerMute(bool enable) override;
    int32_t SpeakerMute(bool* enabled) const override;
    
    int32_t MicrophoneMuteIsAvailable(bool* available) override;
    int32_t SetMicrophoneMute(bool enable) override;
    int32_t MicrophoneMute(bool* enabled) const override;
    
    int32_t StereoPlayoutIsAvailable(bool* available) const override;
    int32_t SetStereoPlayout(bool enable) override;
    int32_t StereoPlayout(bool* enabled) const override;
    int32_t StereoRecordingIsAvailable(bool* available) const override;
    int32_t SetStereoRecording(bool enable) override;
    int32_t StereoRecording(bool* enabled) const override;
    
    int32_t PlayoutDelay(uint16_t* delay_ms) const override;
    
    bool BuiltInAECIsAvailable() const override { return false; }
    int32_t EnableBuiltInAEC(bool enable) override { return -1; }
    bool BuiltInAGCIsAvailable() const override { return false; }
    int32_t EnableBuiltInAGC(bool enable) override { return -1; }
    bool BuiltInNSIsAvailable() const override { return false; }
    int32_t EnableBuiltInNS(bool enable) override { return -1; }
    
    int32_t GetPlayoutUnderrunCount() const override { return -1; }
#if defined(WEBRTC_IOS)
    int GetPlayoutAudioParameters(
                                  webrtc::AudioParameters* params) const override {
                                      return -1;
                                  }
    int GetRecordAudioParameters(webrtc::AudioParameters* params) const override {
        return -1;
    }
#endif  // WEBRTC_IOS
    
    // End of functions inherited from webrtc::AudioDeviceModule.
    
    // The following function is inherited from rtc::MessageHandler.
    void OnMessage(rtc::Message* msg) override;

    bool PushAudioFrame(AudioFrame pcm_audio_frame);

protected:
    ExternalAudioDeviceModule(IRtcEventHandler* event_handler, const std::string& stream_id);
    virtual ~ExternalAudioDeviceModule();

private:
    // Returns true/false depending on if recording or playback has been
    // enabled/started.
    bool ShouldStartProcessing() RTC_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
    
    // Starts or stops the pushing and pulling of audio frames.
    void UpdateProcessing(bool start) RTC_LOCKS_EXCLUDED(mutex_);
    
    // Starts the periodic calling of ProcessFrame() in a thread safe way.
    void StartProcessP();
    // Periodcally called function that ensures that frames are pulled and pushed
    // periodically if enabled/started.
    void ProcessFrameP() RTC_LOCKS_EXCLUDED(mutex_);
    // Pulls frames from the registered webrtc::AudioTransport.
    void ReceiveFrameP() RTC_EXCLUSIVE_LOCKS_REQUIRED(mutex_);
    // Pushes frames to the registered webrtc::AudioTransport.
    void SendFrameP() RTC_EXCLUSIVE_LOCKS_REQUIRED(mutex_);

private:
    // Callback for playout and recording.
    webrtc::AudioTransport* audio_callback_ RTC_GUARDED_BY(mutex_);
    
    bool recording_ RTC_GUARDED_BY(mutex_);  // True when audio is being pushed from the instance.
    bool playing_ RTC_GUARDED_BY(mutex_);  // True when audio is being pulled by the instance.
    
    bool play_is_initialized_;  // True when the instance is ready to pull audio.
    bool rec_is_initialized_;   // True when the instance is ready to push audio.
    // Input to and output from RecordedDataIsAvailable(..) makes it possible to
    // modify the current mic level. The implementation does not care about the
    // mic level so it just feeds back what it receives.
    uint32_t current_mic_level_ RTC_GUARDED_BY(mutex_);
    
    // next_frame_time_ is updated in a non-drifting manner to indicate the next
    // wall clock time the next frame should be generated and received. started_
    // ensures that next_frame_time_ can be initialized properly on first call.
    bool started_ RTC_GUARDED_BY(mutex_);
    int64_t next_frame_time_ RTC_GUARDED_BY(process_thread_checker_);

    std::unique_ptr<rtc::Thread> process_thread_;
    // Protects variables that are accessed from process_thread_ and
    // the main thread.
    mutable webrtc::Mutex mutex_;
    webrtc::SequenceChecker process_thread_checker_;

    IRtcEventHandler* event_handler_;
    std::string stream_id_;

    rtc::BufferT<char> record_buffer_;
    rtc::TimestampAligner timestamp_aligner_;
    int64_t capture_timestamp_us_ = 0;
    int64_t last_capture_timestamp_us_ = 0;

    std::vector<int16_t> playout_buffer_;
};

}    // end of namespace


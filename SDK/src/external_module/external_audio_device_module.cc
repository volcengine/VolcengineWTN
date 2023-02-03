/*
 *  Copyright 2022 Beijing Volcanoengine Technology Ltd. All Rights Reserved.
 *
 *  SPDX-license-identifier: BSD-3-Clause
 */

#include "external_audio_device_module.h"
#include "rtc_base/ref_counted_object.h"
#include "rtc_base/time_utils.h"
#include "rtc_base/logging.h"

namespace vewtn {

static const int kHighSampleValue = 10000;

// Constants here are derived by running VoE using a real ADM.
// The constants correspond to 10ms of mono audio at 44kHz.
static const int kTimePerFrameMs = 10;
static uint8_t kNumberOfChannels = 1;
static int kSamplesPerSecond = 44100;
static const int kTotalDelayMs = 0;
static const int kClockDriftMs = 0;
static const uint32_t kMaxVolume = 14392;
static std::string kFilename;
static size_t kNumberSamples = 480;

enum {
    MSG_START_PROCESS,
    MSG_RUN_PROCESS,
};

rtc::scoped_refptr<ExternalAudioDeviceModule> ExternalAudioDeviceModule::Create(IRtcEventHandler* event_handler, const std::string& stream_id) {
    auto capture_module = rtc::make_ref_counted<ExternalAudioDeviceModule>(event_handler, stream_id);
    return capture_module;
}

ExternalAudioDeviceModule::ExternalAudioDeviceModule(IRtcEventHandler* event_handler, const std::string& stream_id)
    : event_handler_(event_handler),
      stream_id_(stream_id) {
    process_thread_checker_.Detach();
    started_ = false;
}

ExternalAudioDeviceModule::~ExternalAudioDeviceModule() {
    if (process_thread_) {
        process_thread_->Stop();
    }
}

int32_t ExternalAudioDeviceModule::ActiveAudioLayer(AudioLayer* audio_layer) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::RegisterAudioCallback(webrtc::AudioTransport* audio_callback) {
    webrtc::MutexLock lock(&mutex_);
    audio_callback_ = audio_callback;
    return 0;
}

int32_t ExternalAudioDeviceModule::Init() {
    return 0;
}

int32_t ExternalAudioDeviceModule::Terminate() {
    return 0;
}

bool ExternalAudioDeviceModule::Initialized() const {
    return 0;
}
    
int16_t ExternalAudioDeviceModule::PlayoutDevices()  {
    return 0;
}

int16_t ExternalAudioDeviceModule::RecordingDevices()  {
    return 0;
}

int32_t ExternalAudioDeviceModule::PlayoutDeviceName(uint16_t index,
                              char name[webrtc::kAdmMaxDeviceNameSize],
                              char guid[webrtc::kAdmMaxGuidSize]) {
    return 0;
}

int32_t ExternalAudioDeviceModule::RecordingDeviceName(uint16_t index,
                                char name[webrtc::kAdmMaxDeviceNameSize],
                                char guid[webrtc::kAdmMaxGuidSize])  {
    return 0;
}
    
int32_t ExternalAudioDeviceModule::SetPlayoutDevice(uint16_t index) {
    return 0;
}

int32_t ExternalAudioDeviceModule::SetPlayoutDevice(WindowsDeviceType device)  {
    return 0;
}

int32_t ExternalAudioDeviceModule::SetRecordingDevice(uint16_t index)  {
    return 0;
}

int32_t ExternalAudioDeviceModule::SetRecordingDevice(WindowsDeviceType device)  {
    return 0;
}
    
int32_t ExternalAudioDeviceModule::PlayoutIsAvailable(bool* available)  {
    return 0;
}

int32_t ExternalAudioDeviceModule::InitPlayout()  {
    play_is_initialized_ = true;
    return 0;
}

bool ExternalAudioDeviceModule::PlayoutIsInitialized() const {
    return play_is_initialized_;
}

int32_t ExternalAudioDeviceModule::RecordingIsAvailable(bool* available)  {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::InitRecording() {
    rec_is_initialized_ = true;
    return 0;
}

bool ExternalAudioDeviceModule::RecordingIsInitialized() const {
    return rec_is_initialized_;
}
    
int32_t ExternalAudioDeviceModule::StartPlayout()  {
    if (!play_is_initialized_) {
        return -1;
    }
    {
        webrtc::MutexLock lock(&mutex_);
        playing_ = true;
    }
    bool start = true;
    UpdateProcessing(start);
    return 0;
}

int32_t ExternalAudioDeviceModule::StopPlayout() {
    bool start = false;
    {
        webrtc::MutexLock lock(&mutex_);
        playing_ = false;
        start = ShouldStartProcessing();
    }
    UpdateProcessing(start);
    return 0;
}

bool ExternalAudioDeviceModule::Playing() const {
    webrtc::MutexLock lock(&mutex_);
    return playing_;
    }

int32_t ExternalAudioDeviceModule::StartRecording() {
    if (!rec_is_initialized_) {
        return -1;
    }

    {
        webrtc::MutexLock lock(&mutex_);
        recording_ = true;
    }
    bool start = true;
    UpdateProcessing(start);
    return 0;
}

int32_t ExternalAudioDeviceModule::StopRecording() {
    bool start = false;
    {
        webrtc::MutexLock lock(&mutex_);
        recording_ = false;
        start = ShouldStartProcessing();
    }
    UpdateProcessing(start);
    return 0;
}

bool ExternalAudioDeviceModule::Recording() const  {
    webrtc::MutexLock lock(&mutex_);
    return recording_;
}
    
int32_t ExternalAudioDeviceModule::InitSpeaker() {
    // No speaker, just playing from file. Return success.
    return 0;
}

bool ExternalAudioDeviceModule::SpeakerIsInitialized() const  {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::InitMicrophone()  {
    // No microphone, just playing from file. Return success.
    return 0;
}

bool ExternalAudioDeviceModule::MicrophoneIsInitialized() const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}
    
int32_t ExternalAudioDeviceModule::SpeakerVolumeIsAvailable(bool* available) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::SetSpeakerVolume(uint32_t volume) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::SpeakerVolume(uint32_t* volume) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::MaxSpeakerVolume(uint32_t* max_volume) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::MinSpeakerVolume(uint32_t* min_volume) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}
    
int32_t ExternalAudioDeviceModule::MicrophoneVolumeIsAvailable(bool* available) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::SetMicrophoneVolume(uint32_t volume){
    webrtc::MutexLock lock(&mutex_);
    current_mic_level_ = volume;
    return 0;
}

int32_t ExternalAudioDeviceModule::MicrophoneVolume(uint32_t* volume) const {
    webrtc::MutexLock lock(&mutex_);
    *volume = current_mic_level_;
    return 0;
}

int32_t ExternalAudioDeviceModule::MaxMicrophoneVolume(uint32_t* max_volume) const {
    *max_volume = kMaxVolume;
    return 0;
}
    
int32_t ExternalAudioDeviceModule::MinMicrophoneVolume(uint32_t* min_volume) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}
    
int32_t ExternalAudioDeviceModule::SpeakerMuteIsAvailable(bool* available) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::SetSpeakerMute(bool enable) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::SpeakerMute(bool* enabled) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}
    
int32_t ExternalAudioDeviceModule::MicrophoneMuteIsAvailable(bool* available) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::SetMicrophoneMute(bool enable) {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::MicrophoneMute(bool* enabled) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}
    
int32_t ExternalAudioDeviceModule::StereoPlayoutIsAvailable(bool* available) const {
    // No recording device, just dropping audio. Stereo can be dropped just
    // as easily as mono.
    *available = true;
    return 0;
}

int32_t ExternalAudioDeviceModule::SetStereoPlayout(bool enable) {
    // No recording device, just dropping audio. Stereo can be dropped just
    // as easily as mono.
    return 0;
}

int32_t ExternalAudioDeviceModule::StereoPlayout(bool* enabled) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}

int32_t ExternalAudioDeviceModule::StereoRecordingIsAvailable(bool* available) const {
    // Keep thing simple. No stereo recording.
    *available = false;
    return 0;
}

int32_t ExternalAudioDeviceModule::SetStereoRecording(bool enable) {
    if (!enable) {
        return 0;
    }
    return -1;
}

int32_t ExternalAudioDeviceModule::StereoRecording(bool* enabled) const {
    RTC_DCHECK_NOTREACHED();
    return 0;
}
    
int32_t ExternalAudioDeviceModule::PlayoutDelay(uint16_t* delay_ms) const {
    // No delay since audio frames are dropped.
    *delay_ms = 0;
    return 0;
}

bool ExternalAudioDeviceModule::ShouldStartProcessing() {
    return recording_ || playing_;
}

void ExternalAudioDeviceModule::UpdateProcessing(bool start) {
    if (start) {
        if (!process_thread_) {
            process_thread_ = rtc::Thread::Create();
            process_thread_->Start();
        }
        process_thread_->Post(RTC_FROM_HERE, this, MSG_START_PROCESS);
    } else {
        if (process_thread_) {
            process_thread_->Stop();
            process_thread_.reset(nullptr);
            process_thread_checker_.Detach();
        }
        webrtc::MutexLock lock(&mutex_);
        started_ = false;
    }
}

void ExternalAudioDeviceModule::OnMessage(rtc::Message* msg) {
    switch (msg->message_id) {
        case MSG_START_PROCESS:
            StartProcessP();
            break;
        case MSG_RUN_PROCESS:
            ProcessFrameP();
            break;
        default:
            // All existing messages should be caught. Getting here should never
            // happen.
            RTC_DCHECK_NOTREACHED();
    }
}

void ExternalAudioDeviceModule::StartProcessP() {
    RTC_DCHECK_RUN_ON(&process_thread_checker_);
    {
        webrtc::MutexLock lock(&mutex_);
        if (started_) {
            // Already started.
            return;
        }
    }
    ProcessFrameP();
}

void ExternalAudioDeviceModule::ProcessFrameP() {
    RTC_DCHECK_RUN_ON(&process_thread_checker_);
    {
        webrtc::MutexLock lock(&mutex_);
        if (!started_) {
            next_frame_time_ = rtc::TimeMillis();
            started_ = true;
        }

        // Receive and send frames every kTimePerFrameMs.
        if (playing_) {
            ReceiveFrameP();
        }

        if (recording_) {
            SendFrameP();
        }
    }

    next_frame_time_ += kTimePerFrameMs;
    const int64_t current_time = rtc::TimeMillis();
    const int64_t wait_time =
        (next_frame_time_ > current_time) ? next_frame_time_ - current_time : 0;
    process_thread_->PostDelayed(RTC_FROM_HERE, wait_time, this,
                                 MSG_RUN_PROCESS);
}

void ExternalAudioDeviceModule::ReceiveFrameP() {
    RTC_DCHECK_RUN_ON(&process_thread_checker_);
    if (!audio_callback_ || !event_handler_) {
        return;
    }
    size_t samples_out = 0;
    int64_t elapsed_time_ms = -1;
    int64_t ntp_time_ms = -1;
    size_t samples = 480;
    uint8_t channels = 2;
    int sampling_frequency = 48000;
    playout_buffer_.clear();
    playout_buffer_.resize(samples * 2 * channels, 0);
    if (audio_callback_->NeedMorePlayData(
            samples, 2 * channels, channels,
            sampling_frequency, playout_buffer_.data(), samples_out, &elapsed_time_ms,
            &ntp_time_ms) != 0) {
        RTC_DCHECK_NOTREACHED();
    }

    AudioFrame audio_frame;
    audio_frame.type = AudioFrameType::PCM_16;
    audio_frame.data = (uint8_t*)playout_buffer_.data();
    audio_frame.sample_length = samples_out * 2;
    audio_frame.timestamp_ms = elapsed_time_ms;
    audio_frame.sample_rate = sampling_frequency;
    event_handler_->OnAudioFrame(stream_id_.c_str(), audio_frame);
}

void ExternalAudioDeviceModule::SendFrameP() {
    if (!audio_callback_) {
        return;
    }
    bool key_pressed = false;
    uint32_t current_mic_level = current_mic_level_;
    const size_t num_elements_10ms = kNumberOfChannels * kSamplesPerSecond / 100 * 2;
    if (record_buffer_.size() < num_elements_10ms) {
        return;
    }

    int64_t capture_timestamp_ns = rtc::kNumNanosecsPerMicrosec *
                        timestamp_aligner_.TranslateTimestamp(capture_timestamp_us_, rtc::TimeMicros());

    capture_timestamp_us_ = capture_timestamp_us_ + 10 * 1000;

    char tmpbuf[num_elements_10ms];
    memcpy(tmpbuf, record_buffer_.data(), num_elements_10ms);
    memmove(record_buffer_.data(), record_buffer_.data() + num_elements_10ms,
                                   record_buffer_.size() - num_elements_10ms);
    record_buffer_.SetSize(record_buffer_.size() - num_elements_10ms);
    
    if (audio_callback_->RecordedDataIsAvailable(tmpbuf, kNumberSamples, kNumberBytesPerSample * kNumberOfChannels,
            kNumberOfChannels, kSamplesPerSecond, kTotalDelayMs, kClockDriftMs,
            current_mic_level, key_pressed, current_mic_level, capture_timestamp_ns) != 0) {
        RTC_DCHECK_NOTREACHED();
    }

    current_mic_level_ = current_mic_level;
}

bool ExternalAudioDeviceModule::PushAudioFrame(AudioFrame pcm_audio_frame) {
    webrtc::MutexLock lock(&mutex_);
    if (started_ && recording_) {
        kSamplesPerSecond = pcm_audio_frame.sample_rate;
        kNumberOfChannels = pcm_audio_frame.channels;
        kNumberSamples = pcm_audio_frame.sample_rate / 100;
        record_buffer_.AppendData(pcm_audio_frame.data, pcm_audio_frame.sample_length);
        if (capture_timestamp_us_ == 0) {
            capture_timestamp_us_ = pcm_audio_frame.timestamp_ms;
        }
        return true;
    }
    return false;
}

}

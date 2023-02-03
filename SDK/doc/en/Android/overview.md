# API
## Engine Management

| API | Description |
| --- | --- |
| [createRtcEngine](api.md#RtcEngine-creatertcengine) | Create the RtcEngine instance. |
| [destroyRtcEngine](api.md#RtcEngine-destroyrtcengine) | Destroy the RtcEngine instance created by [createRtcEngine](api.md#RtcEngine-creatertcengine), and release all related resources. <br> |
| [createPublisher](api.md#RtcEngine-createpublisher) | Create the Publisher. |
| [destroyPublisher](api.md#RtcEngine-destroypublisher) | Destroy the Publisher instance created by [createPublisher](api.md#RtcEngine-createpublisher), and release all related resources. <br> |
| [createSubscriber](api.md#RtcEngine-createsubscriber) | Create the Subscriber. |
| [destroySubscriber](api.md#RtcEngine-destroysubscriber) | Destroy the Subscriber instance created by [createSubscriber](api.md#RtcEngine-createsubscriber), and release all related resources. <br> |

## Publish

| API | Description |
| --- | --- |
| [setVideoConfig](api.md#Publisher-setvideoconfig) | Set the video capture parameters. |
| [setLocalView](api.md#Publisher-setlocalview) | Binds the view to be used for local video rendering. <br> |
| [startVideoCapture](api.md#Publisher-startvideocapture) | Enable video capture. The default setting is off.   <br> |
| [stopVideoCapture](api.md#Publisher-stopvideocapture) | Stop video capture. |
| [switchCamera](api.md#Publisher-switchcamera) | Switch the camera for video capture between front-facing/back-facing cameras. |
| [startAudioCapture](api.md#Publisher-startaudiocapture) | Start audio capture. The default is off. |
| [stopAudioCapture](api.md#Publisher-stopaudiocapture) | Stop audio capture. |
| [startPublish](api.md#Publisher-startpublish) | Publish the captured media stream. |
| [stopPublish](api.md#Publisher-stoppublish) | Stops publishing the media stream. |
| [isPublishing](api.md#Publisher-ispublishing) | Inspect the media stream publishing status. |
| [setLocalAudioMute](api.md#Publisher-setlocalaudiomute) | Mute local audio or not. |
| [setLocalVideoMute](api.md#Publisher-setlocalvideomute) | Mute local video or not. |
| [pushVideoFrame](api.md#Publisher-pushvideoframe) | Push external video frame. |
| [pushAudioFrame](api.md#Publisher-pushaudioframe) | Push external audio data. |
| [pushEncodedVideoFrame](api.md#Publisher-pushencodedvideoframe) | Push external encoded frame. |

## Subscribe

| API | Description |
| --- | --- |
| [setRemoteView](api.md#Subscriber-setremoteview) | Binds the view to be used for remote video rendering. <br> |
| [setEnableSpeaker](api.md#Subscriber-setenablespeaker) | Switch the rendering device for audio stream: earpiece or speakerphone. |
| [startSubscribe](api.md#Subscriber-startsubscribe) | Subscribe the remote media streams. <br><br/>After calling the API, you will receive [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged). |
| [stopSubscribe](api.md#Subscriber-stopsubscribe) | Stop subscribing to the media stream. |
| [isSubscribed](api.md#Subscriber-issubscribed) | Inspect the subscribing state. |
| [setRemoteAudioMute](api.md#Subscriber-setremoteaudiomute) | Mute the remote audio. Do not subscribe the captured audio. |
| [setRemoteVideoMute](api.md#Subscriber-setremotevideomute) | Mute the remote video. Do not subscribe the captured video. |

# Callback
## Publish

| Callback | Description |
| --- | --- |
| [onLocalStats](callback.md#RtcEventHandler-onlocalstats) | Release flow state cycle callback with 1 second period.. |
| [onPublishStateChanged](callback.md#RtcEventHandler-onpublishstatechanged) | Receive this event when the publish state of the media stream changes. |
| [onMuteAudioSuccess](callback.md#RtcEventHandler-onmuteaudiosuccess) | Mute audio success callback. |
| [onMuteAudioFailed](callback.md#RtcEventHandler-onmuteaudiofailed) | Mute audio failed callback. |
| [onMuteVideoSuccess](callback.md#RtcEventHandler-onmutevideosuccess) | Mute video success callback. |
| [onMuteVideoFailed](callback.md#RtcEventHandler-onmutevideofailed) | Mute video failed callback. |

## Subscribe

| Callback | Description |
| --- | --- |
| [onRemoteStats](callback.md#RtcEventHandler-onremotestats) | Release flow state cycle callback with 1 second period.. |
| [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged) | Receive this event when the subscribing state changes. |
| [onFirstRemoteVideoFrameRendered](callback.md#RtcEventHandler-onfirstremotevideoframerendered) | Release flow state cycle callback with 1 second period.. |
| [onVideoFrame](callback.md#RtcEventHandler-onvideoframe) | Video frame callback with period determined by frame rate (1000/fps ms). |
| [onAudioFrame](callback.md#RtcEventHandler-onaudioframe) | Audio frame callback with 10ms period. |
| [onEncodedVideoFrame](callback.md#RtcEventHandler-onencodedvideoframe) | Video frame callback with period determined by frame rate (1000/fps ms). |
| [onStallInfo](callback.md#RtcEventHandler-onstallinfo) | Stuck message callback, period 1 sec. |


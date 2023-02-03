# API
## Publish

| API | Description |
| --- | --- |
| [startVideoCapture](api.md#VEWTNPublisher-startvideocapture) | Enable video capture. The default setting is off.   <br> |
| [stopVideoCapture](api.md#VEWTNPublisher-stopvideocapture) | Stop video capture. |
| [switchCamera](api.md#VEWTNPublisher-switchcamera) | Switch the camera for video capture between front-facing/back-facing cameras. |
| [startAudioCapture](api.md#VEWTNPublisher-startaudiocapture) | Start audio capture. The default is off. |
| [stopAudioCapture](api.md#VEWTNPublisher-stopaudiocapture) | Stop audio capture. |
| [stopPublish](api.md#VEWTNPublisher-stoppublish) | Stops publishing the media stream. |
| [isPublishing](api.md#VEWTNPublisher-ispublishing) | Inspect the media stream publishing status. |

## Subscribe

| API | Description |
| --- | --- |
| [stopSubscribe](api.md#VEWTNSubscriber-stopsubscribe) | Stop subscribing to the media stream. |
| [isSubscribed](api.md#VEWTNSubscriber-issubscribed) | Inspect the subscribing state. |

# Callback
## Publish

| Callback | Description |
| --- | --- |
| [onLocalStats:audioStats:videoStats:](callback.md#VEWTNEngineDelegate-onlocalstats-audiostats-videostats) | Periodic callback of the stats of locally published stream in every second. |
| [onPublishStateChanged:publishState:reason:](callback.md#VEWTNEngineDelegate-onpublishstatechanged-publishstate-reason) | Callback of media stream publishing state change. |
| [onMuteAudioSuccess:](callback.md#VEWTNEngineDelegate-onmuteaudiosuccess) | Success to mute audio callback. |
| [onMuteAudioFailed:reason:](callback.md#VEWTNEngineDelegate-onmuteaudiofailed-reason) | Failure to mute audio callback. |
| [onMuteVideoSuccess:](callback.md#VEWTNEngineDelegate-onmutevideosuccess) | Success to mute video callback. |
| [onMuteVideoFailed:reason:](callback.md#VEWTNEngineDelegate-onmutevideofailed-reason) | Failure to mute video callback. |

## Subscribe

| Callback | Description |
| --- | --- |
| [onRemoteStats:audioStats:videoStats:](callback.md#VEWTNEngineDelegate-onremotestats-audiostats-videostats) | Periodic callback of the stats of the subscribed remote stream in every second. |
| [onSubscribeStateChanged:subscribeState:reason:](callback.md#VEWTNEngineDelegate-onsubscribestatechanged-subscribestate-reason) | Callback of media stream subscribing state change. |
| [onFirstRemoteVideoFrameRendered:](callback.md#VEWTNEngineDelegate-onfirstremotevideoframerendered) | Receive the callback when the first frame of the subscribed remote frame is rendered. |
| [onVideoFrame:frame:](callback.md#VEWTNEngineDelegate-onvideoframe-frame) | Periodic encoded video frame callback. The period is the reciprocal of the framerate. |
| [onAudioFrame:frame:](callback.md#VEWTNEngineDelegate-onaudioframe-frame) | Periodic audio frame callback in every 10ms. |
| [onEncodedVideoFrame:frame:](callback.md#VEWTNEngineDelegate-onencodedvideoframe-frame) | Periodic encoded video frame callback. The period is the reciprocal of the framerate. |
| [onStallInfo:stallInfo:](callback.md#VEWTNEngineDelegate-onstallinfo-stallinfo) | Periodic callback of the stall information in every second. |


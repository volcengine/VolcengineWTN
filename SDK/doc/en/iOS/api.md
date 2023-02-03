# VEWTNEngine
```objectivec
@interface VEWTNEngine : NSObject
```

VEWTNEngine Class


## Member Functions

| Return | Name |
| --- | --- |
| **instancetype** | [initWithDelegate:](#VEWTNEngine-initwithdelegate) |
| **VEWTNPublisher*** | [createPublisher:](#VEWTNEngine-createpublisher) |
| **void** | [destroyPublisher:](#VEWTNEngine-destroypublisher) |
| **VEWTNSubscriber*** | [createSubscriber:](#VEWTNEngine-createsubscriber) |
| **void** | [destroySubscriber:](#VEWTNEngine-destroysubscriber) |


## Instructions
<span id="VEWTNEngine-initwithdelegate"></span>
### initWithDelegate:
```objectivec

- (instancetype)initWithDelegate:(id<VEWTNEngineDelegate>)delegate;
```
Create the engine instance.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| delegate | **id<VEWTNEngineDelegate>** | See [VEWTNEngineDelegate](callback.md#vewtnenginedelegate). |

**Return Value**
A successfully created engine instance.


**Notes**
You can create no more than one engine instance.

<span id="VEWTNEngine-createpublisher"></span>
### createPublisher:
```objectivec

- (VEWTNPublisher *)createPublisher:(VEWTNPubExtraParam*)pub_extra_param;
```
Create the Publisher instance.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| pub_extra_param | **VEWTNPubExtraParam*** | See [VEWTNPubExtraParam](keytype.md#vewtnpubextraparam). |

**Return Value**

+ Publisher: A successfully created [VEWTNPublisher](#vewtnpublisher) instance.  
+ Null: Failed to create. Possible reasons are: Creating an internal source Publisher when an internal one exists.


**Notes**
You may create multiple Publisher with external sources, but no more than one Publisher with internal source.

<span id="VEWTNEngine-destroypublisher"></span>
### destroyPublisher:
```objectivec

- (void)destroyPublisher:(VEWTNPublisher*)publisher;
```
Destroy the Publisher instance created by [createPublisher:](#VEWTNEngine-createpublisher), and release all related resources. <br>


**Notes**

+ Call this API after all business scenarios related to the instance are destroyed.  
+ When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  


<span id="VEWTNEngine-createsubscriber"></span>
### createSubscriber:
```objectivec

- (VEWTNSubscriber *)createSubscriber:(VEWTNSubExtraParam*)sub_extra_param;
```
Create the Subscriber instance.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| sub_extra_param | **VEWTNSubExtraParam*** | See [VEWTNSubExtraParam](keytype.md#vewtnsubextraparam). |

**Return Value**

+ Subscriber: A successfully created [VEWTNSubscriber](#vewtnsubscriber) instance.  
+ Null: Failed to create. Possible reasons are: Creating an internal source Subscriber when an internal one exists.


**Notes**
You may create multiple Subscriber with external sources, but no more than one Subscriber with internal source.

<span id="VEWTNEngine-destroysubscriber"></span>
### destroySubscriber:
```objectivec

- (void)destroySubscriber:(VEWTNSubscriber*)subscriber;
```
Destroy the Subscriber instance created by [createSubscriber:](#VEWTNEngine-createsubscriber), and release all related resources. <br>


**Notes**

+ Call this API after all business scenarios related to the [VEWTNSubscriber](#vewtnsubscriber) instance are destroyed.  
+ When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  


# VEWTNPublisher
```objectivec
@interface VEWTNPublisher : NSObject
```

VEWTNPublisher Class


## Member Functions

| Return | Name |
| --- | --- |
| **int** | [startPublish:](#VEWTNPublisher-startpublish) |
| **int** | [stopPublish](#VEWTNPublisher-stoppublish) |
| **BOOL** | [isPublishing](#VEWTNPublisher-ispublishing) |
| **void** | [setLocalView:](#VEWTNPublisher-setlocalview) |
| **int** | [setVideoConfig:](#VEWTNPublisher-setvideoconfig) |
| **int** | [startVideoCapture](#VEWTNPublisher-startvideocapture) |
| **int** | [stopVideoCapture](#VEWTNPublisher-stopvideocapture) |
| **int** | [switchCamera](#VEWTNPublisher-switchcamera) |
| **int** | [startAudioCapture](#VEWTNPublisher-startaudiocapture) |
| **int** | [stopAudioCapture](#VEWTNPublisher-stopaudiocapture) |
| **int** | [setLocalAudioMute:](#VEWTNPublisher-setlocalaudiomute) |
| **int** | [setLocalVideoMute:](#VEWTNPublisher-setlocalvideomute) |
| **BOOL** | [pushEncodedVideoFrame:](#VEWTNPublisher-pushencodedvideoframe) |
| **BOOL** | [pushVideoFrame:](#VEWTNPublisher-pushvideoframe) |
| **BOOL** | [pushAudioFrame:](#VEWTNPublisher-pushaudioframe) |


## Instructions
<span id="VEWTNPublisher-startpublish"></span>
### startPublish:
```objectivec

- (int)startPublish:(NSString*)url;
```
Publish the captured media stream.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| url | **NSString*** | The url for WHIP request. <br/>Concatenate the fixed url `https://wtn.volcvideo.com`, PUB method, appID, your specified streamID, token and TokenType into the value of url. <br/>The url is like `https://wtn.volcvideo.com/pub/<appID>/<streamID>?Token=<token>&TokenType=Bearer`. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ You can only publish one media stream with one VEWTNPublisher instance. Calling this API during publishing will fail. 
+ You can call this API to start publishing media streams with or without media capture. 
+ After calling this API, you will receive [onPublishStateChanged:publishState:reason:](callback.md#VEWTNEngineDelegate-onpublishstatechanged-publishstate-reason). 
+ Call [stopPublish](#VEWTNPublisher-stoppublish) to stop publishing the media stream.


<span id="VEWTNPublisher-stoppublish"></span>
### stopPublish
```objectivec

- (int)stopPublish;
```
Stops publishing the media stream.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [startPublish:](#VEWTNPublisher-startpublish) to publish the media stream. 
+ After calling this API, you will receive [onPublishStateChanged:publishState:reason:](callback.md#VEWTNEngineDelegate-onpublishstatechanged-publishstate-reason).


<span id="VEWTNPublisher-ispublishing"></span>
### isPublishing
```objectivec

- (BOOL)isPublishing;
```
Inspect the media stream publishing status.

**Return Value**

+ true: Publishing. 
+ false：Not publishing. 


<span id="VEWTNPublisher-setlocalview"></span>
### setLocalView:
```objectivec

- (void)setLocalView:(UIView*)view;
```
Binds the view to be used for local video rendering. <br>

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| view | **UIView*** | View object. The object must be of effective lifecycle. |

**Return Value**

+ 0: Success 
+ -1: Failure. The view is empty. 


**Notes**

+ To unbind the view, call this API with the view set to `null`. 
+ Bind the view before calling [startVideoCapture](#VEWTNPublisher-startvideocapture). 


<span id="VEWTNPublisher-setvideoconfig"></span>
### setVideoConfig:
```objectivec

- (int)setVideoConfig:(VEWTNVideoConfig*)video_config;
```
Set the video capture parameters.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| video_config | **VEWTNVideoConfig*** | See [VEWTNVideoConfig](keytype.md#vewtnvideoconfig). |

**Return Value**

+ 0: Success; 
+ < 0: Failure; 


**Notes**

+ You must call this API before calling [startVideoCapture](#VEWTNPublisher-startvideocapture). 


<span id="VEWTNPublisher-startvideocapture"></span>
### startVideoCapture
```objectivec

- (int)startVideoCapture;
```
Enable video capture. The default setting is off.   <br>

**Return Value**

+ 0: Success 
+ -1: Failure. The view is empty. 


**Notes**

+ You can call this API before or after publishing the stream. 
+ Call [stopVideoCapture](#VEWTNPublisher-stopvideocapture) to stop video capture. 
+ The camera used for video capture is set by [switchCamera](#VEWTNPublisher-switchcamera).


<span id="VEWTNPublisher-stopvideocapture"></span>
### stopVideoCapture
```objectivec

- (int)stopVideoCapture;
```
Stop video capture.

**Return Value**

+ 0: Success 
+ -1: Failure. The view is empty. 


**Notes**

+ Call [startVideoCapture](#VEWTNPublisher-startvideocapture) to enable video capture. 
+ Without calling this API, the video capture sustains until the Publisher instance is destroyed.


<span id="VEWTNPublisher-switchcamera"></span>
### switchCamera
```objectivec

- (int)switchCamera;
```
Switch the camera for video capture between front-facing/back-facing cameras.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Front-facing camera is the default camera. 
+ If the video capture is on-going, the switch is effective immediately. If the video capture is off, the set camera is used when video capture starts.


<span id="VEWTNPublisher-startaudiocapture"></span>
### startAudioCapture
```objectivec

- (int)startAudioCapture;
```
Start audio capture. The default is off.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [stopAudioCapture](#VEWTNPublisher-stopaudiocapture) to stop audio capture. 
+ You can call this API to start video capture whether to publish the audio stream.


<span id="VEWTNPublisher-stopaudiocapture"></span>
### stopAudioCapture
```objectivec

- (int)stopAudioCapture;
```
Stop audio capture.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [startAudioCapture](#VEWTNPublisher-startaudiocapture) to enable audio capture. 
+ Without calling this API, the audio capture will sustain until the Publisher instance is destroyed.


<span id="VEWTNPublisher-setlocalaudiomute"></span>
### setLocalAudioMute:
```objectivec

- (int)setLocalAudioMute:(BOOL)mute;
```
Mute the local audio. Do not publish the captured audio.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **BOOL** | Whether to publish the locally captured audio. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ You can call this API before or after the media stream is published. 
+ You can call this API before or after the media is captured.


<span id="VEWTNPublisher-setlocalvideomute"></span>
### setLocalVideoMute:
```objectivec

- (int)setLocalVideoMute:(BOOL)mute;
```
Mute the local video. Do not publish the captured video.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **BOOL** | Whether to publish the locally captured video. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ You can call this API before or after the media stream is published. 
+ You can call this API before or after the media is captured.


<span id="VEWTNPublisher-pushencodedvideoframe"></span>
### pushEncodedVideoFrame:
```objectivec

- (BOOL)pushEncodedVideoFrame:(VEWTNEncodedVideoFrame*)encoded_frame;
```
Push external encoded frame.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| encoded_frame | **VEWTNEncodedVideoFrame*** | Encoded frame. See [VEWTNEncodedVideoFrame](keytype.md#vewtnencodedvideoframe). During the process, the resolution of the frames must remain the same. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**

+ Do not support encoding feedback and request keyframe feedback. 
+ Push externally encoded frames periodically according to the actual frame rate.


<span id="VEWTNPublisher-pushvideoframe"></span>
### pushVideoFrame:
```objectivec

- (BOOL)pushVideoFrame:(VEWTNVideoFrame*)video_frame;
```
Push external video frame.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| video_frame | **VEWTNVideoFrame*** | Encoded frame. See [VEWTNVideoFrame](keytype.md#vewtnvideoframe). During the process, the resolution of the frames must remain the same. The frame width and the frame height must be multiples of 16. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**
Push external frames periodically according to the actual frame rate.

<span id="VEWTNPublisher-pushaudioframe"></span>
### pushAudioFrame:
```objectivec

- (BOOL)pushAudioFrame:(VEWTNAudioFrame*)audio_frame;
```
Push external audio data.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| audio_frame | **VEWTNAudioFrame*** | See [VEWTNAudioFrame](keytype.md#vewtnaudioframe). During the process, the parameters of the frames must remain the same. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**
Push external frames periodically. The period is recommended to be 10 ms.

# VEWTNSubscriber
```objectivec
@interface VEWTNSubscriber : NSObject
```

VEWTNSubscriber Class


## Member Functions

| Return | Name |
| --- | --- |
| **int** | [startSubscribe:](#VEWTNSubscriber-startsubscribe) |
| **int** | [stopSubscribe](#VEWTNSubscriber-stopsubscribe) |
| **BOOL** | [isSubscribed](#VEWTNSubscriber-issubscribed) |
| **int** | [setEnableSpeaker:](#VEWTNSubscriber-setenablespeaker) |
| **int** | [setRemoteView:](#VEWTNSubscriber-setremoteview) |
| **int** | [setRemoteAudioMute:](#VEWTNSubscriber-setremoteaudiomute) |
| **int** | [setRemoteVideoMute:](#VEWTNSubscriber-setremotevideomute) |


## Instructions
<span id="VEWTNSubscriber-startsubscribe"></span>
### startSubscribe:
```objectivec

- (int)startSubscribe:(NSString*)url;
```
Subscribe the remote media streams. <br>
After calling the API, you will receive [onSubscribeStateChanged:subscribeState:reason:](callback.md#VEWTNEngineDelegate-onsubscribestatechanged-subscribestate-reason).

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| url | **NSString*** | The url for WHIP request. <br/>Concatenate the fixed url `https://wtn.volcvideo.com`, SUB method, appID, and your specified streamID into the value of url. <br/>The url is like `https://wtn.volcvideo.com/sub/<appID>/<streamID>`. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ You can only subscribe to one media stream with one VEWTNSubscriber instance. 
+ Ensure the stream is published before subscribe to the stream. 
+ Call [setRemoteView:](#VEWTNSubscriber-setremoteview) to set the remote view for the media stream before subscribing. 
+ Call [stopSubscribe](#VEWTNSubscriber-stopsubscribe) to stop subscribing.


<span id="VEWTNSubscriber-stopsubscribe"></span>
### stopSubscribe
```objectivec

- (int)stopSubscribe;
```
Stop subscribing to the media stream.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [startSubscribe:](#VEWTNSubscriber-startsubscribe) to start subscribing. 
+ Receive [onSubscribeStateChanged:subscribeState:reason:](callback.md#VEWTNEngineDelegate-onsubscribestatechanged-subscribestate-reason) after calling this API.


<span id="VEWTNSubscriber-issubscribed"></span>
### isSubscribed
```objectivec

- (BOOL)isSubscribed;
```
Inspect the subscribing state.

**Return Value**

+ true: Subscribing 
+ false: Not subscribing 


<span id="VEWTNSubscriber-setenablespeaker"></span>
### setEnableSpeaker:
```objectivec

- (int)setEnableSpeaker:(BOOL)enable;
```
Switch the rendering device for audio stream: earpiece or speakerphone.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| enable | **BOOL** | • true: earpiece <br/>• false：speakerphone |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Speakerphone is the default device. 
+ You can switch the audio rendering device before or after the audio rendering.


<span id="VEWTNSubscriber-setremoteview"></span>
### setRemoteView:
```objectivec

- (int)setRemoteView:(UIView*)view;
```
Binds the view to be used for remote video rendering. <br>

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| view | **UIView*** | View object. The object must be of effective lifecycle. |

**Return Value**

+ 0: Success 
+ -1: Failure. 


**Notes**

+ You must bind the view before subscribe to the video stream. 
+ To unbind the view, call this API with the view set to `null`.


<span id="VEWTNSubscriber-setremoteaudiomute"></span>
### setRemoteAudioMute:
```objectivec

- (int)setRemoteAudioMute:(BOOL)mute;
```
Mute the remote audio. Do not subscribe the captured audio.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **BOOL** | Whether to subscribe the remote audio. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**
You can call this API before or after the media stream is published.

<span id="VEWTNSubscriber-setremotevideomute"></span>
### setRemoteVideoMute:
```objectivec

- (int)setRemoteVideoMute:(BOOL)mute;
```
Mute the remote video. Do not subscribe the captured video.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **BOOL** | Whether to subscribe the remote video. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**
You can call this API before or after the media stream is published.


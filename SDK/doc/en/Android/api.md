# Publisher
```java
public interface com.vewtn.Publisher
```

Publisher Class


## Member Functions

| Return | Name |
| --- | --- |
| **int** | [setVideoConfig](#Publisher-setvideoconfig) |
| **int** | [setLocalView](#Publisher-setlocalview) |
| **int** | [startVideoCapture](#Publisher-startvideocapture) |
| **int** | [stopVideoCapture](#Publisher-stopvideocapture) |
| **int** | [switchCamera](#Publisher-switchcamera) |
| **int** | [startAudioCapture](#Publisher-startaudiocapture) |
| **int** | [stopAudioCapture](#Publisher-stopaudiocapture) |
| **int** | [startPublish](#Publisher-startpublish) |
| **int** | [stopPublish](#Publisher-stoppublish) |
| **boolean** | [isPublishing](#Publisher-ispublishing) |
| **int** | [setLocalAudioMute](#Publisher-setlocalaudiomute) |
| **int** | [setLocalVideoMute](#Publisher-setlocalvideomute) |
| **boolean** | [pushEncodedVideoFrame](#Publisher-pushencodedvideoframe) |
| **boolean** | [pushVideoFrame](#Publisher-pushvideoframe) |
| **boolean** | [pushAudioFrame](#Publisher-pushaudioframe) |


## Instructions
<span id="Publisher-setvideoconfig"></span>
### setVideoConfig
```java
int com.vewtn.Publisher.setVideoConfig(
    VideoConfig videoConfig)
```
Set the video capture parameters.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| videoConfig | **VideoConfig** | See [VideoConfig](keytype.md#videoconfig). |

**Return Value**

+ 0: Success; 
+ < 0: Failure; 


**Notes**

+ You must call this API before calling [startVideoCapture](#Publisher-startvideocapture). 


<span id="Publisher-setlocalview"></span>
### setLocalView
```java
int com.vewtn.Publisher.setLocalView(
    View view)
```
Binds the view to be used for local video rendering. <br>

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| view | **View** | View object. The object must be of effective lifecycle. |

**Return Value**

+ 0: Success 
+ -1: Failure. The view is empty. 


**Notes**

+ To unbind the view, call this API with the view set to `null`. 
+ Bind the view before calling [startVideoCapture](#Publisher-startvideocapture). 
+ With this API, you can only render the video data captured by [startVideoCapture](#Publisher-startvideocapture).


<span id="Publisher-startvideocapture"></span>
### startVideoCapture
```java
int com.vewtn.Publisher.startVideoCapture()
```
Enable video capture. The default setting is off.   <br>

**Return Value**

+ 0: Success 
+ -1: Failure. The view is empty. 


**Notes**

+ You can call this API before or after publishing the stream. 
+ Call [stopVideoCapture](#Publisher-stopvideocapture) to stop video capture. 
+ The camera used for video capture is set by [switchCamera](#Publisher-switchcamera).


<span id="Publisher-stopvideocapture"></span>
### stopVideoCapture
```java
int com.vewtn.Publisher.stopVideoCapture()
```
Stop video capture.

**Return Value**

+ 0: Success 
+ -1: Failure. The view is empty. 


**Notes**

+ Call startVideoCapture [startVideoCapture](#Publisher-startvideocapture) to enable video capture. 
+ Without calling this API, the video capture sustains until the Publisher instance is destroyed.


<span id="Publisher-switchcamera"></span>
### switchCamera
```java
int com.vewtn.Publisher.switchCamera()
```
Switch the camera for video capture between front-facing/back-facing cameras.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Front-facing camera is the default camera. 
+ If the video capture is on-going, the switch is effective immediately. If the video capture is off, the set camera is used when video capture starts.


<span id="Publisher-startaudiocapture"></span>
### startAudioCapture
```java
int com.vewtn.Publisher.startAudioCapture()
```
Start audio capture. The default is off.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [stopAudioCapture](#Publisher-stopaudiocapture) to stop audio capture. 
+ You can call this API to start video capture whether to publish the audio stream.


<span id="Publisher-stopaudiocapture"></span>
### stopAudioCapture
```java
int com.vewtn.Publisher.stopAudioCapture()
```
Stop audio capture.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [startAudioCapture](#Publisher-startaudiocapture) to enable audio capture. 
+ Without calling this API, the audio capture will sustain until the Publisher instance is destroyed.


<span id="Publisher-startpublish"></span>
### startPublish
```java
int com.vewtn.Publisher.startPublish(
    String url)
```
Publish the captured media stream.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| url | **String** | The url for WHIP request. <br/>Concatenate the fixed url `https://wtn.volcvideo.com`, PUB method, appID, your specified streamID, token and TokenType into the value of url. <br/>The url is like `https://wtn.volcvideo.com/pub/<appID>/<streamID>?Token=<token>&TokenType=Bearer`. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ You can only publish one media stream with one Publisher instance. Calling this API during publishing will fail. 
+ You can call this API to start publishing media streams with or without media capture. 
+ After calling this API, you will receive [onPublishStateChanged](callback.md#RtcEventHandler-onpublishstatechanged). 
+ Call [stopPublish](#Publisher-stoppublish) to stop publishing the media stream.


<span id="Publisher-stoppublish"></span>
### stopPublish
```java
int com.vewtn.Publisher.stopPublish()
```
Stops publishing the media stream.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [startPublish](#Publisher-startpublish) to publish the media stream. 
+ After calling this API, you will receive [onPublishStateChanged](callback.md#RtcEventHandler-onpublishstatechanged).


<span id="Publisher-ispublishing"></span>
### isPublishing
```java
boolean com.vewtn.Publisher.isPublishing()
```
Inspect the media stream publishing status.

**Return Value**

+ true: Publishing. 
+ false：Not publishing. 


<span id="Publisher-setlocalaudiomute"></span>
### setLocalAudioMute
```java
int com.vewtn.Publisher.setLocalAudioMute(
    boolean mute)
```
Mute local audio or not.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **boolean** | The mute status of the local audio. Not muted by default. <br/>• ture: do not send audio data. <br/>• false: send audio data. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**
You can call this API regardless of the capture status or publishing status of the audio.

<span id="Publisher-setlocalvideomute"></span>
### setLocalVideoMute
```java
int com.vewtn.Publisher.setLocalVideoMute(
    boolean mute)
```
Mute local video or not.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **boolean** | The mute status of the local video. Not muted by default. <br/>• ture: do not send video data. <br/>• false: send video data. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**
You can call this API regardless of the capture status or publishing status of the video.

<span id="Publisher-pushencodedvideoframe"></span>
### pushEncodedVideoFrame
```java
boolean com.vewtn.Publisher.pushEncodedVideoFrame(
    EncodedVideoFrame encodedFrame)
```
Push external encoded frame.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| encodedFrame | **EncodedVideoFrame** | Encoded frame. See [EncodedVideoFrame](keytype.md#encodedvideoframe). During the process, the resolution of the frames must remain the same. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**

+ Do not support encoding feedback and request keyframe feedback. 
+ Push externally encoded frames periodically according to the actual frame rate.


<span id="Publisher-pushvideoframe"></span>
### pushVideoFrame
```java
boolean com.vewtn.Publisher.pushVideoFrame(
    VideoFrame videoFrame)
```
Push external video frame.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| videoFrame | **VideoFrame** | Encoded frame. See [VideoFrame](keytype.md#videoframe). During the process, the resolution of the frames must remain the same. The frame width and the frame height must be multiples of 16. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**
Push external frames periodically according to the actual frame rate.

<span id="Publisher-pushaudioframe"></span>
### pushAudioFrame
```java
boolean com.vewtn.Publisher.pushAudioFrame(
    AudioFrame audioFrame)
```
Push external audio data.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| audioFrame | **AudioFrame** | See [AudioFrame](keytype.md#audioframe). During the process, the parameters of the frames must remain the same. |

**Return Value**

+ 0: Success 
+ <0: Failure


**Notes**
Push external frames periodically. The period is recommended to be 10 ms.

# RtcEngine
```java
public abstract class com.vewtn.RtcEngine
```



## Static Functions

| Return | Name |
| --- | --- |
| **synchronized static RtcEngine** | [createRtcEngine](#RtcEngine-creatertcengine) |


## Member Functions

| Return | Name |
| --- | --- |
| **synchronized int** | [destroyRtcEngine](#RtcEngine-destroyrtcengine) |
| **abstract Publisher** | [createPublisher](#RtcEngine-createpublisher) |
| **abstract void** | [destroyPublisher](#RtcEngine-destroypublisher) |
| **abstract Subscriber** | [createSubscriber](#RtcEngine-createsubscriber) |
| **abstract void** | [destroySubscriber](#RtcEngine-destroysubscriber) |


## Instructions
<span id="RtcEngine-creatertcengine"></span>
### createRtcEngine
```java
public synchronized static RtcEngine com.vewtn.RtcEngine.createRtcEngine(
    Context context,
    RtcEventHandler handler)
```
Create the RtcEngine instance.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| context | **Context** | Android application context |
| handler | **RtcEventHandler** | See [RtcEventHandler](callback.md#rtceventhandler). |

**Return Value**

+ RtcEngine: A successfully created RtcEngine instance.  
+ Null: Failed to load the .so file. No instance is returned. 


**Notes**

+ You can only create 1 instance of RtcEngine. 
+ The lifecycle of the handler must be longer than that of RtcEngine. The handler must be created before calling this API and destroyed after calling [destroyRtcEngine](#RtcEngine-destroyrtcengine).  


<span id="RtcEngine-destroyrtcengine"></span>
### destroyRtcEngine
```java
public synchronized int com.vewtn.RtcEngine.destroyRtcEngine()
```
Destroy the RtcEngine instance created by [createRtcEngine](#RtcEngine-creatertcengine), and release all related resources. <br>

**Return Value**

+ 0: Success. 
+ <0: Failure.


**Notes**

+ Call this API after all business scenarios related to the instance are destroyed.  
+ When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  
+ Calling this API will start the SDK exit logic. The thread is held until the exit logic is complete. The thread is retained until the exit logic is complete.  
Therefore, do not call this API directly in the callback thread, or wait for the execution of the main thread in the callback and call this API in the main thread at the same time. Otherwise, it will cause a deadlock.


<span id="RtcEngine-createpublisher"></span>
### createPublisher
```java
public abstract Publisher com.vewtn.RtcEngine.createPublisher(
    PubExtraParam pubExtraParam)
```
Create the Publisher.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| pubExtraParam | **PubExtraParam** | 设置发布的音视频源 |

**Return Value**

+ Publisher: A successfully created Publisher instance.  
+ Null: Failed to create. Possible reasons are: Creating an internal source Publisher again after it has already been created. 


**Notes**

+ Publisher can create as many external sources as you want, but only up to one internal source for audio and video. 


<span id="RtcEngine-destroypublisher"></span>
### destroyPublisher
```java
public abstract void com.vewtn.RtcEngine.destroyPublisher(
    Publisher publisher)
```
Destroy the Publisher instance created by [createPublisher](#RtcEngine-createpublisher), and release all related resources. <br>


**Notes**

+ Call this API after all business scenarios related to the instance are destroyed.  
+ When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  


<span id="RtcEngine-createsubscriber"></span>
### createSubscriber
```java
public abstract Subscriber com.vewtn.RtcEngine.createSubscriber(
    SubExtraParam subExtraParam)
```
Create the Subscriber.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| subExtraParam | **SubExtraParam** | 设置发布的音视频源 |

**Return Value**

+ Subscriber: A successfully created Subscriber instance.  
+ Null: Failed to create. Possible reasons are: Creating an internal source Subscriber again after it has already been created. 


**Notes**

+ Subscriber can create as many external sources as you want, but only up to one internal source for audio and video. 


<span id="RtcEngine-destroysubscriber"></span>
### destroySubscriber
```java
public abstract void com.vewtn.RtcEngine.destroySubscriber(
    Subscriber subscriber)
```
Destroy the Subscriber instance created by [createSubscriber](#RtcEngine-createsubscriber), and release all related resources. <br>


**Notes**

+ Call this API after all business scenarios related to the instance are destroyed.  
+ When the API is called, the SDK destroys all memory associated with the instance and stops any interaction with the media server.  


# Subscriber
```java
public interface com.vewtn.Subscriber
```

Subscriber Class


## Member Functions

| Return | Name |
| --- | --- |
| **int** | [setRemoteView](#Subscriber-setremoteview) |
| **int** | [setEnableSpeaker](#Subscriber-setenablespeaker) |
| **int** | [startSubscribe](#Subscriber-startsubscribe) |
| **int** | [stopSubscribe](#Subscriber-stopsubscribe) |
| **boolean** | [isSubscribed](#Subscriber-issubscribed) |
| **int** | [setRemoteAudioMute](#Subscriber-setremoteaudiomute) |
| **int** | [setRemoteVideoMute](#Subscriber-setremotevideomute) |


## Instructions
<span id="Subscriber-setremoteview"></span>
### setRemoteView
```java
int com.vewtn.Subscriber.setRemoteView(
    View view)
```
Binds the view to be used for remote video rendering. <br>

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| view | **View** | View object. The object must be of effective lifecycle. |

**Return Value**

+ 0: Success 
+ -1: Failure. 


**Notes**

+ You must bind the view before subscribe to the video stream. 
+ To unbind the view, call this API with the view set to `null`.


<span id="Subscriber-setenablespeaker"></span>
### setEnableSpeaker
```java
int com.vewtn.Subscriber.setEnableSpeaker(
    Boolean enable)
```
Switch the rendering device for audio stream: earpiece or speakerphone.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| enable | **Boolean** | • true: earpiece <br/>• false：speakerphone |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Speakerphone is the default device. 
+ You can switch the audio rendering device before or after the audio rendering.


<span id="Subscriber-startsubscribe"></span>
### startSubscribe
```java
int com.vewtn.Subscriber.startSubscribe(
    String url)
```
Subscribe the remote media streams. <br>
After calling the API, you will receive [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged).

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| url | **String** | The url for WHIP request. <br/>Concatenate the fixed url `https://wtn.volcvideo.com`, SUB method, appID, and your specified streamID into the value of url. <br/>The url is like `https://wtn.volcvideo.com/sub/<appID>/<streamID>`. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ You can only subscribe to one media stream with one Subscriber instance. 
+ Ensure the stream is published before subscribe to the stream. 
+ Call [setRemoteView](#Subscriber-setremoteview) to set the remote view for the media stream before subscribing. 
+ Call [stopSubscribe](#Subscriber-stopsubscribe) to stop subscribing.


<span id="Subscriber-stopsubscribe"></span>
### stopSubscribe
```java
int com.vewtn.Subscriber.stopSubscribe()
```
Stop subscribing to the media stream.

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**

+ Call [startSubscribe](#Subscriber-startsubscribe) to start subscribing. 
+ Receive [onSubscribeStateChanged](callback.md#RtcEventHandler-onsubscribestatechanged) after calling this API.


<span id="Subscriber-issubscribed"></span>
### isSubscribed
```java
boolean com.vewtn.Subscriber.isSubscribed()
```
Inspect the subscribing state.

**Return Value**

+ true: Subscribing 
+ false: Not subscribing 


<span id="Subscriber-setremoteaudiomute"></span>
### setRemoteAudioMute
```java
int com.vewtn.Subscriber.setRemoteAudioMute(
    boolean mute)
```
Mute the remote audio. Do not subscribe the captured audio.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **boolean** | Whether to subscribe the remote audio. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**
You can call this API before or after the media stream is published.

<span id="Subscriber-setremotevideomute"></span>
### setRemoteVideoMute
```java
int com.vewtn.Subscriber.setRemoteVideoMute(
    boolean mute)
```
Mute the remote video. Do not subscribe the captured video.

**Input Parameters**

| Name | Type | Instructions |
| --- | --- | --- |
| mute | **boolean** | Whether to subscribe the remote video. |

**Return Value**

+ 0: Success 
+ < 0: Failure 


**Notes**
You can call this API before or after the media stream is published.


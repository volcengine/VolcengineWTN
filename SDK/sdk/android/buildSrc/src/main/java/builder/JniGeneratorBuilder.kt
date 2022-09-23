package builder

import utils.MyJniGenerator

class JniGeneratorBuilder {
    private val jniGenerator: MyJniGenerator = MyJniGenerator()

    fun build() {
        jniGenerator
            .appendBoard("$PackageName.jni", "NativeFunctions.java")
            .appendBoard("$PackageName.internal", "PublisherImpl.java")
            .appendBoard("$PackageName.internal", "PublisherEventHandlerImpl.java")
            .appendBoard("$PackageName.internal", "SubscriberImpl.java")
            .appendBoard("$PackageName.internal", "SubscriberEventHandlerImpl.java")
            .appendBoard("$WebRTCPackageName", "Histogram.java")
            .appendBoard("$WebRTCPackageName", "JniCommon.java")
            .appendBoard("$WebRTCPackageName", "Logging.java")
            .appendBoard("$WebRTCPackageName", "JNILogging.java")
            .appendBoard("$WebRTCPackageName", "YuvHelper.java")
            .appendBoard("$WebRTCPackageName", "VideoEncoderFactory.java")
            .appendBoard("$WebRTCPackageName", "VideoDecoderFactory.java")
            .appendBoard("$WebRTCPackageName", "H264Utils.java")
            .appendBoard("$WebRTCPackageName", "VideoSink.java")
        jniGenerator.generateJNI()
    }

    fun clean() {
        jniGenerator.cleanJNI()
    }

    companion object {
        const val WebRTCPackageName = "org.webrtc"
        const val PackageName = "com.vewtn"
    }
}
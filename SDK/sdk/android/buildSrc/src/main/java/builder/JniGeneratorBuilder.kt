package builder

import utils.MyJniGenerator

class JniGeneratorBuilder {
    private val jniGenerator: MyJniGenerator = MyJniGenerator()

    fun build() {
        jniGenerator
            .appendBoard("$PackageName.jni", "NativeFunctions.java")
            .appendBoard("$PackageName.internal", "RtcEventHandlerImpl.java")
            .appendBoard("$PackageName.internal", "InternalEncodedVideoFrame.java")
            .appendBoard("$PackageName.internal", "InternalAudioFrame.java")
            .appendBoard("$PackageName.internal", "InternalRtcAudioStats.java")
            .appendBoard("$PackageName.internal", "InternalRtcVideoStats.java")
            .appendBoard("$PackageName.internal", "InternalVideoFrame.java")
            .appendBoard("$PackageName.internal", "InternalStallInfo.java")
        jniGenerator.generateJNI()
    }

    fun clean() {
        jniGenerator.cleanJNI()
    }

    companion object {
        const val PackageName = "com.vewtn"
    }
}
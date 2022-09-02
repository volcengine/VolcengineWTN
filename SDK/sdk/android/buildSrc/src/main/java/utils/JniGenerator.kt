package utils

import java.io.File

open class JniGenerator {
    private val jniGenCommandList = ArrayList<String>()
    protected val projectDir = File(System.getProperty("user.dir"), "../../../veWTN/sdk/android")
    private val jniGeneratorCMD = File(projectDir, "jni_generator/jni_generator.py")

    private val commandTemplate:
            ((model: Model, packageName: String, fileName: String) -> String) = { model, packageName, fileName ->
        val inputFile = File(projectDir, model.javaSrc)
                .let { packageRootPath ->
                    File(packageRootPath, packageName.replace(".", "/").trim('/'))
                }
                .let { packageNamePath ->
                    File(packageNamePath, fileName.trim('/'))
                }.absolutePath

        val outputFile = File(projectDir, model.outPutDir)
                .let { packageRootPath ->
                    File(packageRootPath, packageName.replace(".", "/").trim('/'))
                }
                .apply { if (!exists()) mkdirs() }
                .absoluteFile

        val includeFile = File(projectDir, "../../third_party/webrtc/headers/sdk/android/src/jni/jni_generator_helper.h").absolutePath;
        "python2 ${jniGeneratorCMD.absolutePath}" +
                " --input_file $inputFile" +
                " --output_dir $outputFile " +
                "-n ${model.namespace} " +
                "--includes $includeFile"
    }

    fun append(model: Model, packageName: String, fileName: String): JniGenerator {
        commandTemplate.invoke(model, packageName, fileName).apply {
            jniGenCommandList.add(this)
        }
        return this
    }

    fun generateJNI() {
        jniGenCommandList.forEach {
            println(it)
            it.runCommand(projectDir)
        }
    }

    open fun cleanJNI(){
    }
}
package utils

import java.io.File
import java.util.*
import java.util.concurrent.TimeUnit

fun String.runCommand(workingDir: File) : String? {
    return ProcessBuilder(*this.split("\\s".toRegex()).toTypedArray())
            .directory(workingDir)
            .redirectOutput(ProcessBuilder.Redirect.PIPE)
            .redirectError(ProcessBuilder.Redirect.PIPE)
            .start()
            .apply { waitFor(60, TimeUnit.MINUTES) }
            .inputStream.bufferedReader().readText()
}

fun String.captureName(): String {
    return substring(0, 1).toUpperCase(Locale.ROOT) + substring(1)
}


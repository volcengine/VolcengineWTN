import builder.JniGeneratorBuilder

class Prebuild {
    fun preBuild() {
        println("Generator JNI Files")
        var builder = JniGeneratorBuilder()
        builder.build();
    }

    fun clean(){
        println("clean Generated JNI Files")
        var builder = JniGeneratorBuilder()
        builder.clean();
    }
}

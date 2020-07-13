# SDL2ANDDESK
## Descripción.
Estructura de proyecto para trabajar con SDL2 tanto en plataformas ANDROID y Linux/OSX.

## Entorno.
** WIP **


## Estructura de carpetas.
### ASSETS. ###
Contendrá los assets gráficos y de sonido.

### PLATFORMS. ###
Contiene las plataformas sobre las que se va a construir el binario.

En estos momentos se soporta:
* Android.
* Desktop (Linux / OSX).

### SOURCES. ###
Contiene los fuentes de la librería SDL y algunas extensiones (de momento SDL_image) junto con los fuentes del juego.


## Preparación ##
### Android ###
Se crean accessos directos a las carpetas de los fuentes y de los assets:

Para los fuentes:
~~~~
cd platforms/android/app/jni
ln -s ../../../../sources/SDL
ln -s ../../../../sources/SDL_image
ln -s ../../../../sources/src
ln -s ../../../../sources/CMakeLists.txt
~~~~

Para los assets:
~~~~
cd ..
cd src/main
ln -s ../../../../../assets
~~~~

Editamos el fichero **build.gradle** que se encuentra bajo **platforms/android/app** y comentamos el proceso de construcción basado en makefiles y descomentamos el proceso de construcción basado en cmake.

Para las plataformas he dejado:
* armeabi-v7a (para un dispositivo real)
* x86 y x86_64 (para emuladores)

Establecemos la versión mínima del api a 25.

Nos quedará algo similar a esto:
~~~
def buildAsLibrary = project.hasProperty('BUILD_AS_LIBRARY');
def buildAsApplication = !buildAsLibrary
if (buildAsApplication) {
    apply plugin: 'com.android.application'
}
else {
    apply plugin: 'com.android.library'
}

android {
    compileSdkVersion 26
    defaultConfig {
        if (buildAsApplication) {
            applicationId "org.libsdl.app"
        }
        minSdkVersion 25
        targetSdkVersion 26
        versionCode 1
        versionName "1.0"
        externalNativeBuild {
            // ndkBuild {
            //     arguments "APP_PLATFORM=android-16"
            //     abiFilters 'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'
            // }
            cmake {
                arguments "-DANDROID_APP_PLATFORM=android-25", "-DANDROID_STL=c++_static", "-DANDROID"
                // abiFilters 'armeabi-v7a', 'arm64-v8a', 'x86', 'x86_64'
                // abiFilters 'arm64-v8a'
                abiFilters 'armeabi-v7a', 'x86', 'x86_64'
            }
        }
    }
    buildTypes {
        release {
            minifyEnabled false
            proguardFiles getDefaultProguardFile('proguard-android.txt'), 'proguard-rules.pro'
        }
    }
    if (!project.hasProperty('EXCLUDE_NATIVE_LIBS')) {
        sourceSets.main {
            jniLibs.srcDir 'libs'
        }
        externalNativeBuild {
            // ndkBuild {
            //     path 'jni/Android.mk'
            // }
            cmake {
                path 'jni/CMakeLists.txt'
            }
        }
       
    }
    lintOptions {
        abortOnError false
    }
    
    if (buildAsLibrary) {
        libraryVariants.all { variant ->
            variant.outputs.each { output ->
                def outputFile = output.outputFile
                if (outputFile != null && outputFile.name.endsWith(".aar")) {
                    def fileName = "org.libsdl.app.aar";
                    output.outputFile = new File(outputFile.parent, fileName);
                }
            }
        }
    }
}

dependencies {
    implementation fileTree(include: ['*.jar'], dir: 'libs')
}
~~~



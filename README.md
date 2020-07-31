# SDL2ANDDESK
## Descripción.
Estructura de proyecto para trabajar con SDL2 tanto en plataformas ANDROID y Linux/OSX.

## Entorno.
### Android ###
* java: 1.8.0_144
* Android Studio: 4.0
* cmake: 3.10.2.4988404
* ndk: 21.3.6528147
* build-tools: 29.0.2

### iOS ###
* osx: 10.13.6
* xcode: 10.1


### Desktop ###
#### OSX ####
* osx: 10.13.6
* cmake: 3.17.3
* clang: Apple LLVM version 10.0.0 (clang-1000.10.44.4)

#### Linux ####
*WIP*

## Fuentes SDL2.
Cogemos los fuentes de **http://hg.libsdl.org/**
* SDL (seleccionamos la tag **release-2.0.12**) y descargamos el zip.
* SDL_image (seleccionamos la tag **release-2.0.5**) y descargamos el zip.


## Estructura de carpetas.
Creamos una carpeta p.e **sdl2anddesk** y creamos los siguientes directorios:
~~~
mkdir sdl2anddesk
cd sdl2anddesk
mkdir assets
mkdir sources
mkdir platforms
~~~


### ASSETS. ###
Contendrá los assets gráficos y de sonido.

### SOURCES. ###
Contiene los fuentes de la librería SDL y algunas extensiones (de momento SDL_image) junto con los fuentes del juego.

~~~
mkdir SDL
mkdir SDL_image
mkdir src
~~~

#### SDL. ####
Del zip descargado de la librería SDL vamos a descomprimirlo donde se quiera, y vamos a copiar en esta carpeta **SDL** creada lo siguiente:
* el directorio **cmake**
* el directorio **include**
* el directorio **src**
* el fichero **cmake_uninstall.cmake.in**
* el fichero **CMakeLists.txt**
* el fichero **sdl2.m4**
* el fichero **sdl2-config.in**
* el fichero **sdl2-config-version.cmake.in**
* el fichero **sdl2-config.cmake.in**
* el fichero **sdl2.pc.in**
* el fichero **SDL2.spec.in**
* el fichero **SDL2Config.cmake**

#### SDL_image. ####
Del zip descargado de la librería SDL_image vamos a descomprimirlo donde se quiera, y vamos a copiar en esta carpeta **SDL_image** creada lo siguiente:
* el directorio **external**
* todos los ficheros *.c
* todos los ficheros *.m
* todos los ficheros *.h
* el fichero **SDL2_image.spec**
* el fichero **SDL2_image.pc.in**
* el fichero **SDL2_image.spec.in**

Aquí nos harian falta dos CMakeList.txt, los vamos a coger de aqui: [SDL-mirror](https://github.com/SDL-mirror).
* Descargamos el CMakeList.txt del proyecto SDL_image
* Descargamos el CMakeList.txt de external/jpeg

### PLATFORMS. ###
Contiene las plataformas sobre las que se va a construir el binario.

En estos momentos se soporta:
* Android.
* iOS.
* Desktop (Linux / OSX).

Del zip descargado de la librería SDL copiamos el directorio **android-project** y lo renombramos a **android**, y con esto podemos generar el apk.

Creamos una carpeta desktop para generar el binario en linux/osx.

Creamos una carpeta ios y en ella copiamos dos directorios que encontraremos en los zip descargados, que son:
* SDL/Xcode-iOS/SDL (la dejamos como SDL)
* SDL_image/XCode-iOS (la renombramos como SDL_image)

En ambas debe estar la carpeta de proyecto de xcode.


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

### iOS ###
Tenemos que configurar cada uno de los proyectos que hemos añadido para generar las librerías estáticas.

#### SDL ####
Abrimos este proyecto con xcode.
**Public Headers** y **Library Source** aparecen en rojo porque no encuentra los fuentes. Para solucionar esto hacemos los siguiente:
1. Seleccionamos **Public Headers**.
2. En la columna de la derecha de xcode en la sección **Identity and Type** bajo **Location** hay una ruta relativa incorrecta. Si pulsamos sobre el icono de carpeta, debemos seleccionar el directorio include de SDL. Nos debe de quedar así:

~~~
../../../sources/SDL/include
~~~

3. Seleccionamos **Library Source** y hacemos la misma operación, pero ahora tiene que apuntar al directorio src de SDL. Nos debe de quedar así:

~~~
../../../sources/SDL/src
~~~

Seleccionamos **SDL** bajo *PROJECT* y buscamos la sección **Linking**. En **Other Linker Flags** añadimos:
~~~
$(inherited)
~~~

Compilamos el target **libSDL-iOS**.


#### SDL_image ####
Abrimos este proyecto con xcode.
Van a aparecer en rojo el fichero SDL_image.h y los ficheros fuente. Hay que ir uno por uno y operar de la misma manera que en la sección anterior (pero ahora por cada fichero).

Seleccionamos **SDL_image** bajo *PROJECT* y buscamos la sección:
1. **Linking**. En **Other Linker Flags** añadimos:
~~~
$(inherited)
~~~

2. **Search Path**. En **Header Search Paths** añadimos lo siguiente: 
~~~
$(SRCROOT)/../../../sources/SDL/include
~~~

Compilamos el target **libSDL_image-iOS**.


#### app ####
Creamos un proyecto llamado **app** (p.e) bajo **platforms/ios**.
El tipo de proyecto estará basado en vista.

Hacemos los siguientes cambios:
1. Borramos los ficheros **AppDelegate.h** y **AppDelegate.m**
2. Borramos el fichero **main.m**
3. Borramos el fichero **MainStoryboard**.
4. Editamos el fichero **info.plist** y la entrada **Main storyboard dile base name** la dejamos vacia.

5. Seleccionamos **app** bajo *PROJECT* y buscamos la sección:
    
* **Linking**. En **Other Linker Flags** añadimos:
~~~
$(inherited)
~~~

* **Search Paths**. En **Header Search Paths** añadimos lo siguiente: 

~~~
$(SRCROOT)/../../../sources/SDL/include
$(SRCROOT)/../../../sources/SDL_image
~~~

* **Search Paths**. En **Library Search Paths** añadimos lo siguiente: 

~~~
$(SRCROOT)/../SDL/output/iOS/debug
$(SRCROOT)/../SDL_image/output/iOS/debug
~~~

6. Seleccionamos **app** bajo *TARGETS* y seleccionamos **Build Phases**:

Seleccionamos la sección **Link Binary With Libraries** y añadimos los siguientes *Frameworks*:

~~~
CoreAudio.framework
Foundation.framework
UIKit.framework
CoreGraphics.framework
OpenGLES.framework
QuartzCore.framework
AudioToolbox.framework
CoreMotion.framework
GameController.framework
AVFoundation.framework
Metal.framework
CoreBluetooth.framework
CoreServices.framework
~~~

Y las siguientes librerías:
~~~
libSDL2.a
libSDL2_image.a
~~~

7. Finalmente añadimos los fuentes (*.cpp/*.hpp) bajo el grupo **app** (en amarillo) y también añadimos los **assets** a la altura de **app** (debe quedar en color azul).

Compilamos.

### Desktop ###
En el directorio **SDL_image** compilamos la libreria **zlib** antes de nada, ya que hace uso de ella la librería **libpng**.

~~~
cd SDL_image/external/zlib-1.2.11
mkdir build
cd build
cmake ..
make
~~~

una vez que tengamos la librería compilada creamos bajo **build** dos directorios, uno para las cabeceras publicas y otro para las librerías creadas:

~~~
mkdir include
mkdir lib
~~~

Copiamos las librerías y creamos los enlaces correspondientes:
~~~
cp libz.a lib
cp libz.1.2.11.dylib lib
cd lib
ln -s libz.1.2.11.dylib libz.1.dylib
ln -s libz.1.dylib libz.dylib
cd ..
~~~

Copiamos los archivos de cabecera siguientes:
~~~
cp zconf.h include
cp ../zlib.h include
~~~

Con esto ya se podrá realizar el proceso de compilación completo.

Desde el raiz de nuestro proyecto, vamos a **desktop**:

~~~
cd desktop
mkdir build
cd build
cmake -DZLIB_INCLUDE_DIR=../../../sources/SDL_image/external/zlib-1.2.11/build/include -DZLIB_LIBRARY=../../../sources/SDL_image/external/zlib-1.2.11/build/lib/libz.dylib ../../../sources
make
~~~

Si no hay ningún error, el binario se encuentra en **build/src**, y enlazamos los assets a la altura de este directorio:

~~~
cd src
ln -s ../../../../assets
~~~

Y ya se puede ejecutar:
~~~
./main
~~~



small3d boilerplate
===================

This repository is a basis for building games and other applications with the 
small3d game development library ( https://github.com/dimi309/small3d ). **It is
referencing version 1.8016 of small3d, which was the last version supporting
mobile devices. If you would like to use a more recent version you can reference
small3d directly, or modify this project accordingly.**

The code is reasonably good. It has been extracted from
the Islet Hell game, which has been published on [Steam](https://store.steampowered.com/app/2069750/Islet_Hell/),
and formerly also on the Apple App Store and the Android Play Store.

It provides the following:

- A cross platform project in the root folder that can be built using
  conan: https://conan.io/
- Android project (using the same source code with the necessary
  #defines) in the `android` folder.
- iOS project (using the same source code with the necessary
  #defines) in the `ios` folder. It runs on OpenGL ES.

A basic user interface is supported for the abovementioned platforms.
The keyboard arrows and space work on the PC and corresponding
visual controls are provided on mobile:

![mobile interface](mobile-interface.png)
  
The boilerplate has been published separately from small3d because it 
takes an opinionated approach on how to build small3d-based
projects. small3d on the other hand can be used in many ways.
For example, if you use the library itself without this boilerplate
you are not obliged to build it with conan.io and you can choose
the directory structure of your project.

Notes: 
- Shadow mapping is too slow on the iPad 2.

It is assumed that the necessary development tools are installed
on your machine (e.g. gcc, Visual Studio, cmake, conan, 
Android studio, Xcode) depending on the build that interests
you.

# Building on PC (Windows, MacOS, Linux, FreeBSD)

Deploy my variation of the portaudio package for conan as a prerequisite:

	git clone https://github.com/dimi309/portaudio-conan
	cd portaudio-conan
	conan export . --version=19.7.0
	
And of course, small3d too:

	git clone https://github.com/dimi309/small3d-conan
	cd small3d-conan
	conan export . --version=1.8016
	 
Then clone this repository and build and run the game:

     git clone https://github.com/dimi309/small3d-boilerplate
     cd small3d-boilerplate
     conan build . --build=missing
     cd bin
     ./game
     # (Or just `game` on Windows)
	 


# Building for Android

After cloning: https://github.com/dimi309/small3d

... please make sure to check out the last version that supported mobile 
platforms:

	git checkout 1.8016.last.mobile

Build small3d for android (see small3d README file). Then copy:

	small3d/build/include to small3d-boilerplate/android/app/deps/include
	small3d/build/lib to small3d-boilerplate/android/app/deps/lib
	small3d/build/shaders to small3d-boilerplate/android/app/src/main/assets/resources/shaders

Open and build the project in `small3d-boilerplate/android` with Android studio.

# Building for iOS

After cloning: https://github.com/dimi309/small3d

... please make sure to check out the last version that supported mobile 
platforms:

	git checkout 1.8016.last.mobile

Build small3d for ios (see small3d README file). Then, copy:

	small3d/build/include to small3d-boilerplate/ios/deps/include
	small3d/build/lib to small3d-boilerplate/ios/deps/lib
	small3d/build/shaders to small3d-boilerplate/ios/game/resources1/shaders

Then open the project `small3d-boilerplate/ios/game.xcodeproj` using Xcode.

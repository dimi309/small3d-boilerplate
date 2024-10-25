small3d boilerplate
===================

This repository is a basis for building games and other applications with the 
small3d game development library ( https://github.com/dimi309/small3d ).

The code is reasonably good. It has been extracted from
the Islet Hell game, which has been published on [Steam](https://store.steampowered.com/app/2069750/Islet_Hell/).

It provides the following a cross platform project that can be built using 
conan: https://conan.io/

A basic user interface is provided that uses the keyboard arrows and space.

The boilerplate has been published separately from small3d because it 
takes an opinionated approach on how to build small3d-based
projects. small3d on the other hand can be used in many ways.
For example, if you use the library itself without this boilerplate
you are not obliged to build it with conan.io and you can choose
the directory structure of your project.

It is assumed that the necessary development tools are installed
on your machine (e.g. gcc, Visual Studio, cmake, conan, Xcode) 
depending on the build that interests you.

# Building on PC (Windows, MacOS, Linux, FreeBSD)

Deploy my variation of the portaudio package for conan as a prerequisite:

	git clone https://github.com/dimi309/portaudio-conan
	cd portaudio-conan
	conan export . --version=19.7.0
	
And of course, small3d too:

	git clone https://github.com/dimi309/small3d-conan
	cd small3d-conan
	conan export . --version=master
	 
Then clone this repository and build and run the game:

     git clone https://github.com/dimi309/small3d-boilerplate
     cd small3d-boilerplate
     conan build . --build=missing
     cd bin
     ./game
     # (Or just `game` on Windows)

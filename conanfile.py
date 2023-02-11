from conans import ConanFile, CMake, tools

class Small3dGame(ConanFile):
    settings = "os", "compiler", "build_type", "arch", "cppstd"
    options = {"vulkan": [True, False]}
    default_options = {"vulkan": False}
    generators = "cmake"
    requires = "small3d/master@dimi309/small3d"

    def config_options(self):
        self.options["small3d"].vulkan = self.options.vulkan
        
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        if self.options.vulkan:
            self.copy("*.spv", dst="", src="")
        else:
            self.copy("*.vert", dst="", src="")
            self.copy("*.frag", dst="", src="")
        

from conan import ConanFile
from conan.tools.cmake import cmake_layout

class MyProject(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        # Define dependencies here
        self.requires("asio/1.36.0")
        self.requires("benchmark/1.9.4")
        self.requires("catch2/3.13.0")
        self.requires("ms-gsl/4.2.0")
        self.requires("quill/11.0.2")

    def layout(self):
        # Uses standard CMake layout (build folders, etc.)
        cmake_layout(self)


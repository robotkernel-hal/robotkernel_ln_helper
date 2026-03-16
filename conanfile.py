from conan import ConanFile
from conan import tools
from conan.tools.files import mkdir, chdir, copy, save
from conan.tools.gnu import Autotools, AutotoolsToolchain
import os, re

class MainProject(ConanFile):
    name = "robotkernel_ln_helper"
    license = "GPLv3"
    author = "Robert Burger <robert.burger@dlr.de>"
    generators = "PkgConfigDeps"
    url = "undefined"
    description = "Helpers for links-and-nodes support."
    exports_sources = ["*", "!.gitignore" ]
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("yaml-cpp/0.7.0@3rdparty/stable", transitive_headers=True, transitive_libs=True)

    def source(self):
        self.run(f"sed 's/AC_INIT(.*/AC_INIT([robotkernel], [{self.version}], [{self.author}])/' configure.ac.in > configure.ac")

    def generate(self):
        tc = AutotoolsToolchain(self)
        tc.generate()

    def build(self):
        self.run("autoreconf -if")
        autotools = Autotools(self)
        autotools.libs=[]
        autotools.include_paths=[]
        autotools.library_paths=[]
        if self.settings.build_type == "Debug":
            autotools.flags = ["-O0", "-g", "-fno-builtin-strlen"]
        else:
            autotools.flags = ["-O2"]
        autotools.configure()
        autotools.make()

    def package(self):
        autotools = Autotools(self)
        autotools.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.libs = ['ln_helper']
        self.cpp_info.bindirs = ['bin']
        self.cpp_info.libdirs = ['lib']
        self.cpp_info.resdirs = ['share']

        self.runenv_info.append_path("LD_LIBRARY_PATH", f"{self.package_folder}/lib")
        self.runenv_info.append_path("PATH", f"{self.package_folder}/bin")


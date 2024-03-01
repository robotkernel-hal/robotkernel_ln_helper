import os
import re

from conan import ConanFile
from conan.tools.gnu import Autotools


class MainProject(ConanFile):
    package_type = "shared-library"
    name = "ln_helper"
    license = "GPLv3"
    author = "Robert Burger <robert.burger@dlr.de>"
    generators = "AutotoolsToolchain", "PkgConfigDeps"
    url = "undefined"
    description = "Helpers for links-and-nodes support."
    exports_sources = ["*", "!.gitignore"]
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("libstring_util/[~1]@common/stable", transitive_libs=True)
        self.requires("yaml-cpp/0.7.0@3rdparty/stable", transitive_libs=True)

    def source(self):
        filedata = None
        filename = "project.properties"

        if not os.path.isfile(filename):
            return

        with open(filename, "r") as f:
            filedata = f.read()
        with open(filename, "w") as f:
            f.write(re.sub("VERSION *=.*[^\n]", f"VERSION = {self.version}", filedata))

    def build(self):
        self.run("autoreconf -if")
        autotools = Autotools(self)
        autotools.configure()
        autotools.make()

    def package(self):
        autotools = Autotools(self)
        autotools.install()

    def package_info(self):
        self.cpp_info.includedirs = ["include"]
        self.cpp_info.bindirs = ["bin"]
        self.cpp_info.libdirs = ["lib"]
        self.cpp_info.resdirs = ["share"]

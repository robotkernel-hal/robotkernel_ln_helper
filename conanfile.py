from conans import ConanFile, AutoToolsBuildEnvironment, tools
import os, re

class MainProject(ConanFile):
    name = "ln_helper"
    license = "GPLv3"
    author = "Robert Burger <robert.burger@dlr.de>"
    generators = "pkg_config"
    url = "undefined"
    description = "Helpers for links-and-nodes support."
    exports_sources = ["*", "!.gitignore"] + ["!%s" % x for x in tools.Git().excluded_files()]
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("libstring_util/[~=1]@common/stable")
        self.requires("yaml-cpp/0.6.2@3rdparty/stable")
    
    def source(self):
        filedata = None
        filename = "project.properties"

        if not os.path.isfile(filename):
            return

        with open(filename, 'r') as f:
            filedata = f.read()
        with open(filename, 'w') as f:
            f.write(re.sub("VERSION *=.*[^\n]", f"VERSION = {self.version}", filedata))

    def build(self):
        self.run("autoreconf -if")
        autotools = AutoToolsBuildEnvironment(self)
        autotools.libs=[]
        autotools.include_paths=[]
        autotools.library_paths=[]
        if self.settings.build_type == "Debug":
            autotools.flags = ["-O0", "-g", "-fno-builtin-strlen"]
        else:
            autotools.flags = ["-O2"]
        autotools.configure(configure_dir=".")
        autotools.make()

    def package(self):
        autotools = AutoToolsBuildEnvironment(self)
        autotools.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']
        self.cpp_info.bindirs = ['bin']
        self.cpp_info.libdirs = ['lib']
        self.cpp_info.resdirs = ['share']

    


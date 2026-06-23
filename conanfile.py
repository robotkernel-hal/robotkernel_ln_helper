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
    libs = [ 'ln_helper' ]

    def requirements(self):
        self.requires("yaml-cpp/0.7.0@3rdparty/stable", transitive_headers=True, transitive_libs=True)

    def layout(self):
        self.folders.source = "."
        self.folders.build = "build"
        self.folders.generators = "conan"

        # In the local folder (when the package is in development, or "editable") the artifacts can be found:
        self.cpp.source.includedirs = ["include", "build/include"]
        self.cpp.build.bindirs = ["src"]
        self.cpp.build.libdirs = ["src/.libs"]
        self.cpp.build.libs = self.libs

        # In the Conan cache, we packaged everything at the default standard directories
        self.cpp.package.libs = self.libs
        self.cpp.package.bindirs = ["bin"]

        self.layouts.package.runenv_info.append_path("PATH", "bin")

    def generate(self):
        tc = AutotoolsToolchain(self)
        tc.generate()

    def build(self):
        configure_ac_in = os.path.join(self.source_folder, "configure.ac.in")
        if os.path.exists(configure_ac_in):
            configure_ac = os.path.join(self.source_folder, "configure.ac")
            self.run(
                f'sed "s|PACKAGE_VERSION|{self.version}|" configure.ac.in > configure.ac',
                cwd=self.source_folder
            )
            print(f"[INFO] Created configure.ac from configure.ac.in")
        else:
            print(f"[WARNING] configure.ac.in not found at {configure_ac_in}")

        autotools = Autotools(self)
        autotools.libs=[]
        autotools.include_paths=[]
        autotools.library_paths=[]
        if self.settings.build_type == "Debug":
            autotools.flags = ["-O0", "-g", "-fno-builtin-strlen"]
        else:
            autotools.flags = ["-O2"]
        autotools.autoreconf(args = [ self.source_folder, ])
        autotools.configure(build_script_folder=self.source_folder)
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


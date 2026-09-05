#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
sources = [Glob("src/*.cpp"), "src/classes/speech_model/speech_model.cpp", "src/classes/speech_recognizer/speech_recognizer.cpp"]

env.Append(LIBPATH=["src/vosk/lib"])
env.Append(LIBS=["libvosk"])

if env["target"] in ["editor", "template_debug"]:
    doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("src/doc_classes/*.xml"))
    sources.append(doc_data)

library = env.SharedLibrary(
    "demo/addons/gdvosk/libgdvosk{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)

#!/usr/bin/env python
import os

env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["src/"])
sources = [
    Glob("src/*.cpp"),
    "src/classes/speech_model/speech_model.cpp",
    "src/classes/speech_recognizer/speech_recognizer.cpp",
]

env.Append(LIBPATH=["src/vosk/lib/" + env["platform"]])
env.Append(LIBS=["vosk"])

if env["platform"] == "linux":
    env.Append(LINKFLAGS=["-Wl,-rpath,'$$ORIGIN'"])
elif env["platform"] == "macos":
    env.Append(LINKFLAGS=["-Wl,-rpath,@loader_path"])

if env["target"] in ["editor", "template_debug"]:
    doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("src/doc_classes/*.xml"))
    sources.append(doc_data)

addon_dir = "demo/addons/gdvosk"
platform_dir = os.path.join(addon_dir, env["platform"])

# macOS ships as a flat Mach-O file named ".framework" (not a real bundle)
if env["platform"] == "macos":
    lib_filename = "libgdvosk.macos.template_release.framework"
    vosk_src = "src/vosk/lib/macos/libvosk.dylib"
    vosk_dst = os.path.join(platform_dir, "libvosk.dylib")
else:
    lib_filename = "libgdvosk{}{}".format(env["suffix"], env["SHLIBSUFFIX"])
    vosk_src = "src/vosk/lib/linux/libvosk.so"
    vosk_dst = os.path.join(platform_dir, "libvosk.so")

library = env.SharedLibrary(
    os.path.join(platform_dir, lib_filename),
    source=sources,
)

if env["platform"] in ("linux", "macos"):
    vosk_copy = env.Command(vosk_dst, vosk_src, Copy("$TARGET", "$SOURCE"))
    Depends(library, vosk_copy)

Default(library)
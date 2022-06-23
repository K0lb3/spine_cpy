from setuptools import setup, Extension, find_packages
import os
import sys
from Cython.Build import cythonize


def versioned_extension(version: str):
    fp = f"runtimes/{version}/spine-c"
    ext_name = f"spine_cpy.versions.{version.replace('.','_')}.spine"

    extra_compile_args = []
    # check if the version support .skel / binary skeleton
    if os.path.exists(os.path.join(fp, "include", "spine", "SkeletonBinary.h")):
        extra_compile_args.append("-DSKELETON_BINARY_SUPPORT")
    # the first major version used readAtlasFile(fp) instead of createAtlasFile(fp, renderobject)
    if version.startswith("1."):
        extra_compile_args.append("-DUSE_spAtlas_readAtlasFile")

    # print(version, extra_compile_args)

    # if building inplace, we have to create subdirs
    if "--inplace" in sys.argv:
        os.makedirs(ext_name.rsplit(".", 1)[0].replace(".", "/"), exist_ok=True)

    return Extension(
        ext_name,
        sources=[
            os.path.join(root, f)
            for root, dirs, files in os.walk(f"{fp}/src")
            for f in files
            if f.endswith((".c", ".cpp"))
        ]
        + [
            "spine_cpy_src/spine_cpy.c",
            "spine_cpy_src/extension.c",
            "spine_cpy_src/test.c",
        ],
        include_dirs=[f"{fp}/include", "."],
        py_limited_api=True,
        extra_compile_args=extra_compile_args,
    )


# create sub_modules for each version -> called by main depending on the need
setup(
    name="spine_cpy",
    description=" spine_cpy",
    author="K0lb3",
    packages=["spine_cpy"],
    ext_modules=cythonize(
        [versioned_extension(version) for version in os.listdir("runtimes")]
    ),
)

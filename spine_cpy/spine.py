import os
from PIL import Image
import importlib

LOCAL = os.path.dirname(os.path.realpath(__file__))
VERSION_DIR = os.path.join(LOCAL, "versions")

VERSIONS = {
    f.replace("_", "."): importlib.import_module(f"spine_cpy.versions.{f}.spine")
    for f in os.listdir(VERSION_DIR)
}


class Spine:
    module = None

    def __init__(self, version: str) -> None:
        if version not in VERSIONS:
            raise NotImplementedError(
                f"The spine version {version} isn't included in this build"
            )
        self.module = module = VERSIONS[version]
        module.set_callback(0, lambda x: self.spAtlasPage_createTexture(x))
        module.set_callback(1, lambda x: self.spAtlasPage_disposeTexture(x))
        module.set_callback(2, lambda x: self.spUtil_readFile(x))

    def spAtlasPage_createTexture(self, path: str) -> object:
        # implements the callback for spAtlasPage_createTexture
        print("Py: spAtlasPage_createTexture")
        print(path)
        ret = Image.open(path)
        return ret, ret.width, ret.height

    def spAtlasPage_disposeTexture(self, texture: object) -> None:
        # implements the callback for spAtlasPage_disposeTexture
        print("Py: spAtlasPage_disposeTexture")
        pass

    def spUtil_readFile(self, path: str) -> bytes:
        # implements the callback for spAtlasPage_createTexture
        print("Py: spUtil_readFile")
        print(path)
        with open(path, "rb") as f:
            raw = f.read()
        print(len(raw))
        return raw

    def test_callbacks(
        self,
        json_name: str,
        binary_name: str,
        atlas_name: str,
        scale: float = 1.0,
        animation_name: str = "",
    ):
        print(
            self.module.test_file_load(
                json_name, binary_name, atlas_name, scale, animation_name
            )
        )
        print("\o/")


class Atlas:
    def __init__(
        self,
    ) -> None:
        pass


# class AnimationState:
#     raw_data: object
#     raw: object

#     def __init__(self, skeletonData) -> None:
# 	    self.raw_data = spAnimationStateData_create(skeletonData)
# 	    self.raw = spAnimationState_create(self.raw_data)

#     def setAnimationByName(self, name, a:int = 0, b: int = 0):
#         spAnimationState_setAnimationByName(self.raw, a, name, b)

#     def update(self)

# class Skeleton:
#     raw_data: object
#     raw: object

#     def __init__(self, fp, atlas, scale) -> None:
#         if fp.endswith(".skel"):
#             self.raw_data = readSkeletonBinaryData(fp, atlas, scale)
#         elif fp.endswith(".json"):
#             self.raw_data = readSkeletonJsonData(fp, atlas, scale)
#         else:
#             raise NotImplementedError("Unknown file extension")

#         self.raw = spSkeleton_create(self.raw_data)

#     def update(self, d: int):
#         spSkeleton_update(self.raw_data, d)

#     def updateWorldTransform(self):
#         spSkeleton_updateWorldTransform(self.raw_data)


#     def __del__(self):
#         spSkeleton_dispose(self.raw)
#         spSkeletonData_dispose(self.raw_data)

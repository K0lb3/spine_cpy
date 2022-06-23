#define PY_SSIZE_T_CLEAN
#include "Python.h"
#include "spine_cpy_src/test.h"

#include "spine/Atlas.h"
#include "spine/extension.h"
#include "spine/SkeletonData.h"
#include "spine/SkeletonJson.h"
#ifdef SKELETON_BINARY_SUPPORT
#include "spine/SkeletonBinary.h"
#endif

spSkeletonData *readSkeletonJsonData(const char *filename, spAtlas *atlas, float scale)
{
    spSkeletonJson *json = spSkeletonJson_create(atlas);
    json->scale = scale;
    spSkeletonData *skeletonData = spSkeletonJson_readSkeletonDataFile(json, filename);
    if (!skeletonData)
    {
        printf("%s\n", json->error);
        exit(0);
    }
    spSkeletonJson_dispose(json);
    return skeletonData;
}

#ifdef SKELETON_BINARY_SUPPORT
spSkeletonData *readSkeletonBinaryData(const char *filename, spAtlas *atlas, float scale)
{
    spSkeletonBinary *binary = spSkeletonBinary_create(atlas);
    binary->scale = scale;
    spSkeletonData *skeletonData = spSkeletonBinary_readSkeletonDataFile(binary, filename);
    if (!skeletonData)
    {
        printf("%s\n", binary->error);
        exit(0);
    }
    spSkeletonBinary_dispose(binary);
    return skeletonData;
}
#endif

// static void test(spSkeletonData *skeletonData, spAtlas *atlas, char* animationName) {
// 	UNUSED(atlas);
// 	spSkeleton *skeleton = spSkeleton_create(skeletonData);
// 	spAnimationStateData *animData = spAnimationStateData_create(skeletonData);
// 	spAnimationState *animState = spAnimationState_create(animData);
// 	spAnimationState_setAnimationByName(animState, 0, animationName, 0);

// 	float d = 3;
// 	for (int i = 0; i < 1; i++) {
// 		spSkeleton_update(skeleton, d);
// 		spAnimationState_update(animState, d);
// 		spAnimationState_apply(animState, skeleton);
// 		spSkeleton_updateWorldTransform(skeleton);
// 		for (int ii = 0; ii < skeleton->bonesCount; ii++) {
// 			spBone *bone = skeleton->bones[ii];
// 			printf("%s %f %f %f %f %f %f\n", bone->data->name, bone->a, bone->b, bone->c, bone->d, bone->worldX, bone->worldY);
// 		}
// 		printf("========================================\n");
// 		d += 0.1f;
// 	}

// 	spSkeleton_dispose(skeleton);
// }

PyObject *test_file_load(PyObject *self, PyObject *args)
{
    // define vars
    char *jsonName;
    size_t jsonNameSize;
    char *binaryName;
    size_t binaryNameSize;
    char *atlasName;
    size_t atlasNameSize;
    char *animationName;
    size_t animationNameSize = 0;
    float scale = 1.0;

    if (!PyArg_ParseTuple(args, "s#s#s#|fs#", &jsonName, &jsonNameSize, &binaryName, &binaryNameSize, &atlasName, &atlasNameSize, &scale, &animationName, &animationNameSize))
        return NULL;

    printf("C: json name: %s\n", jsonName);
    printf("C: binary name: %s\n", binaryName);
    printf("C: atlas name: %s\n", atlasName);
    printf("C: scale: %f\n", scale);
    printf("C: animation name: %s\n", animationName);

    printf("C: entering spAtlas_createFromFile\n");
#ifdef USE_spAtlas_readAtlasFile
    // only used in version 1
    spAtlas *atlas = spAtlas_readAtlasFile(atlasName);
#else
    spAtlas *atlas = spAtlas_createFromFile(atlasName, 0);
#endif
    printf("C: /\n");

    printf("C: entering readSkeletonJsonData\n");
    spSkeletonData *skeletonData = readSkeletonJsonData(jsonName, atlas, scale);
    // if (animationNameSize)
    //     test(skeletonData, atlas, animationName);
    spSkeletonData_dispose(skeletonData);
    printf("C: /\n");

#ifdef SKELETON_BINARY_SUPPORT
    printf("C: entering readSkeletonBinaryData\n");
    skeletonData = readSkeletonBinaryData(binaryName, atlas, scale);
    // if (animationNameSize)
    //     test(skeletonData, atlas, animationName);
    spSkeletonData_dispose(skeletonData);
    printf("C: /\n");
#endif

    spAtlas_dispose(atlas);

    Py_INCREF(Py_None);
    return Py_None;
}
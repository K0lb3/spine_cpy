from spine_cpy.spine import Spine

import os
for version in os.listdir("runtimes"):
    print(version)
    s = Spine(version)
    # find test set
    name = "goblins"
    files = os.listdir(f"runtimes/{version}/data")
    cb_files = {
        "json_name": "",
        "binary_name": "",
        "atlas_name": ""
    }
    for file in files:
        if name in file:
            fp = os.path.join("runtimes",version,"data",file)
            if file.endswith(".json"):
                cb_files["json_name"] = fp
            elif file.endswith(".skel"):
                cb_files["binary_name"] = fp
            elif file.endswith(".atlas"):
                cb_files["atlas_name"] = fp
    s.test_callbacks(
        # json_name=r"d:\Projects\python_c\spine2\spine-runtimes-4.0\spine-sfml\c\data\spineboy-pro.json",
        # binary_name=r"d:\Projects\python_c\spine2\spine-runtimes-4.0\spine-sfml\c\data\spineboy-pro.skel",
        # atlas_name=r"d:\Projects\python_c\spine2\spine-runtimes-4.0\spine-sfml\c\data\spineboy-pma.atlas",
        **cb_files,
        scale = 2.0,
        #animation_name="walk"
    )
print()
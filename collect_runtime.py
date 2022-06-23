from urllib.request import urlopen
import json
from zipfile import ZipFile
from io import BytesIO
import os
import re

reSpineCFile = re.compile(r".+/(spine-c/(src|include)/.+)$")
reSpineSample = re.compile(r"^.+?/spine-sfml.*/(data/.+?\.(skel|json|atlas|png))$")

LOCAL = os.path.dirname(os.path.realpath(__file__))
runtimes = os.path.join(LOCAL, "runtimes")
CHUNCK_SIZE = 1024 ** 2

BANNED_VERSIONS = [
    "4.0.31",  # compile error due to findPathConstraintIndex being used without timeline argument
]


def download_with_progress(url) -> bytes:
    data = []
    read = 0
    print(url)
    res = urlopen(url)
    while True:
        print(f"downloaded: {read//1024}kb", end="\r")
        chunk = res.read(CHUNCK_SIZE)
        read += CHUNCK_SIZE
        data.append(chunk)
        if len(chunk) != CHUNCK_SIZE:
            break
    print(f"downloaded: {read//1024}kb")
    return b"".join(data)


def download_n_extract_runtime(version: str, zipball_url: str):
    fp = os.path.join(runtimes, version)
    if os.path.exists(fp):
        return

    zip_data = download_with_progress(zipball_url)
    with ZipFile(BytesIO(zip_data)) as z:
        for path, item in z.NameToInfo.items():
            if item.is_dir():
                continue
            match = reSpineCFile.match(path)
            if not match:
                match = reSpineSample.match(path)
            if match:
                zfp = os.path.join(fp, match[1])
                if os.path.exists(zfp):
                    continue
                os.makedirs(os.path.dirname(zfp), exist_ok=True)
                with open(zfp, "wb") as f:
                    with z.open(path) as zf:
                        f.write(zf.read())


def download_all_tags():
    tags = json.loads(
        urlopen(
            "https://api.github.com/repos/EsotericSoftware/spine-runtimes/tags"
        ).read()
    )
    for tag in tags[::-1]:
        if tag["name"] in BANNED_VERSIONS or tag["name"].startswith("spine-libgdx"):
            continue
        download_n_extract_runtime(tag["name"], tag["zipball_url"])


if __name__ == "__name__":
    download_all_tags()

#!/usr/bin/env python3
"""Fetch the pinned WebAssembly compiler SDK into a tools directory."""
import hashlib
from pathlib import Path
import sys, tarfile, urllib.request
NAME = "wasi-sdk-34.0-x86_64-linux"
SHA256 = "b761e3a0721dbae9c09a0059e5fdb2bf917d1b4a8a7b430fb3b5aafb0984b2c4"
URL = f"https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-34/{NAME}.tar.gz"
if len(sys.argv) != 2: raise SystemExit("usage: fetch-wasi-sdk.py TOOLS_DIRECTORY")
dest = Path(sys.argv[1]).resolve()
dest.mkdir(parents=True, exist_ok=True)
archive = dest / (NAME + ".tar.gz")
if not archive.exists(): urllib.request.urlretrieve(URL, archive)
with archive.open("rb") as f: digest = hashlib.file_digest(f, "sha256").hexdigest()
if digest != SHA256: raise SystemExit("wasi-sdk SHA-256 mismatch; archive retained for inspection")
sdk = dest / NAME
if not sdk.exists():
    with tarfile.open(archive) as bundle: bundle.extractall(dest, filter="data")
print(sdk)

#!/bin/bash
cd ~/xemax/jupyterlite
.venv/bin/python -m http.server 8888 -b 0.0.0.0 -d dist

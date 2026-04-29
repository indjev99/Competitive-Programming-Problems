#!/bin/bash

task="triangle"
stack_size=1073741824  # 1024 MB

ulimit -s "${stack_size}"
"./${task}"

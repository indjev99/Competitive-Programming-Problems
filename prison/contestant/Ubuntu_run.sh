#!/bin/bash

task="prison"
stack_size=1073741824  # 1 GB

ulimit -s "${stack_size}"
"./${task}"


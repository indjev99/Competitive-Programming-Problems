#!/bin/bash

task="navigation"
stack_size=536870912  # 512 MB

ulimit -s "${stack_size}"
"./${task}"


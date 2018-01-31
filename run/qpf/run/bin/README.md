# `WA/run/bin` folder

This folder contains the `RunProcessor.py` script that is used by the **QPF**
to launch a given processor to be executed inside a `Docker` container. In
addition, it includes the `qpfproc` subdirectory, which includes the
*QPF Processor* Python package, with the `Processor` class.  The implementation
of this class will drive the execution of the processors launched by the QLA
Processing Framework

## `RunProcessor.py`

### Usage
The usage of this script is:

    $ python <path>/RunProcessor.py --taskpath <taskPath> --cfg <jsonCfgFile>

where:

* `<path>` : Directory where the RunProcessor.py script is located.

* `<tashPath>` : Directory where the task is to be executed (where the
  actual processor should be run.
* `<jsonCfgFile>` : Configuration file for the execution of the processor.

Alternatively, you can activate the execution permissions of this script, and
call it directly.  This can be done with

    $ chmod ug+x <path>/RunProcessor.py

Note that these script and the package under `qpfproc` expect that you have
defined the env. variable `QPFWA` as the *QPF Working Area* path (usually
`/home/eucops/qpf`). Otherwise they try to presume its value from the location
of the scripts themselves.

### Examples

    $ python ../WA/run/bin/RunProcessor.py --taskpath $(pwd) --cfg ./task.cfg.json

or, if the `RunProcessor.py` script has the execution permission set:

    $ ../WA/run/bin/RunProcessor.py --taskpath $(pwd) --cfg ./task.cfg.json

This will trigger the following set of actions:

* For container-based processors

  1. Read the processor configuration file provided.
  2. Ensure the processor specified exists.
  3. Ensure the docker image is available.
  4. Ensure the `in`/`out`/`log` folders exist in the task path specified.  
     Otherwise, create them.
  5. Expand the names of the input/output/log files specified in the task config.
     file.
  6. Launch the creation of a container with the specified `Docker` image and the
     main script file, as specified in the task config file.


* For Swarm-based processors

  1. Read the processor configuration file provided.
  2. Ensure the processor specified exists.
  3. Ensure the specified docker swarm is running.
  4. Expand the names of the input/output/log files specified in the task config.
     file.
  5. Create input task info so that the swarm services receive it upon request.
  5. Retrieve output data upon notification.

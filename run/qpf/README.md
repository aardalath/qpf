# `WA` folder

This folder is the main QPF Working Area directory. The actual _working area
directory_ can have any name, and be located anywhere in the master host file
system (this is still to be tested), but the content should always be as
stated this document.

Below this directory we should find the following sub-directories:

- `bin` : Contain the processor folders.  They are addressed from the
`RunProcessor.py` script, that is executed by the **QPF**.

- `cfg` : A sub-directory to store the **QPF** configuration files

- `data` : The place where the input and output data files are stored.
  It has the following structure:

          data
            |--- archive
            |        |--- in
            |        '--- out
            |--- gateway
            |        |--- in
            |        '--- out
            |--- inbox
            |--- outbox
            |--- user
            '--- www

- `include` : Directory where the **QPF** COTS header files are installed.

- `lib`/`lib64` : Directory where the **QPF** libraries are installed.
  This(these) directory(ies) should be accessible to the **QPF** applications,
  for example, listed in the `LD_LIBRARY_PATH` environmental variable.

- `log` : Directory where some log files from the execution of the **QPF**
  applications are stored.

- `scripts` : Directory where a set of handy scripts that can be used by the
  operator are stored.

- `run` : Contains the `bin` sub-directory (which includes the
  `RunProcessor.py` script and the `qpfproc` Python package) as well as
  the different QPF session directories.

- `www` : Directory where some static web pages with monitoring information
  are stored.

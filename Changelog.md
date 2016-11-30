Change Log
====================

All notable changes to the [QPF] software project will be documented in this file.


[Unreleased] / 2016-11-28
--------------------------

Second, amendment release to the [V1.0].  The main changes are:

- Now the reference target Linux distribution is LODEEN 1.2.
- Now the integrated QPF application is split in two main components, the QPF Core and the QPF GUI
  + The QPF Core does all the job, but runs in the background with no visible user interface.
  + The QPF GUI is the new passive graphical user interface.
- The [QPF] is now installed together with the [QDT], following the procedure at [QPF & QDT Installation].
- A more complete configuration tool is presented, although is not yet fully functional.
- Improved scripts for building the applications and for the installation of the COTS.

### Improvements

QPF Core:

- Improved Build script, which provides a BUILD_ID identifier for each build.
- Now additional tasks can be coded for the execution in the run loop of each component.

QPF GUI:

- The list of open panels (tabs created showing the content of the different product files in the local archive) is now available.  With the right mouse button a context menu appears for closing all, the selected, or all but the selected element.  This list of panels is now shown in two places:
  + In a dedicated detachable docking window
  + In a combo box that appears when clicking in a button in the top left corner of the tabs bar.
- A more complete configuration tool is presented, although is not yet fully functional.


### New Features

QPF Core:

- User Defined tools to open product files in the local archive.  They are part of the configuration and can be handled via the QPF GUI Config Tool.
- New script to start docker service.
- New PostgreSQL Database initialization script
- New PostgreSQL Server Start script
- New COTS Installation script for LODEEN installations

QPF GUI:

- New integrated JSON data viewer.

### Fixed

- Many bugs in the processing of the data and the handling of the input and output products are fixed.
- Retrieval and displaying of the status of the different components by the QPF GUI is now fixed.
- Solved issue with products left behind in gateway storage area.

### Cosmetic Changes

- A splash screen image is shown when
- An application icon is now shown in the main window at the top right corner.
- The fixed panels in the main window have now a small icon.

### Removed

Removed entire integrated version of QPF Core and QPF GUI


[V1.0] / 2016-04-22
--------------------

First official release of the [QPF].


----

[Unreleased]: http://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QPF/trunk
[V1.0]: http://euclid.esac.esa.int/svn/ESA/SOC/SOC-3-DEV/SOC-3-07-QLook/QPF/tags/V1.0

[QPF & QDT Installation]: https://issues.cosmos.esa.int/euclidwiki/display/QLA/QLA+Processing+Framework+and+Diagnostic+Tool+Installation
[QPF]: https://issues.cosmos.esa.int/euclidwiki/pages/viewpage.action?pageId=4259980
[QDT]: https://issues.cosmos.esa.int/euclidwiki/pages/viewpage.action?pageId=4259917

<!--

Title:       Change Log
Subtitle:    Euclid QLA Processing Framework Change Log
Project:     QPF
Author:      J C Gonzalez
Affiliation: Euclid SOC Team @ ESAC - Telespazio Vega U.K. S.L.
Web:         http://www.cosmos.esa.int/web/euclid
Date:        2016-11-24

The format is based on [Keep a Changelog](http://keepachangelog.com/)
taking into account also [Semantic Versioning](http://semver.org/).

-->
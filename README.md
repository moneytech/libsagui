[![CII Best Practices](https://bestpractices.coreinfrastructure.org/projects/2140/badge)](https://bestpractices.coreinfrastructure.org/projects/2140)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/risoflora/libsagui.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/risoflora/libsagui/context:cpp)
[![Build Status](https://travis-ci.org/risoflora/libsagui.svg?branch=master)](https://travis-ci.org/risoflora/libsagui)

# Overview

Sagui is a cross-platform C library which helps to develop web servers or frameworks. Its core has been developed using the [GNU libmicrohttpd](https://www.gnu.org/software/libmicrohttpd), [uthash](https://troydhanson.github.io/uthash), [PCRE2](https://www.pcre.org) and [GnuTLS](https://www.gnutls.org), that's why it is so fast, compact and useful to run on embedded systems.

# Features

* Requests processing through:
  * Event-driven - _single-thread + main loop + select_
  * Threaded - _one thread per request_
  * Thread pool - _thread pool + select_
* High-performance path routing that supports:
  * Regular expressions using [PCRE2](https://www.pcre.org/current/doc/html/pcre2pattern.html) [syntax](https://www.pcre.org/current/doc/html/pcre2syntax.html)
  * Just-in-time optimization ([JIT](https://www.pcre.org/current/doc/html/pcre2jit.html))
  * Binary search in path entry-points
* HTTPS support using [GnuTLS](https://www.gnutls.org)
* Basic authentication
* Upload/download streaming by:
  * Payload - for raw data transferring as JSON, XML and other
  * File - for large data transferring as videos, images, binaries and so on
* Dynamic strings (makes it easy strings operations in C)
* String map (fast key-value mapping)
* Fields, parameters, cookies, headers under hash table structure
* Several callbacks for total library customization 

# Versioning

Starting from the version 1.0.0, Sagui follows the [SemVer](https://semver.org) rules regarding API changes with backwards compatibility and stable ABI across major releases.

# Licensing

Sagui is released under GNU Lesser General Public License v3.0. Check the [LICENSE file](https://github.com/risoflora/libsagui/blob/master/LICENSE) for more details.

[![GNU Lesser General Public License v3.0](https://www.gnu.org/graphics/lgplv3-88x31.png)](https://www.gnu.org/licenses/lgpl-3.0.html)

# Documentation

The documentation has been written in [Doxygen](https://www.stack.nl/~dimitri/doxygen) and is available in [HTML](https://risoflora.github.io/libsagui-docs/index.html) and [PDF](https://risoflora.github.io/libsagui-docs/ref.html).

# Downloading

All stable releases are available for download at the [releases page](https://github.com/risoflora/libsagui/releases). For Windows, the packages `libsagui-N.N.N-dll.zip` (and their respective GPG signature) contains the compiled DLLs for 32 and 64 bits. For other systems, the packages `Source code (tar.gz|zip)` contains the library source.

# Building/installing

Check the [BUILD.md](https://github.com/risoflora/libsagui/blob/master/BUILD.md) for instructions to build the library, examples, tests and documentation, then, follow the steps in [INSTALL.md](https://github.com/risoflora/libsagui/blob/master/INSTALL.md) to install the library from sources on your system.

# Compatibility

A typical upgrade of the Sagui library does not break the ABI at all. Take a look at the [API/ABI compatibility report](https://abi-laboratory.pro/?view=timeline&l=libsagui) to compare most recent library versions.

See also [Checking backward API/ABI compatibility of Sagui library versions](https://github.com/risoflora/libsagui/blob/master/ABIComplianceChecker.md).

# Projects using Sagui

* [Brook framework](https://github.com/silvioprog/brookframework) -  Pascal framework which helps to develop web applications.

# Contributing

Sagui is totally open source and would not be possible without our [contributors](https://github.com/risoflora/libsagui/blob/master/THANKS). If you want to submit contributions, please fork the project on GitHub and send a pull request. You retain the copyright on your contributions. If you have questions, open a new issue at the [issues page](https://github.com/risoflora/libsagui/issues). For donations to support this project, please click the botton below.

[![Support this project](https://www.paypalobjects.com/en_US/GB/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=silvioprog%40gmail%2ecom&lc=US&item_name=libsagui&item_number=libsagui&currency_code=USD&bn=PP%2dDonationsBF%3aproject%2dsupport%2ejpg%3aNonHosted)

# Support

This project is completely self-explanatory, but, if you need a consulting service to integrate it on your project, [contact us](mailto:silvioprog@gmail.com).
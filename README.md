[![pipeline status](https://gitlab.com/rpdev/qlmdb/badges/master/pipeline.svg)](https://gitlab.com/rpdev/qlmdb/commits/master)


# QLMDB

QLMDB is a Qt wrapper around the LMDB C library. It wraps the base primitives
introduced by LMDB into higher level classes which maintain the lifetime
of these objects following the
[RAII pattern](https://en.cppreference.com/w/cpp/language/raii). The
QLMDB classes integrate well with the Qt framework, so you can easily store
you data and read it back later on.

## Usage Instructions

You can find the API documentation of QLMDB on
https://rpdev.gitlab.io/qlmdb/ - this page also has some usage instruction on
how to work with the library.

In particular, pay attention to the way the library deals with multi-threading
and multi-processes (which basically is derived from the LMDB C library).


## Building

You will need the following pre-requisites for building the project:

* Qt 5.9 or up.
* A suitable compiler.

In addition, the following optional prerequisites can be installed:

* LMDB (library and development files).


### Building with qmake

The project can be build via `qmake`:

```bash
mkdir build
cd build
qmake CONFIG+=release ..
make

# Optional: Install into the Qt installation
sudo make install
```

The following configuration option can be passed to the `CONFIG` variable to
customize the build:

* `qlmdb_with_builtin_lmdb`: If this option is set, the library will be build
  against a built-in version of the LMDB C library.
* `qlmdb_with_static_libs`: If this option is given, the library is built as
  a static library.


### Building with cmake

The project can be build via `qmake`:

```bash
mkdir build
cd build
cmake ..
make
```

The following options can be set to `ON` to change the behavior of the
build:

* `QLMDB_WITH_STATIC_LIBS`: Build the library as a static library.
* `QLMDB_WITH_BUILTIN_LMDB`: Build the library with a built in version of the
  LMDB C library.


## License

QLMDB is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

QLMDB is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

Please find a copy of the [GNU General Public License](COPYING)
as well as the [GNU Lesser General Public License](COPYING.LESSER) in
the sources of the project.

The projects relies on the LMDB C library, which is licensed under the
terms of the [OpenLDAP Public License](3rdparty/liblmdb/LICENSE). If you
want to deliver your project with a copy of the LMDB C library or if you
compile it with the LMDB library built in, make sure you comply to that license.


## Reporting Issues and Feature Requests

In case you discover any problems or if you want to file a feature request,
please head over to the project's
[issue tracker](https://gitlab.com/rpdev/qlmdb/issues).

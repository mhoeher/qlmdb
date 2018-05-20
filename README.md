A Qt wrapper for LMDB.


# Building

You will need the following pre-requisites for building the project:

* Qt 5.9 or up.
* A suitable compiler.

In addition, the following optional prerequisites can be installed:

* LMDB (library and development files).


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

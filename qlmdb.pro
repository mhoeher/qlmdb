TEMPLATE = subdirs

SUBDIRS += \
    qlmdb

!ios {
    SUBDIRS += tests
    tests.depends += qlmdb
}

OTHER_FILES += \
    .gitlab-ci.yml \
    $$files(templates/*) \
    Doxyfile \
    README.md \
    $$files(bin/ci/*.sh) \
    $$files(bin/*) \
    $$files(CMakeLists.txt,true)

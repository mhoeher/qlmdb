TEMPLATE = subdirs

SUBDIRS += \
    qlmdb \
    tests

OTHER_FILES += \
    .gitlab-ci.yml \
    $$files(templates/*)

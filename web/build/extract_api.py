#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Extract functions for EXPORTED_FUNCTIONS from C++ source files.

import re
import fire


def extract_api(*files):
    prefix = 'extern "C" '
    pattern = re.compile(r'%s[^(;]+[ *&]([A-Za-z0-9_]+)\s*[(]' % prefix, re.M)
    functions = []
    assert files, 'need source files'
    for fn in list(files):
        if '.c' in fn:
            content = open(fn).read()
            names = pattern.findall(content)
            functions.extend(['_' + s for s in names])
    return str(functions)


if __name__ == '__main__':
    fire.Fire(extract_api)

## LapisParser

------

*llylly*


A useful parser for Lapis scripts. Provides Python API as interface.

#### Environment: g++, cmake, python

#### Library dependencies:

+ libxml (headers should be contained in the proj & installed in OS)

    `lib/libxml`

+ libyaml (headers should be contained in the proj & installed in OS)

    `lib/libyaml`

+ libcurl headers (headers should be contained in the proj & installed in OS)

    `lib/libcurl`

+ openssl (should be installed in OS)

#### Build instructions:

1. Download the repository

2. Download libyaml (0.1.7) & compile

    [Download Page](http://pyyaml.org/wiki/LibYAML)

3. Copy `yaml.h` (the header file) to `lib/libyaml` folder

4. Download libxml2 (2.9.6) & compile

    [Download Page](https://git.gnome.org/browse/libxml2/tag/?h=v2.9.6)

5. Copy the header file **folder** (the `include/libxml` folder) to `lib/libxml` folder

    ".h" header files lies in `lib/libxml/libxml` folder then.

6. Download libcurl (7.56.1) & compile

    [Download Page](https://curl.haxx.se/download.html)

7. Copy the header file foler (the `include/curl` folder) to `lib/libcurl` folder

    ".h" header files lies in `lib/libcurl/curl` folder then.

8. Run `cmake .` in project root directory

9. Run `make` in project root directory, 
    then you will get `libVParserPy.dylib` & executable `VParser` in root directory

10. Run `cd interface/python/`

11. Run `python demo.py` & executable file for test

#### CPP API

Header files: `interface/Interface.h`

Include the header file, then call `init()` before calling any other interfaces.

Then enjoy it :)

#### Python API

- Usage: 
    ```Python
      import LapisParser
      
      result = LapisParser.{FuncName}(params...)
    ```

- Demo:

    `interface/python/demo.py`

    Where to get to know the usage.

- API:

    Browse `html/index.html` for interface document. It's in the document for `interface/Interface.h`.

    **Every C++ interface here has a strictly corresponding Python one except for `init()`, which is executed during package initialization.**

    + string (C++) - String (Python)
    + BaseDataObject*(C++) - String / Bool / int / float / dict / list (all accepted) (Python)
    + Default parameters are also valid for Python inferfaces


#### Document

​	Please view `html/_interface_8h.html`.

#### Progress

Basic functions, including parse, request and scenario test, all finished.

Some small features and extensions to be done (mainly about expression parse):

+  schema/APIObject.cpp

  consumes & produces check

+ schema/data_schema/StringSchema.cpp

  String Type:

  'format:date' validation

  'format:dataTime' validation

  'regex' validation (currently always true)

  'regex' generating by pattern (currently return empty string)

  'regex' check by pattern (currently return true)

+ scenario/module/ModuleInputConstraintObject.cpp

  Expression support of Input Constraints:

  ​	'expression' field is stored but always ignored

  ​	Only use the value itself

+ scenario/module/ModuleSetEffectObject.cpp

  Delete expression support:

  ​	current only store but always ignored

  ​	currently delete all elements
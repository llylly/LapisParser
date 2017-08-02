## LapisParser

------

*llylly*


A useful parser for Lapis scripts. Provides Python API as interface.

#### Environment: g++, cmake, python

#### Libraries dependencies:

+ libxml

    `parser/libxml`
    
+ libyaml

    `parser/libyaml`
    
#### Build instructions:

1. Download the repository

2. Download libyaml(>0.1.7) & compile

    [Download Page](https://github.com/yaml/libyaml)
  
3. Copy `libyaml.a` (the static library file) to `parser/libyaml` folder, 
and `yaml.h` (the header file) to the same folder

4. Download libxml2(>2.9.0) & compile

    [Download Page](http://www.xmlsoft.org/downloads.html)
   
5. Copy `libxml2.a` (the static library file) to `parser/libxml` folder,
and the header file folder(the `include/libxml` folder) to `parser/libxml` folder

6. Run `cmake .` in project root directory

7. Run `make` in project root directory, 
then you will get `libVParserPy.dylib` & executable `VParser` in root directory

8. Run `cd interface/python/`

9. Run `python demo.py` & executable file for test

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
    
    ** Every C++ interface here has a strictly corresponding Python one except for `init()`, which is executed during package initialization. **

    + string (C++) - String (Python)
    
    + BaseDataObject*(C++) - String / Bool / int / float / dict / list (all accepted) (Python)
    
    + Default parameters are also valid for Python inferfaces
    

*TODO:*

+ Scenario Parse

+ API Request

+ Scenario Execution


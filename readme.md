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

9. Run `python demo.py` for test

#### Python API docs

- Usage: 
    ```Python
      import LapisParser
      
      result = LapisParser.{FuncName}(params...)
    ```

- Demo:

    `interface/python/demo.py`
   
    Where to get to know the usage.

- API:
    ##### IO
    
    - `ret = read(files)`
    
          Read scripts. Use format suffix to automatically choose the parser used.
      
          **Input:**
          + files: a **list** of string, which are the paths of YAML file to be parsed
                
          **Output:**
            An integer, 0 means succeed, 1 means failed. 
            
    - `ret = readYAML(files)`
    
          Read YAML format Lapis scripts.
    
          **Input:**
          + files: a **list** of string, which are the paths of YAML file to be parsed
          
          **Output:**
          	An integer, 0 means succeed, 1 means failed. 
      
    - `ret = readXML(files)`
    
          Read XML format Lapis scripts.
        
          **Input:**
          + files: a **list** of string, which are the paths of XML file to be parsed
          
          **Output:**
          	An integer, 0 means succeed, 1 means failed. 
      
    - `ErrorList = getErrors()`
    
          Print errors when parsing.
        
          **Output:**
          	A ErrorList. Each element of the list is a map.
          	
          + The map has 4 elements:
          	- fileName: string, the error file name
          	- line: int, the error line number
          	- col: int, the error column number
          	- msg: string, the error reason
          
          You can use getErrors() at any time to gather the detailed error messages.
      
    - `ret = translate2YAML(outFileName)`
    
          Translate and output script as a YAML doc.
        
          **Input:**
          + outFileName: A string, specify the output YAML file name or path
          
          **Output:**
          	An integer, 0 means succeed, 1 means failed.
      
    - `ret = translate2XML(outFileName)`
    
          Translate and output script as a XML doc.
        
          **Input:**
          + outFileName: A string, specify the output XML file name or path
          
          **Output:**
          	An integer, 0 means succeed, 1 means failed. 
    
    ##### Get Basic Infos
    
    - `info = getInfo()`
    
          Get the Info Object of the root.
          
          **Input**: None
          
          **Output**: A Python dict which contains all parsed fields and subfields in InfoObject.
    
    - `host = getHost()`
    
          Get the host url of the root.
          
          **Input**: None
          
          **Output**: A Python string of host URL.
    
    - `basePath = getBasePath()`
        
          Get the base path of the root.
          
          **Input**: None
          
          **Output**: A Python string of the basePath. If not exists, return an empty string.
    
    - `schemes = getSchemes()`
    
          Get the supported schemes list.
          
          **Input**: None
          
          **Output**: A Python list of string, each of which is a scheme. If not exists, return None.
    
    - `consumes = getConsumes()`
    
          Get the 'consumes' field of root.
        
          **Input**: None
          
          **Output**: A Python list of string, each of which is a MIME type name. If not exists, return None.
        
    - `produces = getProduces()`
    
          Get the 'produces' field of root.
        
          **Input**: None
          
          **Output**: A Python list of string, each of which is a MIME type name. If not exists, return None.
        
    - `tags = getTags()`
    
          Get the 'tags' field of root.
        
          **Input**: None
          
          **Output**: A Python list of dict, each of which is a TagObject. If not exists, return None.
          
    - `externalDocs = getExternalDocs()`
    
          Get the 'exeternalDocs' field of root.
        
          **Input**: None
          
          **Output**: A Python dict corresponding to the ExternalDocObject. If not exists, return None.
        
    ##### Data Schema
    
    - `nameList = getDataSchemaNames()`
    
          Get the name list of Data Schema Object.
          
          You can request a Data Schema by its name in the name list.
          All Data Schema defined in 'definitions' field can be requested.
          And their request names, i.e., names in the list here, are the same as their keys in the script.
        
          **Input**: None
          
          **Output**: A Python list
        
    - `schema = getDataSchemaByName(name)`
    
          Get the data schema by its name.
          
          The name should be an element of name list.
          
          **Input**: A string, the schema request name, described as above.
          
          **Output**: The data schema object, of dict type.
        
    - `data = randFromDataSchema(name)`
    
          Get a random data instance which conforms to the data schema.
          The data instance is generated according to the random policy.
          
          The name should be an element of name list.
          
          **Input**: A string, the schema request name, described as above.
          
          **Output**: The data instance. Its type is determined by the schema.
          
    - `legal = checkData(obj, schema_name)`
      
          Check whether a data object confoms to a data schema.
          
          Data schema is quoted by its name.
          
          **Input**:
          
          + An object, the data object to be cheked
          
          + A string, the schema request name, should be an element of name list.
          It's the schema used for check.
          
          **Output**:
          
          A Boolean: True: legal; False: Illegal.
        
    


*TODO:*

*允许实时增加，删改API，数据类型，请求和响应等*


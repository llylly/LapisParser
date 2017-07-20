### LapisParser

------

*llylly*


A useful parser for Lapis scripts. Provides Python API as interface.

Dependencies: g++, cmake, python

- Usage: 
    ```Python
      import LapisParser
      
      result = LapisParser.{FuncName}(params...)
    ```

- Demo:

    `interface/python/demo.py`
   
    Where to get to know the usage.

- API:
    #####Basic
    - `ret = readYAML(files)`
    
      Read YAML format Lapis scripts.
    
      **Input:**
      + files: a list of string, which are the paths of YAML file to be parsed
      
      **Output:**
      	An integer, 0 means succeed, 1 means failed. 
      
    - `ret = readXML(files)`
    
      Read XML format Lapis scripts.
    
      **Input:**
      + files: a list of string, which are the paths of XML file to be parsed
      
      **Output:**
      	An integer, 0 means succeed, 1 means failed. 
      
    - `ErrorList = getErrors()`
    
      Print errors when parsing.
    
      **Output:**
      	A ErrorList. Each element of the list is a map.
      	
        + The map has 4 elements:
      		fileName: string, the error file name
      		line: int, the error line number
      		col: int, the error column number
      		msg: string, the error reason
      
      You can use getErrors() at any time to gather the detailed error messages.
      
    - `ret = translate2YAML(outFileName)`
    
      Translate and output script as a YAML doc.
    
      **Input:**
      + outFileName: a string, specify the output YAML file name or path
      
      **Output:**
      	An integer, 0 means succeed, 1 means failed.
      
    - `ret = translate2XML(outFileName)`
    
      Translate and output script as a XML doc.
    
      **Input:**
      + outFileName: a string, specify the output XML file name or path
      
      **Output:**
      	An integer, 0 means succeed, 1 means failed. 
    
    #####Get Things
    
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
    
      **Input**: None
      
      **Output**: A Python list of string, each of which is a MIME type name. If not exists, return None.
    
    - `produces = getProduces()`
    
      **Input**: None
      
      **Output**: A Python list of string, each of which is a MIME type name. If not exists, return None.
    
    - `tags = getTags()`
    
      **Input**: None
      
      **Output**: A Python list of dict, each of which is a TagObject. If not exists, return None.
      
    - `externalDocs = getExternalDocs()`
    
      **Input**: None
      
      **Output**: A Python dict corresponding to the ExternalDocObject. If not exists, return None.
    
    #####Modify Things
    

To be done:

	允许实时增加，删改API，数据类型，请求和响应等


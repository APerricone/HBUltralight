#include <hbclass.ch>

class JSVALUE
    DATA pValue HIDDEN
//    DATA pObject HIDDEN

    METHOD IsNull()
    METHOD IsUndefined()
    METHOD IsBoolean()
    METHOD IsNumber()
    METHOD IsString()
    METHOD IsObject()
    METHOD IsArray()
    METHOD IsFunction()

    //v can be nil, number, logic, character, array, hash, codeblock or symbol
    CONSTRUCTOR New(v)
    // return a value base on its type, in case of object return a hash, in case of function throw an error
    METHOD ToValue() 

    //CONSTRUCTOR FromJSON(cJSON) 
    //METHOD ToJSON()

    // NOTE: IT is not possible copy 2 JSValue

    // ** methods valid for array and object **

    // return another JSVALUE but takes as as new and JSVALUE
    method Attribute OPERATOR [] 
    method HasProperty(cPropertyName) // => L
    method DeleteProperty(cPropertyName) //=> L

    // ** end of methods valid for array and object **
    
    // valid only for function
    Method CallNoThis()
    Method Call(thisObj)

    DESTRUCTOR Destroy()
endclass

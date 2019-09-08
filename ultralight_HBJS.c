#include "ultralight_hb.h"
#include <hbjson.h>

//forward decl
JSValueRef HB_toJS(PHB_ITEM);
JSStringRef HB_TOJSString(PHB_ITEM);
// lObject:
// 0 return a JSValue in case of object
// 1 returns a JSValue every time
// 2 never returns a JSValue (function->nil)
void hb_FromJS(PHB_ITEM, JSValueRef, int lObject);
JSObjectRef HB_CreateJSCallback(PHB_ITEM);

static JSContextRef ctx;
HB_FUNC(SETJSCONTEXT) { ctx = hb_parptr(1); }
HB_FUNC(GETJSCONTEXT) { hb_retptr((void*)ctx); }

static HB_USHORT HB_JSValueClassId = 0;
static HB_SIZE HB_JSValue_ValuePtr = 0;
//static HB_SIZE HB_JSValue_ObjectPtr = 0;

HB_FUNC_EXTERN(JSVALUE);
void InitJSValueClass() {
	if(HB_JSValueClassId!=0) return;
	HB_JSValueClassId = hb_clsFindClass("JSVALUE", NULL);
	if(HB_JSValueClassId==0) {
		HB_FUNC_EXEC(JSVALUE);
		HB_JSValueClassId = hb_clsFindClass("JSVALUE", NULL);
	}
	HB_JSValue_ValuePtr = hb_clsGetVarIndex(HB_JSValueClassId,hb_dynsymGet("pValue"));
	//HB_JSValue_ObjectPtr = hb_clsGetVarIndex(HB_JSValueClassId,hb_dynsymGet("pObject"));
}
#define SELF_VALUE() (JSValueRef)hb_itemGetPtr( hb_itemArrayGet(hb_stackSelfItem(),HB_JSValue_ValuePtr) )

JSObjectRef SELF_OBJECT() {
	JSValueRef value = SELF_VALUE();
	if(!JSValueIsObject(ctx,value)) {
    	return 0;
	}
  	return JSValueToObject(ctx, value, 0);
}

HB_FUNC( JSVALUE_ISNULL ) {
	hb_retl(JSValueIsNull(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISUNDEFINED ) {
	hb_retl(JSValueIsUndefined(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISBOOLEAN ) {
	hb_retl(JSValueIsBoolean(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISNUMBER ) {
	hb_retl(JSValueIsNumber(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISSTRING ) {
	hb_retl(JSValueIsString(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISOBJECT ) {
	hb_retl(JSValueIsObject(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISARRAY ) {
	hb_retl(JSValueIsArray(ctx,SELF_VALUE()));
}

HB_FUNC( JSVALUE_ISFUNCTION ) {
  	JSObjectRef obj = SELF_OBJECT();
	hb_retl(obj && JSObjectIsFunction(ctx, obj));
}

HB_FUNC( JSVALUE_NEW ) {
	JSValueRef v = HB_toJS(hb_param(1,HB_IT_ANY));
	PHB_ITEM pSelf;
	InitJSValueClass();
    hb_clsAssociate( HB_JSValueClassId );
   	pSelf = hb_stackReturnItem();
    hb_itemArrayPut(pSelf, HB_JSValue_ValuePtr, hb_itemPutPtr(0, (void*)v)); 
	JSValueProtect(ctx,v);
}

HB_FUNC( JSVALUE_DESTROY ) {
	JSValueUnprotect(ctx,SELF_VALUE());
}

HB_FUNC( JSVALUE_TOVALUE ) {
	hb_FromJS(hb_stackReturnItem(), SELF_VALUE(), 2);
}

//CONSTRUCTOR FromJSON(cJSON) 
//METHOD ToJSON()

HB_FUNC( JSVALUE_ATTRIBUTE ) {
	PHB_ITEM pSelf  = hb_stackSelfItem();
   	PHB_ITEM pIndex = hb_param( 1, HB_IT_ANY );
	JSObjectRef obj = SELF_OBJECT();
	if(!obj) {
		hb_errRT_BASE(EG_ARG, 10, "Operator[] allowed only on object type", "[]", 2, pSelf, pIndex);
		return;
	}
   	if( hb_pcount() == 2 )  { /* ASSIGN */ 
		PHB_ITEM pValue = hb_param( 2, HB_IT_ANY );
		if(HB_IS_NUMINT(pIndex))
			JSObjectSetPropertyAtIndex(ctx, obj, hb_itemGetNI(pIndex), HB_toJS(pValue), 0);
		else if(HB_IS_STRING(pIndex)) {
			JSStringRef cIndex = HB_TOJSString(pIndex);
			JSObjectSetProperty(ctx, obj, cIndex, HB_toJS(pValue), 0, 0);
			JSStringRelease(cIndex);
		} else {
			hb_errRT_BASE(EG_ARG, 11, "unimplemented", "[]", 3, pSelf, pIndex, pValue);
			return;
		}
		/*
			JSObjectSetProperty(ctx_, nativeFunction, JSString("name"), JSValue(string_idx_), kJSPropertyAttributeReadOnly, nullptr);
		*/
	} else { /* ACCESS */
		JSValueRef pValue;
		if(HB_IS_NUMINT(pIndex))
			pValue = JSObjectGetPropertyAtIndex(ctx, obj, hb_itemGetNI(pIndex),  0);
		else if(HB_IS_STRING(pIndex)) {
			JSStringRef cIndex = HB_TOJSString(pIndex);
			pValue = JSObjectGetProperty(ctx, obj, cIndex, 0);
			JSStringRelease(cIndex);
		} else {
			hb_errRT_BASE(EG_ARG, 11, "unimplemented", "[]", 2, pSelf, pIndex);
			return;
		}
		hb_FromJS(hb_stackReturnItem(), pValue,0);
	}
}

HB_FUNC( JSVALUE_HASPROPERTY ) {
  	JSObjectRef obj = SELF_OBJECT();	  
	PHB_ITEM pIndex = hb_param(1, HB_IT_STRING);
	if(!obj || !pIndex) {
		hb_retl(HB_FALSE);
		return;
	}
	JSStringRef cIndex = HB_TOJSString(pIndex);
	hb_retl(JSObjectHasProperty(ctx,obj,cIndex));
	JSStringRelease(cIndex);
}

HB_FUNC( JSVALUE_DELETEPROPERTY ) {
  	JSObjectRef obj = SELF_OBJECT();	  
	PHB_ITEM pIndex = hb_param(1, HB_IT_STRING);
	if(!obj || !pIndex) {
		hb_retl(HB_FALSE);
		return;
	}
	JSStringRef cIndex = HB_TOJSString(pIndex);
	hb_retl(JSObjectDeleteProperty(ctx,obj,cIndex,0));
	JSStringRelease(cIndex);
}

void ValueCall(bool thisObjParam) {
	JSObjectRef obj = SELF_OBJECT();
    JSObjectRef thisObj;
    JSValueRef result, *args;
    int s=1,i;
    if(!obj || !JSObjectIsFunction(ctx,obj)) {
	    hb_errRT_BASE(EG_ARG, 15, "JSValue is not a function",  HB_ERR_FUNCNAME, 1);
        return;
    }

    if(thisObjParam) {
        thisObj = JSValueToObject(ctx,HB_toJS(hb_param(1,HB_IT_ANY)),0);
        s=2;
    } else
        thisObj = JSContextGetGlobalObject(ctx);

    args = (JSValueRef*)malloc(sizeof(JSValueRef)*(hb_pcount()-s+1));
    for(i=s;i<=hb_pcount();i++) {
        args[i-s] = HB_toJS(hb_param(i,HB_IT_ANY));
    }
    result = JSObjectCallAsFunction(ctx, obj, thisObj, hb_pcount()-s+1, args, 0);
    for(i=s;i<=hb_pcount();i++) {
        JSValueUnprotect(ctx,args[i-s]);
    }

    hb_FromJS(hb_stackReturnItem(), result,0);
}

HB_FUNC( JSVALUE_CALL ) { ValueCall(true); }
HB_FUNC( JSVALUE_CALLNOTHIS ) { ValueCall(false); }


HB_FUNC( JSEVAL ) {
	JSStringRef script = HB_TOJSString(hb_param(1,HB_IT_STRING));
	JSValueRef value = JSEvaluateScript(ctx,script,0,0,0,0);
	JSStringRelease(script);
	hb_FromJS(hb_stackReturnItem(), value,0);
}

HB_FUNC( JSGLOBALOBJECT ) {
	JSObjectRef obj = JSContextGetGlobalObject(ctx);
	JSValueRef v = obj; //JSObjectToValue
	PHB_ITEM pRet;
	InitJSValueClass();
    hb_clsAssociate( HB_JSValueClassId );
   	pRet = hb_stackReturnItem();
    hb_itemArrayPut(pRet, HB_JSValue_ValuePtr, hb_itemPutPtr(0, (void*)v)); 
	JSValueProtect(ctx,v);
}

JSValueRef HB_toJS_JSON(PHB_ITEM v) {
	JSValueRef val;
	char* hb_json = hb_jsonEncodeCP(v,0,0,hb_cdpFind("UTF8"));
	JSStringRef json = JSStringCreateWithUTF8CString(hb_json);
	hb_xfree(hb_json);
	val = JSValueMakeFromJSONString(ctx,json);
	JSStringRelease(json);
	return val;
}

JSValueRef HB_toJS(PHB_ITEM v) {
	HB_TYPE t = hb_itemType(v);
	switch(t) {
	case HB_IT_NIL:       //0x00000
		return JSValueMakeUndefined(ctx);
	case HB_IT_INTEGER:   //0x00002
	case HB_IT_DOUBLE:    //0x00010
	case HB_IT_LONG:      //0x00008
	case HB_IT_NUMERIC:   //( HB_IT_INTEGER | HB_IT_LONG | HB_IT_DOUBLE )
	case HB_IT_NUMINT:    //( HB_IT_INTEGER | HB_IT_LONG )
		return JSValueMakeNumber(ctx,hb_itemGetND(v));
	case HB_IT_LOGICAL:   //0x00080
		return JSValueMakeBoolean(ctx,hb_itemGetL(v));
	case HB_IT_SYMBOL:    //0x00100
	case HB_IT_BLOCK:     //0x01000
		return HB_CreateJSCallback(v);
	case HB_IT_STRING:    //0x00400
	case HB_IT_MEMOFLAG:  //0x00800
	case HB_IT_MEMO:      //( HB_IT_MEMOFLAG | HB_IT_STRING )
	case HB_IT_BYREF:     //0x02000
	case HB_IT_MEMVAR:    //0x04000
	{
		JSStringRef str = HB_TOJSString(v);
		JSValueRef val = JSValueMakeString(ctx,str);
		JSStringRelease(str);
		return val;
	}
	case HB_IT_HASH:      //0x00004
		return HB_toJS_JSON(v); // TODO better

	case HB_IT_ARRAY:     //0x08000
	{
		HB_USHORT clsId = hb_objGetClass(v);
		if(clsId>0 && clsId!=HB_JSValueClassId) break; //Error
		if(clsId==0) {
			return HB_toJS_JSON(v); // TODO better
		} else {
			// from JSValue HBobj
			return hb_itemArrayGet(v,HB_JSValue_ValuePtr);
		}
	}
	//case HB_IT_OBJECT:    //HB_IT_ARRAY

	case HB_IT_POINTER:
		if(hb_itemGetPtr(v)==0)
			return JSValueMakeNull(ctx);
		break;
	// unsupported types:
	//case HB_IT_POINTER:   //0x00001
	//case HB_IT_DATE:      //0x00020
	//case HB_IT_TIMESTAMP: //0x00040
	//case HB_IT_ALIAS:     //0x00200
	//case HB_IT_ENUM:      //0x10000
	//case HB_IT_EXTREF:    //0x20000
	//case HB_IT_DEFAULT:   //0x40000
	//case HB_IT_RECOVER:   //0x80000
	//case HB_IT_DATETIME:  //( HB_IT_DATE | HB_IT_TIMESTAMP )
	}
	hb_errRT_BASE(EG_ARG, 12, "unable to convert",  HB_ERR_FUNCNAME, 1, v);
	return 0;
}

JSStringRef HB_TOJSString(PHB_ITEM v) {
	void * hStr;
	HB_SIZE len;
	const HB_WCHAR* hb_str = hb_itemGetStrU16(v,HB_CDP_ENDIAN_NATIVE,&hStr,&len);
	JSStringRef val = JSStringCreateWithCharacters((JSChar*)hb_str,len);
	hb_strfree(hStr);
	return val;
}

void HB_FromJS_JSON(PHB_ITEM dest,JSValueRef src) {
	JSStringRef json = JSValueCreateJSONString(ctx,src,0,0);
	size_t dim = JSStringGetMaximumUTF8CStringSize(json);
	char* hb_json = hb_xalloc(dim);
	JSStringGetUTF8CString(json,hb_json,dim);
	hb_jsonDecodeCP(hb_json,dest,hb_cdpFind("UTF8"));
	hb_xfree(hb_json);
	JSStringRelease(json);
}

void hb_FromJSArray(PHB_ITEM dest,JSValueRef src) {
	JSStringRef tmpStr = JSStringCreateWithUTF8CString("length");
	int i, length = (int)JSValueToNumber(ctx,JSObjectGetProperty(ctx, (JSObjectRef)src, tmpStr, 0),0);
	PHB_ITEM tmpEle,tmp = hb_itemArrayNew(length);
	JSValueRef ele;
	JSStringRelease(tmpStr);
	for(i=0;i<length;i++) {
		ele = JSObjectGetPropertyAtIndex(ctx, (JSObjectRef)src, i, 0);
		tmpEle = hb_itemNew(0);
		hb_FromJS(tmpEle,ele,0);
		hb_itemArrayPut(tmp,i+1,tmpEle);
		hb_itemRelease(tmpEle);
	}
	hb_itemCopy(dest,tmp);
	hb_itemRelease(tmp);
}

// lObject:
// 0 return a JSValue HBObj in case of object
// 1 returns a JSValue HBObj every time
// 2 never returns a JSValue HBObj
void hb_FromJS(PHB_ITEM dest, JSValueRef src, int lObject) {	
	JSType type;
	if(lObject!=1) {
		type = JSValueGetType(ctx,src);
		switch(type) {
			case kJSTypeUndefined:
				hb_itemClear(dest);
				return;
			case kJSTypeNull:
				hb_itemPutPtr(dest,0);
				return;
			case kJSTypeBoolean:
				hb_itemPutL(dest, JSValueToBoolean(ctx, src));
				return;
			case kJSTypeNumber:
				hb_itemPutND(dest, JSValueToNumber(ctx, src,0));
				return;			
			case kJSTypeString:
			{
				JSStringRef strValue = JSValueToStringCopy(ctx, src, 0);
				const JSChar *u16Str = JSStringGetCharactersPtr(strValue);
                size_t len = JSStringGetLength(strValue);
				hb_itemPutStrLenU16(dest, HB_CDP_ENDIAN_NATIVE, (HB_WCHAR*)u16Str,len);
				JSStringRelease(strValue);
				return;			
			}
			case kJSTypeObject:
			{
				if(JSValueIsArray(ctx,src)) {
					hb_FromJSArray(dest,src);
					//HB_FromJS_JSON(dest,src); //TODO better
					return;
				}
				//JSObjectRef obj = JSValueToObject(ctx, src, 0);
				//if(JSObjectIsFunction(ctx,obj)) {
				//	//hb_errRT_BASE(EG_ARG, 12, "Convert JS function to Symbol or CodeBlock", "hb_FromJS", 0);
				//	return;
				//}
				if(lObject==0) {
					lObject=1;
					break;
				} else {
					HB_FromJS_JSON(dest,src); //TODO better
					return;
				}
			}

		}
	}
	if(lObject==1) {
        if(dest==hb_stackReturnItem()) {
            hb_clsAssociate( HB_JSValueClassId );    
        } else
        {
            PHB_ITEM pTemp = hb_itemNew(hb_stackReturnItem());
            hb_clsAssociate( HB_JSValueClassId );
            hb_itemCopy(dest, hb_stackReturnItem());
            hb_itemCopy(hb_stackReturnItem(), pTemp);
			hb_itemRelease(pTemp);
        }
    	hb_itemArrayPut(dest, HB_JSValue_ValuePtr, hb_itemPutPtr(0, (void*)src)); 
		JSValueProtect(ctx,src);
	}
}

JSValueRef hb_functionCallback(JSContextRef ctx_, JSObjectRef function, 
		JSObjectRef thisObject, size_t argumentCount, 
		const JSValueRef arguments[], JSValueRef* exception) {
	PHB_ITEM pCallback = JSObjectGetPrivate(function);
	PHB_ITEM pThis, pArgs, tmp;
	JSContextRef old_ctx = ctx;
	unsigned int i;
	HB_SYMBOL_UNUSED(exception);
	if(!HB_IS_EVALITEM( pCallback )) return 0;
	ctx = ctx_;
	pArgs = hb_itemArrayNew(argumentCount);
	for(i=0;i<argumentCount;i++) {
		tmp = hb_itemNew(0);
		hb_FromJS(tmp,arguments[i],0);
		hb_itemArrayPut(pArgs,i+1,tmp);
		hb_itemRelease(tmp);
	}
	pThis = hb_itemNew(0);
	hb_FromJS(pThis, thisObject,1);
	hb_evalBlock(pCallback, pThis, pArgs, NULL );
	hb_itemRelease(pThis);
	hb_itemRelease(pArgs);
	ctx = old_ctx;
	return HB_toJS(hb_stackReturnItem());
}

void hb_functionFinalize(JSObjectRef function) {
	PHB_ITEM pCallback = JSObjectGetPrivate(function);
  	hb_itemRelease(pCallback);
}

JSClassRef HB_FunctionClass() {
  static JSClassRef instance = 0;
  if (!instance) {
    JSClassDefinition def;
    memset(&def, 0, sizeof(def));
    def.className = "HB_FunctionClass";
    def.attributes = kJSClassAttributeNone;
    def.callAsFunction = hb_functionCallback;
    def.finalize = hb_functionFinalize;
    instance = JSClassCreate(&def);
  }
  return instance;
}

JSObjectRef HB_CreateJSCallback(PHB_ITEM src) {
	
	JSObjectRef	fnObj;
	//JSStringRef name = 0;
	if(!HB_IS_EVALITEM( src )) return 0;
	//if(HB_IS_SYMBOL(src)) {
	//	PHB_SYMB sym = hb_itemGetSymbol(src);
	//	name = JSStringCreateWithUTF8CString(sym->szName);
	//}
	//fnObj = JSObjectMakeFunctionWithCallback(ctx,name,hb_standardCallback);
	fnObj = JSObjectMake(ctx, HB_FunctionClass(), hb_itemNew(src));
	//bool saved = JSObjectSetPrivate(fnObj,src);
	//JSValueProtect(ctx, fnObj);
	//if(name) JSStringRelease(name);
	return fnObj;
}

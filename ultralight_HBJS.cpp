#include "ultralight_hb.h"
#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSStringRef.h>
#include <hbjson.h>

using namespace ultralight;

//forward decl
JSValueRef HB_toJS(PHB_ITEM);
JSStringRef HB_TOJSString(PHB_ITEM);
// lObject:
// 0 return a JSValue in case of object
// 1 returns a JSValue every time
// 2 never returns a JSValue (function->nil)
PHB_ITEM hb_FromJS(PHB_ITEM, JSValueRef, int lObject);
JSObjectRef HB_CreateJSCallback(PHB_ITEM);

//DEFINE_GETCLASSID(JSVALUE)
HB_FUNC_EXTERN(JSVALUE);
HB_USHORT JSVALUEClassId = 0;
HB_USHORT getJSVALUEClassId() {
    if(JSVALUEClassId) { return JSVALUEClassId; }
    JSVALUEClassId = hb_clsFindClass("JSVALUE", NULL);
    if(JSVALUEClassId) { return JSVALUEClassId; }
    HB_FUNC_EXEC(JSVALUE);
    JSVALUEClassId = hb_clsFindClass("JSVALUE", NULL);
    return JSVALUEClassId;
}

HB_SIZE pValueIdx = 0;

HB_SIZE getValueIdx() {
    if(pValueIdx) return pValueIdx;
    pValueIdx = hb_clsGetVarIndex(getJSVALUEClassId(),hb_dynsymGet("pValue"));
    return pValueIdx;
}

static JSContextRef ctx;
HB_FUNC(SETJSCONTEXT) { ctx = (JSContextRef)hb_parptr(1); }
HB_FUNC(GETJSCONTEXT) { hb_retptr((void*)ctx); }

JSValueRef hb_selfJSValue(PHB_ITEM pSelf = 0) {
    if(!pSelf) pSelf = hb_stackSelfItem();
    if(!HB_IS_OBJECT(pSelf)) return 0;
    return (JSValueRef)hb_arrayGetPtr(pSelf,getValueIdx());
}

JSObjectRef hb_selfJSObject() {
	JSValueRef value = hb_selfJSValue();
	if(!JSValueIsObject(ctx,value)) {
    	return 0;
	}
  	return JSValueToObject(ctx, value, 0);
}

HB_FUNC( JSVALUE_ISNULL )       { hb_retl(JSValueIsNull(ctx,hb_selfJSValue())); }
HB_FUNC( JSVALUE_ISUNDEFINED )  { hb_retl(JSValueIsUndefined(ctx,hb_selfJSValue())); }
HB_FUNC( JSVALUE_ISBOOLEAN )    { hb_retl(JSValueIsBoolean(ctx,hb_selfJSValue())); }
HB_FUNC( JSVALUE_ISNUMBER )     { hb_retl(JSValueIsNumber(ctx,hb_selfJSValue())); }
HB_FUNC( JSVALUE_ISSTRING )     { hb_retl(JSValueIsString(ctx,hb_selfJSValue())); }
HB_FUNC( JSVALUE_ISOBJECT )     { hb_retl(JSValueIsObject(ctx,hb_selfJSValue())); }
HB_FUNC( JSVALUE_ISARRAY )      { hb_retl(JSValueIsArray(ctx,hb_selfJSValue())); }

HB_FUNC( JSVALUE_ISFUNCTION ) {
  	JSObjectRef obj = hb_selfJSObject();
	hb_retl(obj && JSObjectIsFunction(ctx, obj));
}

HB_FUNC( JSVALUE_NEW ) {
    JSValueRef v = HB_toJS(hb_param(1,HB_IT_ANY));
    hb_clsAssociate(getJSVALUEClassId());
    PHB_ITEM pSelf = hb_stackSelfItem();
    hb_arraySetPtr(pSelf, getValueIdx(), (void*)v);
    JSValueProtect(ctx, v);
    hb_itemCopy(hb_stackReturnItem(),hb_stackSelfItem());
}

HB_FUNC( JSVALUE_COPY ) {
	PHB_ITEM pItem = hb_param(1, HB_IT_OBJECT);
    JSValueRef v = hb_selfJSValue();
    hb_arraySetPtr(pItem, getValueIdx(), (void*)v);
    JSValueProtect(ctx, v);
}

HB_FUNC( JSVALUE_DESTROY ) {
    JSValueRef v = hb_selfJSValue();
    if(v) JSValueUnprotect(ctx,hb_selfJSValue());
}

HB_FUNC( JSVALUE_TOVALUE ) {
	hb_FromJS(hb_stackReturnItem(), hb_selfJSValue(), 2);
}

//CONSTRUCTOR FromJSON(cJSON)
//METHOD ToJSON()

HB_FUNC( JSVALUE_ATTRIBUTE ) {
	PHB_ITEM pSelf  = hb_stackSelfItem();
   	PHB_ITEM pIndex = hb_param( 1, HB_IT_ANY );
	JSObjectRef obj = hb_selfJSObject();
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
  	JSObjectRef obj = hb_selfJSObject();
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
  	JSObjectRef obj = hb_selfJSObject();
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
	JSObjectRef obj = hb_selfJSObject();
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
    hb_clsAssociate( getJSVALUEClassId() );
   	pRet = hb_stackReturnItem();
    hb_arraySetPtr(pRet, getValueIdx(), (void*)v);
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
		if(clsId>0 && clsId!=getJSVALUEClassId()) break; //Error
		if(clsId==0) {
			return HB_toJS_JSON(v); // TODO better
		} else {
			// from JSValue HBobj
			return (JSValueRef)hb_itemArrayGet(v,getValueIdx());
		}
	}
	//case HB_IT_OBJECT:    //HB_IT_ARRAY

	case HB_IT_POINTER:
		if(hb_itemGetPtr(v)==0)
			return JSValueMakeNull(ctx);
		break;
	// unsupported types:
	//case HB_IT_DATE:      //0x00020 JSObjectMakeDate
	//case HB_IT_TIMESTAMP: //0x00040 JSObjectMakeDate
	//case HB_IT_ALIAS:     //0x00200
	//case HB_IT_ENUM:      //0x10000
	//case HB_IT_EXTREF:    //0x20000
	//case HB_IT_DEFAULT:   //0x40000
	//case HB_IT_RECOVER:   //0x80000
	//case HB_IT_DATETIME:  //( HB_IT_DATE | HB_IT_TIMESTAMP ) JSObjectMakeDate
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

PHB_ITEM HB_FromJS_JSON(PHB_ITEM dest,JSValueRef src) {
	JSStringRef json = JSValueCreateJSONString(ctx,src,0,0);
	size_t dim = JSStringGetMaximumUTF8CStringSize(json);
	char* hb_json = (char*)hb_xalloc(dim);
	JSStringGetUTF8CString(json,hb_json,dim);
    if(!dest) dest=hb_itemNew(0);
	hb_jsonDecodeCP(hb_json,dest,hb_cdpFind("UTF8"));
	hb_xfree(hb_json);
	JSStringRelease(json);
    return dest;
}

PHB_ITEM hb_FromJSArray(PHB_ITEM dest,JSValueRef src) {
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
    if(dest) {
    	hb_itemCopy(dest,tmp);
	    hb_itemRelease(tmp);
    } else dest=tmp;
    return dest;
}

// lObject:
// 0 return a JSValue HBObj in case of object
// 1 returns a JSValue HBObj every time
// 2 never returns a JSValue HBObj
PHB_ITEM hb_FromJS(PHB_ITEM dest, JSValueRef src, int lObject) {
	JSType type;
	if(lObject!=1) {
		type = JSValueGetType(ctx,src);
		switch(type) {
			case kJSTypeUndefined:  return hb_itemPutNil(dest);
			case kJSTypeNull:       return hb_itemPutPtr(dest,0);
			case kJSTypeBoolean:    return hb_itemPutL(dest, JSValueToBoolean(ctx, src));
			case kJSTypeNumber:     return hb_itemPutND(dest, JSValueToNumber(ctx, src,0));
			case kJSTypeString:
			{
				JSStringRef strValue = JSValueToStringCopy(ctx, src, 0);
				const JSChar *u16Str = JSStringGetCharactersPtr(strValue);
                size_t len = JSStringGetLength(strValue);
				dest = hb_itemPutStrLenU16(dest, HB_CDP_ENDIAN_NATIVE, (HB_WCHAR*)u16Str,len);
				JSStringRelease(strValue);
				return dest;
			}
			case kJSTypeObject:
			{
				if(JSValueIsArray(ctx,src)) {
					return hb_FromJSArray(dest,src);
					//HB_FromJS_JSON(dest,src); //TODO better
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
					return HB_FromJS_JSON(dest,src); //hash, TODO better
				}
			}

		}
	}
	if(lObject==1) {
        if(dest==hb_stackReturnItem()) {
            hb_clsAssociate( getJSVALUEClassId() );
        } else
        {
            PHB_ITEM pTemp = hb_itemNew(hb_stackReturnItem()); // temp with actual return
            hb_clsAssociate( getJSVALUEClassId() );              // class in return
            if(!dest) dest = hb_itemNew(hb_stackReturnItem());
                else  hb_itemCopy(dest, hb_stackReturnItem()); // class in dest
            hb_itemCopy(hb_stackReturnItem(), pTemp);          // restore old return value
			hb_itemRelease(pTemp);                             // remove old
        }
    	hb_itemArrayPut(dest, getValueIdx(), hb_itemPutPtr(0, (void*)src));
		JSValueProtect(ctx,src);
	}
    return dest;
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
		tmp = hb_FromJS(0,arguments[i],0);
		hb_itemArrayPut(pArgs,i+1,tmp);
		hb_itemRelease(tmp);
	}
	pThis = hb_FromJS(0, thisObject, 1);
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

#include "vtdNav.h"

static int __getChar(VTDNav *vn);
static int __getCharResolved(VTDNav *vn);
static int __getCharUnit(VTDNav *vn);
static inline Boolean __isElement(VTDNav  *vn, int index);
static inline Boolean __isWS(VTDNav *vn, int ch);
static Boolean __matchRawTokenString(VTDNav *vn, int offset, int len, UCS2Char *s);
static Boolean __matchRawTokenString2(VTDNav *vn, Long l, UCS2Char *s);
static Boolean __matchTokenString(VTDNav *vn, int offset, int len, UCS2Char *s);
static Boolean __matchTokenString2(VTDNav *vn, Long l, UCS2Char *s);
static inline int __NSval(VTDNav *vn, int i);
static int __parseInt(VTDNav *vn, int index, int radix);
static int __parseLong(VTDNav *vn, int index, int radix);
static int __resolveLC(VTDNav *vn);
static Boolean __resolveNS(VTDNav *vn, UCS2Char *URL);
static Boolean __resolveNS2(VTDNav *vn, UCS2Char *URL, UCS2Char *ln);



//Create VTDNav object

VTDNav *createVTDNav(int r, encoding enc, Boolean ns, int depth,
					 Byte *x, int xLen, FastLongBuffer *vtd, FastLongBuffer *l1,
					 FastLongBuffer *l2, FastIntBuffer *l3, int so, int len){
	
						 exception e;
						 VTDNav* vn = NULL;
						 int i;

						 if (l1 == NULL ||
							 l2 == NULL ||
							 l3 == NULL ||
							 vtd == NULL||
							 x == NULL ||
							 so<0 ||
							 len < 0 ||
							 xLen < 0 || // size of x
							 r < 0 ||
							 depth < 0 ||
							 (enc <FORMAT_ASCII || 
							  enc>FORMAT_UTF_16LE) 
							 )
						 {
							e.et = invalid_argument;
							e.msg = " invalid argument when creating VTDGen failed ";
							Throw e;							
						 }

						 vn = (VTDNav *) malloc(sizeof(VTDNav));
						 if (vn==NULL){
							e.et = out_of_mem;
							e.msg = "VTDNav allocation failed ";
							Throw e;
						 }
						 vn->l1Buffer = l1;
						 vn->l2Buffer = l2;
						 vn->l3Buffer = l3;
						 vn->vtdBuffer= vtd;
						 vn->XMLDoc = x;

						 vn->encoding = enc;
						 vn->nestingLevel = depth +1;

						 vn->ns = ns;
						 vn->context = (int *)malloc(vn->nestingLevel*sizeof(int));
						 if (vn->context == NULL){
							free(vn);
							e.et = out_of_mem;
							e.msg = "VTDNav allocation failed ";
							Throw e;
						 }
						 vn->context[0] = 0;
						 for (i=0;i<vn->nestingLevel;i++){
							vn->context[i] = -1;
						 }
						 vn->currentOffset = 0;
						 
						 vn->contextBuf = createContextBuffer2(10, vn->nestingLevel+7);
						 vn->stackTemp = (int *)malloc((vn->nestingLevel+7)*sizeof(int));

						 if (vn->contextBuf == NULL 
							 || vn->stackTemp == NULL){
							 freeContextBuffer(vn->contextBuf);
							 free(vn->stackTemp);
							 free(vn->context);
							 free(vn);
							 e.et = out_of_mem;
							 e.msg = "VTDNav allocation failed ";
							 Throw e;
                          }
						  vn->l1index = vn->l2index = vn->l3index = -1;
						  vn->l2lower = vn->l2upper = -1;
						  vn->l3lower = vn->l3upper = -1;
						  vn->offset = so;
						  vn->docLen = len;
						  vn->vtdSize = vtd->size;
						  vn->bufLen = xLen;

						  return vn;
					 }

//Free VTDNav object
void freeVTDNav(VTDNav *vn)
{	
	if (vn!=NULL){
		freeContextBuffer(vn->contextBuf);
		freeFastLongBuffer(vn->vtdBuffer);
		freeFastLongBuffer(vn->l1Buffer);
		freeFastLongBuffer(vn->l2Buffer);
		freeFastIntBuffer(vn->l3Buffer);
		free(vn->context);
		free(vn->stackTemp);
		free(vn->XMLDoc);
	}
	free(vn);
}

//Return the attribute count of the element at the cursor position.
int getAttrCount(VTDNav *vn);

//Get the token index of the attribute value given an attribute name.     
int getAttrVal(VTDNav *vn, UCS2Char *attrName);

//Get the token index of the attribute value of given URL and local name.
//If ns is not enabled, the lookup will return -1, indicating a no-found.
//Also namespace nodes are invisible using this method.
int getAttrValNS(VTDNav *vn, UCS2Char* URL, UCS2Char *localName);

//This function decodes the underlying byte array into corresponding 
//UCS2 char representation .
//It doesn't resolves built-in entity and character references.
//Length will never be zero
static int __getChar(VTDNav *vn);

//This method decodes the underlying byte array into corresponding 
//UCS2 char representation .
//Also it resolves built-in entity and character references.
static int __getCharResolved(VTDNav *vn);

//Get the next char unit which gets decoded automatically
static int __getCharUnit(VTDNav *vn);

//Get the depth (>=0) of the current element.
inline int getCurrentDepth(VTDNav *vn);

// Get the index value of the current element.
int getCurrentIndex(VTDNav *vn);

// Get the starting offset and length of an element
// encoded in a long, upper 32 bit is length; lower 32 bit is offset
Long getElementFragment(VTDNav *vn);

/**
 * Get the encoding of the XML document.
 * <pre>   0  ASCII       </pre>
 * <pre>   1  ISO-8859-1  </pre>
 * <pre>   2  UTF-8       </pre>
 * <pre>   3  UTF-16BE    </pre>
 * <pre>   4  UTF-16LE    </pre>
 */
inline int getEncoding(VTDNav *vn);

// Get the maximum nesting depth of the XML document (>0).
// max depth is nestingLevel -1
inline int getNestingLevel(VTDNav *vn);

// Get root index value.
inline int getRootIndex(VTDNav *vn);


// This function returns of the token index of the type character data or CDATA.
// Notice that it is intended to support data orient XML (not mixed-content XML).
int getText(VTDNav *vn);

//Get total number of VTD tokens for the current XML document.
inline int getTokenCount(VTDNav *vn);

//Get the depth value of a token (>=0)
int getTokenDepth(VTDNav *vn, int index);

//Get the token length at the given index value
//please refer to VTD spec for more details
int getTokenLength(VTDNav *vn, int index);

//Get the starting offset of the token at the given index.
int getTokenOffset(VTDNav *vn, int index);

//Get the XML document 
inline Byte* getXML(VTDNav *vn);

//Get the token type of the token at the given index value.
int getTokenType(VTDNav *vn, int index);

//Test whether current element has an attribute with the matching name.
Boolean hasAttr(VTDNav *vn, UCS2Char *attrName);

//Test whether the current element has an attribute with 
//matching namespace URL and localname.
Boolean hasAttrNS(VTDNav *vn, UCS2Char *URL, UCS2Char *localName);

//Test the token type, to see if it is a starting tag.
static inline Boolean __isElement(VTDNav  *vn, int index);

//Test whether ch is a white space character or not.
static inline Boolean __isWS(VTDNav *vn, int ch);

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes.
int iterate(VTDNav *vn, int dp, UCS2Char *en);

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes .
//When URL is "*" it will match any namespace
//if ns is false, return false immediately
int iterateNS(VTDNav *vn, int dp, UCS2Char *URL, UCS2Char *ln);

//Test if the current element matches the given name.
Boolean matchElement(VTDNav *vn, UCS2Char *en);

//Test whether the current element matches the given namespace URL and localname.
//URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
//ln is the localname that, when set to *, matches any localname
Boolean matchElementNS(VTDNav *vn, UCS2Char *URL, UCS2Char *ln);

//Match a string against a token with given offset and len, entities 
//doesn't get resolved.
static Boolean __matchRawTokenString(VTDNav *vn, int offset, int len, UCS2Char *s);

//Match a string with a token represented by a long (upper 32 len, lower 32 offset).
static Boolean __matchRawTokenString2(VTDNav *vn, Long l, UCS2Char *s);

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchRawTokenString(VTDNav *vn, int index, UCS2Char *s);

//Match a string against a token with given offset and len, entities get 
//resolved properly.
static Boolean __matchTokenString(VTDNav *vn, int offset, int len, UCS2Char *s);

//Match a string against a "non-extractive" token represented by a 
//long (upper 32 len, lower 32 offset).
static Boolean __matchTokenString2(VTDNav *vn, Long l, UCS2Char *s);

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchTokenString(VTDNav *vn, int index, UCS2Char *s);

//Evaluate the namespace indicator in bit 31 and bit 30.
static inline int __NSval(VTDNav *vn, int i);

//Convert a vtd token into a double.
double parseDouble(VTDNav *vn, int index);

//Convert a vtd token into a float.
float parseFloat(VTDNav *vn, int index);

//Convert a vtd token into an int
int parseInt(VTDNav *vn, int index);

//Convert a vtd token into an Int according to given radix.
static int __parseInt(VTDNav *vn, int index, int radix);

//Convert a vtd token into a long
int parseLong(VTDNav *vn, int index);

//Convert a vtd token into a long according to given radix.
static int __parseLong(VTDNav *vn, int index, int radix);

//Load the context info from ContextBuffer.
//Info saved including LC and current state of the context 
Boolean pop(VTDNav *vn);

//Store the context info into the ContextBuffer.
//Info saved including LC and current state of the context 
Boolean push(VTDNav *vn);

//Sync up the current context with location cache.
static int __resolveLC(VTDNav *vn);

//Test whether the URL is defined in the document.
static Boolean __resolveNS(VTDNav *vn, UCS2Char *URL);

//Test whether the URL is defined in the document.
//Null is allowed to indicate the name space should be undefined.
static Boolean __resolveNS2(VTDNav *vn, UCS2Char *URL, UCS2Char *ln);

// A generic navigation method.
// Move the current to the element according to the direction constants
// If no such element, no position change and return false (0).
/* Legal direction constants are 	<br>
	 * <pre>		ROOT            0  </pre>
	 * <pre>		PARENT          1  </pre>
	 * <pre>		FIRST_CHILD     2  </pre>
	 * <pre>		LAST_CHILD      3  </pre>
	 * <pre>		NEXT_SIBLING    4  </pre>
	 * <pre>		PREV_SIBLING    5  </pre>
	 * <br>
	 */
Boolean toElement(VTDNav *vn, navDir direction);

/**
 * A generic navigation method.
 * Move the current to the element according to the direction 
 * constants and the element name
 * If no such element, no position change and return false (0).
 * "*" matches any element
 * Legal direction constants are 	<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 * for ROOT and PARENT, element name will be ignored.
 */
Boolean toElement2(VTDNav *vn, navDir direction, UCS2Char *en);
/*	
 * A generic navigation function with namespace support.
 * Move the current to the element according to the direction constants and the prefix and local names
 * If no such element, no position change and return false(0).
 * URL * matches any namespace, including undefined namespaces
 * a null URL means hte namespace prefix is undefined for the element
 * ln *  matches any localname
 * Legal direction constants are<br>
 * <pre>		ROOT            0  </pre>
 * <pre>		PARENT          1  </pre>
 * <pre>		FIRST_CHILD     2  </pre>
 * <pre>		LAST_CHILD      3  </pre>
 * <pre>		NEXT_SIBLING    4  </pre>
 * <pre>		PREV_SIBLING    5  </pre>
 * <br>
 * for ROOT and PARENT, element name will be ignored.
 * If not ns enabled, return false immediately with no position change.
 */
Boolean toElementNS(VTDNav *vn, navDir direction, UCS2Char *URL, UCS2Char *ln);

//This method normalizes a token into a string in a way that resembles DOM.
//The leading and trailing white space characters will be stripped.
//The entity and character references will be resolved
//Multiple whitespaces char will be collapsed into one.
UCS2Char *toNormalizedString(VTDNav *vn, int index);

//Convert a token at the given index to a String, 
//(built-in entity and char references not resolved)
//(entities and char references not expanded).
UCS2Char *toRawString(VTDNav *vn, int index);

//Convert a token at the given index to a String, (entities and char 
//references resolved).
// An attribute name or an element name will get the UCS2 string of qualified name 
UCS2Char *toString(VTDNav *vn, int index);
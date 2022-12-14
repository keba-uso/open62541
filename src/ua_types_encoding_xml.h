/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef UA_TYPES_ENCODING_XML_H_
#define UA_TYPES_ENCODING_XML_H_

#include <open62541/types.h>

#include "ua_util_internal.h"

_UA_BEGIN_DECLS

#define UA_XML_MAXMEMBERSCOUNT 256
#define UA_XML_ENCODING_MAX_RECURSION 100

/* XML schema type definitions */

#define XML_ENCODE_BOOLEAN_TYPE_DEFINITION "<xs:element name=\"Boolean\" nillable=\"true\" type=\"xs:boolean\"/>"
#define XML_ENCODE_BOOLEAN_TYPE_DEFINITION_LEN 62

#define XML_ENCODE_SBYTE_TYPE_DEFINITION "<xs:element name=\"SByte\" nillable=\"true\" type=\"xs:byte\"/>"
#define XML_ENCODE_SBYTE_TYPE_DEFINITION_LEN 57

#define XML_ENCODE_BYTE_TYPE_DEFINITION "<xs:element name=\"Byte\" nillable=\"true\" type=\"xs:unsignedByte\"/>"
#define XML_ENCODE_BYTE_TYPE_DEFINITION_LEN 64

#define XML_ENCODE_INT16_TYPE_DEFINITION "<xs:element name=\"Int16\" nillable=\"true\" type=\"xs:short\"/>"
#define XML_ENCODE_INT16_TYPE_DEFINITION_LEN 58

#define XML_ENCODE_UINT16_TYPE_DEFINITION "<xs:element name=\"UInt16\" nillable=\"true\" type=\"xs:unsignedShort\"/>"
#define XML_ENCODE_UINT16_TYPE_DEFINITION_LEN 67

#define XML_ENCODE_INT32_TYPE_DEFINITION "<xs:element name=\"Int32\" nillable=\"true\" type=\"xs:int\"/>"
#define XML_ENCODE_INT32_TYPE_DEFINITION_LEN 56

#define XML_ENCODE_UINT32_TYPE_DEFINITION "<xs:element name=\"UInt32\" nillable=\"true\" type=\"xs:unsignedInt\"/>"
#define XML_ENCODE_UINT32_TYPE_DEFINITION_LEN 65

#define XML_ENCODE_INT64_TYPE_DEFINITION "<xs:element name=\"Int64\" nillable=\"true\" type=\"xs:long\"/>"
#define XML_ENCODE_INT64_TYPE_DEFINITION_LEN 57

#define XML_ENCODE_UINT64_TYPE_DEFINITION "<xs:element name=\"UInt64\" nillable=\"true\" type=\"xs:unsignedLong\"/>"
#define XML_ENCODE_UINT64_TYPE_DEFINITION_LEN 66

#define XML_ENCODE_FLOAT_TYPE_DEFINITION "<xs:element name=\"Float\" nillable=\"true\" type=\"xs:float\"/>"
#define XML_ENCODE_FLOAT_TYPE_DEFINITION_LEN 58

#define XML_ENCODE_DOUBLE_TYPE_DEFINITION "<xs:element name=\"Double\" nillable=\"true\" type=\"xs:double\"/>"
#define XML_ENCODE_DOUBLE_TYPE_DEFINITION_LEN 60

#define XML_ENCODE_STRING_TYPE_DEFINITION "<xs:element name=\"String\" nillable=\"true\" type=\"xs:string\"/>"
#define XML_ENCODE_STRING_TYPE_DEFINITION_LEN 60

#define XML_ENCODE_DATETIME_TYPE_DEFINITION "<xs:element name=\"DateTime\" nillable=\"true\" type=\"xs:dateTime\"/>"
#define XML_ENCODE_DATETIME_TYPE_DEFINITION_LEN 64

#define XML_ENCODE_GUID_TYPE_DEFINITION "<xs:complexType name=\"Guid\">" \
                                          "<xs:sequence>" \
                                            "<xs:element name=\"String\" type=\"xs:string\" minOccurs=\"0\" />" \
                                          "</xs:sequence>" \
                                        "</xs:complexType>"
#define XML_ENCODE_GUID_TYPE_DEFINITION_LEN 131

#define XML_ENCODE_NODEID_TYPE_DEFINITION "<xs:complexType name=\"NodeId\">" \
                                            "<xs:sequence>" \
                                              "<xs:element name=\"Identifier\" type=\"xs:string\" minOccurs=\"0\" />" \
                                            "</xs:sequence>" \
                                          "</xs:complexType>"
#define XML_ENCODE_NODEID_TYPE_DEFINITION_LEN 137

#define XML_ENCODE_EXPANDEDNODEID_TYPE_DEFINITION "<xs:complexType name=\"ExpandedNodeId\">" \
                                                    "<xs:sequence>" \
                                                      "<xs:element name=\"Identifier\" type=\"xs:string\" minOccurs=\"0\" />" \
                                                    "</xs:sequence>" \
                                                  "</xs:complexType>"
#define XML_ENCODE_EXPANDEDNODEID_TYPE_DEFINITION_LEN 145

typedef struct {
    uint8_t *pos;
    const uint8_t *end;

    uint16_t depth; /* How often did we encoding recurse? */
    UA_Boolean calcOnly; /* Only compute the length of the decoding */
    UA_Boolean prettyPrint;
    UA_Boolean printValOnly; /* Encode only data value. */

    const UA_DataTypeArray *customTypes;
} CtxXml;

typedef struct XmlData XmlData;

typedef enum {
    XML_DATA_TYPE_PRIMITIVE,
    XML_DATA_TYPE_COMPLEX
} XmlDataType;

typedef struct {
    UA_Boolean prevSectEnd; /* Identifier of the previous XML parse segment. */
    char *onCharacters;
    size_t onCharLength;
    XmlData *data;
} XmlParsingCtx;

typedef struct {
    const char *value;
    size_t length;
} XmlDataTypePrimitive;

typedef struct {
    size_t membersSize;
    XmlData **members;
} XmlDataTypeComplex;

struct XmlData {
    const char* name;
    XmlDataType type;
    union {
        XmlDataTypePrimitive primitive;
        XmlDataTypeComplex complex;
    } value;
    XmlData *parent;
};

typedef struct {
    UA_Boolean isArray;
    UA_NodeId typeId;
    XmlData *data;
} XmlValue;

typedef struct {
    XmlParsingCtx *parseCtx;
    XmlValue *value;
    XmlData **dataMembers;      /* Ordered XML data elements (for better iterating). */
    unsigned int membersSize;   /* Number of data members (>= 1 for complex types). */
    unsigned int index;         /* Index of current value member being processed. */

    const UA_DataTypeArray *customTypes;
} ParseCtxXml;

typedef UA_StatusCode
(*encodeXmlSignature)(CtxXml *ctx, const void *src, const UA_DataType *type);

typedef UA_StatusCode
(*decodeXmlSignature)(ParseCtxXml *ctx, void *dst, const UA_DataType *type);

/* Expose the jump tables and some methods */
extern const encodeXmlSignature encodeXmlJumpTable[UA_DATATYPEKINDS];
extern const decodeXmlSignature decodeXmlJumpTable[UA_DATATYPEKINDS];

_UA_END_DECLS

#endif /* UA_TYPES_ENCODING_XML_H_ */

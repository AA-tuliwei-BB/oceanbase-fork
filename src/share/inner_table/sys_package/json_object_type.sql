CREATE OR REPLACE TYPE JSON_OBJECT_T OID '300024' AS OPAQUE
(
  STATIC FUNCTION parse(jsn VARCHAR2) return JSON_OBJECT_T,
  STATIC FUNCTION parse(jsn CLOB)     return JSON_OBJECT_T,
  STATIC FUNCTION parse(jsn BLOB)     return JSON_OBJECT_T,

  CONSTRUCTOR FUNCTION JSON_OBJECT_T RETURN SELF AS RESULT,
  CONSTRUCTOR FUNCTION JSON_OBJECT_T(jsn JSON_ELEMENT_T) RETURN SELF AS RESULT,
  CONSTRUCTOR FUNCTION JSON_OBJECT_T(jsn JSON_OBJECT_T) RETURN SELF AS RESULT,
  CONSTRUCTOR FUNCTION JSON_OBJECT_T(jsn VARCHAR2) RETURN SELF AS RESULT,
  CONSTRUCTOR FUNCTION JSON_OBJECT_T(jsn CLOB) RETURN SELF AS RESULT,
  CONSTRUCTOR FUNCTION JSON_OBJECT_T(jsn BLOB) RETURN SELF AS RESULT,

  MEMBER FUNCTION is_Object RETURN BOOLEAN,
  MEMBER FUNCTION is_Array RETURN BOOLEAN,
  MEMBER FUNCTION is_Scalar RETURN BOOLEAN,
  MEMBER FUNCTION is_String RETURN BOOLEAN,
  MEMBER FUNCTION is_Number RETURN BOOLEAN,
  MEMBER FUNCTION is_Boolean RETURN BOOLEAN,
  MEMBER FUNCTION is_True RETURN BOOLEAN,
  MEMBER FUNCTION is_False RETURN BOOLEAN,
  MEMBER FUNCTION is_Null RETURN BOOLEAN,
  MEMBER FUNCTION is_Date RETURN BOOLEAN,
  MEMBER FUNCTION is_Timestamp RETURN BOOLEAN,
  MEMBER FUNCTION get_Size     RETURN NUMBER,

  MEMBER FUNCTION to_String RETURN VARCHAR2,
  MEMBER FUNCTION to_Number    RETURN NUMBER,
  MEMBER FUNCTION to_Date      RETURN DATE,
  MEMBER FUNCTION to_Timestamp RETURN TIMESTAMP,
  MEMBER FUNCTION to_Boolean   RETURN BOOLEAN,
  MEMBER FUNCTION to_Clob      RETURN CLOB,
  MEMBER FUNCTION to_Blob      RETURN BLOB,

  MEMBER PROCEDURE to_Clob(c IN OUT CLOB),
  MEMBER PROCEDURE to_Blob(c IN OUT BLOB),

  MEMBER FUNCTION get_String(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return VARCHAR2,
  MEMBER FUNCTION get_Number(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return NUMBER,
  MEMBER FUNCTION get_Date(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return DATE,
  MEMBER FUNCTION get_Timestamp(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return TIMESTAMP,
  MEMBER FUNCTION get_Boolean(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return BOOLEAN,
  MEMBER FUNCTION get_Clob(self IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return CLOB,
  MEMBER FUNCTION get_Blob(self IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return BLOB,
  MEMBER FUNCTION get_Object(self IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return JSON_OBJECT_T,
  MEMBER FUNCTION get(self IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2) return JSON_ELEMENT_T,

  MEMBER PROCEDURE get_Clob(self IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, c IN OUT CLOB),
  MEMBER PROCEDURE get_Blob(self IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, c IN OUT BLOB),

  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value BOOLEAN),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value JSON_OBJECT_T),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value JSON_ELEMENT_T),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value JSON_ARRAY_T),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value VARCHAR2),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value NUMBER),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value DATE),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value TIMESTAMP),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value BLOB),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value CLOB),
  MEMBER PROCEDURE put(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2, value JSON),
  MEMBER PROCEDURE put_Null(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2),
  MEMBER PROCEDURE remove(SELF IN OUT NOCOPY JSON_OBJECT_T, key VARCHAR2),
  MEMBER PROCEDURE rename_Key(SELF IN OUT NOCOPY JSON_OBJECT_T, keyOld VARCHAR2, keyNew VARCHAR2),
  MEMBER PROCEDURE on_Error(SELF IN OUT NOCOPY JSON_OBJECT_T, val NUMBER),

  MEMBER FUNCTION has(key VARCHAR2) return BOOLEAN,
  MEMBER FUNCTION get_Type(key VARCHAR2) return VARCHAR2,
  MEMBER FUNCTION clone(self IN OUT NOCOPY JSON_OBJECT_T) RETURN JSON_OBJECT_T
);
//
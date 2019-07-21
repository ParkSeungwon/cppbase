namespace ber {
	enum Tag {
		EOC,//0
		BOOLEAN,//1
		INTEGER,//2
		BIT_STRING,//3 
		OCTET_STRING, //4
		NULL_TYPE,//5
		OBJECT_IDENTIFIER,//6
		OBJECT_DESCRIPTOR, //7
		EXTERNAL, //8
		REAL, //9
		ENUMERATED,//10
		EMBEDDED_PDV, //11
		UTF8STRING, //12
		RELATIVE_OID, //13
		RESERVED1, //14
		RESERVED2, //15
		SEQUENCE,//16
		SET, //17
		NUMERIC_STRING, //18
		PRINTABLE_STRING, //19
		T61_STRING, //20
		VIDEOTEX_STRING,//21 
		IA5_STRING, //22
		UTCTIME, //23
		GENERALIZED_TIME, //24
		GRAPHIC_STRING, //25
		VISIBLE_STRING, //26
		GENERAL_STRING, //27
		UNIVERSAL_STRING, //28
		CHARACTER_STRING, //29
		BMP_STRING//30
	};

	enum Class {
		UNIVERSAL, APPLICATION, CONTEXT_SPECIFIC, PRIVATE
	};

	enum PC {
		PRIMITIVE, CONSTRUCTED
	};

	struct Type {
		Tag tag;
		Class cls;
		PC pc;
	};
}





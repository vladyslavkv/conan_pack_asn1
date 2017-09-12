/*
 * @(#)AsnRuntime.h
 *
 * Copyright (c) 2009-2015, Shannon Information Co., Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 	1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * 	2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * 	3. Neither the name of Shannon Information nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef ASNRUNTIME_H_
#define ASNRUNTIME_H_

#include <sstream>
#include <cstring>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <map>
#include "../bigint/BigIntegerLibrary.hh"

using namespace std;

namespace asnrt {

#define		ASNRT_VERSION		"v3.14.15.r20150602"

#ifdef _MSC_VER /* VS */
	typedef __int64 xlong;
	typedef unsigned __int64 uxlong;
	#define NL "\r\n"
#else /* Unix */
	typedef long long xlong;
	typedef unsigned long long uxlong;
	#define NL "\n"
#endif

/* depreciated, use xlong instead */
typedef xlong int64_t;
typedef uxlong uint64_t;

#ifndef offsetof
#define offsetof(type, field) ((size_t)(&((type *)0)->field))
#endif /* offsetof */


/* The encoding rules */

#define		BASIC_ENCODING_RULES             0
#define		CANONICAL_ENCODING_RULES         1
#define		DISTINGUISHED_ENCODING_RULES     2
#define		UNALIGNED_PACKED_ENCODING_RULES  3
#define		ALIGNED_PACKED_ENCODING_RULES    4

// C++ values types for ASN.1 values

typedef std::basic_string<bool> bitstring;

typedef string	octetstring;

typedef std::basic_string<wchar_t> wstring;

typedef std::basic_string<unsigned int> ustring;

typedef std::basic_string<size_t> objectid;

class Date {
private:
	xlong fastTime;
	struct tm breakTime;
	int milliseconds;

public:
	Date();
	Date(xlong time);
	Date(int year, int month, int date, int hours, int minutes, int seconds, int milleseconds);

	int getYear(); /* 1970~2037 */
	int getMonth(); /* 1~12 */
	int getDate();  /* 1~31 */
	int getHours(); /* 0~23 */
	int getMinutes(); /* 0~59 */
	int getSeconds(); /* 0~59 */
	int getMilliSeconds(); /* 0~999 */

	xlong getTime();

	Date& toUTCTime();
	Date& fromUTCTime(int year, int month, int date, int hours, int minutes, int seconds, int milleseconds);

	bool operator==(const Date &t) const;

private:
	int getTimeZoneOffset();
};

// ASN.1 Exception

/* error codes */
#define		INVALID_TAG                     1
#define		INVALID_LENGTH                  2
#define		INVALID_INTEGER                 3
#define		INVALID_ENUM                    4
#define		INVALID_REAL                    5
#define		OUT_OF_MEMORY                   6
#define		INVALID_TIME                    7
#define		MISSING_COMPONENT               8
#define		EXTRA_COMPONENT                 9
#define		INVALID_INDEX                   10
#define		BUFFER_OVERFLOW                 11
#define		BUFFER_UNDERFLOW                12
#define		INVALID_ENCODE_RULE             13
#define		NULL_POINTER_EXCEPTION          14
#define		NOT_PERMITTED_ALPHABET          15
#define		NO_MATCH_INFO_OBJ               16
#define		INVALID_LICENSE                 17
#define		INVALID_SIZE                    18
#define		IO_EXCEPTION                    19
#define		UNKNOWN_EXCEPTION               20
#define		ABSTRACT_FUNCTION               21

#ifdef _DEBUG_
	#define ASN_EXCEPTION(errorCode, message) { AsnException ex(__FILE__, __LINE__, errorCode, message); throw ex; }
	#define ASN_ERROR(errorCode) { AsnException ex(__FILE__, __LINE__, errorCode); throw ex; }
#else
	#define ASN_EXCEPTION(errorCode, message) { AsnException ex(errorCode, message); throw ex; }
	#define ASN_ERROR(errorCode) { AsnException ex(errorCode); throw ex; }
#endif

class AsnException {
public:
	static string err_msgs[];

private:
	int errorCode;
	stringstream message;

public:
	AsnException(char* file, int line, int errorCode, string message);
	AsnException(char* file, int line, int errorCode);
	AsnException(int errorCode, string message);
	AsnException(int errorCode);
	AsnException(const AsnException& o);
	int getErrorCode();
	string getMessage();
	AsnException& operator<<(long __n);
	AsnException& operator<<(unsigned long __n);
	AsnException& operator<<(bool __n);
	AsnException& operator<<(short __n);
	AsnException& operator<<(unsigned short __n);
	AsnException& operator<<(int __n);
	AsnException& operator<<(unsigned int __n);
#ifdef _GLIBCXX_USE_LONG_LONG
	AsnException& operator<<(long long __n);
	AsnException& operator<<(unsigned long long __n);
#endif
	AsnException& operator<<(double __f);
	AsnException& operator<<(float __f);
	AsnException& operator<<(long double __f);
	AsnException& operator<<(const void* __p);
	AsnException& operator<<(stringstream __sb);
	AsnException& operator<<(string __s);
	AsnException& operator<<(char* __cstr);
};

// ASN.1 converters

struct AsnConverter {
	virtual ~AsnConverter() {}
};

struct BooleanConverter: public AsnConverter {
	bool toValue(void* object);
	void toObject(void* object, bool value);
};

struct NullConverter: public AsnConverter {
};

class AsnBuffer;
class IntegerType;
struct IntegerConverter : public AsnConverter {
	virtual ~IntegerConverter() {}
	virtual BigInteger toValue(void* object) = 0;
	virtual void toObject(void* object, BigInteger bi) = 0;
	virtual void ber_encode(AsnBuffer* buffer, void* object, IntegerType* type) = 0;
	virtual void ber_decode(AsnBuffer* buffer, void* object, IntegerType* type) = 0;
	virtual void per_encode(AsnBuffer* buffer, void* object, IntegerType* type) = 0;
	virtual void per_decode(AsnBuffer* buffer, void* object, IntegerType* type) = 0;
};

struct IntConverter : public IntegerConverter {
	BigInteger toValue(void* object);
	void toObject(void* object, BigInteger bi);
	void ber_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void ber_decode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_decode(AsnBuffer* buffer, void* object, IntegerType* type);
};

struct LongConverter : public IntegerConverter {
	BigInteger toValue(void* object);
	void toObject(void* object, BigInteger bi);
	void ber_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void ber_decode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_decode(AsnBuffer* buffer, void* object, IntegerType* type);
};

struct XLongConverter : public IntegerConverter {
	BigInteger toValue(void* object);
	void toObject(void* object, BigInteger bi);
	void ber_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void ber_decode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_decode(AsnBuffer* buffer, void* object, IntegerType* type);
};

struct BigIntegerConverter : public IntegerConverter {
	BigInteger toValue(void* object);
	void toObject(void* object, BigInteger bi);
	void ber_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void ber_decode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_encode(AsnBuffer* buffer, void* object, IntegerType* type);
	void per_decode(AsnBuffer* buffer, void* object, IntegerType* type);
};

struct AbstractEnumeratedConverter : public AsnConverter {
	virtual ~AbstractEnumeratedConverter() {}
	virtual int toValue(void* object) = 0;
	virtual void toObject(void* object, int value) = 0;
};

template<typename _Tp>
struct EnumeratedConverter: public AbstractEnumeratedConverter {

	int toValue(void* object) {
		_Tp* _tp = (_Tp*)object;
		int value = *_tp;
		return value;
	}

	void toObject(void* object, int value) {
		::new(object) _Tp(value);
	}

};

struct RealConverter: public AsnConverter {
	virtual ~RealConverter() {}
	virtual double toValue(void* object) = 0;
	virtual void toObject(void* object, double value) = 0;
};

struct FloatConverter: RealConverter {
	double toValue(void* object);
	void toObject(void* object, double value);
};

struct DoubleConverter: RealConverter {
	double toValue(void* object);
	void toObject(void* object, double value);
};

struct BitStringConverter: public AsnConverter {
	bitstring* toBitString(void* object);
};

struct OctetStringConverter: public AsnConverter {
	octetstring* toOctetString(void* object);
};

struct ObjectIdConverter: public AsnConverter {
	objectid* toObjectid(void* object);
};

struct StringConverter: public AsnConverter {
	string* toString(void* object);
};

struct WStringConverter: public AsnConverter {
	wstring* toString(void* object);
};

struct UStringConverter: public AsnConverter {
	ustring* toString(void* object);
};

struct DateConverter : public AsnConverter {
	Date* toDate(void* object);
};

struct AbstractListConverter : public AsnConverter {
	AsnConverter* componentConverter;

	AbstractListConverter(AsnConverter* componentConverter) {
		this->componentConverter = componentConverter;
	}

	virtual ~AbstractListConverter() {}

	/**
	 *  Returns the number of elements in the list.
	 **/
	virtual size_t getSize(void* listObject) = 0;

	/**
	 *  Removes all of the elements from this list.
	 **/
	virtual void clearAll(void* listObject) = 0;

	/**
	 *  Adds a new element to the list.
	 **/
	virtual void* addElement(void* listObject) = 0;

	/**
	 * A new element has been decoded completed
	 **/
	virtual void doneElement(void* listObject, void* element) = 0;

    /**
     * Returns the component at the specified index.
     */
	virtual void* elementAt(void* listObject, size_t index) = 0;
};

template<typename _Tp>
struct VectorConverter : public AbstractListConverter {
	typedef vector<_Tp>	vector_type;

	VectorConverter(AsnConverter* componentConverter) : AbstractListConverter(componentConverter) {}

	size_t getSize(void* listObject) {
		vector_type* _vector = (vector_type* ) listObject;
		return _vector->size();
	}

	void clearAll(void* listObject) {
		vector_type* _vector = (vector_type* ) listObject;
		_vector->clear();
	}

	void* addElement(void* listObject) {
		vector_type* _vector = (vector_type* ) listObject;
		_Tp e;
		_vector->push_back(e);
		_Tp& ref = _vector->back();
		return &ref;
	}

	void doneElement(void* listObject, void* element) {};

	void* elementAt(void* listObject, size_t index) {
		vector_type* _vector = (vector_type*) listObject;
		_Tp& ref =_vector->at(index);
		return &ref;
	}

};

struct Allocator {

	virtual ~Allocator() {}

	/**
	 * @parameters
	 * 		value - pointer points to member of class
	 */
	virtual void* allocate(void* value) = 0;

	virtual void dealloc(void* value) = 0;
};

template<typename _Tp>
struct DefaultAllocator : public Allocator {
	void* allocate(void* value) {
        _Tp* _tp =  new _Tp();
		void* vp = _tp;
		*(void**)value = vp;
		return vp;
	}

	void dealloc(void* value) {
		void* vp = *(void**)value;
		if(vp!=NULL) {
			_Tp* _tp =  (_Tp*) vp;
			delete _tp;
			*(void**)value = NULL;
		}
	}
};

struct GenericAllocator : public Allocator {
	size_t size;
	GenericAllocator(size_t size) { this->size = size; }
	void* allocate(void* value) {
		char* vp = new char[size];
		for(size_t i=0; i<size; i++) {
			vp[i] = 0;
		}
		*(void**)value = vp;
		return vp;
	}
	void dealloc(void* value) {
		char* vp = *(char**)value;
		delete vp;
		*(void**)value = NULL;
	}
};

struct MemberDescriptor {
	int offset;
	AsnConverter* converter;
	Allocator* allocator;

	void* get(void* composite) {
	    void* p = (char*)composite + offset;
	    void* member = allocator==NULL? p: (*(void**)p);
	    return member;
	}

	void* create(void* composite) {
	    void* p = (char*)composite + offset;
	    void* member = allocator==NULL? p: allocator->allocate(p);
	    return member;
	}

	void free(void* composite) {
		if(allocator!=NULL) {
			void* p = (char*)composite + offset;
			allocator->dealloc(p);
		}
	}

};

struct CompositeConverter : public AsnConverter {
	MemberDescriptor* componentDescriptors;
	CompositeConverter(MemberDescriptor* componentDescriptors) throw() {
		this->componentDescriptors = componentDescriptors;
	}
	virtual ~CompositeConverter() {}
	virtual AsnConverter* getComponentConverter(int index) throw();
	virtual void* getComponentObject(void* object, unsigned int index) throw();
	virtual void* createComponentObject(void* object, unsigned int index) throw();
	virtual void doneComponentObject(void* object, unsigned int index) throw();

};

struct ChoiceConverter : public AsnConverter {
	MemberDescriptor* alternativeDescriptors;
	ChoiceConverter() {}
	ChoiceConverter(MemberDescriptor* alternativeDescriptors) {
		this->alternativeDescriptors = alternativeDescriptors;
	}
	virtual ~ChoiceConverter() {}
	virtual AsnConverter* getAlternativeConverter(int index);
	virtual int getAlternativeIndex(void* object) = 0;
	virtual void* getAlternativeObject(void* object);
	virtual void* createAlternativeObject(void* object, int index);
};

struct OpenValueConverter : public AsnConverter {
	MemberDescriptor* actualValueDescriptors;
	OpenValueConverter(MemberDescriptor* actualValueDescriptors);
	AsnConverter* getActualConverter(unsigned int index);
	void* getActualObject(void* object, unsigned int index);
	void* createActualObject(void* object, unsigned int index);
};

// Useful macros and templates

#define DECLARE_TYPE() \
public:	 static AsnType* TYPE

#define DECLARE_CONVERTER(ConverterClass) \
private: static MemberDescriptor _MemberDescriptors_[]; \
public:	 static ConverterClass CONVERTER

#define DECLARE_OPEN_CONVERTER(component) \
private: \
	static MemberDescriptor _##component##AcutalTypesDescriptors_[]; 	\
	static OpenValueConverter _##component##Converter_

#define ASN_COMPONENT(CompositeClass, component, converter, allocator) { offsetof(CompositeClass, component), converter, allocator }
#define ASN_ALTERNATIVE(ChoiceClass, alternative, converter, allocator) { offsetof(ChoiceClass, alternative), converter, allocator }
#define MEMBER_DESCRIPTOR(offset, converter, allocator) { offset, converter, allocator }

#define ASN_COMPOSITE(ConverterClass, CompositeClass, ...) 	\
	MemberDescriptor CompositeClass::_MemberDescriptors_[] = {	\
		__VA_ARGS__ \
	};	\
	ConverterClass CompositeClass::CONVERTER(_MemberDescriptors_)

#define ASN_CHOICE(ConverterClass, ChoiceClass, ...) \
	MemberDescriptor ChoiceClass::_MemberDescriptors_[] = {	\
		__VA_ARGS__ \
	};	\
	ConverterClass ChoiceClass::CONVERTER(_MemberDescriptors_)

#define ASN_OPEN_TYPE(CompositeClass, component, ...) \
	MemberDescriptor CompositeClass::_##component##AcutalTypesDescriptors_[] = {	\
		__VA_ARGS__ \
	};	\
	OpenValueConverter CompositeClass::_##component##Converter_(_##component##AcutalTypesDescriptors_)

#define ASN_CONVERTER(Module, ConverterType) \
	Module::newConverter(new ConverterType())

#define LIST_CONVERTER(Module, componentCType, componentConverter) \
	Module::newConverter(new VectorConverter< componentCType >(componentConverter))

#define ASN_ALLOCATOR(Module, AllocatorType) \
	Module::newAllocator(new AllocatorType())

// Public API

class AsnBuffer;
class AsnType;

struct AsnTriplet {
	void* object;
	AsnType* type;
	AsnConverter* converter;
	int typeId;
};

#define  TYPE_ID_BOOLEAN           0
#define  TYPE_ID_NULL              1
#define  TYPE_ID_INTEGER           2
#define  TYPE_ID_ENUMERATED        3
#define  TYPE_ID_REAL              4
#define  TYPE_ID_BITSTRING         5
#define  TYPE_ID_OCTETSTRING       6
#define  TYPE_ID_OBJECTIDENTIFIER  7
#define  TYPE_ID_RELATIVEOID       8
#define  TYPE_ID_NUMERICSTRING     9
#define  TYPE_ID_PRINTABLESTRING   10
#define  TYPE_ID_VISIBLESTRING     11
#define  TYPE_ID_IA5STRING         12
#define  TYPE_ID_TELETEXSTRING     13
#define  TYPE_ID_VIDEOTEXSTRING    14
#define  TYPE_ID_GRAPHICSTRING     15
#define  TYPE_ID_GENERALSTRING     16
#define  TYPE_ID_UNIVERSALSTRING   17
#define  TYPE_ID_BMPSTRING         18
#define  TYPE_ID_UTF8STRING        19
#define  TYPE_ID_CHARACTERSTRING   20
#define  TYPE_ID_OBJECTDESCRIPTOR  21
#define  TYPE_ID_GENERALIZEDTIME   22
#define  TYPE_ID_UTCTIME           23
#define  TYPE_ID_SEQUENCE          24
#define  TYPE_ID_SET               25
#define  TYPE_ID_SEQUENCEOF        26
#define  TYPE_ID_SETOF             27
#define  TYPE_ID_CHOICE            28

/* Buffer Allocation */

/**
 * Wraps an array into a buffer, basically, this buffer is for decoding.
 *
 * The new buffer will be backed by the given byte array;
 * that is, modifications to the buffer will cause the array to be modified and vice versa.
 * For Basic Encoding Rules, the new buffer's capacity and limit will be <tt>numBytes</tt>,
 * and for Pack Encoding Rules, the new buffer's capacity and limit will be <tt>numBytes*8</tt>.
 */
AsnBuffer* wrap_buffer(char* array, unsigned long numBytes, char encodingRules);

/**
 * Allocates a new buffer, basically, this buffer is for encoding.
 *
 * For Basic Encoding Rules, the new buffer's capacity and limit will be <tt>numBytes</tt>,
 * and for Pack Encoding Rules, the new buffer's capacity and limit will be <tt>numBytes*8</tt>.
 */
AsnBuffer* alloc_buffer(unsigned long numBytes, bool autoExpand, char encodingRules);

/**
 * Create a buffer out of an istream, this buffer is for decoding.
 */
AsnBuffer* istream_buffer(istream* in, unsigned long numBytes, char encodingRules);

/**
 * Create a buffer out of an ostream, this buffer is for encoding.
 */
AsnBuffer* ostream_buffer(ostream* out, unsigned long numBytes, char encodingRules);

/**
 * Get the buffer's encoding content
 */
void buffer_content(AsnBuffer* buffer, octetstring* content);

/**
 * Free a buffer. The memory of byte array and the buffer itself will
 * be freed.
 */
void free_buffer(AsnBuffer*& buffer);


/* Encoding/Decoding Routines */

/**
 * Encode the object with encoding rules specified in the buffer
 **/
void encode_object(AsnBuffer* buffer, void* object, AsnType* type, AsnConverter* converter) throw (AsnException&);

/**
 * Decode the object with encoding rules specified in the buffer
 **/
void decode_object(AsnBuffer* buffer, void* object, AsnType* type, AsnConverter* converter) throw (AsnException&);


/*  Utilities Routines */

/**
 * print the object as ASN.1 value
 **/
void print_object(const void* object, AsnType* type, AsnConverter* converter, ostream* out);

/**
 * determine if the two object equal as ASN.1 value
 **/
bool equals_to(const void* o1, const void* o2, AsnType* type, AsnConverter* converter);

/**
 * deep clone object o2 to o1 as ASN.1 value
 * that is o1 = o2
 **/
void clone_object(const void* o1, const void* o2, AsnType* type, AsnConverter* converter);

/**
 * deep free object content
 **/
void free_object(void* object, AsnType* type, AsnConverter* converter);


// Miscellaneous

class TypeEntry {
public:
	long id;
	AsnType* type;
};

class AsnModule : public ChoiceConverter {
private:
	map<int, AsnType*> basic_types;
	map<long, AsnType*> types_table;
	list<AsnType*> types;
	list<AsnConverter*> converters;
	list<Allocator*> allocators;

public:
	AsnModule();
	AsnModule(const char* metafile);
	virtual ~AsnModule();
	AsnType* lookup(long id);
	void addConverter(AsnConverter* converter);
	void addAllocator(Allocator* allocator);

protected:
	bool loaded;
	void load(const char* metafile);
	void unload();

private:
	void decode(vector<TypeEntry>& entries, octetstring& octets);
	void linkTypesHierarchy(vector<TypeEntry>& entries);
	AsnType* linkTypeHierarchy(AsnType* type);

public:
	int getAlternativeIndex(void* object);
	void* getAlternativeObject(void* object);
	void* createAlternativeObject(void* object, int index);
};


struct Foo {
	int bar;
};

// ~END

}

#endif /*ASNRUNTIME_H_*/

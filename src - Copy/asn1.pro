include(../../props/extCommon.pri)

win32 {
    DESTDIR = ../lib/windows/$${PLATFORM}/$${CONFIGNAME}/
}
macx {
    DESTDIR = ../lib/macosx/$${PLATFORM}/$${CONFIGNAME}/
}
unix {
    CONFIG(debug, debug|release) {
        DESTDIR = lib/linux/centos7/Debug
    } else {
        DESTDIR = lib/linux/centos7/Release
    }

    QMAKE_CXXFLAGS += -stdlib=libc++ -std=c++11
}

#OBJECTS_DIR = /../obj/$$CONFIGNAME/

TARGET = asn1
TEMPLATE = lib

CONFIG -= qt
CONFIG += staticlib

INCLUDEPATH += ../include/
win32:INCLUDEPATH += ../../VsHeaders

SOURCES += \
    ANY.c \
    asn_codecs_prim.c \
    asn_SEQUENCE_OF.c \
    asn_SET_OF.c \
    ber_decoder.c \
    ber_tlv_length.c \
    ber_tlv_tag.c \
    BIT_STRING.c \
    BMPString.c \
    BOOLEAN.c \
    constr_CHOICE.c \
    constr_SEQUENCE.c \
    constr_SEQUENCE_OF.c \
    constr_SET.c \
    constr_SET_OF.c \
    constr_TYPE.c \
    constraints.c \
    der_encoder.c \
    ENUMERATED.c \
    GeneralizedTime.c \
    GeneralString.c \
    GraphicString.c \
    IA5String.c \
    INTEGER.c \
    ISO646String.c \
    NativeEnumerated.c \
    NativeInteger.c \
    NativeReal.c \
    NULL.c \
    NumericString.c \
    OBJECT_IDENTIFIER.c \
    ObjectDescriptor.c \
    OCTET_STRING.c \
    per_decoder.c \
    per_encoder.c \
    per_support.c \
    PrintableString.c \
    REAL.c \
    RELATIVE-OID.c \
    T61String.c \
    TeletexString.c \
    UniversalString.c \
    UTCTime.c \
    UTF8String.c \
    VideotexString.c \
    VisibleString.c \
    xer_decoder.c \
    xer_encoder.c \
    xer_support.c

HEADERS += \
    ../include/ANY.h \
    ../include/asn_application.h \
    ../include/asn_codecs.h \
    ../include/asn_codecs_prim.h \
    ../include/asn_internal.h \
    ../include/asn_SEQUENCE_OF.h \
    ../include/asn_SET_OF.h \
    ../include/asn_system.h \
    ../include/AsnRuntime.h \
    ../include/ber_decoder.h \
    ../include/ber_tlv_length.h \
    ../include/ber_tlv_tag.h \
    ../include/BIT_STRING.h \
    ../include/BMPString.h \
    ../include/BOOLEAN.h \
    ../include/constr_CHOICE.h \
    ../include/constr_SEQUENCE.h \
    ../include/constr_SEQUENCE_OF.h \
    ../include/constr_SET.h \
    ../include/constr_SET_OF.h \
    ../include/constr_TYPE.h \
    ../include/constraints.h \
    ../include/der_encoder.h \
    ../include/ENUMERATED.h \
    ../include/GeneralizedTime.h \
    ../include/GeneralString.h \
    ../include/GraphicString.h \
    ../include/IA5String.h \
    ../include/INTEGER.h \
    ../include/ISO646String.h \
    ../include/NativeEnumerated.h \
    ../include/NativeInteger.h \
    ../include/NativeReal.h \
    ../include/NULL.h \
    ../include/NumericString.h \
    ../include/OBJECT_IDENTIFIER.h \
    ../include/ObjectDescriptor.h \
    ../include/OCTET_STRING.h \
    ../include/per_decoder.h \
    ../include/per_encoder.h \
    ../include/per_support.h \
    ../include/PrintableString.h \
    ../include/REAL.h \
    ../include/RELATIVE-OID.h \
    ../include/T61String.h \
    ../include/TeletexString.h \
    ../include/UniversalString.h \
    ../include/UTCTime.h \
    ../include/UTF8String.h \
    ../include/VideotexString.h \
    ../include/VisibleString.h \
    ../include/xer_decoder.h \
    ../include/xer_encoder.h \
    ../include/xer_support.h

###########################################
#
#       Makefile for HidDemo Application
#   To build for a 64 bit jdk and 64 bit machine
#   make JDK64=1
#
###########################################

ifeq (${JDK64},1)
	CC=x86_64-w64-mingw32-gcc-4.7.3
	CXX=x86_64-w64-mingw32-g++
	CCFLAGS=-m64
	JDK="/cygdrive/c/jdk64"
else
	CC=i686-pc-mingw32-gcc
	CXX=i686-pc-mingw32-g++
	CCFLAGS=-m32
	JDK="/cygdrive/c/jdk32"
endif

JNICFLAGS=-c -I${JDK}/include -I${JDK}/include/win32 -std=gnu99 -D__int64="long long"

BUILD_DIR=./build
LIB_DIR=./lib
JNI_SRC=com/ti/msp430/usb/hiddemo/management
HIDAPI_ROOT=jni/hidapi
HIDAPI_DIR=jni/hidapi/hidapi
WINOBJS=${HIDAPI_ROOT}windows/hid.c
JNILDFLAGS=-Wall -D_JNI_IMPLEMENTATION_ -Wl,--kill-at
WINLIBS=-lsetupapi
JNISRC=com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.c
JNIOBJS=com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.o


all: management javah jni gui jar

management:
	${JDK}/bin/javac com/ti/msp430/usb/hiddemo/management/*.java


javah: management
	${JDK}/bin/javah -d ${JNI_SRC} -jni com.ti.msp430.usb.hiddemo.management.HidCommunicationManager

gui:
	${JDK}/bin/javac com/ti/msp430/usb/hiddemo/*.java

jni: init
	${CC} ${CCFLAGS} -I${HIDAPI_DIR} -c ${HIDAPI_ROOT}/windows/hid.c -o ${BUILD_DIR}/hid.o
	${CC} ${JNICFLAGS} -I${HIDAPI_DIR} -I${JNI_SRC} jni/${JNISRC} -o ${BUILD_DIR}/${JNIOBJS}
	${CC} ${BUILD_DIR}/${JNIOBJS} ${BUILD_DIR}/hid.o ${WINLIBS} -o ${LIB_DIR}/HidCommunicationManager.dll ${JNILDFLAGS} -shared -static-libgcc

init:
	mkdir -p build
	mkdir -p $(LIB_DIR)

jar:
	${JDK}/bin/jar cmf mainClass HidDemo.jar com icons lib jni
ifeq (${JDK64},1)
	cp lib//HidCommunicationManager.dll HidCommunicationManager64.dll
else
	cp lib//HidCommunicationManager.dll .
endif


clean:
	rm -rf ${BUILD_DIR}
	rm -rf ${LIB_DIR}
	rm -f com/ti/msp430/usb/hiddemo/management/*.class
	rm -f com/ti/msp430/usb/hiddemo/management/*.h

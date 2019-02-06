###########################################
#
#       Makefile for HidDemo Application
#  
#   You will need the libusb-1.0.0-dev package installed on your mac system
#
###########################################

BUILD_DIR=./build
LIB_DIR=./lib

JNI_SRC=com/ti/msp430/usb/hiddemo/management
HIDAPI_ROOT=jni/hidapi
HIDAPI_DIR=jni/hidapi/hidapi
JNISRC=com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.c
JNIOBJS=com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.o



CC=gcc
CXX=g++
CCFLAGS=-fPIC
JDK="/Library/Java/JavaVirtualMachines/jdk1.7.0_71.jdk/Contents/Home"
LIB_NAME=libHidCommunicationManagerMac64.dylib
JNICFLAGS=-c -I${JDK}/include -I${JDK}/include/darwin -std=gnu99 -D__int64="long long" -fPIC
JNILDFLAGS=-Wall -D_JNI_IMPLEMENTATION_ 
LNXOBJS=${HIDAPI_ROOT}/mac/hid.c
LNXLIBS=-framework IOKit -framework CoreFoundation
JNI_FLAGS=

all: management javah jni gui jar

management:
	${JDK}/bin/javac com/ti/msp430/usb/hiddemo/management/*.java


javah: management
	${JDK}/bin/javah ${JNI_FLAGS} -d ${JNI_SRC} -jni com.ti.msp430.usb.hiddemo.management.HidCommunicationManager

gui:
	${JDK}/bin/javac com/ti/msp430/usb/hiddemo/*.java

jni: init
	${CC} ${CCFLAGS} -I${HIDAPI_DIR} -c ${LNXOBJS} -o ${BUILD_DIR}/hid.o
	${CC} ${JNICFLAGS} -I${HIDAPI_DIR} -I${JNI_SRC} jni/${JNISRC} -o ${BUILD_DIR}/${JNIOBJS}
	${CC} ${BUILD_DIR}/${JNIOBJS} ${BUILD_DIR}/hid.o ${LNXLIBS} -o ${LIB_DIR}/${LIB_NAME} ${JNILDFLAGS} -shared

init:
	mkdir -p build
	mkdir -p $(LIB_DIR)

jar:
	${JDK}/bin/jar cmf mainClass HidDemo.jar com icons lib jni
	cp ${LIB_DIR}/${LIB_NAME} .


clean:
	rm -rf ${BUILD_DIR}
	rm -rf ${LIB_DIR}
	rm -f com/ti/msp430/usb/hiddemo/management/*.class
	rm -f com/ti/msp430/usb/hiddemo/management/*.h

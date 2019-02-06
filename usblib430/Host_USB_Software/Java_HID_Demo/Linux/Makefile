###########################################
#
#       Makefile for HidDemo Application
#   To build for a 64 bit jdk and 64 bit machine
#   make Linux=1
#  
#   You will need the libusb-1.0.0-dev package installed on your linux system
#
###########################################

BUILD_DIR=./build
LIB_DIR=./lib

JNI_SRC=com/ti/msp430/usb/hiddemo/management
HIDAPI_ROOT=jni/hidapi
HIDAPI_DIR=jni/hidapi/hidapi
JNISRC=com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.c
JNIOBJS=com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.o



ifeq (${Linux},1)
	CC=gcc
	CXX=gcc
	CCFLAGS=-m64 -fPIC
	JDK="/usr/lib/jvm/java-6-sun-1.6.0.26"
	LIB_NAME=libHidCommunicationManagerLinux64.so
	JNICFLAGS=-c -I${JDK}/include -I${JDK}/include/linux -std=gnu99 -D__int64="long long" -fPIC
	JNILDFLAGS=-Wall -D_JNI_IMPLEMENTATION_ 
	LNXOBJS=${HIDAPI_ROOT}/linux/hid-libusb.c
	LNXLIBS=-lusb-1.0
	JNI_FLAGS=
else
	CC=gcc
	CXX=gcc
	CCFLAGS=-m32 -fPIC
	JDK="/usr/lib/jvm/java-6-sun-1.6.0.26"
	LIB_NAME=libHidCommunicationManagerLinux.so
	JNICFLAGS=-c -I${JDK}/include -I${JDK}/include/linux -std=gnu99 -D__int64="long long" -fPIC
	#JNICFLAGS=-c -I${JDK}/include -I${JDK}/include/linux -std=gnu99 -D__int64="long long" -fPIC
	JNILDFLAGS=-Wall -D_JNI_IMPLEMENTATION_
	LNXOBJS=${HIDAPI_ROOT}/linux/hid.c
	LNXLIBS=-lusb-1.0 -ludev -lrt
	JNI_FLAGS=
endif


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
	${CC} ${BUILD_DIR}/${JNIOBJS} ${BUILD_DIR}/hid.o ${LNXLIBS} -o ${LIB_DIR}/${LIB_NAME} ${JNILDFLAGS} -shared -static-libgcc

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

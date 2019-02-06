/* --COPYRIGHT--,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
#include "com_ti_msp430_usb_hiddemo_management_HidCommunicationManager.h"

#include <hidapi.h>
#include <string.h>
#include <stdlib.h>
#include <TI_aes.h> 
#include <stdio.h>  

unsigned char key[]   = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

/**
 * Converts a Java JNI jstring to a wide character array (what HidAPI uses)
 */
 wchar_t * javaToWSZ(JNIEnv* env, jstring string)
{
    if (string == NULL)
        return NULL;
        
    int len = (*env)->GetStringLength(env,string);
    const jchar* raw = (*env)->GetStringChars(env,string, NULL);
    
    if (raw == NULL)
        return NULL;

    wchar_t* wsz = malloc(len+1);
    memcpy(wsz, raw, len*2);
    wsz[len] = 0;

    (*env)->ReleaseStringChars(env,string, raw);
    return wsz;
}


/*
 * Useful funct to dump the hid_device_info structure.
 * Not used in the code...
*/
void dumpHidDeviceInfo(struct hid_device_info *devs) {

                     printf ("path=%s\n",devs->path);
                     printf ("vendor_id=%d\n", devs->vendor_id);
                     printf ("product_id=%d\n", devs->product_id);
                     printf ("serial_number=%ls\n", devs->serial_number);
                     printf ("release_number=%d\n", devs->release_number);
                     printf ("manufacturer_string=%ls\n",devs->manufacturer_string);
                     printf ("product_string=%ls\n", devs->product_string);
                     printf ("usage_page=%d\n", devs->usage_page);
                     printf ("usage=%d\n", devs->usage);
                     printf ("interface_number=%d\n", devs->interface_number);
                     printf ("END\n\n");

}

/**
 * Returns the total number of connected HID Devces on a system with a valid VID
 * and PID number.
 */
JNIEXPORT jint JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_getNumberOfInterfaces
  (JNIEnv * env, jobject obj, jint vid, jint pid ) {

        struct hid_device_info *devs, *cur_dev;
        int count = 0;
        
        devs = hid_enumerate(0x0, 0x0);
        cur_dev = devs;	
        while (cur_dev) {
            // To see the contents of hid_device_info structure, un-comment the next line
            // dumpHidDeviceInfo(cur_dev);
            
            if(!cur_dev || !cur_dev->vendor_id || !cur_dev->product_id)
                continue;
            
            if(cur_dev->vendor_id == vid && cur_dev->product_id == pid) {
                count++;
            }
            cur_dev = cur_dev->next;
        }
        
        hid_free_enumeration(devs);
        
        return count;
  }
  
/**
 * Returns an JNI object array with the serials of all devices with the 
 * specified VID and PID number.
 */
JNIEXPORT jobjectArray JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_getSerialsForVidPid
  (JNIEnv * env, jobject obj, jint vid, jint pid) {
        
        struct hid_device_info *devs, *cur_dev;
        jobjectArray ret;
        int count = 0;
        int dev_count = Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_getNumberOfInterfaces(env,obj,vid,pid);
       
        //Creating our Java Array
        ret = (jobjectArray)(*env)->NewObjectArray(env,dev_count, 
                (*env)->FindClass(env,"java/lang/String"),(*env)->NewStringUTF(env,""));
        
        // Iterating through our list of HID devices
        devs = hid_enumerate(0x0, 0x0);
        cur_dev = devs;	
        
        while (cur_dev) {

            if(!cur_dev->serial_number) {
                    cur_dev = cur_dev->next;
                    continue;
            }
                
            jstring serial= (*env)->NewString(env,cur_dev->serial_number,wcslen(cur_dev->serial_number));
            
            if(cur_dev->vendor_id == vid && cur_dev->product_id == pid) {
                (*env)->SetObjectArrayElement(env,ret,count,serial);
                count++;
            }
            
            cur_dev = cur_dev->next;
        }
        
        hid_free_enumeration(devs);
        
        return ret;
  }
  
/**
 * Returns an JNI integer array with the interfaces of all devices with the 
 * specified VID and PID number.
*/
  JNIEXPORT jintArray JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_getInterfacesForVidPid
  (JNIEnv * env, jobject obj, jint vid, jint pid) {
  
        struct hid_device_info *devs, *cur_dev;
        jintArray ret;
        int count;
        int dev_count = Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_getNumberOfInterfaces(env,obj,vid,pid);
        
        //Creating our Java Array
        ret = (jintArray)((*env)->NewIntArray(env, dev_count));
        jint buf[dev_count];
        
        // Iterating through our list of HID devices
        devs = hid_enumerate(0x0, 0x0);
        cur_dev = devs;	
        count = 0;
        
        while (cur_dev) {
                            
            if(cur_dev->vendor_id == vid && cur_dev->product_id == pid) {
                buf[count] = cur_dev->interface_number;
                count++;
            }
            
            cur_dev = cur_dev->next;          
        }
        
        (*env)->SetIntArrayRegion(env,ret,0,dev_count,(jint*)buf);
        
        hid_free_enumeration(devs);
        
        return ret;
  }
   
/**
 * Connects to the HID device with the given parameters and returns a pointer
 * to the open handle. The pointer is then kept track of in the Java layer and
 * passed through the other communication functions. If the serial is null or
 * if the interface is a negative value, this function will try to connect to
 * the first available matching VID/PID
 */
  JNIEXPORT jint JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_connectDeviceNative
  (JNIEnv * env, jobject obj, jint vid, jint pid, jstring javaserial, jint device) {
  
        hid_device *handle;
        struct hid_device_info *devs, *cur_dev;
        
      	devs = hid_enumerate(0x0, 0x0);
        cur_dev = devs;
        const char* serial = (*env)->GetStringUTFChars(env,javaserial,0);
        
        /* Iterating over all the connected devices and finding the one we want */
        while (cur_dev) {
      		
            if(cur_dev->vendor_id == vid && cur_dev->product_id == pid) {

                /* If we have an interface number, check it to make sure it matches */
                if(device >= 0 && cur_dev->interface_number != device) {
                    cur_dev = cur_dev->next;
                    continue;
                }
                
                /* Same goes with the serial */
                if(cur_dev->serial_number && javaserial != NULL) {
                    
                    jstring curjavaSerial= (*env)->NewString(env,cur_dev->serial_number,wcslen(cur_dev->serial_number));
                    const char* curSerial=(*env)->GetStringUTFChars(env,javaserial,0);
                    
                    if(strcmp(serial,curSerial) != 0) {
                        cur_dev = cur_dev->next;
                        continue;
                    }
                }
                                
                handle = hid_open_path(cur_dev->path);  
                
                if(handle == NULL) {
                    printf("HID open of path failed\n");
                    hid_free_enumeration(devs);
                    return -1;
                }
                hid_set_nonblocking(handle, 1);
                
                hid_free_enumeration(devs);
                return (int)handle;
            }
    
            cur_dev = cur_dev->next;
        }
    
        hid_free_enumeration(devs);        
        printf("Could not find HID device\n");
        return -1;   
  }
  
/**
 * Close the connection to the HID device with the provided handle and exit the
 * HID API.
 */
  JNIEXPORT void JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_disconnectDeviceNative
  (JNIEnv * env, jobject obj, jint longhandle) {
  
        hid_device *handle = (hid_device*)(longhandle);
        hid_close(handle);
        hid_exit();
  }
  
  /**
   * Send data to the connected HID device. Size is the total size of the buffer
   * (including the Report ID and Data Size bytes). Since Windows and other OSs
   * tend to like 64 Byte chunks of data, if the data size is above 62 bytes
   * the data is carved into 64 byte chunks (2 byte header + 62 bytes of data)
   * each. The last packet's data chunk is padded with zeros to make a complete
   * 62 bytes worth of data.
   */


JNIEXPORT jint JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_sendDataNative
  (JNIEnv * env, jobject obj, jint longhandle, jbyteArray data, jint size) {
        
        int res = 0;
        int bytes_written = 0;
        int totalsize;
        int reportid;
        hid_device *handle;
        unsigned char trans_data[64];
        unsigned char *buf;

        int bytesForPad = 0, i, j;
        int numWords = 0;
        int bufPtr = 2;
        unsigned char tempBuf[16];
        int bytesLeft = 0;
        int aes128DataSize = 16;
        unsigned char tempData[64];  //this buf is required otherwise GUI crashes if read directly done from buf

        
        // Opening the device 
        handle = (hid_device*)longhandle;
        
        // The buffer should be at least 3 (ReportID,Size,At least 1 byte) 
        if(size < 3) {
            printf("Error: Invalid HID packet size\n");
            return -1;
        }
        
        if(!handle){
            printf("Error: Invalid handle passed to write\n");
            return -1;
        }
        
        buf = (*env)->GetByteArrayElements(env, data, NULL);
        totalsize = buf[1];  //actual data size of data sent by user

        memcpy (tempData, buf, totalsize+2);  //add two more bytes to totalsize for reportid and byte count
        
        if(!buf){
            printf("Error: Unable to translate buffer to write\n");
            return -1;
        }
        
        // Extracting/Setting the header 
        reportid = tempData[0];

        trans_data[0] = reportid;

        if (totalsize <= 48)  //since AES-128 requires data to be of 16 byte lengths so the maximum data length
                              //after byte[0]=reportId, byte[1]=AES data byte length (padded + unpadded) and byte[3] =
                              //actual data size sent by user has to be up to 48 bytes and not 64 bytes
		{
            //calculate number of words based on 16 byte increments
			numWords = totalsize / aes128DataSize;
            //bytes not divisible by 16
			bytesLeft = totalsize - (numWords * aes128DataSize);
        }
        //some if not all of user's data is divisible by 16
		if (numWords > 0)
		{
			for (i = 0; i < numWords; i++)
			{
				for (j = 0; j < aes128DataSize; j++)
				{
					//copy only 16 bytes of data into temporary array
                    tempBuf[j] = tempData[bufPtr + j];
				}
                //encrypt the data
				aes_encrypt(tempBuf, key);
				for (j = 0; j < aes128DataSize; j++)
				{
					//write the encrypted data into temporary array (tempData[0] and [1] are not encrypted)
					tempData[bufPtr + j] = tempBuf[j];

				}
				bufPtr +=16;
			}
		}
		if (bytesLeft > 0)
		{
            //calculate the number of bytes that will be padded
			bytesForPad = aes128DataSize - bytesLeft;
            //after padding increase the word count by 1
            numWords = numWords + 1; 
			for (i = 0; i < bytesLeft; i++)
			{
				//copy the extra bytes to temporary array
				tempBuf[i] = tempData[bufPtr + i];

			}
            //pad the rest of the 16 bytes with zero
			for (j = 0; j < bytesForPad; j++)
			{
				tempBuf[bytesLeft + j] = 0;

			}
            //encrypt the data including the zeros
			aes_encrypt(tempBuf, key);
            //copy the encrypted data back to temporary array
			for (i = 0; i < aes128DataSize; i++)
			{
				tempData[bufPtr + i] = tempBuf[i];
			}
		}
        /*
        while(totalsize > 62) {

            trans_data[1] = 62;
            memcpy(trans_data+2,buf+bytes_written+2,62);
            res = hid_write(handle,trans_data,64);

            if(res < 0) {
                printf("Error Writing: %ls\n", hid_error(handle));
                printf("bytes_written: %d totalsize %d handle 0x%x\n",bytes_written,totalsize,handle);
                return res;
            }
            
            bytes_written += 62;
            totalsize -= 62;
        }
        */

        // Corner case where no more bytes exist 
        if(totalsize == 0)
            return bytes_written;


        //encrypted data size plus one more byte that designates the encrypted data size      
        trans_data[1] = (aes128DataSize * numWords) + 1;

        trans_data[2] = totalsize; //actual size of original data without padding

        // Prepare one last 64 byte buffer padded with zeros 
		memcpy(trans_data+3,tempData+bytes_written+2,(aes128DataSize * numWords)); 
        memset(trans_data+3+(aes128DataSize * numWords),0,(61-(aes128DataSize * numWords))); 
   
        res = hid_write(handle,trans_data,64);
        
        if(res < 0) {
            printf("Error Writing: %ls\n", hid_error(handle));
            printf("bytes_written: %d totalsize %d handle 0x%x\n",bytes_written,totalsize,handle);
            return res;
        }
        
        bytes_written += res;
        
        return bytes_written;
            
  }

 
/**
 * Queries the HID port for data to read. If there is data to read, the data is
 * returned in the standard HID packet format (1 byte Report ID, 1 byte size,
 * 62 bytes worth of data. It is up to the Java layer to keep calling this
 * function until there is no more data to be written. Since the HidAPI is set
 * to nonblocking during initialization, if there is no data to be read a 0
 * value is returned from the hid_read api call. A "dummy" two byte packet in
 * this case will be returned to the Java layer to signify that no data has been
 * read.
 */
  JNIEXPORT jbyteArray JNICALL Java_com_ti_msp430_usb_hiddemo_management_HidCommunicationManager_receiveDataNative
  (JNIEnv * env, jobject obj, jint longhandle) {  
  
        hid_device *handle;
        jbyteArray ret;
        int res;
        int i, j; 
        int aes128Len=16;  //AES-128 data array length
		int dataSize=0;
        int paddedDataSize=0;
		int numWords = 0;
		int bytesLeft = 0;
		int bufPtr = 3;  //actual data starts at index 2.  First byte lists report ID (0x3f) and second byte (data length)
        int ptr = 0;
		unsigned char dataBuf[aes128Len];  //temporary array of length 16 used to do decryption
        unsigned char tempBuf[64];  //temporary array
       
        
        /* Opening the device */
        handle = (hid_device*)longhandle;
        
        if(handle == NULL) {
            printf("Error: Opening handle in read\n");
            return NULL;
        }
            
        jbyte buf[64]; 
        buf[0] = 0x3f; 

        res = hid_read(handle,(unsigned char *) buf,64);
 
        if(res < 0) {
            printf("Error: %ls\n", hid_error(handle));
            return NULL;
        }
        
        /* If res is zero, there is nothing to read and we have to signal the host */
        if(res == 0) {
            ret = (*env)->NewByteArray(env,1);
            jbyte fakeBuf[1];
            fakeBuf[0] = 0x0;
            (*env)->SetByteArrayRegion(env,ret,0,1,fakeBuf);
            return ret;
        }
		//the first three bytes of data received from device are report ID, padded data size
        //required for encryption and decryption and actual data size without padding   
        paddedDataSize = buf[1];
        //printf("receive padded dataSize: %d\n", paddedDataSize);

		dataSize = buf[2];  //actual data length
        //printf("receive acutal dataSize: %d\n", dataSize);
        buf[1] = dataSize;  //copy actual data size to byte[1] of buf.  This is required for GUI
                            //to process the data

        numWords = paddedDataSize / aes128Len;  //calculate data words in 16 byte increment
 
        //calculates the number of bytes not divisible by 16
        // bytesLeft = dataSize - (numWords * aes128Len);
        bytesLeft = (numWords * aes128Len) - dataSize;

        //printf("receive bytesLeft: %d\n", bytesLeft);
  
        //decrypt data for each 16  byte word length
		for (i = 0; i < numWords; i++)
		{
            //copies 16 bytes of encrypted data into temporary buffer
			for (j = 0; j < aes128Len; j++)
			{              
				dataBuf[j] = buf[bufPtr + j];
			}
            //decrypt data using given known key
			aes_decrypt(dataBuf, key);

            //copy the decrypted data into temporary array
			for (j = 0; j< aes128Len; j++)
			{
				tempBuf[ptr + j] = dataBuf[j];

			}
            //increment buffer pointer by 16
			bufPtr += 16;
            ptr += 16;
		}
        bufPtr = 2;  //reset pointer to index 2
        //copy the decrypted data into original array
        for (i = 0; i < dataSize; i++)
		{
			buf[bufPtr + i] = tempBuf[i];
		}
        //zero out rest of the 64 byte array
        memset(buf+2+dataSize, 0, 64-(dataSize+2)); 
              
        /* Reading a 64 byte chunk and returning it */
        ret = (*env)->NewByteArray(env,64);
        (*env)->SetByteArrayRegion(env,ret,0,64,buf);  
  
        return ret;
          
  }



//Released_Version_5_20_06_02

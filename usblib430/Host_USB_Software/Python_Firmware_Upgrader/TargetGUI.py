"""
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
"""

"""
TargetGUI provides a graphical front end to python-msp430-tools.  This does not modify anything
in python-msp430-tools.  Because of this, we still create command line parser classes
used by the command line even though they are not used by the GUI.  They are just
required to exist by python-msp430-tools.

Reason for not modifying python-msp430-tools is to allow independent updates of the command
line version without requiring any changes.   

"""
import tkMessageBox
import tkFileDialog
import sys
import os
import struct
import logging
import time
import pkgutil
import ttk
import traceback
import logging
import msp430.target
from cStringIO import StringIO
from Tkinter import *
from msp430 import memory
from logging import Handler
from msp430.bsl5 import bsl5
from msp430.bsl5 import hid_1
#from msp430.bsl5 import hid
if sys.platform == 'win32':
   from pywinusb import hid as HID
if sys.platform == 'darwin':
   import hid


"""
LogerRedirect class redirects an existing stream to the
testArea window.  This way we can detect when an error
occurs with a stack trace in the GUI windows instead
of the console.
"""
class LoggerRedirect(object):

   def __init__(self, logger, textField, log_level=logging.INFO):
      self.logger = logger
      self.log_level = log_level
      self.textField=textField
      self.linebuf = ''
 
   def write(self, buf):
      textField.insert(END, buf,"\n")

# Change the verbosity here, 'DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL'
logging.basicConfig( level=logging.INFO )

class TargetGUI(object):

   # Our access to the command line msp430 flasher utility
   bsl5Target = msp430.bsl5.hid_1.HIDBSL5Target();
   #bsl5Target = msp430.bsl5.hid.HIDBSL5Target();

   """
   Inner class to redirect logging output
   from default output to our text area 

   It does this by taking an instance of TargetGui 
   and then calling TargetGUI's outText in the emit() 
   function.

   """
   class TextAreaHander(Handler):
      global tg
     
      def emit(self,record):
          tg.outText(record.msg+'\n')
      
      def setTG(self,targetGUI):
          global tg
          tg=targetGUI   

           
   def __init__(self):
        # Get the default logger and redirect it to out Handler
        self.logger = logging.getLogger('BSL')

        # logger.propagate = False stops the text from also going to a console 
        self.logger.propagate = False        

        self.myHandler=self.TextAreaHander()
        self.myHandler.setTG(self)
	self.logger.addHandler(self.myHandler)



        
   def doAbout(self):
        """
        Standard about box...
        """
        tkMessageBox.showinfo('About Firmware Updater', 'Example firmware BSL application\n\nTexas Instruments, Copyright 2012')


   def doBlinkLED(self):
        """
        Loads the blinky LED demo
        """
        self.logger.info("In blinkLed...")
        self.doLoad('5529_LED_Blink.txt')

   def doHIDEcho(self):
        """
        Loads the HID echo demo.  This may be used with the 
        HID demo GUI.
        """
        self.doLoad('5529_HID_echo.txt')

   def doCDCEcho(self):
        """
        Loads the CDC echo demo.  This cause windows to detect a new CDC device
        and ask for a driver (if not already installed)
        """
        self.doLoad('5529_CDC_echo.txt')


   def updateBar(self,step=10):
        """
        Updates the progress bar.  The progress bar is setup
        for a 0-100 range and the default increment is
        10
        """
        progressbar.step(step)
        progressbar.update()

   def outText(self,text):
        """
        Writes text to the text area (textField)
        """
        textField.insert(END,text)


   def doLoad(self,filename):
        """
        Load the file from disk and flash the device.
        """
        progressbar["value"]=0
        progressbar.update()
        textField.delete(1.0,END);
        self.bsl5Target.create_option_parser()
        self.bsl5Target.add_extra_options()
        self.bsl5Target.parse_args()
        self.updateBar()
        data = msp430.memory.load(filename, format=self.bsl5Target.options.input_format)
        self.open_connection()
        self.bsl5Target.program_file(data)
        self.updateBar()
        #Trigger a software reset
        self.bsl5Target.reset()
        self.updateBar()
        # output uploaded data
        if self.bsl5Target.upload_data is not None:
            memory.save(self.upload_data, self.output, self.options.output_format)
            self.updateBar()

        if self.bsl5Target.options.do_wait:
            raw_input()

        # Wait for the just programmed device to reset
        time.sleep(1)
        self.updateBar(10)
        self.lookForDevice()

   def open_connection(self):
        """
        Opens the device and download the RAM BSL
        """
        self.updateBar()
        self.bsl5Target.logger = logging.getLogger('BSL')
        self.bsl5Target.open(self.bsl5Target.options.device)
        # only fast mode supported by USB boot loader
        self.bsl5Target.use_fast_mode = True
        self.bsl5Target.buffer_size = 48
  
        self.bsl5Target.logger.info("Mass erase...")
        try:
           #Mass erase initiated due to incorrect password
           self.bsl5Target.BSL_RX_PASSWORD('\xff'*30 + '\0'*2)
           self.updateBar()
        except bsl5.BSL5Error:
            pass # it will fail - that is our intention to trigger the erase
        time.sleep(1)
        self.updateBar()

        # after erase, unlock device
        self.bsl5Target.BSL_RX_PASSWORD('\xff'*32)
        self.updateBar()

        # download full BSL
        sys.stderr.write('Download full BSL...\n')
        bsl_version_expected = (0x00, 0x08, 0x08, 0x39)
              
        basedir = os.path.dirname(__file__)
        if sys.platform == 'win32' or sys.platform == 'cygwin':
                full_bsl_txt = open(os.path.join(basedir, '.\\python-msp430-tools\\msp430\\bsl5\\RAM_BSL_00.08.08.39.txt'), 'rb').read()	    
        else:
                full_bsl_txt = open(os.path.join(basedir, './python-msp430-tools/msp430/bsl5/RAM_BSL_00.08.08.39.txt'), 'rb').read()

        full_bsl = msp430.memory.load('BSL', StringIO(full_bsl_txt), format='titext')
        self.updateBar()
        self.bsl5Target.program_file(full_bsl, quiet=False)
        #loading the start of BSL code so that BSL version and connection check can pass in
        #the if statement below.
        self.bsl5Target.BSL_LOAD_PC(0x2504)  

        # must re-initialize communication, BSL or USB system needs some time
        # to be ready
        self.bsl5Target.logger.info("Waiting for BSL...")
        time.sleep(3)
        self.updateBar()
        self.bsl5Target.close()
        sys.stderr.write('Closed!...\n')

        self.bsl5Target.open(self.bsl5Target.options.device)
        self.updateBar()
        # checking version, this is also a connection check
        # This does not work in Linux. os.read(self.hid_device, 64) will hang forever
        # waiting for a response...
        if sys.platform == 'win32' or sys.platform == 'cygwin':
                bsl_version = self.bsl5Target.BSL_VERSION()
                if bsl_version_expected !=  bsl_version_expected:
                        self.bsl5Target.logger.error("BSL version mismatch (continuing anyway)")
                else:
                        self.bsl5Target.logger.debug("BSL version OK")


   def selectFile(self):
       """
       Filters on .txt files and returns a file name.
       """
       self.file_opt = options = {}
       options['defaultextension'] = '.txt'
       options['filetypes'] = [('text files', '.txt'),('all files', '.*')]
       options['parent'] = root
       options['title'] = 'Choose a file'
       file = tkFileDialog.askopenfilename(**self.file_opt)
       if file != '':
          self.doLoad(file)
          

   def lookForDevice(self, showMessage='No'):
       """
       Looks for a device in BSL mode and if found sets the
       load menu options to enabled.  If not sets them to disabled.
       An optional showMesage is provide if we want to display a consle 
       (text area) message.
       """
       global submenu
       global file

       all_devices = []
       if sys.platform == 'win32' or sys.platform == 'cygwin':
          filter = HID.HidDeviceFilter(vendor_id = 0x2047, product_id = 0x0200)
          all_devices = filter.get_devices()
       elif sys.platform == 'darwin':
          all_devices = hid.enumerate(0x2047, 0x200)
       else:
          import glob
          device='None'
          for path in glob.glob('/sys/class/hidraw/hidraw*'):
              try:
                 #~ self.logger.debug('trying %r' % (path,))
                 for line in open(os.path.join(path, 'device/uevent')):
                     if 'HID 2047:0200' in line:
                          device = os.path.join('/dev', os.path.basename(path))
                          all_devices.append(device)
                          break
              except IOError:
                 pass # file could not be opened
         
       if all_devices.__len__() > 0:
           submenu.entryconfig(0,state='normal')
           submenu.entryconfig(1,state='normal')
           submenu.entryconfig(2,state='normal')
           file.entryconfig(1,state='normal')
           file.entryconfig(2,state='normal')
           if showMessage == 'Yes':
               textField.delete(1.0,END);
               self.logger.info("ready...")
       else:
           submenu.entryconfig(0,state='disabled')
           submenu.entryconfig(1,state='disabled')
           submenu.entryconfig(2,state='disabled')
           file.entryconfig(2,state='normal')
           if showMessage == 'Yes':
               textField.delete(1.0,END);
               self.logger.info("USB VID:PID 2047:0200 not found (maybe device not in BSL mode?)")
               self.logger.info("Please set device in BSL mode and select File->Rescan HID Bus...")





   def makeGUI(self,win):
       """
       Created the main tkinter GUI for the Firmware updater.
       """
       global submenu
       global file
       global progressbar 
       global textField

       # Make the root frame
       top = Menu(win)                                # win=top-level window
       win.config(menu=top)                           # set its menu option
       file = Menu(top)

       # Create a progress bar and a textArea for the display
       progressbar= ttk.Progressbar(orient=HORIZONTAL, length=200, maximum='100', mode='determinate')
       progressbar.pack(side="top")
       textField=Text(win,height=20,width=132,background='white', foreground='black', borderwidth='5',relief='sunken')

       # put a scroll bar in the frame
       scroll=Scrollbar(win)
       scroll.pack(side=RIGHT,fill=Y)
       textField.pack(side="top", fill='both', expand = YES)
       scroll.config(command=textField.yview)
       textField.config(yscrollcommand=scroll.set)

       # Redirect stderr in case any exceptions occur
       stderr_logger = logging.getLogger('STDERR')
       sl = LoggerRedirect(stderr_logger, logging.ERROR,textField)
       sys.stderr = sl

       # Make the menu
       submenu = Menu(top, tearoff=False)
       submenu.add_command(label='Blink LED Example', state='disabled', command=self.doBlinkLED, underline=0)
       submenu.add_command(label='HID Echo Example', state='disabled', command=self.doHIDEcho,  underline=0)
       submenu.add_command(label='CDC Echo Example', state='disabled', command=self.doCDCEcho,  underline=0)
       file.add_cascade(label='Load Demo Firmware',   menu=submenu,     underline=0)
       file.add_command(label='Open User Firmware...', state='disabled', command=self.selectFile,  underline=0)
       file.add_command(label='Rescan HID Bus...', command=lambda: self.lookForDevice('Yes'),  underline=0)
       file.add_command(label='Quit',    command=win.quit, underline=0)
       top.add_cascade(label='File',     menu=file,        underline=0)
   
       about = Menu(top, tearoff=False)
       about.add_command(label='Firmware Updater',     command=self.doAbout,  underline=0)
       top.add_cascade(label='About',     menu=about,        underline=0)


          
 
  
   # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

   def main(self):
        global root
        root = Tk() # root window
        if sys.platform == 'win32' or sys.platform == 'cygwin':
           root.iconbitmap('./TI_Bug_Icon_Red.ico') 
        if sys.platform == 'darwin':
           root.iconbitmap('.\TI_Bug_Icon_Red.icns')
     
        root.title('MSP430 USB Firmware Upgrade Example 3.1')   # set window-mgr info
        self.makeGUI(root)                                     # associate a menu bar
        self.lookForDevice('Yes')
        root.mainloop()

#  Startup...

if __name__ == '__main__':
   t = TargetGUI()
   t.main();

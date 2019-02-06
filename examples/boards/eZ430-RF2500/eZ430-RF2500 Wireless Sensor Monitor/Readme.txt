===========================================
ez430-RF2500 Wireless Sensor Monitor v1.0.6
===========================================

Embedded   - Contains code for embedded (MSP430) side
|-- CCS    - Source code for CCSv4 compiler
|-- IAR    - Source code for IAR compiler
PC         - Contains Windows GUI
Benchmark  - Information about current benchmarking

Notes for CCS
=============
The file macros.ini in Embedded/CCS/eZ430-RF2500_WSM is used to solve portability issues.
When you import the project into CCS, macros.ini automatically adds a macro 
defining the project's root directory. For more details on how macros.ini works
with CCS, see the following TI wiki page:
  
  http://processors.wiki.ti.com/index.php/Portable_Projects_in_CCSv4_for_C2000

Previous versions required the eZ430_WSM_ROOT to be defined manually. This is 
no longer necessary.

Steps to successfully build the CCS project: 

1) Launch CCS and open a workspace

2) Once created, the eZ430-RF2500_WSM project should be imported into the workspace:
    a. Select "Project > Import Existing CCS/CCE Eclipse Project" 
    b. Navigate to your workspace and click OK
    c. Make sure the eZ430-RF2500_WSM project is selected and click Finish

If an older revision of the project (<1.04) is present on your system, 
CCS may warn you that the project being imported will replaced old macros of the
same name. You should only work with one revision in your workspace at a time; 
confirm the replacement and delete the old revision from your workspace.

Notes for Full version of IAR (Does not apply to Kickstart version)
===================================================================
1. Open IAR Project workspace
2. Click on Project --> Options --> Linker --> Extra Options
3. Uncheck the box "Use command line options"

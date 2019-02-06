#ifndef BUILD_H_
#define BUILD_H_

#ifndef BUILDLEVEL 

/*------------------------------------------------------------------------------
Following is the list of the Build Level choices.
------------------------------------------------------------------------------*/
#define LEVEL1  1      		// 
#define LEVEL2  2           // 
#define LEVEL3  3           // 
#define LEVEL4  4           //
#define LEVEL5  5           // 
#define LEVEL6  6           // 

/*------------------------------------------------------------------------------
This line sets the BUILDLEVEL to one of the available choices.
------------------------------------------------------------------------------*/
#define   BUILDLEVEL LEVEL1


#ifndef BUILDLEVEL    
#error  Critical: BUILDLEVEL must be defined !!
#endif  // BUILDLEVEL

#endif  // BUILDLEVEL



#endif /*BUILD_H_*/

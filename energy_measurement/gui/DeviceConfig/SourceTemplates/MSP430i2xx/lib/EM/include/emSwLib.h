/* --COPYRIGHT--,TI
 *MSP Source and Object Code Software License Agreement
 *
 *
 **IMPORTANT - PLEASE CAREFULLY READ THE FOLLOWING LICENSE AGREEMENT, WHICH IS LEGALLY BINDING.  AFTER YOU READ IT, YOU WILL BE ASKED WHETHER YOU ACCEPT AND AGREE TO ITS TERMS.  DO NOT CLICK  "I ACCEPT" UNLESS: (1) YOU WILL USE THE LICENSED MATERIALS FOR YOUR OWN BENEFIT AND PERSONALLY ACCEPT, AGREE TO AND INTEND TO BE BOUND BY THESE TERMS; OR (2) YOU ARE AUTHORIZED TO, AND INTEND TO BE BOUND BY, THESE TERMS ON BEHALF OF YOUR COMPANY.
 *
 *
 **Important - Read carefully: This Source and Object Code Software License Agreement ("Agreement") is a legal agreement between you and Texas Instruments Incorporated ("TI").  In this Agreement "you" means you personally if you will exercise the rights granted for your own benefit, but it means your company (or you on behalf of your company) if you will exercise the rights granted for your company's benefit.  The "Licensed Materials" subject to this Agreement include the software programs and any associated electronic documentation (in each case, in whole or in part) that accompany this Agreement, are set forth in the applicable software manifest and you access "on-line", as well as any updates or upgrades to such software programs or documentation, if any, provided to you at TI's sole discretion.  The Licensed Materials are specifically designed and licensed for use solely and exclusively with MSP microcontroller devices manufactured by or for TI ("TI Devices").  By installing, copying or otherwise using the Licensed Materials you agree to abide by the provisions set forth herein.  This Agreement is displayed for you to read prior to using the Licensed Materials.  If you choose not to accept or agree with these provisions, do not download or install the Licensed Materials.
 *
 **Note Regarding Possible Access to Other Licensed Materials:  The Licensed Materials may be bundled with software and associated electronic documentation, if any, licensed under terms other than the terms of this Agreement (in whole or in part, "Other Licensed Materials"), including, for example Open Source Software and/or TI-owned or third party Proprietary Software licensed under such other terms.  "Open Source Software" means any software licensed under terms requiring that (A) other software ("Proprietary Software") incorporated, combined or distributed with such software or developed using such software: (i) be disclosed or distributed in source code form; or (ii) otherwise be licensed on terms inconsistent with the terms of this Agreement, including but not limited to permitting use of the Proprietary Software on or with devices other than TI Devices, or (B) require the owner of Proprietary Software to license any of its patents to users of the Open Source Software and/or Proprietary Software incorporated, combined or distributed with such Open Source Software or developed using such Open Source Software.
 *
 **If by accepting this Agreement, you gain access to Other Licensed Materials, they will be listed in the applicable software manifest.  Your use of the Other Licensed Materials is subject to the applicable other licensing terms acknowledgements and disclaimers as specified in the applicable software manifest and/or identified or included with the Other Licensed Materials in the software bundle.  For clarification, this Agreement does not limit your rights under, or grant you rights that supersede, the terms of any applicable Other Licensed Materials license agreement.  If any of the Other Licensed Materials is Open Source Software that has been provided to you in object code only under terms that obligate TI to provide to you or show you where you can access the source code versions of such Open Source Software, TI will provide to you, or show you where you can access, such source code if you contact TI at Texas Instruments Incorporated, 12500 TI Boulevard, Mail Station 8638, Dallas, Texas 75243, Attention: Contracts Manager, Embedded Processing.  In the event you choose not to accept or agree with the terms in any applicable Other Licensed Materials license agreement, you must terminate this Agreement.
 *
 **1.    License Grant and Use Restrictions.
 *
 **a.    Licensed Materials License Grant.  Subject to the terms of this Agreement, TI hereby grants to you a limited, non-transferable, non-exclusive, non-assignable, non-sublicensable, fully paid-up and royalty-free license to:
 *
 *            i.    Limited Source Code License:  make copies, prepare derivative works, display internally and use internally the Licensed Materials provided to you in source code for the sole purpose of developing object and executable versions of such Licensed Materials, or any derivative thereof, that execute solely and exclusively on TI Devices, for end use in Licensee Products, and maintaining and supporting such Licensed Materials, or any derivative thereof, and Licensee Products.  For purposes of this Agreement, "Licensee Product" means a product that consists of both hardware, including one or more TI Devices, and software components, including only executable versions of the Licensed Materials that execute solely and exclusively on such TI Devices.
 *
 *            ii.    Object Code Evaluation, Testing and Use License:  make copies, display internally, distribute internally and use internally the Licensed Materials in object code for the sole purposes of evaluating and testing the Licensed Materials and designing and developing Licensee Products, and maintaining and supporting the Licensee Products;
 *
 *            iii.    Demonstration License:  demonstrate to third parties the Licensed Materials executing solely and exclusively on TI Devices as they are used in Licensee Products, provided that such Licensed Materials are demonstrated in object or executable versions only and
 *
 *        iv.    Production and Distribution License:  make, use, import, export and otherwise distribute the Licensed Materials as part of a Licensee Product, provided that such Licensee Products include only embedded executable copies of such Licensed Materials that execute solely and exclusively on TI Devices.
 *
 *    b.    Contractors.  The licenses granted to you hereunder shall include your on-site and off-site contractors (either an individual or entity), while such contractors are performing work for or providing services to you, provided that such contractors have executed work-for-hire agreements with you containing applicable terms and conditions consistent with the terms and conditions set forth in this Agreement and provided further that you shall be liable to TI for any breach by your contractors of this Agreement to the same extent as you would be if you had breached the Agreement yourself.
 *
 *    c.    No Other License.  Nothing in this Agreement shall be construed as a license to any intellectual property rights of TI other than those rights embodied in the Licensed Materials provided to you by TI.  EXCEPT AS PROVIDED HEREIN, NO OTHER LICENSE, EXPRESS OR IMPLIED, BY ESTOPPEL OR OTHERWISE, TO ANY OTHER TI INTELLECTUAL PROPERTY RIGHTS IS GRANTED HEREIN.
 *
 *    d.    Covenant not to Sue.  During the term of this Agreement, you agree not to assert a claim against TI or its licensees that the Licensed Materials infringe your intellectual property rights.
 *
 *    e.    Restrictions.  You shall maintain the source code versions of the Licensed Materials under password control protection and shall not disclose such source code versions of the Licensed Materials, to any person other than your employees and contractors whose job performance requires access.  You shall not use the Licensed Materials with a processing device other than a TI Device, and you agree that any such unauthorized use of the Licensed Materials is a material breach of this Agreement.  You shall not use the Licensed Materials for the purpose of analyzing or proving infringement of any of your patents by either TI or TI's customers.  Except as expressly provided in this Agreement, you shall not copy, publish, disclose, display, provide, transfer or make available the Licensed Materials to any third party and you shall not sublicense, transfer, or assign the Licensed Materials or your rights under this Agreement to any third party.  You shall not mortgage, pledge or encumber the Licensed Materials in any way.  You may use the Licensed Materials with Open Source Software or with software developed using Open Source Software tools provided you do not incorporate, combine or distribute the Licensed Materials in a manner that subjects the Licensed Materials to any license obligations or any other intellectual property related terms of any license governing such Open Source Software.
 *
 *    f.    Termination.  This Agreement is effective on the date the Licensed Materials are delivered to you together with this Agreement and will remain in full force and effect until terminated.  You may terminate this Agreement at any time by written notice to TI.  Without prejudice to any other rights, if you fail to comply with the terms of this Agreement or you are acquired, TI may terminate your right to use the Licensed Materials upon written notice to you.  Upon termination of this Agreement, you will destroy any and all copies of the Licensed Materials in your possession, custody or control and provide to TI a written statement signed by your authorized representative certifying such destruction. Except for Sections 1(a), 1(b) and 1(d), all provisions of this Agreement shall survive termination of this Agreement.
 *
 **2.    Licensed Materials Ownership.  The Licensed Materials are licensed, not sold to you, and can only be used in accordance with the terms of this Agreement.  Subject to the licenses granted to you pursuant to this Agreement, TI and its licensors own and shall continue to own all right, title and interest in and to the Licensed Materials, including all copies thereof.  You agree that all fixes, modifications and improvements to the Licensed Materials conceived of or made by TI that are based, either in whole or in part, on your feedback, suggestions or recommendations are the exclusive property of TI and all right, title and interest in and to such fixes, modifications or improvements to the Licensed Materials will vest solely in TI.  Moreover, you acknowledge and agree that when your independently developed software or hardware components are combined, in whole or in part, with the Licensed Materials, your right to use the combined work that includes the Licensed Materials remains subject to the terms and conditions of this Agreement.
 *
 **3.    Intellectual Property Rights.
 *
 *    a.    The Licensed Materials contain copyrighted material, trade secrets and other proprietary information of TI and its licensors and are protected by copyright laws, international copyright treaties, and trade secret laws, as well as other intellectual property laws.  To protect TI's and its licensors' rights in the Licensed Materials, you agree, except as specifically permitted by statute by a provision that cannot be waived by contract, not to "unlock", decompile, reverse engineer, disassemble or otherwise translate to a human-perceivable form any portions of the Licensed Materials provided to you in object code format only, nor permit any person or entity to do so.  You shall not remove, alter, cover, or obscure any confidentiality, trade secret, trade mark, patent, copyright or other proprietary notice or other identifying marks or designs from any component of the Licensed Materials and you shall reproduce and include in all copies of the Licensed Materials the copyright notice(s) and proprietary legend(s) of TI and its licensors as they appear in the Licensed Materials.  TI reserves all rights not specifically granted under this Agreement.
 *
 *    b.    Certain Licensed Materials may be based on industry recognized standards or software programs published by industry recognized standards bodies and certain third parties may claim to own patents, copyrights, and other intellectual property rights that cover implementation of those standards.  You acknowledge and agree that this Agreement does not convey a license to any such third party patents, copyrights, and other intellectual property rights and that you are solely responsible for any patent, copyright, or other intellectual property right claim that relates to your use or distribution of the Licensed Materials or your use or distribution of your products that include or incorporate the Licensed Materials.  Moreover, you acknowledge that you are responsible for any fees or royalties that may be payable to any third party based on such third party's interests in the Licensed Materials or any intellectual property rights that cover implementation of any industry recognized standard, any software program published by any industry recognized standards bodies or any other proprietary technology.
 *
 **4.    Confidential Information.  You acknowledge and agree that the Licensed Materials contain trade secrets and other confidential information of TI and its licensors.  You agree to use the Licensed Materials solely within the scope of the licenses set forth herein, to maintain the Licensed Materials in strict confidence, to use at least the same procedures and degree of care that you use to prevent disclosure of your own confidential information of like importance but in no instance less than reasonable care, and to prevent disclosure of the Licensed Materials to any third party, except as may be necessary and required in connection with your rights and obligations hereunder; provided, however, that you may not provide the Licensed Materials to any business organization or group within your company or to customers or contractors that design or manufacture semiconductors unless TI gives written consent.  You agree to obtain executed confidentiality agreements with your employees and contractors having access to the Licensed Materials and to diligently take steps to enforce such agreements in this respect.  TI may disclose your contact information to TI's licensors.
 *
 **5.    Warranties and Limitations.  THE LICENSED MATERIALS ARE PROVIDED "AS IS".  FURTHERMORE, YOU ACKNOWLEDGE AND AGREE THAT THE LICENSED MATERIALS HAVE NOT BEEN TESTED OR CERTIFIED BY ANY GOVERNMENT AGENCY OR INDUSTRY REGULATORY ORGANIZATION OR ANY OTHER THIRD PARTY ORGANIZATION.  YOU AGREE THAT PRIOR TO USING, INCORPORATING OR DISTRIBUTING THE LICENSED MATERIALS IN OR WITH ANY COMMERCIAL PRODUCT THAT YOU WILL THOROUGHLY TEST THE PRODUCT AND THE FUNCTIONALITY OF THE LICENSED MATERIALS IN OR WITH THAT PRODUCT AND BE SOLELY RESPONSIBLE FOR ANY PROBLEMS OR FAILURES.
 *
 **TI AND ITS LICENSORS MAKE NO WARRANTY OR REPRESENTATION, EITHER EXPRESS, IMPLIED OR STATUTORY, REGARDING THE LICENSED MATERIALS, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT OF ANY THIRD PARTY PATENTS, COPYRIGHTS, TRADE SECRETS OR OTHER INTELLECTUAL PROPERTY RIGHTS.  YOU AGREE TO USE YOUR INDEPENDENT JUDGMENT IN DEVELOPING YOUR PRODUCTS.  NOTHING CONTAINED IN THIS AGREEMENT WILL BE CONSTRUED AS A WARRANTY OR REPRESENTATION BY TI TO MAINTAIN PRODUCTION OF ANY TI SEMICONDUCTOR DEVICE OR OTHER HARDWARE OR SOFTWARE WITH WHICH THE LICENSED MATERIALS MAY BE USED.
 *
 **IN NO EVENT SHALL TI OR ITS LICENSORS, BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL, PUNITIVE OR CONSEQUENTIAL DAMAGES, HOWEVER CAUSED, ON ANY THEORY OF LIABILITY, IN CONNECTION WITH OR ARISING OUT OF THIS AGREEMENT OR THE USE OF THE LICENSED MATERIALS REGARDLESS OF WHETHER TI HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF REMOVAL OR REINSTALLATION, OUTSIDE COMPUTER TIME, LABOR COSTS, LOSS OF DATA, LOSS OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF USE OR INTERRUPTION OF BUSINESS.  IN NO EVENT WILL TI'S OR ITS LICENSORS' AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF YOUR USE OF THE LICENSED MATERIALS EXCEED FIVE HUNDRED U.S. DOLLARS (US$500).
 *
 *    Because some jurisdictions do not allow the exclusion or limitation of incidental or consequential damages or limitation on how long an implied warranty lasts, the above limitations or exclusions may not apply to you.
 *
 **6.    Indemnification Disclaimer.  YOU ACKNOWLEDGE AND AGREE THAT TI SHALL NOT BE LIABLE FOR AND SHALL NOT DEFEND OR INDEMNIFY YOU AGAINST ANY THIRD PARTY INFRINGEMENT CLAIM THAT RELATES TO OR IS BASED ON YOUR MANUFACTURE, USE, OR DISTRIBUTION OF THE LICENSED MATERIALS OR YOUR MANUFACTURE, USE, OFFER FOR SALE, SALE, IMPORTATION OR DISTRIBUTION OF YOUR PRODUCTS THAT INCLUDE OR INCORPORATE THE LICENSED MATERIALS.
 *
 **7.    No Technical Support.  TI and its licensors are under no obligation to install, maintain or support the Licensed Materials.
 *
 **8.    Notices.  All notices to TI hereunder shall be delivered to Texas Instruments Incorporated, 12500 TI Boulevard, Mail Station 8638, Dallas, Texas 75243, Attention: Contracts Manager - Embedded Processing, with a copy to Texas Instruments Incorporated, 13588 N. Central Expressway, Mail Station 3999, Dallas, Texas 75243, Attention: Law Department - Embedded Processing.  All notices shall be deemed served when received by TI.
 *
 **9.    Export Control.  The Licensed Materials are subject to export control under the U.S. Commerce Department's Export Administration Regulations ("EAR").  Unless prior authorization is obtained from the U.S. Commerce Department, neither you nor your subsidiaries shall export, re-export, or release, directly or indirectly (including, without limitation, by permitting the Licensed Materials to be downloaded), any technology, software, or software source code, received from TI, or export, directly or indirectly, any direct product of such technology, software, or software source code, to any person, destination or country to which the export, re-export, or release of the technology, software, or software source code, or direct product is prohibited by the EAR.  You represent and warrant that you (i) are not located in, or under the control of, a national or resident of Cuba, Iran, North Korea, Sudan and Syria or any other country subject to a U.S. goods embargo; (ii) are not on the U.S. Treasury Department's List of Specially Designated Nationals or the U.S. Commerce Department's Denied Persons List or Entity List; and (iii) will not use the Licensed Materials or transfer the Licensed Materials for use in any military, nuclear, chemical or biological weapons, or missile technology end-uses.  Any software export classification made by TI shall not be construed as a representation or warranty regarding the proper export classification for such software or whether an export license or other documentation is required for the exportation of such software.
 *
 **10.    Governing Law and Severability; Waiver.  This Agreement will be governed by and interpreted in accordance with the laws of the State of Texas, without reference to conflict of laws principles.  If for any reason a court of competent jurisdiction finds any provision of the Agreement to be unenforceable, that provision will be enforced to the maximum extent possible to effectuate the intent of the parties, and the remainder of the Agreement shall continue in full force and effect.  This Agreement shall not be governed by the United Nations Convention on Contracts for the International Sale of Goods, or by the Uniform Computer Information Transactions Act (UCITA).  The parties agree that non-exclusive jurisdiction for any dispute arising out of or relating to this Agreement lies within the courts located in the State of Texas.  Notwithstanding the foregoing, any judgment may be enforced in any United States or foreign court, and either party may seek injunctive relief in any United States or foreign court.  Failure by TI to enforce any provision of this Agreement shall not be deemed a waiver of future enforcement of that or any other provision in this Agreement or any other agreement that may be in place between the parties.
 *
 **11.    PRC Provisions.  If you are located in the People's Republic of China ("PRC") or if the Licensed Materials will be sent to the PRC, the following provisions shall apply:
 *
 *    a.    Registration Requirements.  You shall be solely responsible for performing all acts and obtaining all approvals that may be required in connection with this Agreement by the government of the PRC, including but not limited to registering pursuant to, and otherwise complying with, the PRC Measures on the Administration of Software Products, Management Regulations on Technology Import-Export, and Technology Import and Export Contract Registration Management Rules.  Upon receipt of such approvals from the government authorities, you shall forward evidence of all such approvals to TI for its records.  In the event that you fail to obtain any such approval or registration, you shall be solely responsible for any and all losses, damages or costs resulting therefrom, and shall indemnify TI for all such losses, damages or costs.
 *
 **b.    Governing Language.  This Agreement is written and executed in the English language and shall be authoritative and controlling, whether or not translated into a language other than English to comply with law or for reference purposes.  If a translation of this Agreement is required for any purpose, including but not limited to registration of the Agreement pursuant to any governmental laws, regulations or rules, you shall be solely responsible for creating such translation.
 *
 **12.    Contingencies.    TI shall not be in breach of this Agreement and shall not be liable for any non-performance or delay in performance if such non-performance or delay is due to a force majeure event or other circumstances beyond TI's reasonable control.
 *
 **13.        Entire Agreement.  This is the entire agreement between you and TI and this Agreement supersedes any prior agreement between the parties related to the subject matter of this Agreement.  Notwithstanding the foregoing, any signed and effective software license agreement relating to the subject matter hereof and stating expressly that such agreement shall control regardless of any subsequent click-wrap, shrink-wrap or web-wrap, shall supersede the terms of this Agreement.  No amendment or modification of this Agreement will be effective unless in writing and signed by a duly authorized representative of TI.  You hereby warrant and represent that you have obtained all authorizations and other applicable consents required empowering you to enter into this Agreement.
 *
 *
 *
 * --/COPYRIGHT--*/

#ifndef _EMSWLIB_H_
#define _EMSWLIB_H_

//#############################################################################
//
//! \file   emSwLib.h
//!
//! \brief  Contains all Energy Measurement SW Library enums, structs, macros, 
//!            function and global variables defintions
//!         
//
//  Group:          MSP
//  Target Device:  MSP430F677x1A, MSP430F673xA and MSP430i20xx 
//
//  (C) Copyright 2017, Texas Instruments, Inc.
//#############################################################################
// TI Release: MSP430_ENERGY_MEASUREMENT 
// Release Date: 10Jul2018
//#############################################################################


//*****************************************************************************
// includes
//*****************************************************************************
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>
#include "IQmathLib.h"
#include "QmathLib.h"

#ifdef __cplusplus

extern "C" {
#endif

//*****************************************************************************
// defines
//*****************************************************************************

 //! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
 //! and determine if the VRMS is calculated by the EM_processSample().
 #define EM_PHASE_RESULT_CALC_VRMS                          BIT0
 //! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
 //! and determine if the IRMS is calculated by the EM_processSample().
 #define EM_PHASE_RESULT_CALC_IRMS                          BIT1
 //! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
 //! and determine if the V-PEAK is calculated by the EM_processSample().
 #define EM_PHASE_RESULT_CALC_V_PEAK                        BIT2
 //! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
 //! and determine if the I-PEAK is calculated by the EM_processSample().
 #define EM_PHASE_RESULT_CALC_I_PEAK                        BIT3
 //! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
 //! and determine if the POWER FACTOR is calculated by the EM_processSample().
 #define EM_PHASE_RESULT_CALC_POWER_FACTOR                  BIT4
 //! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
 //! and determine if the Frequency is calculated by the EM_processSample().
 #define EM_PHASE_RESULT_CALC_FREQ                          BIT5
//! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
//! and determine if the ACTIVE POWER is calculated by the EM_processSample(). 
#define EM_PHASE_RESULT_CALC_ACTIVE_POWER					BIT6
//! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
//! and determine if the REACTIVE POWER is calculated by the EM_processSample(). 
#define EM_PHASE_RESULT_CALC_REACTIVE_POWER					BIT7
//! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
//! and determine if the APPARENT POWER is calculated by the EM_processSample(). 
#define EM_PHASE_RESULT_CALC_APPARENT_POWER					BIT8
//! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
//! and determine if the ACTIVE ENERGY is calculated by the EM_processSample(). 
#define EM_PHASE_RESULT_CALC_ACTIVE_ENERGY					BIT9
//! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
//! and determine if the REACTIVE ENERGY is calculated by the EM_processSample(). 
#define EM_PHASE_RESULT_CALC_REACTIVE_ENERGY				BIT10
//! \brief This define can be used to mask the EM_Phase->phaseResultsSupported variable
//! and determine if the APPARENT ENERGY is calculated by the EM_processSample(). 
#define EM_PHASE_RESULT_CALC_APPARENT_ENERGY				BIT11

 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase A
#define EM_PULSE_GEN_PHASE_NONE                             (0x00)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase A
#define EM_PULSE_GEN_PHASE_A                                (0x01)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase B
#define EM_PULSE_GEN_PHASE_B                                (0x02)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase C
#define EM_PULSE_GEN_PHASE_C                                (0x04)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase D
#define EM_PULSE_GEN_PHASE_D                                (0x08)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase E
#define EM_PULSE_GEN_PHASE_E                                (0x10)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase F
#define EM_PULSE_GEN_PHASE_F                                (0x20)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseIndPhase() to generate pulses for Phase
 //! neutral.
#define EM_PULSE_GEN_PHASE_NEUTRAL                          (0x40)
 //! \brief This define can be used to mask the EM_System_Configuration->pulseGeneration
 //! variable . This is used by the EM_genPulseTotalPhases() to generate pulses for the
 //! aggregate total.
#define EM_PULSE_GEN_PHASE_AGGREGATE                        (0x80)

//*****************************************************************************
// typedefs
//*****************************************************************************

//! \brief The following enumeration contain all possible EM SW Library return
//!         message codes.
//!
typedef enum _EM_message_code_
{

	EM_message_code_no_error=0,
	//!< This message code is returned when no error was detected while 
	//!< invoking the API.

	// 1-20 reserved for system configuration messages
    EM_message_code_pulseArray_error=1,
    //!< This error is returned if pulseArrayLength is greater than 0
    //!< and pulseArray is 0
	EM_message_code_indPhaseMeter_error=2,
    //!< This error is returned if pulseArrayLength is greater than 0
	//!< and indPhaseMeterConstant is 0
	EM_message_code_indPhasePulseDuration_error=3,
    //!< This error is returned if pulseArrayLength is greater than 0
    //!< and indPhasePulseDuration is 0
    EM_message_code_totalPhaseMeter_error=4,
    //!< This error is returned if pulseArrayLength is greater than 0,
    //!< totalPhaseMeterConstant is 0 and the pulseGeneration has
    //!< the EM_PULSE_GEN_PHASE_AGGREGATE flag set
    EM_message_code_totalPhasePulseDuration_error=5,
    //!< This error is returned if pulseArrayLength is greater than 0
    //!< and totalPhasePulseDuration is 0 and the pulseGeneration has
    //!< the EM_PULSE_GEN_PHASE_AGGREGATE flag set

	// 21-40 reserved for algorithm configuration messages
    EM_message_code_samplingFrequency_error=21,
    //!< This error is returned if the sampling frequency is not greater than 0
    EM_message_code_reactPowLength_error=22,
    //!< This error is returned if the reactPowIIRTable ptr is null and
    //!< the reactPowIIRLength variable is greater than 0
    EM_message_code_reactPowIIRTable_error=23,
    //!< This error is returned if the reactPowIIRLength is greater than 0
    //!< and the reactPowIIRTable ptr is equal to 0
	
	// 41-59 reserved for meter configuration messages
    EM_message_code_phaseLength_error=41,
    //!< This error is returned if the phaseLength is equal to 0
    EM_message_code_phaseConfig_error=41,
    //!< This error is returned if the phaseConfigPtr is equal to 0

	// 60-249 reserved for calibration messages
	EM_message_code_calibrate_invalidPhase_error=81,
	//!< This error is returned if the phase number is invalid

	//240-255 error
	EM_message_code_error_occurred=255
	//!< This is a generic error this is error that is returned if an unhandled
	//!< error occurs
} EM_message_code;

//! \brief Selects appropriate EM Sensor for the corresponding channel
//!
typedef enum _EM_Current_Sensor_
{
	EM_Phase_Sensor_CT=0,
	//!< Current Transformer (CT) 
	EM_Phase_Sensor_Shunt=1,
	//!< Shunt
    EM_Phase_Sensor_RC=2,
    //!< Rogowski Coil (RC)
} EM_Current_Sensor;

//! \brief Type for Power variables
//!
typedef int64_t EM_Power_t;

//! \brief Type for Energy variables
//!
typedef uint64_t EM_Energy_t;

//! \brief Selects appropiate phase to provide results for
//!
typedef enum _EM_Phase_
{
	EM_Phase_A=0x01,
	//!< - Results requested for phase A
	EM_Phase_B=0x02,
	//!< - Results requested for phase B
	EM_Phase_C=0x04,
	//!< - Results requested for phase C
	EM_Phase_D=0x08,
	//!< - Results requested for phase D
	EM_Phase_E=0x10,
	//!< - Results requested for phase E
	EM_Phase_F=0x20,
	//!< - Results requested for phase F
	EM_Phase_Neutral=0x40
	//!< - Results requested for phase Neutral
} EM_Phase;


//! \brief Contains EM SW Lib Phase Calibration
//!
typedef struct _EM_Pulse_Settings_
{
	uint8_t activePulseGpioPort;
	//!< Each phase uses a GPIO port used to generate a pulse used for calibration
	//!< Possible options for each port:
	//!< - GPIO_PORT_P1
	//!< - GPIO_PORT_P2
	//!< - GPIO_PORT_P3
	//!< - GPIO_PORT_P4
	//!< - GPIO_PORT_P5
	//!< - GPIO_PORT_P6
	//!< - GPIO_PORT_P7
	//!< - GPIO_PORT_P8
	//!< - GPIO_PORT_P9
	uint8_t reactivePulseGpioPort;
	//!< Each phase uses a GPIO port used to generate a pulse used for calibration
	//!< Possible options for each port:
	//!< - GPIO_PORT_P1
	//!< - GPIO_PORT_P2
	//!< - GPIO_PORT_P3
	//!< - GPIO_PORT_P4
	//!< - GPIO_PORT_P5
	//!< - GPIO_PORT_P6
	//!< - GPIO_PORT_P7
	//!< - GPIO_PORT_P8
	//!< - GPIO_PORT_P9
	uint16_t activePulseGpioPin;
	//!< Each phase uses a GPIO pin used to generate a pulse used for calibration
	//!< Possible options for each pin:
	//!< - GPIO_PIN0
	//!< - GPIO_PIN1
	//!< - GPIO_PIN2
	//!< - GPIO_PIN3
	//!< - GPIO_PIN4
	//!< - GPIO_PIN5
	//!< - GPIO_PIN6
	//!< - GPIO_PIN7
	uint16_t reactivePulseGpioPin;
	//!< Each phase uses a GPIO pin used to generate a pulse used for calibration
	//!< Possible options for each pin:
	//!< - GPIO_PIN0
	//!< - GPIO_PIN1
	//!< - GPIO_PIN2
	//!< - GPIO_PIN3
	//!< - GPIO_PIN4
	//!< - GPIO_PIN5
	//!< - GPIO_PIN6
	//!< - GPIO_PIN7
    uint64_t activeEnergyCounter;
    //!< Ongoing Active Energy Sum
    uint16_t activeEnergyPulseRemaining;
    //!< Counter used to keep track of remaining ADC samples to keep pulse high
	uint64_t reactiveEnergyCounter;
	//!< Ongoing Reactive Energy Sum
    uint16_t reactiveEnergyPulseRemaining;
    //!< Counter used to keep track of remaining ADC samples to keep pulse high
} EM_Pulse_Settings;

//! \brief Contains EM SW Lib System Configuration
//!
typedef struct _EM_System_Configuration_
{
	EM_Pulse_Settings *pulseArray;
	//!< Pointer to the location where the pulse configuration is stored 
	//!< (one for each phase)
	uint8_t pulseArrayLength;
	//!< Length of the pulseArray configuration
	uint64_t indPhaseMeterConstant;
	//!< Number of pulses for each / kWH (active energy) or number of pulses 
	//!< for each / kVarh (reactive energy) for each phase
	uint16_t indPhasePulseDuration;
	//!< Pulse duration in ADC counts for each active / reactive energy
	uint64_t totalPhaseMeterConstant;
	//!< Number of pulses for each / kWH for the cumulative total active
	//!< energy or number of pulses for each / kVarh for the cumulative
	//!< reactive energy
	uint16_t totalPhasePulseDuration;
	//!< Pulse duration in ADC counts for the cumulative total of active
	//!< / reactive energy
	uint8_t pulseGeneration;
    //!< Register used to enable / disable the phase pulse flag:
    //!< - EM_PULSE_GEN_PHASE_A
	//!< - EM_PULSE_GEN_PHASE_B
	//!< - EM_PULSE_GEN_PHASE_C
	//!< - EM_PULSE_GEN_PHASE_D
	//!< - EM_PULSE_GEN_PHASE_E
	//!< - EM_PULSE_GEN_PHASE_F
	//!< - EM_PULSE_GEN_PHASE_NEUTRAL
	//!< - EM_PULSE_GEN_PHASE_AGGREGATE
} EM_System_Configuration;


//! \brief Contains EM SW Lib Phase Calibration
//!
typedef struct _EM_Phase_Calibration_
{
	uint32_t voltageSF;
	//!< Voltage Scaling Factor - used to calculate VRMS
	uint32_t currentSF;
	//!< Current Scaling Factor - used to calculate IRMS
	uint32_t activePowerSF;
	//!< Active Power Scaling Factor
	uint32_t reactivePowerSF;
	//!< Active Power Scaling Factor
	int16_t phaseCorrection;
	//!< Phase Correction between voltage and current (register units)

} EM_Phase_Calibration;

//! \brief Contains EM SW Lib System Configuration
//!
typedef struct _EM_Phase_Configuration_
{
	int32_t *currentSamplePtr;
	//!< Pointer to the current sample
	int16_t *voltageSamplePtr;
	//!< Pointer to the voltage sample
	EM_Current_Sensor currentSensor;
	//!< Sensor connected to the current (I) channel
	uint16_t phaseResultsSupported;
	//!< Register used to read what results will be calculated for this phase
	//!< Note: This is a Read-Only register. 
	//!< Possible mask values are:
    //!< - EM_PHASE_RESULT_CALC_VRMS
    //!< - EM_PHASE_RESULT_CALC_IRMS
    //!< - EM_PHASE_RESULT_CALC_V_PEAK
    //!< - EM_PHASE_RESULT_CALC_I_PEAK
    //!< - EM_PHASE_RESULT_CALC_POWER_FACTOR
    //!< - EM_PHASE_RESULT_CALC_FREQ
	//!< - EM_PHASE_RESULT_CALC_ACTIVE_POWER
	//!< - EM_PHASE_RESULT_CALC_REACTIVE_POWER
	//!< - EM_PHASE_RESULT_CALC_APPARENT_POWER
	//!< - EM_PHASE_RESULT_CALC_ACTIVE_ENERGY
	//!< - EM_PHASE_RESULT_CALC_REACTIVE_ENERGY
	//!< - EM_PHASE_RESULT_CALC_APPARENT_ENERGY
	EM_Phase_Calibration *phaseCalibPtr;
	//!< Pointer to EM_Phase_Calibration struct
} EM_Phase_Configuration;

//! \brief Contains EM SW Lib Meter Configuration
//!
typedef struct _EM_Meter_Configuration_
{
	EM_Phase_Configuration *phaseConfigPtr;
	//!< Pointer to the EM_Phase_Configuration struct buffer
	uint8_t phaseLength;
	//!< Length of the emPhasePtr buffer
	uint8_t *meterName;
	//!< Pointer to meter name string
	uint8_t *libVersionNumber;
	//!< Pointer to library version number string
	uint8_t *meterPW;
	//~< Pointer to meter password string
} EM_Meter_Configuration;

//! \brief Contains EM SW Lib Algorithm Configuration
//!
typedef struct _EM_Algorithm_Configuration_
{
    EM_Power_t residualPowerCutoff;
	//!< Residual Power Cutoff per each phase. This threshold is used as the minimum 
	//!< value of power to store. Power values below this value will not be stored.
    EM_Power_t totalResidualPowerCutoff;
	//!< Residual Power Cutoff for the sum of all the phases. If the sum of all phase  
	//!< power values is below this value, the total power value will not be updated.
	uint8_t mainNomFreq;
	//!< Nominal Freq value for the system (Hz)
    _iq29 sqrtTwoConst;
    //!< Square of 2 constant used for Vpeak and Ipeak calculations
    _iq30 energyCalcConst;
    //!< One dividing by (3600 x sampling frequency)
    uint16_t freqSampling;
    //!< Sampling Frequency (Hz)
    uint16_t reactPowIIRMinFreq;
    //!< Min Frequency for the IIR coefficient table
    uint8_t reactPowIIRShift;
    //!< Number of shifts required to calculated index for IIR table
    uint16_t reactPowIIRLength;
    //!< Length of the IIR coefficient table
    _iq30 *reactPowIIRTable;
} EM_Algorithm_Configuration;

//! \brief Contains EM SW Lib Config Handle
//!
typedef struct _EM_SW_Lib_Config_Handle_
{
	EM_System_Configuration *systemConfig;
	//!< Pointer to EM_System_Configuration struct
	EM_Algorithm_Configuration *algorithmConfig;
	//!< Pointer to EM_Algorithm_Configuration struct
	EM_Meter_Configuration *meterConfig;
	//!< Pointer to EM_Meter_Configuration struct
} EM_SW_Lib_Config_Handle;

typedef struct _EM_Metrology_
{
    int16_t sampleCount;
    //!< Number of samples being accumulated in the block of processed
    //!< signals being gathered used for the oneDivSC calculation.
    int64_t actPowDotProd;
    //!< Accumulated multiplication of the phase corrected current and voltage (I.V)
    //!< used for the active power calculation.
    int64_t reactPowDotProd;
    //!< Accumulated multiplication of the phase corrected current and voltage 90 (I.V90)
    //!< used for the reactive power calculation.
    int16_t vDotProd[3];
    //!< Accumulated squares of the phase corrected voltage (V.V). These are used to
    //!< calculate the RMS voltage, and the power factor. We need separate accumulation
    //!< for each current sensor, as we accumulate the phase corrected voltage which matches
    //!< the current sensor's phase shift.
    int64_t iDotProd;
    //!< Accumulated squares of the phase corrected current (I.I). These are used to
    //!< calculate the RMS current, and the power factor. We need separate accumulation
    //!< for each current sensor, as we accumulate the phase corrected voltage which matches
    //!< the current sensor's phase shift.
    uint16_t cycleSampleCount;
    //!< This is a count of the number of samples to date in the current mains cycle. This
    //!< is used to calculate the mainPeriod.
    _iq30 reactPowGConst;
    //!< G Constant used for the IIR filter to calculate Reactive Power
    uint32_t mainPeriod;
    //!< This a running average of the length of a mains cycle. (samples/cycle * 256)
    uint32_t prevMainPeriod;
    //!< This a running average of the length of a mains cycle. (samples/cycle * 256)
    _iq30 oneDivSC;
    //!< Constant used to calculate the VRMS,IRMS, power and energy values (1/sample_count)
    uint16_t prevSampleCount;
    //!< Stores the prev sample count used to calculate oneDivSC. This is used to avoid
    //!< calculating 1/SC each cycle.
    uint8_t cycleCount;
    //!< Number of zero crossings. Once this number has reached the # of nominal freq.
    //!< For example for 50.2 Hz, once it reaches 50 cycles then the foreground is triggered.
} EM_Metrology;

//! \brief Contains EM Phase Background Results
//!
typedef struct _EM_Phase_BG_Results_
{
    int32_t vDCEstimate;
    //!< DC Estimate for Voltage
    int16_t iDCEstimate[3];
    //!< DC Estimate for Current
    int16_t reactPowPrevV90;
    //!< Previous V90 voltage used for the IIR filter in the reactive power calculation.
    int16_t vPrevSample;
    //!< Previous Voltage sample used for the IIR filter in the reactive power calculation.
    int32_t rogowskiPrevI90;
    //!< Previous I90 current used for the IIR filter in the Rogowski ADC current calculation.
    int32_t iPrevSample;
    //!< Previous Current sample used for the IIR filter in the Rogowski ADC current calculation.
}EM_Phase_BG_Results;

//! \brief Contains EM Phase Result
//!
typedef struct _EM_Phase_Results_
{
    uint32_t VRMS;
    //!< VRMS for previous frame (mV)
    uint32_t IRMS;
    //!< IRMS for previous frame (uA)
    uint32_t vPeak;
    //!< VPeak since system initialization (mV)
    uint32_t iPeak;
    //!< IPeak since system initialization (uA)
    int32_t powerFactor;
    //!< Power Factor calculated when the EM_powerFactor
    uint16_t frequency;
    //!< The Frequency is stored in 0.01Hz steps. A 16 bit unsigned number supports the range
    //!< 0.01Hz to 655.35Hz, which is clearly more than we will ever need for the fundamental.
    EM_Power_t activePower;
	//!< Active Power for previous frame (uW)
	EM_Power_t reactivePower;
	//!< Reactive Power for previous frame (uVAr)
	EM_Power_t apparentPower;
	//!< Apparent Power for previous frame (uVA)
	EM_Energy_t activeEnergy;
	//!< Active Energy for previous frame (uWh)
	EM_Energy_t reactiveEnergy;
	//!< Reactive Energy for previous frame (uVArh)
	EM_Energy_t apparentEnergy;
	//!< Apparent Energy for previous frame (uVAh)
	EM_Phase phaseID;
	//!< Present Phase ID Name
} EM_Phase_Results;

//! \brief Contains EM Total Results
//!
typedef struct _EM_Total_Results_
{
	EM_Power_t activePower;
	//!< Total Active Power since system initialization (uW)
	EM_Power_t reactivePower;
	//!< Total Reactive Power since system initialization (uVAr)
	EM_Power_t apparentPower;
	//!< Total Apparent Power since system initialization (uVA)
    EM_Energy_t activeEnergy;
    //!< Total Active Energy consumed since system initialization (uWh)
    EM_Energy_t reactiveEnergy;
    //!< Total Reactive Energy consumed since system initialization (uVArh)
    EM_Energy_t apparentEnergy;
    //!< Total Apparent Energy consumed since system initialization (UVAh)
} EM_Total_Results;

//! \brief Contains EM SW Lib Result Handle
//!
typedef struct _EM_SW_Lib_Result_Handle_
{
	EM_Total_Results *emTotalResult;
	//!< Pointer to EM_Total_Results struct
	EM_Phase_Results *phaseResults;
	//!< Pointer to EM_Phase_Result struct buffer
	EM_Phase_BG_Results *phaseBGResults;
	//!<  Pointer to the EM_Phase_BG_Results struct buffer
	EM_Metrology **phaseMetrologyPing;
	//!< Pointer to EM_Metrology struct Ping buffer
    EM_Metrology **phaseMetrologyPong;
    //!< Pointer to EM_Metrology struct Pong buffer
	uint8_t phaseResultsSupportedLen;
	//!< Length of the phaseResultsSupportedPtr buffer
} EM_SW_Lib_Result_Handle;

//*****************************************************************************
// globals
//*****************************************************************************
//! \brief The following variable is used as a maximum ADC count to detect frequency.
extern uint16_t * EMSWLIB_EM_HAL_ADC_maxFreqCount;

//! \brief The following variable is used as a minimum ADC count to detect frequency.
extern uint16_t * EMSWLIB_EM_HAL_ADC_minFreqCount;

//*****************************************************************************
// the function prototypes
//*****************************************************************************

/*******************************************************************************
 * EM SW LIBRARY CONFIGURATION APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup configuration_api
//! @{
//
//******************************************************************************
//! \brief The following API initializes the variables needed by the library at
//! run time. This includes the G constant value used for the IIR filter,  1 over
//! sample count and 1 / (3600 * sampling frequency) which are used to calculate
//! metrology results. This function also checks for configuration errors.
//!
//! \param[in] config  is a pointer to the EM SW Library user configuration
//! \param[in] result  is a pointer to the EM SW Result configuration
//!
//! \return EM_message_code
//!
extern EM_message_code EM_configureEnergyMeasurement(
		EM_SW_Lib_Config_Handle *config,
		EM_SW_Lib_Result_Handle *result);

//! \brief The following API initializes the voltage / current DC filters to 0.
//!
//! \param[in] config  is a pointer to the EM SW Library user configuration
//! \param[in] result  is a pointer to the EM SW Result configuration
//!
//! \return None
//!
extern void EM_initDCFilters(
        EM_SW_Lib_Config_Handle *config,
        EM_SW_Lib_Result_Handle *result);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * EM SW LIBRARY CALIBRATION APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup calibration_api
//! @{
//
//******************************************************************************
//! \brief    The following API is used to copies calibration values from
//!           flapFlashCalibration to pWorkingCalibration if the flash has been
//!           written to during the calibration process. If the flash
//!           buffer has not been written to, then the default values stored in
//!           pWorkingCalibration will be used at run time.
//!
//! \param[in] pWorkingCalibration  is a pointer to the working calibration buffer
//! \param[in] pFlashCalibration  is a pointer to the flash calibration buffer
//!
//! \return None
//!
extern void EM_initalizeCalibrationValues(
        uint8_t *pWorkingCalibration,
        uint8_t *pFlashCalibration);

//! \brief    The following API is used  store working (RAM) pWorkingCalibration
//!           values to the pFlashCalibration structure stored in flash. When
//!           there is a power cycle this values will be loaded to the working
//!           calibration buffer by using the EM_initalizeCalibrationValues API.
//!
//! \param[in] pWorkingCalibration  is a pointer to the working calibration buffer
//! \param[in] pFlashCalibration  is a pointer to the flash calibration buffer
//!
//! \return None
//!
extern void EM_updateFlashCalibrationValues(
        uint8_t *pWorkingCalibration,
        uint8_t *pFlashCalibration);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * EM SW LIBRARY RESULT APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup result_api
//! @{
//
//******************************************************************************
//! \brief The following API is intended for the removal of the DC content from
//! 16 bit 50Hz/60Hz mains signals. All samples should be passed through the
//! filter, in sequence, to obtain a signal free of DC content. The estimation
//! is based on a noise shaped single pole LPF.The cutoff frequency of this
//! filter is set very low, so its gain is essentially flat from 45Hz upwards.
//! This means the filter can take several seconds to stabilize when the
//! signal is initially applied.
//!
//! \param[in] p  is a pointer to the filter state variable
//! \param[in] x  is the next signal sample
//!
//! \return None
//!
extern int16_t EM_voltageDCFilter(
        int32_t *p,
        int16_t x);

//! \brief The following API is intended for the removal of the DC content from
//! 24 bit 50Hz/60Hz mains signals. All samples should be passed through the
//! filter, in sequence, to obtain a signal free of DC content. The estimation
//! is based on a noise shaped single pole LPF.The cutoff frequency of this
//! filter is set very low, so its gain is essentially flat from 45Hz upwards.
//! This means the filter can take several seconds to stabilize when the
//! signal is initially applied.
//!
//! \param[in] p  is a pointer to the filter state variable
//! \param[in] x  is the next signal sample
//!
//! \return None
//!
extern int32_t EM_currentDCFilter(
        int16_t p[3],
        int32_t x);

//! \brief    The following API is used update the previous voltage
//! value for a phase and to increment the number of samples processed
//! in the ISR. This API must be called inside the ISR after all the background
//! APIs are executed for a phase.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] bgData  is a pointer to the EM RP Phase Results structure
//!
//! \return None
//!
extern void EM_perSample(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_BG_Results *bgData);

//! \brief    The following API is used to swap the metrology ping and pong
//! pointers. This API must be executed after the Frequency Function
//! has processed N number of cycles (where N = config->algoConfig->mainNomFreq).
//! The Metrology Ping struc is the buffer updated in the ISR, and Metrology Pong
//! struct is used by the foreground APIs.
//!
//! \param[in] config  is a pointer to the EM SW Library user configuration
//! \param[in] result  is a pointer to the EM SW Library result structure
//! \param[in] phaseIdx  is the index for the phase
//!
//! \return None
//!
extern void EM_perDataChunk(
        EM_SW_Lib_Config_Handle *config,
        EM_SW_Lib_Result_Handle *result,
        uint16_t phaseIdx);

//! \brief    The following API is used to delay the current degree by 90 degrees.
//! It uses an IIR filter with 1 pole to shift current: (all pass 90 degree filter
//! for w = PI*f/fs, f=signal freq and fs = sampling frequency)
//! I90(IDX0 = I(IDX-1)+G*[I(IDX) - I90(IDX-1)] G = (tan(w) - 1)/(tan(w) + 1)
//! The G constant used by the filter is based on signal
//! frequency which is updated ~ once a second. It updates metro Result structure;
//! storing the temporary multiply accumulate (MAC) calculations.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] bgData  is a pointer to the EM SW BG Result structure
//!
//! \return None
extern void EM_delayCurrent90Degrees(EM_Metrology *metro,
                                     EM_Phase_Configuration *phase,
                                     EM_Phase_BG_Results *bgData);

//! \brief    The following API is used to generate pulses for an individual
//! phase. This API checks if the config->pulseGeneration has the result->
//! phaseID set. If is set then it will generate EM_PHASE_IMP_PER_KWHR impulses
//! per kWhr. This API requires that the pulse settings are initialized for the
//! activePulse and reactivePulse Port/Pins.
//!
//! \param[in] config  is a pointer to the EM SW Library user configuration
//! \param[in] pulse  is a pointer to the EM SW Library pulse structure
//! \param[in] result  is a pointer to the EM SW Library result structure
//!
//! \return None
//!
extern void EM_genPulseIndPhase(
        EM_System_Configuration *config,
        EM_Pulse_Settings *pulse,
        EM_Phase_Results *result);

//! \brief    The following API is used to generate pulses for the total
//! phase. This API checks if the config->pulseGeneration has EM_PULSE_GEN_PHASE_AGGREGATE
//!  set. If is set then it will generate EM_PHASE_IMP_PER_KWHR impulses
//! per kWhr. This API requires that the pulse settings are initialized for the
//! activePulse and reactivePulse Port/Pins.
//!
//! \param[in] config  is a pointer to the EM SW Library user configuration
//! \param[in] pulse  is a pointer to the EM SW Library pulse structure
//! \param[in] result  is a pointer to the EM SW Library result structure
//!
//! \return None
//!
extern void EM_genPulseTotalPhases(
        EM_System_Configuration *config,
        EM_Pulse_Settings *pulse,
        EM_Total_Results *result);

//! \brief    The following API is used to calculate the 1/sample_count constant.
//! This function must be executed after EM_perDataChunk API for each phase inside
//! the foreground code. The 1/sample_count constant will be used to calculate
//! the results parameters.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//!
//! \return None
//!
extern void EM_updateOneDivSC(
        EM_Metrology *metro);

//! \brief    The following API is used to clear the temporary variables inside
//! the metro structure. This API must be called as the last API in the
//! foreground code.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//!
//! \return None
//!
extern void EM_clearMetrology(
        EM_Metrology *metro);

//! \brief    The following API is used to look up the IIR G constant used to
//! calculate the Reactive Power using the current frequency value. The
//! index of the G constant in the reactPowIIRTable is calculated:
//! index = (Current Frequency - reactPowIIRMinFreq) >> reactPowIIRShift
//! if the index is > reactPowIIRLength, then index = reactPowIIRLength
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] result  is a pointer to the EM SW Result structure
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//!
//! \return None
//!
extern void EM_updateReactPowConst(EM_Metrology *metro,
                                   EM_Phase_Results *result,
                                   EM_Algorithm_Configuration *algo);

//! \brief    The following API is used process the samples. It is used to 
//! calculate intermediate dot product results that are fed into the foreground 
//! process for the calculation of metrology readings.
//!
//! \param[in] config  is a pointer to the EM SW Library user configuration
//!
//! \return EM_message_code
//!
extern EM_message_code EM_processSample(
		EM_SW_Lib_Config_Handle *config);

//! \brief    The following API is used to calculate the dot product and
//! accumulation of Voltage and Current. This API reads the current and voltage
//! values using the phase structure.It updates metro Result structure; storing
//! the temporary multiply accumulate (MAC) calculations. The MAC value is used
//! by the foreground function EM_calcActivePowerDiv to calculate Active Power.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//!
//! \return None
//!
extern void EM_calcActivePowerMultAcc(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase);

//! \brief    The following API is used to calculate the active power in the
//! foreground code and store activePower in the result structure. The formula used:
//! (metro->AP_dot_prod->P_accum x metro->oneDivSC) x phase->phaseCalibPtr->activePowerSF
//! If the activePower calculated is less than algo->residualPowerCutoff
//! then activePower will be set to 0.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] result  is a pointer to the EM SW Result structure
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//!
//! \return None
//!
extern void EM_calcActivePowerDiv(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_Results *result,
        EM_Algorithm_Configuration *algo);

//! \brief    The following API is used to calculate the active energy result.
//! The formula used to calculate activeEnergy (uWh):
//! (result->activePower x metro->sampleCount) x energyCalcConst
//! Where energyCalcConst = (1/(sampling freq  x 3600))
//! This API must be called after EM_calcActivePowerDiv.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//! \param[in] result  is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcActiveEnergy(
        EM_Metrology *metro,
        EM_Algorithm_Configuration *algo,
        EM_Phase_Results *result);

//! \brief    The following API is used to calculate the dot product and
//! accumulation of Voltage_90 and Current. This API reads the current and voltage
//! values using the phase structure.It uses an IIR filter with 1 pole to shift the
//! the voltage 90 degrees: (all pass 90 degree filter for w = PI*f/fs, f=signal freq
//! and fs = sampling frequency)
//! V90(IDX0 = V(IDX-1)+G*[V(IDX) - V90(IDX-1)] G = (tan(w) - 1)/(tan(w) + 1)
//! The G constant used by the filter is based on signal
//! frequency which is updated ~ once a second. It updates metro Result structure;
//! storing the temporary multiply accumulate (MAC) calculations. The MAC value is
//! used by the foreground function EM_calcReactivePowerDiv to calculate Reactive Power.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] bgData  is a pointer to the EM SW BG Result structure
//!
//! \return None
//!
extern void EM_calcReactivePowerMultAcc(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_BG_Results *bgData);

//! \brief    The following API is used to calculate the reactive power in the
//! foreground code and store reactivePower in the result structure. The formula used:
//! (metro->RP_dot_prod->P_accum x metro->oneDivSC) x phase->phaseCalibPtr->reactivePowerSF
//! If the reactivePower calculated is less than algo->residualPowerCutoff
//! then reactivePower will be set to 0.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] result  is a pointer to the EM SW Result structure
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//!
//! \return None
//!
extern void EM_calcReactivePowerDiv(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_Results *result,
        EM_Algorithm_Configuration *algo);

//! \brief    The following API is used to calculate the reactive energy result.
//! The formula used to calculate reactiveEnergy (uVArh):
//! (result->reactivePower x metro->sampleCount) x energyCalcConst
//! Where energyCalcConst = (1/(sampling freq  x 3600))
//! This API must be called after EM_calcReactivePowerDiv.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//! \param[in] result  is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcReactiveEnergy(
        EM_Metrology *metro,
        EM_Algorithm_Configuration *algo,
        EM_Phase_Results *result);

//! \brief    The following API is used to calculate apparent power.
//! The formula used to calculate apparentPower (uVA):
//! Sqrt Root (result->activePower ^ 2 + result->reactivePower ^ 2)
//! This API must be called after EM_calcActivePowerMultAcc and
//! EM_calcReactivePowerMultAcc.
//!
//! \param[in] result  is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcApparentPower(
        EM_Phase_Results *result);

//! \brief    The following API is used to calculate the apparent energy result.
//! The formula used to calculate apparentEnergy (uVAh):
//! (result->apparentPower x metro->sampleCount) x energyCalcConst
//! Where energyCalcConst = (1/(sampling freq  x 3600))
//! This API must be called after EM_calcApparentPower.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//! \param[in] result  is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcApparentEnergy(
        EM_Metrology *metro,
        EM_Algorithm_Configuration *algo,
        EM_Phase_Results *result);
		
//! \brief    The following API is used to calculate power factor. The formula
//! used to calculate it:
//! activePower / apparentPower
//! This API must be called after EM_calcActivePowerMultAcc and
//! EM_calcApparentPower.
//!
//! \param[in] result  is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcPowerFactor(
        EM_Phase_Results *result);

//! \brief    The following API is used to calculate the dot product and
//! accumulation for VRMS. This API reads the voltage value from the voltage ptr
//! in the phase structure and updates the Metro Result structure; storing
//! the temporary multiply accumulate (MAC) calculations. The MAC value is used
//! by the foreground function EM_calcVRMSDivSqrt to calculate VRMS.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//!
//! \return None
//!
extern void EM_calcVRMSMultAcc(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase);

//! \brief    The following API is used to calculate the division and
//! square root for VRMS. This API uses the dot product calculated by
//! the EM_calcVRMSMultAcc function. It updates the global Phase Result structure.
//! The result can be read inside the result structure.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] result  is a pointer to the EM SW Library result handle
//!
//! \return None
//!
extern void EM_calcVRMSDivSqrt(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_Results *result);
		
//! \brief    The following API is used to calculate the dot product and
//! accumulation for IRMS. This API reads the current value from the current ptr
//! in the phase structure and updates the Metro Result structure; storing
//! the temporary multiply accumulate (MAC) calculations. The MAC value is used
//! by the foreground function EM_calcIRMSDivSqrt to calculate IRMS.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//!
//! \return None
//!
extern void EM_calcIRMSMultAcc(EM_Metrology *metro,
                               EM_Phase_Configuration *phase);

//! \brief    The following API is used to calculate the division and
//! square root for IRMS. This API uses the dot product calculated by
//! the EM_calcIRMSMultAcc function. It updates the global Phase Result structure.
//! The result can be read inside the result structure.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] result  is a pointer to the EM SW Library result handle
//!
//! \return None
//!
extern void EM_calcIRMSDivSqrt(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_Results *result);
		
//! \brief    The following API is used to calculate I Peak. This API updates
//! the global Phase Result structure. The formula used to calculate:
//! result->IRMS x algo->sqrtTwoConst
//! Where algo->sqrtTwoConst = Sqrt(2)
//! The result can be read inside the result structure.
//!
//! \param[in] result  is a pointer to the EM SW Result structure
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//!
//! \return None
//!
extern void EM_calcIPeak(
        EM_Phase_Results *result,
        EM_Algorithm_Configuration *algo);
		
//! \brief    The following API is used to calculate V Peak. This API updates
//! the global Phase Result structure.  The formula used to calculate:
//! result->VRMS x algo->sqrtTwoConst
//! Where algo->sqrtTwoConst = Sqrt(2)
//! The result can be read inside the result structure.
//!
//! \param[in] result  is a pointer to the EM SW Result structure
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//!
//! \return None
//!
extern void EM_calcVPeak(
        EM_Phase_Results *result,
        EM_Algorithm_Configuration *algo);

//! \brief    The following API is used to calculate the period of the voltage
//! signal. For systems with 2 voltages, this API must be used with only one
//! of the voltage signals. This API reads the voltage using the phase structure.
//! This API requires the initialization of the following using the function
//! count = (256 * Sampling Freq. / Fs ):
//! - EMSWLIB_EM_HAL_ADC_minFreqCount - should be calculated for Fs = 70 Hz
//! - EMSWLIB_EM_HAL_ADC_maxFreqCount - should be calculated for Fs = 40 Hz
//! For example For Sampling Freq = 4096,
//! minFreqCount  = (256*4096 / 70) = 14,980
//! maxFreqCount  = (256*4096 / 40) = 26,214
//! The Main Period is calculated using a running average of the main_period.
//! To calculate the period interpolation is used using a resolution of 256.
//! The period between two zero crossings (ZC_1 and ZC_2) is calculated using
//! the following formula:
//! Given,
//! XC_N (interpolated x value) = - 256 * Y_Prev / (Y_Act - Y_Prev)
//! Where Y_Prev and Y_Act are the Y values in the negative to positive transition
//! (e.g. Y_Prev < 0 and Y_Act >= 0 )
//! Delta_N = 256 - XC_N
//! Then,
//! Period_N = Delta_N-1 + 256*samples (not including Y_Prev and Y_Act) + XC_N
//! (e.g. for Period 2) = Delta_1 + 256*samples + XC_2
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Ping Ptr)
//! \param[in] phase  is a pointer to the EM SW Phase structure
//! \param[in] bgData  is a pointer to the EM SW BG Result structure
//!
//! \return None
//!
extern void EM_calcFreqPeriod(
        EM_Metrology *metro,
        EM_Phase_Configuration *phase,
        EM_Phase_BG_Results *bgData);

//! \brief    The following API is used to calculate frequency. This API updates
//! the global Phase Result structure.  The formula used to calculate:
//! algo->freqSampling * 256L*100L / metro->mainPeriod
//! The result can be read inside the result structure.
//!
//! \param[in] metro  is a pointer to the EM SW Metrology structure (Pong Ptr)
//! \param[in] result  is a pointer to the EM SW Result structure
//! \param[in] algo  is a pointer to the EM SW Algorithm structure
//!
//! \return None
//!
extern void EM_calcFreq(
        EM_Metrology *metro,
        EM_Phase_Results *result,
        EM_Algorithm_Configuration *algo);

//! \brief    The following API is used to calculate total active power.
//! This API will aggregate the total power of the result->phaseResults
//! buffer using the indexes included in the phaseIdx buffer.
//!
//! \param[in] phaseIdx is a pointer to the phase indexes buffer
//! \param[in] length is the length of the phaseIdx buffer
//! \param[in] result is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcTotalActivePower(
        uint8_t *phaseIdx,
        uint8_t length,
        EM_SW_Lib_Result_Handle *result);

//! \brief    The following API is used to calculate total reactive power.
//! This API will aggregate the total power of the result->phaseResults
//! buffer using the indexes included in the phaseIdx buffer.
//!
//! \param[in] phaseIdx is a pointer to the phase indexes buffer
//! \param[in] length is the length of the phaseIdx buffer
//! \param[in] result is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcTotalReactivePower(
        uint8_t *phaseIdx,
        uint8_t length,
        EM_SW_Lib_Result_Handle *result);

//! \brief    The following API is used to calculate total apparent power.
//! This API will aggregate the total power of the result->phaseResults
//! buffer using the indexes included in the phaseIdx buffer.
//!
//! \param[in] phaseIdx is a pointer to the phase indexes buffer
//! \param[in] length is the length of the phaseIdx buffer
//! \param[in] result is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcTotalApparentPower(
        uint8_t *phaseIdx,
        uint8_t length,
        EM_SW_Lib_Result_Handle *result);


//! \brief    The following API is used to calculate total active energy.
//! This API will aggregate the total energy of the result->phaseResults
//! buffer using the indexes included in the phaseIdx buffer.
//!
//! \param[in] phaseIdx is a pointer to the phase indexes buffer
//! \param[in] length is the length of the phaseIdx buffer
//! \param[in] result is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcTotalActiveEnergy(
        uint8_t *phaseIdx,
        uint8_t length,
        EM_SW_Lib_Result_Handle *result);

//! \brief    The following API is used to calculate total reactive energy.
//! This API will aggregate the total energy of the result->phaseResults
//! buffer using the indexes included in the phaseIdx buffer.
//!
//! \param[in] phaseIdx is a pointer to the phase indexes buffer
//! \param[in] length is the length of the phaseIdx buffer
//! \param[in] result is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcTotalReactiveEnergy(
        uint8_t *phaseIdx,
        uint8_t length,
        EM_SW_Lib_Result_Handle *result);

//! \brief    The following API is used to calculate total apparent energy.
//! This API will aggregate the total energy of the result->phaseResults
//! buffer using the indexes included in the phaseIdx buffer.
//!
//! \param[in] phaseIdx is a pointer to the phase indexes buffer
//! \param[in] length is the length of the phaseIdx buffer
//! \param[in] result is a pointer to the EM SW Result structure
//!
//! \return None
//!
extern void EM_calcTotalApparentEnergy(
        uint8_t *phaseIdx,
        uint8_t length,
        EM_SW_Lib_Result_Handle *result);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of  _EMSWLIB_H_ definition

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

#ifndef _USSSWLIB_H_
#define _USSSWLIB_H_

//#############################################################################
//
//! \file   ussSwLib.h
//!
//! \brief  Contains all USS SW Library enums, structs, macros, function and
//!            global variables definitions
//!         
//
//  Group:          MSP
//  Target Device:  __DEVICE_NAME__
//
//  (C) Copyright 2018, Texas Instruments, Inc.
//#############################################################################
// TI Release: USSLib_02_20_00_16 
// Release Date: January 23, 2019
//#############################################################################


//*****************************************************************************
// includes
//*****************************************************************************
#include <msp430.h>

#include <stdint.h>
#include <stdbool.h>
#include <QmathLib.h>
#include <IQmathLib.h>

#ifdef __cplusplus

extern "C" {
#endif

//*****************************************************************************
// defines
//*****************************************************************************

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status variable
//! and determine if USS UUPS Power request ignored interrupt has been triggered.
//!
#define USS_UUPS_PWR_REQ_IGN_INTERRUPT                    BIT0

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status variable
//! and determine if USS UUPS Power up time out interrupt has been triggered.
//!
#define USS_UUPS_PWR_UP_TM_OUT_INTERRUPT                BIT1

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status variable
//! and determine if USS SAPH Data error abort interrupt has been triggered.
//!
#define USS_SAPH_DATA_ERR_ABORT_INTERRUPT                BIT2

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status variable
//! and determine if USS SAPH Time Mark 4 time out interrupt has been triggered.
//!
#define USS_SAPH_TMF_TM_OUT_INTERRUPT                    BIT3

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status variable
//! and determine if USS SAPH sequence acquisition done interrupt has
//! been triggered.
//!
#define USS_SAPH_SEQ_ACQ_DN_INTERRUPT                    BIT4

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status variable
//! and determine if USS SAPH ping transmit done interrupt has
//! been triggered.
//!
#define USS_SAPH_PING_DN_INTERRUPT                        BIT5

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS HSPLL PLL unlock interrupt has been triggered.
//!
#define USS_HSPLL_PLL_UNLOCK_INTERRUPT                    BIT6
//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SDHS WINLO interrupt has been triggered.
//!
#define USS_SDHS_WINLO_INTERRUPT                        BIT7

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SDHS WINHI interrupt has been triggered.
//!
#define USS_SDHS_WINHI_INTERRUPT                        BIT8

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SDHS DTRDY interrupt has been triggered.
//!
#define USS_SDHS_DTRDY_INTERRUPT                        BIT9

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SDHS SSTRG interrupt has been triggered.
//!
#define USS_SDHS_SSTRG_INTERRUPT                        BITA

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SDHS SMPSZ interrupt has been triggered.
//!
#define USS_SDHS_ACQDONE_INTERRUPT                        BITB

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SDHS OVF interrupt has been triggered.
//!
#define USS_SDHS_OVF_INTERRUPT                            BITC

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if USS SW Library delay generator timer has
//! expired.
//!
#define USS_TIMER_EXPIRED_INTERRUPT                        BITD

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if SAPH has been stopped
//!
#define USS_SAPH_SEQUENCE_STOPPED                        BITE

//! \brief This defines can be used to mask USSSWLIB_USS_interrupt_status
//! variable and determine if UUPS has been stopped by the debugger
//!
#define USS_UUPS_STOPPED_BY_DEBUGGER                    BITF

 //! \brief This bit determines if the first event of period measurement is complete
 //!
 #define USS_TIMER_EXPIRED_EVENT1_INTERRUPT             0x10000

 //! \brief This define can be used to mask USSSWLIB_USS_interrupt_status
 //! variable and determine if SAPH generated an DMA acknowledgment violation
 #define USS_SAPH_DMA_ACK_VIOLATION                   (0x00010000)

#define HARDWAREWREG16(x)    (*((volatile uint16_t *)((uint16_t)x)))


// Backwards compatibility defines
// This backwards compatibility layer might be removed in future releases

#define USS_Alg_dToF_Calculation_Option_gas USS_Alg_dToF_Calculation_Option_estimate


//*****************************************************************************
// typedefs
//*****************************************************************************

//! \brief The following enumeration contain all possible USS SW Library return
//!         message codes.
//!
typedef enum _USS_message_code_
{

	USS_message_code_no_error = 0,
	//!< These message codes are use to provide status/error messages to the
	//!< user all APIs will return a message code after the execution.

	// 1-20 reserved for SAPH messages
	USS_message_code_SAPH_pulseLowPhasePeriod_error = 1,
	//!< This error is returned when the SAPH low phase period configuration is
	//!< invalid
	USS_message_code_SAPH_pulseHighPhasePeriod_error = 2,
	//!< This error is returned when the SAPH high phase period configuration is
	//!< invalid
	USS_message_code_SAPH_numOfExcitationPulses_error = 3,
	//!< This error is returned when SAPH number of excitation pulses is invalid
	USS_message_code_SAPH_numOfStopPulses_error = 4,
	//!< This error is returned when SAPH number of stop pulses is invalid
	USS_message_code_SAPH_update_error_ongoing_conversion = 5,
	//!< This error is returned if user attempted to update SAPH during an
	//!< active conversion  
	USS_message_code_SAPH_time_startPPGCount_error = 6,
	//!< This error is returned if startPPGCount is invalid
	USS_message_code_SAPH_time_turnOnADCCount_error = 7,
	//!< This error is returned if turnOnADCCount is invalid
	USS_message_code_SAPH_time_startADCsamplingCount_error = 8,
	//!< This error is returned if startADCsamplingCount is invalid
	USS_message_code_SAPH_time_restartCaptureCount_error = 9,
	//!< This error is returned if restartCaptureCount is invalid
	USS_message_code_SAPH_time_captureTimeOutCount_error = 10,
	//!< This error is returned if captureTimeOutCount is invalid
	USS_message_code_SAPH_time_startPGAandINBiasCount_error = 11,
	//!< This error is returned if startPGAandINBiasCount is invalid
	USS_message_code_SAPH_invalid_bias_impedance_error=12,
	//!< This error is returned if an invalid bias impedance value is selected
    USS_message_code_SAPH_invalid_rx_charge_pump_mode_error=13,
    //!< This error is returned if an invalid charge pump configuration is selected
    USS_message_code_SAPH_invalid_pulse_configuration_error=14,
    //!< This error is returned if an invalid pulse generation parameters are
    //!< invalid. This error is returned in the following cases:
    //!< <ul>
    //!<    <li>for multi-tone (hspll_freq/f1) or (hspll_freq/f2) greater than 510.</li>
    //!<    <li>for mone tone [(hspll_freq/f1)*(duty_cycle)] and [(hspll_freq/f1) *(1-duty_cycle)] greater than 255</li>
    //!<    <li>for dual tone the same check as in mono tone is performed but against f2 parameter</li>
    //!< </ul>

	//    21-40 reserved for HSPLL messages
	USS_message_code_HSPLL_pllXtalFreq_inHz_error = 21,
	//!< This error is returned if input frequency for HSPLL module is invalid
	USS_message_code_HSPLL_pllOutputFreq_inHz_error = 22,
	//!< This error is returned if the output frequency of the HSPLL is invalid
	USS_message_code_HSPLL_plllock_error =23,
	//!< This error is returned if there is an error locking the PLL
	USS_message_code_HSPLL_pll_unlock_error = 24,
	//!< This error is returned if HSPLL detects pll has unlocked
	USS_message_code_HSPLL_update_error_ongoing_conversion = 26,
	//!< This error is returned if user attempted to update HSPLL during an
	//!< active conversion 
	USS_message_code_HSPLL_verification_expected_count_error=27,
	//!< This error is returned if USS_verifyHSPLLFrequency API if
	//!< abs(expected count-actualCount)>tolerance
	USS_message_code_HSPLL_invalid_settling_count_error=28,
	//!< This error is returned if ussXTALsettlelingCount is 0

	// 41-60 reserved for SDHS messages
	//!< SDHS messages
	USS_message_code_SDHS_threshold_error = 41,
	//!< This error is returned if high and/or low threshold are invalid
	USS_message_code_SDHS_conversion_overflow_error = 43,
	//!< This error is returned if a conversion overflow is detected during 
	//!< conversion
	USS_message_code_SDHS_sample_size_error = 44,
	//!< This error is returned if sample size is invalid
	USS_message_code_SDHS_update_error_ongoing_conversion = 45,
	//!< This error is returned if user attempted to update SDHS during an
	//!< active conversion 
	USS_message_code_SDHS_window_low_threshold_reached = 46,
	//!< This error is returned if conversion reached low threshold during 
	//!< conversion
	USS_message_code_SDHS_window_high_threshold_reached = 47,
	//!< This error is returned if conversion reached high threshold during 
	//!< conversion
	USS_message_code_SDHS_max_size_error = 48,
	//!< This error is returned if max sample size is entered

	// 61-80 reserved for UUPS messages
	//!< UUPS messages
	USS_message_code_UUPS_update_error_ongoing_conversion = 61,
	//!< This error is returned if user attempted to update UUPS during an
	//!< active conversion 
	USS_message_code_UUPS_power_up_time_out_error = 62,
	//!< This error is returned if a UUPS Power up time out is detected
	USS_message_code_UUPS_power_up_error = 63,
	//!< This error is returned if a USS power up error is detected
	USS_message_code_UUPS_power_down_error = 64,
	//!< This error is returned if a USS power down error is detected


	// 81-100 reserved for measurement messages
	USS_message_code_data_error_abort = 81,
	//!< This error is returned if a data error is triggered during sequence
	USS_message_code_ASQ_time_out= 82,
	//!< This error is returned if an ASQ time out occurs.

	// 101- 120 reserved for stop error messages
	USS_message_code_measurement_stopped = 101,
	//!< This error is returned to indicate an ongoing measurement was stopped
	USS_message_code_error_conversion_stopped_by_debugger = 102,
	//!< This error is returned if a USS sequence is stopped by the debugger

	// 121 - 140 reserved for algorithms messages
	//!< USS Algorithms message codes
	USS_message_code_parameter_check_failed = 121,
	//!< This error is returned if invalid parameters are used to initialize 
	//!< algorithms
	USS_message_code_valid_results = 122,
	//!< This error is returned if NO problems where encountered while running
	//!< the algorithms
	USS_message_code_algorithm_error = 123,
	//!< This error is returned if problems were encountered while running
	//!< the algorithms.
	//!< The results return by USS_runAlgorihtms or USS_runAlgorithmsFixedPoint
	//!< USS_runAlgorithmsFixedPoint are invalid if this error is returned
	USS_message_code_algorithm_error_invalid_iteration_value = 124,
	//!< This error is returned the user has selected an invalid number of
	//!< absTOFInterval. The results return by USS_runAlgorithms or
	//!< USS_runAlgorithmsFixedPoint are invalid if this error is returned
	USS_message_code_algorithm_error_no_signal_detected_ups_channel = 125,
	//!< This error is returned if no signal is detected in UP stream channel.
	//!< The results return by USS_runAlgorihtms or USS_runAlgorithmsFixedPoint
	//!< are invalid if this error is returned.
	USS_message_code_algorithm_error_no_signal_detected_ups_dns_channel = 126,
	//!< This error is returned if no signal is detected in UP AND DOWN stream channel.
	//!< The results return by USS_runAlgorihtms or USS_runAlgorithmsFixedPoint
	//!< are invalid if this error is returned.
	USS_message_code_algorithm_error_no_signal_detected_dns_channel = 127,
	//!< This error is returned if no signal is detected in DOWN stream channel
	//!< The results return by USS_runAlgorihtms or USS_runAlgorithmsFixedPoint
	//!< are invalid if this error is returned.
	USS_message_code_algorithm_captures_accumulated = 128,
	//!< This message is returned by USS_runAlgorihtms or USS_runAlgorithmsFixedPoint
	//!< to indicate that UPS/DNS have only being accumulated
	USS_message_code_algorithm_error_invalid_clock_relative_error = 129,
	//!< This message is returned by USS_updateClockRelativeError to indicate 
	//!< that clockRelativeError is outside tha valid range.
	USS_message_code_algorithm_error_invalid_filter_length = 130,
	//!< This message is returned by USS_initAlgorithms API if user
    //!< has enabled USS_diagnostics_mode_2. This error is returned if
    //!< filterLenght is greater than 20
    USS_message_code_algorithm_error_generating_binary_pattern = 131,
    //!< This message is returned by USS_initAlgorithms API. This error is
    //!< returned if binary pattern does not fit in memory reserved by user.
    //!< Minimum binary size
    //!< <ul>
    //!<    <li>monotone: ((HSPLLFREQ*USS_NUM_OF_EXCITATION_PULSES_F1)/(USS_F1_FREQ*downSampleFactor))</li>
    //!<    <li>dualtone: ((HSPLLFREQ*USS_NUM_OF_EXCITATION_PULSES_F1)/(USS_F1_FREQ*downSampleFactor)) + ((HSPLLFREQ*USS_NUM_OF_EXCITATION_PULSES_F2)/(USS_F2_FREQ*downSampleFactor))</li>
    //!<    <li>multitone:((HSPLLFREQ*USS_NUM_OF_TRILL_PULSES *2)/((USS_F1_FREQ + USS_F2_FREQ/2)*downSampleFactor)) + ((HSPLLFREQ*USS_NUM_OF_EXCITATION_PULSES_F2)/(USS_F2_FREQ*downSampleFactor)) </li>
    //!< </ul>
    USS_message_code_algorithm_error_invalid_absToF_computation_option = 132,
    //!< This message is returned by USS_initAlgorithms API if user
    //!< has enabled USS_diagnostics_mode_2. This error is returned if
    //!< user has selected an invalid absTof computation option
    USS_message_code_algorithm_error_invalid_meter_constant_calib_data = 133,
    //!< This message is returned by USS_initAlgorithms API if user
    //!< has enabled USS_diagnostics_mode_2 and
    //!< user has an invalid USS_Meter_Configuration configuration
    USS_message_code_algorithm_error_meter_constant_calib = 134,
    //!< This message is returned if the minimum volume flow rate falls outside
    //!< the lowest alphaMin calibration point.

	// 141 - 160 reserved for algorithms messages
	USS_message_code_interrupt_update_error_ongoing_conversion = 142,
	//!< This error is returned if user attempted to update the interrupt 
	//!< configuration during an active conversion 

	//161-170 reserved for ISTOP messages
	//!< USS ISTOP messages
	USS_message_code_invalid_conversion_data = 161,
	//! This error is returned when conversion was interrupted

	//171-180 reserved for calibration messages
	USS_message_code_Calibration_DAC_success = 171,
	//! Indicates DAC calibration completed successfully
	USS_message_code_Calibration_DAC_error = 172,
	//! Indicates an error during DAC calibration routine
	USS_message_code_Calibration_Gain_error = 173,
	//! Indicates an error during Gain calibration routine
	USS_message_code_Calibration_DCO_error = 174,
	//! Indicates an error during DCO calibration routine
	USS_message_code_Signal_Gain_Calibration_timeout= 175,
	//! Indicates a timeout occurred during signal gain calibration routine
	USS_message_code_Signal_Gain_Calibration_settling= 176,
	//! Indicates signal gain calibration routine ended during signal settling
	//! loop
	USS_message_code_Signal_Gain_Calibration_successful =177,
	//! Indicates signal gain calibration completed successfully
	USS_message_code_Calibration_DAC_timeout_error = 178,
	//! Indicates a timeout occurred during DAC calibration
	USS_message_code_Calibration_ToF_Offset_error = 179,
	//! Indicates a error has occurred during Abs ToF and DToF calculation
	USS_message_code_Calibration_Offset_invalid_configuration = 180,
	//! Indicated a test configuration has occured while running AbsToF and DToF
	//! offset calculation

	//240-255 error
	USS_message_code_ACLK_settling_time_out =240,
	//!< This error is return when the USS module attempted to configure
	//!< the timer module but the oscillation flag was not cleared within
	//!< the expected timeout period (~800 msec)
	USS_meesage_code_silicon_version_does_not_support_this_functionality = 241,
	//!< This error is return when a silicon rev B or later functionality is
	//!< attempted to be used in rev A silicon (XMS material).
	USS_message_code_USS_ongoing_active_conversion_error =254,
	//!< This error is return when the USS module was attempted to be updated while
	//!< USS_ BUSY bit was set
	USS_message_code_error_occurred =255
	//!< This is a generic error this is error that is returned if an unhandled
	//!< error occurs
} USS_message_code;

//! \brief Selects diagnostic mode to be used by the library. For optimal power
//! consumption it is recommended to use USS_diagnostics_mode_0. During
//! development it is recommended to use USS_diagnostics_mode_2.
//!
typedef enum _USS_diagnostics_mode_
{
	USS_diagnostics_mode_0 = 0,
	//!< This mode will allow to achieve lowest power consumption
	USS_diagnostics_mode_1 = 1,
	USS_diagnostics_mode_2 = 2
	//!< This mode enables diagnostics in mode 2 and USS configuration parameter
	//!< checking
} USS_diagnostics_mode;

//! \brief Selects the desired power mode while USS capture is done, API will
//! remain in this mode until capture is done.
//!
typedef enum _USS_capture_power_mode_
{
	USS_capture_power_mode_active = (GIE),
	//!< MSP remains in ACTIVE power mode until USS capture is done
	USS_capture_power_mode_low_power_mode_0 = (LPM0_bits | GIE),
	//!< MSP remains in LPM0 power mode until USS capture is done
} USS_capture_power_mode_option;


//! \brief Selects USS state after capture has been stopped
//!
typedef enum _USS_stop_measurement_state_
{
	//!< USS goes to OFF state
	USS_stop_measurement_turn_off_USS = 0,
	//!< USS remains in READY state
	USS_stop_measurement_state_ready = 1
} USS_stop_measurement_state;

//! \brief Allows user to select USS channel drive strength. User can select
//!           between max strength and pre-trimmed drive.
//!
typedef enum _USS_measurement_drive_strength_
{
	USS_measurement_drive_strength_pre_trimmed_drive = 0,
	//!< Pre-trimmed drive strength will be applied
	USS_measurement_drive_strength_max_strength = 0x0100
	//!< Max drive strength will be applied
} USS_measurement_drive_strength;

//! \brief Allows user to select pause state. Pause state is considered to be
//!           time before and after USS Channel Pulse Generation
//!
typedef enum _USS_measurement_pause_state_
{
	USS_measurement_pause_state_high_impedance = 0,
	//!< USS channel remains in high impedance state
	USS_measurement_pause_state_low = 1,
	//!< USS channel remains in low state
	USS_measurement_pause_state_high = 2
	//!< USS channel remains in high state
} USS_measurement_pause_state;

//! \brief Allows user to select starting pulse state
//! 
typedef enum _USS_measurement_pulse_polarity_
{
	USS_measurement_pulse_polarity_excitation_starts_on_high_pulse = 0x0000,
	//!< Start pulse generation with high pulse
	USS_measurement_pulse_polarity_excitation_starts_on_low_pulse = 0x000F
	//!< Start pulse generation with low pulse
} USS_measurement_pulse_polarity;

//! \brief Allows user to select the ULP bias delay during USS Power up.
//! sequence. The ULP bias allows a faster channel setting on reactive input
//! loads. Please refer the device User's guide UUPS and SAPH chapter for
//! further recommendations regarding ULP bias.
typedef enum _USS_measurement_ULP_bias_delay_
{
    USS_measurement_ULP_bias_delay_no_delay,
    //!< Disables ULP bias delay functionality
    USS_measurement_ULP_bias_delay_100_usec,
    //!< Generates an approximate ULP bias delay of 100 usec during power
    //!< up sequence
    USS_measurement_ULP_bias_delay_200_usec,
    //!< Generates an approximate ULP bias delay of 200 usec during power
    //!< up sequence
    USS_measurement_ULP_bias_delay_300_usec,
    //!< Generates an approximate ULP bias delay of 300 usec during power
    //!< up sequence
}USS_measurement_ULP_bias_delay;

//! \brief The following parameter allows the application to control the RX/TX
//! bias during the acquisition sequence. For resistive loads a low impedance
//! generates the fastest settling times. For reactive loads a different
//! impedance setting might be needed to generate a gentler build up of the bias
//! voltage. Please refer to device User's guide SAPH chapter for further
//! recommended usage.
typedef enum _USS_measurement_bias_impedance_
{
    USS_measurement_bias_impedance_200_Ohms,
    //!< Generates a 200 Ohms bias impedance
    USS_measurement_bias_impedance_600_Ohms,
    //!< Generates a 600 Ohms bias impedance
    USS_measurement_bias_impedance_1200_Ohms,
    //!< Generates a 1200 Ohms bias impedance
    USS_measurement_bias_impedance_2800_Ohms,
    //!< Generates a 2800 Ohms bias impedance
}USS_measurement_bias_impedance;


//! \brief The following parameter allow application to turn on/off the PGA
//! input multiplexer during the whole firing/capture sequence. Having the the
//! charge always on allows to reduce the RX signal noise. Please refer to the
//! SAPH chapter in the device User's guide for detailed recommended usage of
//! this functionality.
//!
typedef enum _USS_measurement_mux_charge_pump_
{
    USS_measurement_mux_charge_pump_always_on,
    //!< The mux charge pump remains ON during the firing/capture sequence
    USS_measurement_mux_charge_pump_always_off,
    //!< The mux charge pump remains OFF during the firing/capture sequence
}USS_measurement_mux_charge_pump;


//! \brief Lists pulse generation options
//!
typedef enum _USS_measurement_pulse_generation_mode_
{
    USS_measurement_pulse_generation_mode_single_tone,
    //!< Generate a single tone pulse
    USS_measurement_pulse_generation_mode_dual_tone,
    //!< Generate a dual tone pulse
    USS_measurement_pulse_generation_mode_multi_tone,
    //!< Generate a multi tone pulse
} USS_measurement_pulse_generation_mode;

//! \brief Selects USS channel to be used for test (Used for Application Debug
//! API)
//!
typedef enum _USS_test_channel_
{
	USS_test_channel_0 = 0x0000,
	//!< Select USS Channel 0
	USS_test_channel_1 = 0x0001
	//!< Select USS Channel 1
} USS_test_channel;

//! \brief Allows user to select USS capture sequence.
//!
typedef enum _USS_measurement_capture_sequence_
{
	USS_measurement_capture_sequence_ch0 = 0,
	//!< Configures ASQ for single channel capture
	USS_measurement_capture_sequence_ch1 = 1,
	USS_measurement_capture_sequence_ch0_ch1 = 2,
	//!< Configures ASQ for double channel capture
	USS_measurement_capture_sequence_ch1_ch0 = 3,
	USS_measurement_capture_sequence_ch0_ch0 = 4,
	USS_measurement_capture_sequence_ch1_ch1 = 5,
	USS_measurement_capture_sequence_ch0_ch1_ch0 = 6,
	//!< Configures ASQ for triple channel capture
	USS_measurement_capture_sequence_ch1_ch0_ch1 = 7,
	USS_measurement_capture_sequence_ch0_ch0_ch0 = 8,
	USS_measurement_capture_sequence_ch1_ch1_ch1 = 9,
	USS_measurement_capture_sequence_ch0_ch1_ch0_ch1 = 10,
	//!< Configures ASQ for four channel capture
	USS_measurement_capture_sequence_ch1_ch0_ch1_ch0 = 11,
	USS_measurement_capture_sequence_ch0_ch0_ch0_ch0 = 12,
	USS_measurement_capture_sequence_ch1_ch1_ch1_ch1 = 13
} USS_measurement_capture_sequence;

//! \brief This is the USS state after capture is done
//!
typedef enum _USS_measurement_end_of_sequence_state_
{
	USS_measurement_end_of_sequence_state_standby = 0,
	//!< USS goes to STANDBY state after capture is done
	USS_measurement_end_of_sequence_state_power_off = ESOFF,
	//!< USS goes to POWER OFF state after capture is done
	USS_measurement_end_of_sequence_state_ready = STDBY+ ESOFF
	//!< USS stays in READY state after capture is done
} USS_measurement_end_of_sequence_state;

//! \brief Configures SDHS Over Sample Rate
//!    \n \n <b>Registers modified:</b> <i>SDHS.CTL0.OBR</i>
typedef enum _USS_Capture_Over_Sample_Rate_
{
	USS_Capture_Over_Sample_Rate_10 = 0x0000,
	//!< Configures SDHS Over Sample Rate to 10
	USS_Capture_Over_Sample_Rate_20 = 0x0001,
	//!< Configures SDHS Over Sample Rate to 20
	USS_Capture_Over_Sample_Rate_40 = 0x0002,
	//!< Configures SDHS Over Sample Rate to 40
	USS_Capture_Over_Sample_Rate_80 = 0x0003,
	//!< Configures SDHS Over Sample Rate to 80
	USS_Capture_Over_Sample_Rate_160 = 0x0004
	//!< Configures SDHS Over Sample Rate to 160
} USS_Capture_Over_Sample_Rate;

//! \brief Selects Programmable Gain Amplifier (PGA) range
//!
typedef enum _USS_Capture_Gain_Range_
{
	USS_Capture_Gain_Range_minus_6_5 = 17,
	//!< Configure PGA range to -6.5 dB
	USS_Capture_Gain_Range_minus_5_5 = 18,
	//!< Configure PGA range to -5.5 dB
	USS_Capture_Gain_Range_minus_4_6 = 19,
	//!< Configure PGA range to -4.6 dB
	USS_Capture_Gain_Range_minus_4_1 = 20,
	//!< Configure PGA range to -4.1 dB
	USS_Capture_Gain_Range_minus_3_3 = 21,
	//!< Configure PGA range to -3.3 dB
	USS_Capture_Gain_Range_minus_2_3 = 22,
	//!< Configure PGA range to -2.3 dB
	USS_Capture_Gain_Range_minus_1_4 = 23,
	//!< Configure PGA range to -1.4 dB
	USS_Capture_Gain_Range_minus_0_8 = 24,
	//!< Configure PGA range to -0.8 dB
	USS_Capture_Gain_Range_0_1 = 25,
	//!< Configure PGA range to 0.1 dB
	USS_Capture_Gain_Range_1_0 = 26,
	//!< Configure PGA range to 1.0 dB
	USS_Capture_Gain_Range_1_9 = 27,
	//!< Configure PGA range to 1.9 dB
	USS_Capture_Gain_Range_2_6 = 28,
	//!< Configure PGA range to 2.6 dB
	USS_Capture_Gain_Range_3_5 = 29,
	//!< Configure PGA range to 3.5 dB
	USS_Capture_Gain_Range_4_4 = 30,
	//!< Configure PGA range to 4.4 dB
	USS_Capture_Gain_Range_5_2 = 31,
	//!< Configure PGA range to 5.2 dB
	USS_Capture_Gain_Range_6_0 = 32,
	//!< Configure PGA range to 6.0 dB
	USS_Capture_Gain_Range_6_8 = 33,
	//!< Configure PGA range to 6.8 dB
	USS_Capture_Gain_Range_7_7 = 34,
	//!< Configure PGA range to 7.7 dB
	USS_Capture_Gain_Range_8_7 = 35,
	//!< Configure PGA range to 8.7 dB
	USS_Capture_Gain_Range_9_0 = 36,
	//!< Configure PGA range to 9.0 dB
	USS_Capture_Gain_Range_9_8 = 37,
	//!< Configure PGA range to 9.8 dB
	USS_Capture_Gain_Range_10_7 = 38,
	//!< Configure PGA range to 10.7 dB
	USS_Capture_Gain_Range_11_7 = 39,
	//!< Configure PGA range to 11.7 dB
	USS_Capture_Gain_Range_12_2 = 40,
	//!< Configure PGA range to 12.2 dB
	USS_Capture_Gain_Range_13_0 = 41,
	//!< Configure PGA range to 13.0 dB
	USS_Capture_Gain_Range_13_9 = 42,
	//!< Configure PGA range to 13.9 dB
	USS_Capture_Gain_Range_14_9 = 43,
	//!< Configure PGA range to 14.9 dB
	USS_Capture_Gain_Range_15_5 = 44,
	//!< Configure PGA range to 15.5 dB
	USS_Capture_Gain_Range_16_3 = 45,
	//!< Configure PGA range to 16.3 dB
	USS_Capture_Gain_Range_17_2 = 46,
	//!< Configure PGA range to 17.2 dB
	USS_Capture_Gain_Range_18_2 = 47,
	//!< Configure PGA range to 18.2 dB
	USS_Capture_Gain_Range_18_8 = 48,
	//!< Configure PGA range to 18.8 dB
	USS_Capture_Gain_Range_19_6 = 49,
	//!< Configure PGA range to 19.6 dB
	USS_Capture_Gain_Range_20_5 = 50,
	//!< Configure PGA range to 20.5 dB
	USS_Capture_Gain_Range_21_5 = 51,
	//!< Configure PGA range to 21.5 dB
	USS_Capture_Gain_Range_22_0 = 52,
	//!< Configure PGA range to 22.0 dB
	USS_Capture_Gain_Range_22_8 = 53,
	//!< Configure PGA range to 22.8 dB
	USS_Capture_Gain_Range_23_6 = 54,
	//!< Configure PGA range to 23.6 dB
	USS_Capture_Gain_Range_24_6 = 55,
	//!< Configure PGA range to 24.6 dB
	USS_Capture_Gain_Range_25_0 = 56,
	//!< Configure PGA range to 25.0 dB
	USS_Capture_Gain_Range_25_8 = 57,
	//!< Configure PGA range to 25.8 dB
	USS_Capture_Gain_Range_26_7 = 58,
	//!< Configure PGA range to 26.7 dB
	USS_Capture_Gain_Range_27_7 = 59,
	//!< Configure PGA range to 27.7 dB
	USS_Capture_Gain_Range_28_1 = 60,
	//!< Configure PGA range to 28.1 dB
	USS_Capture_Gain_Range_28_9 = 61,
	//!< Configure PGA range to 28.9 dB
	USS_Capture_Gain_Range_29_8 = 62,
	//!< Configure PGA range to 29.8 dB
	USS_Capture_Gain_Range_30_8 = 63
	//!< Configure PGA range to 30.8 dB
} USS_Capture_Gain_Range;

//! \brief Selects Programmable Gain Amplifier (PGA) range
//!
typedef enum _USS_Capture_Calibration_
{
	USS_Capture_Calibration_disabled =0x0100,
	USS_Capture_Calibration_enabled = 0x8100
} USS_Capture_Calibration;

//! \brief Selects USS trigger option
//!
typedef enum _USS_Trigger_Configuration_Trigger_
{
	USS_Triger_Configuration_Software_Trigger = 0x00,
	//!< USS module is triggered via software trigger (User must use
	//!< USS_startUltrasonicCapture API to trigger an USS capture)
} USS_Trigger_Configuration_Trigger;

//! \brief Selects USS Crystal Frequency
//!
typedef enum _USS_HSPLL_input_clock_freq_
{
    USS_HSPLL_input_clock_freq_4_MHz,
    //!< Select this option if USS Crystal is 4 MHz
    USS_HSPLL_input_clock_freq_8_MHz,
    //!< Select this option if USS Crystal is 8 MHz
} USS_HSPLL_input_clock_freq;

//! \brief Selects USS HSPLL input clock Type
//!
typedef enum _USS_HSPLL_input_clock_type_
{
	USS_HSPLL_input_clock_type_crystal = 0x0000,
	//!< Select this option if HSPLL input clock is a high accuracy crystal
	USS_HSPLL_input_clock_type_ceramic_resonator = 0x0200
	//!< Select this option if HSPLL input clock is a ceramic resonator
} USS_HSPLL_input_clock_type;


//! \brief Selects USS HSPLL output clock frequency
//!
typedef enum _USS_HSPLL_output_clk_freq_
{
	USS_HSPLL_output_clk_freq_80_MHz = 0,
	//!< USS HSPLL output frequency of 80 MHz
	USS_HSPLL_output_clk_freq_79_MHz = 1,
	//!< USS HSPLL output frequency of 79 MHz
	USS_HSPLL_output_clk_freq_78_MHz = 2,
	//!< USS HSPLL output frequency of 78 MHz
	USS_HSPLL_output_clk_freq_77_MHz = 3,
	//!< USS HSPLL output frequency of 77 MHz
	USS_HSPLL_output_clk_freq_76_MHz = 4,
	//!< USS HSPLL output frequency of 76 MHz
	USS_HSPLL_output_clk_freq_75_MHz = 5,
	//!< USS HSPLL output frequency of 75 MHz
	USS_HSPLL_output_clk_freq_74_MHz = 6,
	//!< USS HSPLL output frequency of 74 MHz
	USS_HSPLL_output_clk_freq_73_MHz = 7,
	//!< USS HSPLL output frequency of 73 MHz
	USS_HSPLL_output_clk_freq_72_MHz = 8,
	//!< USS HSPLL output frequency of 72 MHz
	USS_HSPLL_output_clk_freq_71_MHz = 9,
	//!< USS HSPLL output frequency of 71 MHz
	USS_HSPLL_output_clk_freq_70_MHz = 10,
	//!< USS HSPLL output frequency of 70 MHz
	USS_HSPLL_output_clk_freq_69_MHz = 11,
	//!< USS HSPLL output frequency of 69 MHz
	USS_HSPLL_output_clk_freq_68_MHz = 12,
	//!< USS HSPLL output frequency of 68 MHz
} USS_HSPLL_output_clk_freq;


//! \brief Selects appropiate USS SDHS interrupt (Used to register interrupt
//!           callback functions)
//!
typedef enum USS_SDHS_Interrupt
{
	USS_SDHS_Interrupt_OVF= 0,
	//!< Interrupt for SDHS Data Overflow
	USS_SDHS_Interrupt_ACQDONE= 1,
	//!< Interrupt for SDHS Over Input Level
	USS_SDHS_Interrupt_SSTRG= 3,
	//!< Interrupt for SDHS Start conversion Trigger
	USS_SDHS_Interrupt_DTRDY = 4,
	//!< Interrupt for SDHS Data Ready
	USS_SDHS_Interrupt_WINHI = 5,
	//!< Interrupt for SDHS Window High
	USS_SDHS_Interrupt_WINLO = 6
	//!< Interrupt for SDHS Window Low
} USS_SDHS_Interrupt;


//! \brief Selects appropiate USS SAPH interrupt (Used to register interrupt
//!           callback functions)
//!
typedef enum USS_SAPH_Interrupt
{
	USS_SAPH_Interrupt_DATAERR= 0, 
	//!< Interrupt for data error pending
	USS_SAPH_Interrupt_TMFTO= 1,  
	//!< Interrupt for Time Mark F time out
	USS_SAPH_Interrupt_SEQDN= 2,   
	//!< Interrupt for sequence acquisition done
	USS_SAPH_Interrupt_PNGDN= 3    
	//!< Interrupt for ping transmit done
} USS_SAPH_Interrupt;

//! \brief Used to configure USS UUPS interrupt callback
//!
typedef enum USS_UUPS_Interrupt
{
	USS_UUPS_Interrupt_PREQIG = 0,  
	//!< Interrupt for Power Request Ignored
	USS_UUPS_Interrupt_PTMOUT = 1,
	//!< Interrupt for UUPS Power Up Time Out
	USS_UUPS_Interrupt_STPBYDB = 2
	//!< Interrupt for UUPS to indicate USS has been interrupted by debug
} USS_UUPS_Interrupt;


//! \brief Used to configure USS HSPLL interrupt callback
//!
typedef enum USS_HSPLL_Interrupt
{
	USS_HSPLL_Interrupt_PLLUNLOCK = 0,  
	//!< Interrupt for PLL Unlock
} USS_HSPLL_Interrupt;

//! \brief Defines high flow computation options
typedef enum _USS_highFlow_option_ {
    USS_highFlow_option_disabled = 0,
    //!< High flow version of the algorithms are disabled. No cycle slip
    //!< correction is performed.
    USS_highFlow_option_version1 = 1,
    //!< High flow version of the algorithms is disabled. Cycle slip
    //!< correction method 1 is performed.
    USS_highFlow_option_version2 = 2
    //!< High flow version of the algorithms is disabled. Cycle slip
    //!< correction method 2 is performed.
} USS_highFlow_option;

//! \brief Selects the desired power mode while USS capture is done
//!
typedef enum _USS_low_power_mode_option_
{

	USS_low_power_mode_option_low_power_mode_0 = LPM0_bits,
	//!< MSP remains in LPM0 power mode until timer expires
	USS_low_power_mode_option_low_power_mode_1 = LPM1_bits,
	//!< MSP remains in LPM1 power mode until timer expires
	USS_low_power_mode_option_low_power_mode_2 = LPM2_bits,
	//!< MSP remains in LPM2 power mode until timer expires
	USS_low_power_mode_option_low_power_mode_3 = LPM3_bits
	//!< MSP remains in LPM3 power mode until timer expires
} USS_low_power_mode_option;

//! \brief Selects the interval to calculate Delta ToF and Volume Flow
//!
typedef enum _USS_dtof_volume_flow_calculation_interval_
{
	USS_dtof_volume_flow_calculation_interval_1 = 1,
	//!< DeltaToF and volume flow are calculated every time USS_runAlgorithms or
	//!< USS_runAlgorithmsFixedPoint are invoked
	USS_dtof_volume_flow_calculation_interval_2 = 2,
	//!< DeltaToF and volume flow are calculated based on accumulation of 2 and
	//!< scaling of 1 bit upstream and downstream captures.
	USS_dtof_volume_flow_calculation_interval_4 = 4,
	//!< DeltaToF and volume flow are calculated based on accumulation of 4 and
	//!< scaling of 2 bits upstream and downstream captures.
	USS_dtof_volume_flow_calculation_interval_8 = 8
	//!< DeltaToF and volume flow are calculated based on accumulation of 8 and
	//!< scaling of 3 bits upstream and downstream captures.
} USS_dtof_volume_flow_calculation_interval;

//! \brief Selects the UPS/DNS DC offset estimation mode used by
//! USS_estimateDCoffset API
//!
typedef enum _USS_dcOffEst_Calc_Mode_
{
    USS_dcOffEst_Calc_Mode_use_existing_UPS_DNS_capture,
    //!< In this mode USS_estimateDCoffset API will use the existing UPS/DNS
    //!< capture and will not trigger a new UPS/DNS capture sequence to estimate
    //!< UPS/DNS DC offset UPS/DNS DC offset. This mode is recommended when the
    //!< DC offset estimation API is called immediately after a
    //!< USS_startUltrasonicMeasurement or USS_startLowPowerUltrasonicCapture
    //!< API
    USS_dcOffEst_Calc_Mode_trigger_UPS_DNS_capture_controlled_by_ASQ,
    //!< In this mode USS_estimateDCoffset API will use will trigger a new
    //!< UPS/DNS capture as part of DC offset cancellation using
    //!< USS_startUltrasonicMeasurement API. This mode is recommended if
    //!< USS_startUltrasonicMeasurement API is used in the application and
    //!< USS_estimateDCoffset API is not called immediately after
    //!< USS_startUltrasonicMeasurement API
    USS_dcOffEst_Calc_Mode_trigger_UPS_DNS_capture_using_LPM_capture,
    //!< In this mode USS_estimateDCoffset API will use will trigger a new
    //!< UPS/DNS capture as part of DC offset cancellation using
    //!< USS_startLowPowerUltrasonicCapture API. This mode is recommended if
    //!< USS_startLowPowerUltrasonicCapture API is used in the application and
    //!< USS_estimateDCoffset API is not called immediately after
    //!< USS_startLowPowerUltrasonicCapture API
} USS_dcOffEst_Calc_Mode;


//! \brief Enumerated type for absTOF algorithm selection
//!
typedef enum _USS_Alg_AbsToF_Calculation_Option_{
    USS_Alg_AbsToF_Calculation_Option_lobe = 0,
    //!< Calculates absTOF using lobe method.
    USS_Alg_AbsToF_Calculation_Option_hilbert = 1,
    //!< Calculates absTOF using Hilbert method.
    USS_Alg_AbsToF_Calculation_Option_lobeWide = 2,
    //!< Calculate absTOF using lobe wide method.
    USS_Alg_AbsToF_Calculation_Option_hilbertWide = 3
    //!< Calculate absTOF using hilbert wide method.
} USS_Alg_AbsToF_Calculation_Option;

//! \brief Enumerated type for dToF algorithm selection.
//!
typedef enum _USS_Alg_dToF_Calculation_Option_{
    USS_Alg_dToF_Calculation_Option_estimate = 0,
    USS_Alg_dToF_Calculation_Option_water,
} USS_Alg_dToF_Calculation_Option;

//! \brief Enumerated type for volume flow rate algorithm selection.
//!
typedef enum _USS_Alg_volume_flow_Calculation_Option_{
    USS_Alg_volume_flow_Calculation_Option_water = 0,
    USS_Alg_volume_flow_Calculation_Option_gas = 1,
    USS_Alg_volume_flow_Calculation_Option_generic = 1,
} USS_Alg_volume_flow_Calculation_Option;

//! \brief Contain meter constant calibration data
//!
typedef struct _USS_Algorithms_Meter_Constant_Calib_
{
    float  alphaMin;
    //!< specifies range limit
    float slope;
    //!< meter calibration constant
    float offset;
    //!< delta time of flight calibration constant
}USS_Algorithms_Meter_Constant_Calib;


//! \brief Contains USS SW Lib System Configuration
//!
typedef struct _USS_System_Configuration_
{
	uint32_t mCLKFrequency;
	//!< System Clock (MCLK) Frequency in Hz
	uint16_t LFXTFrequency;
	//!< Low Frequency Crystal (LFXT) frequency in Hz
	uint16_t timerBaseAddress;
	//!< Base address of Timer to be used to generate USS delays
	//!< Possible options:
	//!< - TIMER_A0_BASE
	//!< - TIMER_A1_BASE
	//!< - TIMER_A2_BASE
	//!< - TIMER_A3_BASE
	//!< - TIMER_A4_BASE
	USS_diagnostics_mode diagMode;
	//!< Used to USS SW Library diagnostic mode.
} USS_System_Configuration;

//! \brief Structure to configure additional multi tone pulse generation
typedef struct _USS_Pulse_Additional_End_of_Sequence_Configuration_
{
    uint16_t numOfTrillcycles;
    //!< En e-pulse followed by an x-pulse sequence is considered a trill cycle
    uint16_t * pNumOfEPulsArray;
    uint16_t * pEPulsLPerArray;
    uint16_t * pEPulsHPerArray;
    uint16_t * pNumOfXPulsArray;
    uint16_t * pXPulsLPerArray;
    uint16_t * pXPulsHPerArray;
}USS_Pulse_Additional_End_of_Sequence_Configuration;

//! \brief structure to configure multi tone pulse generation
typedef struct _USS_Pulse_Multitone_Configuration_
{
    uint16_t origNumOfThrillCycles;
    //!< This value specifies the number of thrill cycles (An e-pulse followed
    //!< by an x-pulse sequence is considered a trill cycle) requested by the
    //!< user. NOTE: is multi-tone frequency is above 420KHz then number of
    //!< thrill cycle might not be the originally requested by the user.
    //!< For example if the user originally requested 10 pulses from 460-520 KHz
    //!< this requires 5 single pulse thrill cycles. However, since the pulses
    //!< are above 420KHz threshold (when generating multi-tone frequencies the
    //!< MCU must be running at 16MHz), then the thrill pulse configuration is modified
    //!< by the library to generate 3 thrill pulses in 2 pulse configuration. In
    //!< total 12 pulses will be generated. The modified number of thrill cycles
    //!< will be available via numOfTrillcycles parameter
    uint16_t numOfTrillcycles;
    //!< Specifies the actual number of thrill pulses when using multi tone
    //!< generation
    uint16_t * pNumOfEPulsArray;
    //!< Pointer to number of e-pulses array. This array is automatically
    //!< configured when calling USS_configureUltrasonicMeasurement API
    uint16_t * pEPulsLPerArray;
    //!< Pointer to number of e-pulses array. This array is automatically
    //!< configured when calling USS_configureUltrasonicMeasurement API
    uint16_t * pEPulsHPerArray;
    //!< Pointer to number of e-pulses high period array. This array is
    //!< automatically configure when calling USS_configureUltrasonicMeasurement
    //!< API
    uint16_t * pNumOfXPulsArray;
    //!< Pointer to number of x-pulses array. This array is
    //!< automatically configure when calling USS_configureUltrasonicMeasurement
    //!< API
    uint16_t * pXPulsLPerArray;
    //!< Pointer to number of x-pulses low period array. This array is
    //!< automatically configure when calling USS_configureUltrasonicMeasurement
    //!< API
    uint16_t * pXPulsHPerArray;
    //!< Pointer to number of x-pulses high period array. This array is
    //!< automatically configure when calling USS_configureUltrasonicMeasurement
    //!< API
    uint16_t * pRAMPulseConfigArray;
    //!< Pointer to RAM memory array used configure multi tone. This array is
    //!< automatically configure when calling USS_configureUltrasonicMeasurement
    //!< API
    uint16_t * pRAMDMAConfigArray;
    //!< Pointer to RAM memory array used reconfigure DMA2 and DMA3. This array
    //!< is automatically configure when calling USS_configureUltrasonicMeasurement API
    USS_Pulse_Additional_End_of_Sequence_Configuration *pEOFSeqConfig;
    //!< Pointer to USS_Pulse_Additional_End_of_Sequence_Configuration.
    //!< This sequence is appended to the end of the main multitone configuration
}USS_Pulse_Multitone_Configuration;

//! \brief The following structure allows user to specify single and dual tone
//! configuration
typedef struct _USS_Pulse_Single_Dual_Tone_Configuration_
{
    uint8_t F1DutyCycle;
    //!< In single tone mode, it specifies the single tone duty cycle.
    //!< In dual tone mode, it specifies the first tone duty cycle.
    uint8_t F2DutyCycle;
    //!< In dual tone mode, it specifies the second tone duty cycle.
    //!< In single tone mode, this parameter is not used.
    uint8_t numOfF1Cycles;
    //!< In single tone, specifies the number on the single tone cycles.
    //!< In dual tone, it specifies the number of first tone cycles.
    uint8_t numOfF2Cycles;
    //!< In dual tone, specifies the number cycles for the second tone cycles.
    //!< In single tone mode, this parameter is not used.
}USS_Pulse_Single_Dual_Tone_Configuration;

//! \brief Allows user to specify the pulse configuration
typedef struct _USS_Pulse_Configuration_
{
    USS_measurement_pulse_generation_mode pulseGenMode;
    //!< Determines the type of pulse to be generated.
    USS_measurement_pause_state pauseState;
    //!< Specifies the line state during the acquisition sequence
    USS_measurement_pulse_polarity pulsePolarity;
    //!< Allows user to control pulse polarity
    uint32_t F1Frequency;
    //!< In single tone mode, specifies the single tone frequency
    //!< In dual tone mode, specifies the first tone frequency
    //!< In multi tone tone mode, specifies the start frequency for the tone
    uint32_t F2Frequency;
    //!< In single tone mode, this parameter is not used
    //!< In dual tone mode, specifies the second tone frequency
    //!< In multi tone tone mode, specifies the stop frequency for the tone.
    void *pToneConfig;
    //!< Pointer to USS_Pulse_Single_Dual_Tone_Configuration or
    //!< USS_Pulse_Multitone_Configurationthe tone configuration structure.
    //!< This pointer structure should match the USS_measurement_pulse_generation_mode.
    //!< For USS_measurement_pulse_generation_mode_single_tone and
    //!< USS_measurement_pulse_generation_mode_dual_tone pointer should reference
    //!< USS_Pulse_Single_Dual_Tone_Configuration structure.
    //!< For USS_measurement_pulse_generation_mode_multi_tone
    //!< pointer should reference a USS_Pulse_Multitone_Configuration structure.
    uint8_t numOfStopPulses;
    //!< Specifies the number of stop pulses.
    //!< Minimum: 0
    //!< Maximum: 15
}USS_Pulse_Configuration;


//! \brief Contains USS SW Lib Meter Configuration
//!
typedef struct _USS_Meter_Configuration_
{
	float volumeScaleFactor; 
	//!< Volume scale factor (Area x Length) length^3.
	//!< This value is used by the library to calculate volume flow rate and is 
	//!< converted to iq21 format as follows
	//!< _IQ21(volumeScaleFactor/1e6). Therefore, to prevent saturation of 
	//!< volumeflow rate calculation volumeScaleFactor should fall beetween 
	//!< this range [-1.024e9 and 1.023e9]
	float acousticLength;    
	//!< Acoustic length in usec at room temperature
	uint32_t transducerFreq; 
	//!< Transducer Frequency in Hz
} USS_Meter_Configuration;


//! \brief Contains USS SW Lib Measurement Configuration
//!
typedef struct _USS_Measurement_Configuration_
{
	USS_measurement_capture_sequence sequenceSelection;
	USS_measurement_end_of_sequence_state eofSequenceState;
	USS_measurement_drive_strength ch0DriveStrength;
	USS_measurement_drive_strength ch1DriveStrength;
	USS_Pulse_Configuration *pulseConfig;
	uint_least16_t startPPGCount;
	//!< This count determines the time to start the PPG pulse trigger. The
	//!< count is based on <b>1 ASQ clock = (HSPLL_CLOCK_FREQ/16)</b> period.
	//!< For examples if startPPGCount = 15 and ASQ clock is 68 MHz, then PPG
	//!< pulse will be triggered \n
	//!< (16/68MHz)*(1)*(startPPGCount) = (16/68MHz)*(1)*(15)= 3.52 usec, after \n
	//!< acquisiton sequence has been triggered. \n\n
	//!< \"startPPGCount\" valid range (0x0002 - 0xFFFF)
	//!< \n \n <b>Registers modified:</b> <i>SAPH.ATM_A.TimeMarkA</i>
	uint_least16_t turnOnADCCount;
	//!< This count determines the time to arm (turn on ADC). This
	//!< count is based on <b>1 ASQ clock = (HSPLL_CLOCK_FREQ/16)</b> period.
	//!< For examples if turnOnADCCount = 15 and ASQ clock is 68 MHz, then ADC
	//!< will be turned on \n
	//!< (1/ASQ_clock_freq)*(1)*(turnOnADCCount)= (1/68MHz)*(1)*(15) =0.22 usec,
	//!< \n
	//!< after acquisiton sequence has been triggered. \n\n
	//!< \"turnOnADCCount\" valid range (0x0002 - 0xFFFF)
	//!< \n \n <b>Registers modified:</b> <i>SAPH.ATM_B.TimeMarkB</i>
	uint_least16_t startPGAandINBiasCount;
	//!< This count determines the time start the PGA and input biasing. This
	//!< count is based on <b>1 ASQ clock = (HSPLL_CLOCK_FREQ/16)</b> period.
	//!< For examples if turnOnADCCount = 15 and ASQ clock is 68 MHz, then ADC
	//!< will be turned on \n
	//!< (1/ASQ_clock_freq)*(1)*(turnOnADCCount)= (1/68MHz)*(1)*(15)=0.22 usec,
	//!< \n
	//!< after acquisiton sequence has been triggered. \n\n
	//!< \"turnOnADCCount\" valid range (0x0002 - 0xFFFF)
	//!< \n \n <b>Registers modified:</b> <i>SAPH.ATM_C.TimeMarkC</i>
	uint_least16_t startADCsamplingCount;
	//!< This count determines the time to start the ADC acquisition. This
	//!< count is based on <b>1 ASQ clock = (HSPLL_CLOCK_FREQ/16)</b> period.
	//!< For examples if turnOnADCCount = 15 and ASQ clock is 68 MHz, then ADC
	//!< sampling will start \n
	//!< (1/ASQ_clock_freq)*(1)*(startADCsamplingCount) = (1/68MHz)*(1)*(15)
	//!< = 0.22 usec \n
	//!< after acquisiton sequence has been triggered. \n\n
	//!< \"startADCsamplingCount\" valid range (0x0002 - 0xFFFF)
	//!< \n \n <b>Registers modified:</b> <i>SAPH.ATM_D.TimeMarkD</i>
	uint_least16_t restartCaptureCount;
	//!< This count determined the time to trigger next capture. This
	//!< count is based on <b>16 ASQ clock= (HSPLL_CLOCK_FREQ/256)</b> period.
	//!< For examples if turnOnADCCount = 15 and ASQ clock is 68 MHz, then next
	//!< capture will be triggered \n
	//!< (16/ASQ_clock_freq)*(16)*(startADCsamplingCount) = (16/68MHz)*(16)*(15)
	//!< = 56.4 usec\n
	//!< after acquisiton sequence has been triggered. \n\n
	//!< \"restartCaptureCount\" valid range (0x0002 - 0xFFFF)
	//!< \n \n <b>Registers modified:</b> <i>SAPH.ATM_E.TimeMarkE</i>
	uint_least16_t captureTimeOutCount;
	//!< This count determined the time from start to acknowledge of acquisition
	//!< data complete. This count is based on <b>4 ASQ clock = (HSPLL_CLOCK_FREQ/64)</b> period.
	//!< For examples if turnOnADCCount = 15 and ASQ clock is 68 MHz, then time
	//!< out event will happen \n
	//!< (1/ASQ_clock_freq)*(64)*(startADCsamplingCount) = (16/68MHz)*(4)*(15)
	//!< = 14.08 usec \n
	//!< after acquisiton sequence has been triggered. \n\n
	//!< \"captureTimeOutCount\" valid range (0x0002 - 0xFFFF)
	//!< \n \n <b>Registers modified:</b> <i>SAPH.ATM_F.TimeMarkF</i>
	uint_least16_t restartLowPowerCaptureCount;
	//!< This should be calculated using the following formula
	//!< restartLowPowerCaptureCount=F1/F2*restartCaptureCnt where (F1=PLL/16, F2=32k)
	USS_measurement_ULP_bias_delay ulpBiasDelay;
	//!< Allows user to specify delay between triggering an acquisition sequence
	//!< and starting the acquisition sequence. This delay is used to allow PGA
	//!< to settle.
	USS_measurement_bias_impedance biasImpedance;
	//!< Allows user to control bias channel bias impedance
	USS_measurement_mux_charge_pump muxChargePumpMode;
	//!< Allows user to control multiplexer charge pump during capture
	uint16_t startRxEnCount;
	//!< This parameter is only required when controlling external AFE
	//!< startRxEnCount are based of SMCLK cycles
} USS_Measurement_Configuration;

//! \brief Contains USS SW Lib HSPLL Configuration
//!
typedef struct _USS_HSPLL_Configuration_
{
    USS_HSPLL_input_clock_freq ussXtalFreq;
	USS_HSPLL_input_clock_type HSPLLInputCLKType;
	USS_HSPLL_output_clk_freq pllOutputFreq;
	//!< If this paramater is modified during runtime, two update APIs need to
	//!< called; USS_updateHSPLLConfiguration and USS_updateSDHSCapture.
	//!< USS_updateSDHSCapture needs to be select the correct modulator
	//!< optimization bits and obtain maximum performance of the SDHS
	//!< based on the new HSPLL frequency.
	//!< \n \n <b>Registers modified:</b><i>HSPLL.CTL.PLLM</i> and
	//!< <i>SAPH.CTL7.MODOPTI</i>
	bool outputPLLXtal;
	//!< true = outputs HSPLL crystal , false = does not outputs HSPLL crystal
	//!< \n \n <b>Registers modified:</b><i>HSPLL.PLLXTCTL.XTOUTOFF</i>
	uint16_t hspllTolerance;
	//!< number of hspll cycle variance tolerance. Value can be between 0-255
	//!< This parameter is required when calling USS_verifyHSPLLFrequency
	uint16_t ussXTALsettlingCount;
	//!< Specifies the number of LFXT cycles needed to allow USS XT to settle.
	//!< ussXTALsettlelingCount = ceiling[(LFXTFrequency_inHz)*(settling_in_Sec)]
	//!< For example if settling time for USSXT is  120usec and LFXTFrequency
	//!< is configure for 32768 Hz. Then
	//!< ussXTALsettlelingCount = ceiling[(32768)*(120e-6)] = 4
	//!< Please refer to the device datasheet for recommended USSXT settling
	//!< time
	//!< ussXTALsettlelingCount must a non-zero positive value
    //!< Please also refer to USS_startLowPowerUltrasonicCapture API for more
    //!< detail regarding this parameter usage during low power capture.
} USS_HSPLL_Configuration;

//! \brief This structure is passed to USS_calculateOffsets API and define the
//!    test calibration configuration
typedef struct _USS_dTof_absTof_offset_test_config_
{
    uint16_t numOfTestIterations;
	//!< Specifies the number of time to run offset calibration. Setting a value
    //!< of 0 will run the calibration one, setting a value of 1 will run
    //!< calibration twice, etc.
    //!< minimum: 0
    //!< maximum: 64999
    bool isUseLPMCapture;
	//!< Specifies the type of capture to use for offset calibration test
    //!< true  = invokes USS_startLowPowerUltrasonicCapture
    //!< false = invokes USS_startUltrasonicMeasurement
    bool isCalculateUpsAbsTofOffset;
	//!< Specifies if calibration returns averaged Absolute Time of Flight for UPS
    //!< channel
    //!< true  = averaged Absolute Time of Flight for UPS is calculated
    //!< false = averaged Absolute Time of Flight for UPS is not calculated
    bool isCalculateDnsAbsTofOffset;
    //!< Specifies if calibration returns averaged Absolute Time of Flight for DNS
    //!< channel
    //!< true  = averaged Absolute Time of Flight for DNS is calculated
    //!< false = averaged Absolute Time of Flight for DNS is not calculated
    bool isCalculateDToFOffset;
    //!< Specifies if calibration returns averaged Delta Time of Flight
    //!< true  =  averaged Delta Time of Flight is calculated
    //!< false =  averaged Delta Time of Flight is not calculated
} USS_dTof_absTof_offset_test_config;

//! \brief This structure is passed to USS_calculateOffsets API and calibration
//!        test result will be returned in this structure
typedef struct _USS_dTof_absTof_offset_results_
{
    float upsAbsToFOffset;
    //!< Contains the averaged Abstof computation for UPS channel if
    //!< USS_calculateOffsets API return USS_message_code_no_error and
    //!< USS_dTof_absTof_offset_test_config.isCalculateUpsAbsTofOffset is set to
    //!< true
    float dnsAbsToFOffset;
    //!< Contains the averaged Abstof computation for DNS channel if
    //!< USS_calculateOffsets API return USS_message_code_no_error and
    //!< USS_dTof_absTof_offset_test_config.isCalculateDnsAbsTofOffset is set to
    //!< true
    float dToFOffset;
    //!< Contains the averaged DToF computation if
    //!< USS_calculateOffsets API return USS_message_code_no_error and
    //!< USS_dTof_absTof_offset_test_config.isCalculateDToFOffset is set to
    //!< true
} USS_dTof_absTof_offset_results;


//! \brief This structure is passed to USS_verifyHSPLLFrequency API and test
//!    result will be returned using this structure
typedef struct _USS_calibration_hspll_results_
{
    uint32_t expectedResult;
    //!< Contains the expected Test count value. this is calculated based on
    //!< USS_HSPLL_Configuration  and USS_System_Configuration parameters
    uint32_t actualTestCount;
    //!< Contains the average SAPH timer counts in 1 ACLK period.
    int32_t    difference;
    //!< Contains the difference between expected and actualTestCount SAPH
} USS_calibration_hspll_results;

//! \brief Contains USS SW Lib Capture Configuration
//!
typedef struct _USS_Capture_Configuration_
{
	USS_Capture_Over_Sample_Rate overSampleRate;
	//!< Specifies the oversampling SDHS sample rate. This configuration in combination
	//!< with pllOutputFreq determines the signal sampling frequency.
	uint16_t sampleSize;
	//!< valid range (1-1024)
	//!< Configure number of SDHS samples to capture per acquisition sequence.
	//!< \n \n For example, if acquisition sequence selected is
	//!< \" <b>USS_measurement_capture_sequence_ch0_ch1</b> \" then a total of
	//!< <b>2</b>*sampleSize
	//!< SDHS samples will be captured at the end of the acquisition sequence.
	//!< If acquisition sequence selected is
	//!< \" <b>USS_measurement_capture_sequence_ch0_ch1_ch0</b> \" then a total
	//!< of <b>3</b>*sampleSize SDHS samples will be captured at the end of the
	//!< acquisition sequence.
	USS_Capture_Gain_Range gainRange;
	//!< Configures the internal gain amplifier.
	bool enableWindowHiComp;
	//!< true = enables Window Comparator, false = disables Window Comparator
	bool enableWindowLoComp;
	//!< true = enables Window Comparator, false = disables Window Comparator
	int16_t windowHighThreshold;
	//!< valid range (-2048 to 2047)
	//!< if enableWindowComp == true, window comparator high threshold is set to
	//!< user selected value. Otherwise, threshold values keep default reset
	//!< register value.
	int16_t windowLowThreshold;
	//!< valid range (-2048 to 2047)
	//!< if enableWindowComp == true, window comparator low threshold is set to
	//!< user selected value. Otherwise, threshold values keep default reset
	//!< register value.
	uint16_t agcConstant;
	//!< This value is use to calculate the Optimal Gain Amplifier setting
	//!< this value can be determined with the following formula
	//!< agcConstant = floor(20*log10(adcNom))=floor(20*log10(ADCmax * 10^(-B/20)))
	//!< where 	ADCmax = +/- 2^(12-1) for a 12 Bit ADC
	//!<		     B = number of db that the ADC output should be backoff (typically 3 db)
	//!< For USS module for B = 3,  agcConstant = floor(20*log10((2^11)*(10^(-3/20)))) = 63
	int16_t *pCapturesBuffer;
	//!< Pointer to reserved LEA memory block. Reserved memory block should have
	//!< a minimum size
	//!< (sampleSize*numberOfAcquisitions +  filterLength * 2) bytes and 32 bit
	//!< aligned.<br>
	//!< If USS_measurement_capture_sequence == USS_measurement_capture_sequence_ch0
	//!< then numberOfAcquisitions = 1.<br>
	//!< If USS_measurement_capture_sequence == USS_measurement_capture_sequence_chx_chx
	//!< then numberOfAcquisitions = 2.<br>
	//!< If USS_measurement_capture_sequence == USS_measurement_capture_sequence_chx_chx_chx
    //!< then numberOfAcquisitions = 3.<br>
	//!< If USS_measurement_capture_sequence == USS_measurement_capture_sequence_chx_chx_chx_chx
    //!< then numberOfAcquisitions = 4.<br>
	uint16_t maxCapture;
    //!< Specifies the maximum capture size.
	bool isCapAccumulationEnabled;
	//!< Pointer to LEA RAM were the sample will be accumulated. Number of
	//!< accumulations is  by dtofVolInterval in Algorithm configuration structure
	int16_t *pAccCaptureBuffer;
	//!< Pointer to reserved LEA memory block to perform capture accumulation.
	//!< Reserved memory block should have a minimum size
    //!< (sampleSize*numberOfAcquisitions * 2) bytes and 32 bit aligned.
} USS_Capture_Configuration;

//! \brief Contains USS SW Lib Trigger Configuration
//!
typedef struct _USS_Trigger_Configuration_
{
	USS_Trigger_Configuration_Trigger triggerConfig;
} USS_Trigger_Configuration;

//! \brief Contains USS SW Lib Capture Configuration
//!
typedef struct _USS_Interrupt_Configuration_
{
	bool enableUUPSPREQIGInterrupt;
	//!< true = enables UUPS PREQIG Interrupt,
	//!< false = disables UUPS PREQIG Interrupt
	bool enableSAPHPingTransmitDoneInterrupt;
	//!< true = enables SAPH PNGDN Interrupt,
	//!< false = disables SAPH PNGDN Interrupt
} USS_Interrupt_Configuration;


//! \brief Contains USS SW Lib Algorithms User Configuration
//!
typedef struct _USS_Algorithms_User_Configuration_
{
    uint16_t absTOFInterval;
        //!< This parameter determines the interval to calculate
        //!< absolute Time of Flight. The interval at which Absolute Time of Flight is calculated
        //!< has a dependency with dtofVolInterval configuration.

        //!< If dtofVolInterval == USS_dtof_volume_flow_calculation_interval_1 and
        //!< absTOFInterval == 1 absolute Time of Flight everytime
        //!< USS_runAlgorithmsFixedPoint or USS_runAlgorithms API are called.
        //!< If dtofVolInterval == USS_dtof_volume_flow_calculation_interval_2 and
        //!< absTOFInterval == 10 absolute Time of Flight is calculated the 20th
        //!< time USS_runAlgorithmsFixedPoint and USS_runAlgorithms are invoked.
        //!< The last calculation of Absolute time of flight is return until the
        //!< next update occurs. Increasing the value for "absTOFIteration" reduces
        //!< the average consumption of the application. (valid range 1-65535)
        float ADCAdditionalCaptureDelay;
        //!< The additional time it takes during firing and capturing the data,
        //!< which is not accounted for anywhere else.
        float  dcOffset;
        //!< The constant value calibrated by Zero flow drift (sec)
        float  ratioOfTrackLobeToPeak;
        //!< The ratio of side lobe closest to threshold and peak lobe amplitudes.
        uint8_t  numPulsesBeforeThreshold;
        //!< The number of pulses/cycles before selected threshold value
        bool  enableWindowing;
        //!< The flag to enable or disable windowing
        float  signalValueThresholdCheck;
        //!< The signal value check parameter which has to be crossed for a valid
        //!< signal
        uint8_t  numCycleSearchCorrPeak;
        //!< The number of cycles to search for correlation peak before setting
        //!< the flag Flag_corrOverrun
        uint16_t  searchLobeSampleSteps;
        //!< The number of samples to increment/decrement while searching/tracking
        //!< the side lobe
        float  maxRatioPeak2PeakVariation;
        //!< The maximum ratio of allowed peak to peak variation from one
        //!< measurement to another before setting flag for search.
        uint8_t  winStartIndexBackOffNumCycles;
        //!< The number of cycles to back off from start of received pulse
        //!< (derived from absTOF)
        uint8_t  winTrapzRampOverNumCycles;
        //!< The number of cycles over which the trapezoid window should ramp
        float  corrValueThresholdCheckFactor;
        //!< The factor of maximum lobe energy which has to be crossed for a
        //!< valid correlation value
        int16_t calibratedDCOffsetUPS;
        //!< This is a constant value which is subtracted to UPS captures
        //!< before running the Delta ToF algorithms. This parameter can be obtained
        //!< using USS_estimateDCoffset API
        int16_t calibratedDCOffsetDNS;
        //!< This is a constant value which is subtracted to DNS captures
        //!< before running the Delta ToF algorithms. This parameter can be obtained
        //!< using USS_estimateDCoffset API
        bool isInitAlgorithms;
        //!< This flag is used to indicate if the algorithms have been initialized.
        USS_dtof_volume_flow_calculation_interval dtofVolInterval;
        //!< This configuration specifies the interval at which delta ToF and volume
        //!< flow are calculated. The last calculated DToF and volume flow will be
        //!< return between intervals.
        int16_t *pLEAMemBuffer1;
        //!< pointer to LEA RAM memory reserved to place windowing coefficients.
        //!< memory minimum size (sampleSize*numberOfAcquisitions +  filterLength *
        //!< 2) bytes
        int32_t *pNvMemBuffer;
        //!< pointer to non volatile memory reserved for algorithm computations.
        //!< memory minimum size (sampleSize*2) bytes
        USS_highFlow_option highFlowOption;
        //!< Setting this configuration to true will enable highflow rate (> 5 gpm)
        //!< operation.
        //!< NOTE: enabling this option will increase power consumption
        float cycleSlipThreshold;
        //!< This setting is only need if USS_highFlow_option_version1 is selected
        //!< typical values are 1.1 and 1.2
        int32_t thresholdX1X3;
        _iq27 clockRelativeError;
        //!< This setting is used to correct AbsToF calculation
        //!< clockRelativeError = (clk_freq_measured-clock_freq_nominal)/clock_freq_nominal
        //!< Users can take advantage of USS_verifyHSPLLFrequency API. Which will
        //!< return the the expected timer counts and actual measured timer count
        //!< For more details about USS_verifyHSPLLFrequency implementation please
        //!< refer the Library API guide. Valid range is [_IQ27(-0.1),_IQ27(0.1)]
        //!< If USS_verifyHSPLLFrequency API is used then the error can be
        //!< calculated as follows:
        //!< gUssSWConfig.algorithmsConfig->clockRelativeError =
        //!<        _IQ27div((int32_t)(hspll_results.actualTestCount -
        //!<        hspll_results.expectedResult),hspll_results.expectedResult);
        uint32_t winNumCycles;
        //!< This parameter is used to control the windowLength when enableWindowing
        //!< is set to true.
        //!< winLength = (2* winRampLength) + (numSamplesPerCycle *winNumCycles)
        //!< Where,
        //!< winRampLength = numSamplesPerCycle * winTrapzRampOverNumCycles;
        //!< numSamplesPerCycle = sampleFreq/transducerFreq;
        bool enableFilter;
        //!< This parameter is used to enabling filtering of the signal before running
        //!< AbsToF, DeltaToF and Volume algorithms
        uint16_t filterLength;
        //!< This parameter is used to specify the filter length
        int16_t *filterCoeffs;
        //!< This parameter is used to provide the memory location were the filter coefficients
        //!< will be loaded from
        USS_Alg_AbsToF_Calculation_Option absToFOption;
        //!< User selection for Absolute Time of Flight calculation option.
        int16_t *pLEAMemBuffer2;
        //!< pointer to LEA RAM memory reserved temporary LEA operations.
        //!< This section of memory can be reused in the application for temporary
        //!< operations. The content of in this memory is not preserved during
        //!< when USS_run* functions are called.
        int16_t *hilbertTransformCoeffs;
        //!< Pointer to hilbert coefficients, coefficients must 32-bit aligned
        uint16_t hilbertTransformCoeffsLength;
        //!< Length of hilbert coefficients.
        int16_t negSearchRange;
        //!< Negative search range for tracking envelope maximum.
        int16_t posSearchRange;
        //!< Positive search range for tracking envelope maximum.
        float crossThreshold;
        //!< Threshold for tracking crossing point of envelope relative to envelope maximum.
        int16_t *pBinaryPattern;
        //!< Pointer to array containing binary reference pattern used by
        //!< USS_Alg_AbsToF_Calculation_Option_lobeWide and USS_Alg_AbsToF_Calculation_Option_hilbertWide
        //!< The library can use the binary pattern calculated by USS_generateMonoDualToneBinaryPattern
        //!< and USS_generateMultiToneBinaryPattern APIs or user can passed a
        //!< predefined binary pattern.
        uint16_t binaryPatternLength;
        //!< Length of binary pattern the length is automatically calculated by
        //!< USS_generateMonoDualToneBinaryPattern and USS_generateMultiToneBinaryPattern APIS
        //!< if using predefined binary patterns user must specify the length
        //!< of the pattern.
        uint16_t maxBinaryPatternLenght;
        //!< This value is used by USS_generateMultiToneBinaryPattern and
        //!< USS_generateMonoDualToneBinaryPattern to avoid overflowing the
        //!< binary pattern array
        int16_t *pAdditionalBinaryPattern;
        //!< Pointer to additional binary pattern. This pattern will be append to
        //!< the binary pattern calculated by USS_generateMonoDualToneBinaryPattern
        //!< USS_generateMultiToneBinaryPattern
        uint16_t additionalPatternLength;
        //!< Length of additional binary pattern the length.
        uint16_t maxAdditionalBinaryPatternLength;
        //!< This value is used by USS_generateMultiToneBinaryPattern and
        //!< USS_generateMonoDualToneBinaryPattern to avoid overflowing the
        //!< binary pattern array
        USS_Alg_dToF_Calculation_Option dtoFCalcOption;
        //!< Specifies Delta ToF Calculation option
        USS_Alg_volume_flow_Calculation_Option volumeCalOption;
        //!< Specifies Volume Flow Rate Calculation option
        int16_t maxSampleShift;
        //!<  Maximum shift in samples for calculating deltaTOF (setting for high flow)
        //!< Parameter specifying maximum shift in samples for deltaTOF calculation,
        //!< e.g. setting of 8 with 4 samples per cycle supports high flow +/- 2 cycles.
        const int16_t (*pInterpCorrectionTablePtr)[2];
        //!< Pointer to look-up table (LUT) used for interpolation correction. A null
        //!< pointer will disable using offset correction (less accuracy
        //!< for absTOF).
        int16_t interpCorrectionTableLenght;
        //!< Length of look-up table (LUT) used for interpolation correction. Must be
        //!< a power of two.
        USS_Algorithms_Meter_Constant_Calib *pMeterConfiguration;
        //!< Pointer to USS_Meter_Configuration array
        uint8_t numOfMeterCalibrationRanges;
        //!< Number of Meter Calibration Ranges.
        //!< Minimum numOfMeterCalibrationRanges = 0;
        //!< Maximum numOfMeterCalibrationRanges = maxNumOfMeterCalibrationRanges;
        //!< Setting a value of 0 will disable meter constant calibration
        //!< feature.
}USS_Algorithms_User_Configuration;

//! \brief Contains USS SW Lib Algorithms Results
//!
typedef struct _USS_Algorithms_Results_
{
	USS_message_code messageCode;
	float deltaTOF;
	//!< Delta Time Of Flight calculation result
	float totalTOF_UPS;
	//!< Upstream total Time Of Flight calculation result
	float totalTOF_DNS;
	//!< Downstream total Time Of Flight calculation result
	float volumeFlowRate;
	//!< Volume flow rate calculation result
}USS_Algorithms_Results;

//! \brief Contains USS SW Lib Algorithms Results
//!
typedef struct _USS_Algorithms_Results_fixed_point_
{
	USS_message_code messageCode;
	int32_t iq44DeltaTOF;
	//!< Delta Time Of Flight calculation result
	int32_t iq40TotalTOF_UPS;
	//!< Upstream total Time Of Flight calculation result
	int32_t iq40TotalTOF_DNS;
	//!< Downstream total Time Of Flight calculation result
	int32_t iq16VolumeFlowRate;
	//!< Volume flow rate calculation result
}USS_Algorithms_Results_fixed_point;


//! \brief Contains USS SW Lib configuration
//!
typedef struct _USS_SW_Library_configuration_
{
	USS_System_Configuration *systemConfig;
	//!< Pointer to USS_System_Configuration struct
	USS_Meter_Configuration *meterConfig;
	//!< Pointer to USS_Meter_Configuration struct
	USS_Measurement_Configuration *measurementConfig;
	//!< Pointer to USS_Measurement_Configuration struct
	USS_HSPLL_Configuration *pllConfiguration;
	//!< Pointer to USS_HSPLL_Configuration struct
	USS_Capture_Configuration *captureConfig;
	//!< Pointer to USS_Capture_Configuration struct
	USS_Trigger_Configuration *triggerConfig;
	//!< Pointer to USS_Trigger_Configuration struct
	USS_Algorithms_User_Configuration *algorithmsConfig;
	//!< Pointer to USS_Algorithms_Configuration struct
	USS_Interrupt_Configuration *interruptConfig;
	//!< Pointer to USS_Interrupt_Configuration struct
//	USS_Algorithms_Object *algorithmsObject;
//	//!< Pointer to USS_Algorithms_Handle handle
	uint16_t validationKey;
	//!< Validation Key (used when USS_SW_Library_configuration struct is
	//!< loaded from non-volatile location)
} USS_SW_Library_configuration;

//*****************************************************************************
// globals
//*****************************************************************************
//! \brief The following variable is used to monitor USS interrupt status
//! this variable combines SAPH, UUPS, HSPLL interrupts as well as a Timer
//!    interrupt status
extern volatile uint32_t USSSWLIB_USS_interrupt_status;

#ifdef USS_CALIBRATE_SIGNAL_GAIN_TEST
extern int16_t ussMaxSignal[5];
#endif
//*****************************************************************************
// the function prototypes
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup measurement_api
//! @{
//
//******************************************************************************
/*******************************************************************************
 * USS SW LIBRARY MEASUREMENT APIS
 ******************************************************************************/
//! \brief The following API configures USS module based on user configuration
//! files "ussSwLib_userConfig.c" and "ussSwLib_userConfig.h". This API
//! will internally invoke all USS Configuration APIs. In addition, this
//! API will initialize the timer which will be used to generate delays
//! such as the delay to stabilize PLL XTAL.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code
//!
extern USS_message_code USS_configureUltrasonicMeasurement(
		USS_SW_Library_configuration *config);

//! \brief    The following API can be used to trigger a USS Capture via software
//! trigger.
//!    \n \n \b NOTE: User must call
//! "USS_configureUltrasonicMeasurement" before calling this API and USS
//!    trigger must be set to USS_Triger_Configuration_Software_Trigger.
//!    \n \n The API will perform the following operations:
//!    <ol>
//!       <li>Verify if USS state is READY
//!          <ul>
//!            <li>If USS is not READY state (e.g. OFF or STANDBY) it will set USS
//!                 to READY state following recommended USS Power Up sequence.
//!            </li>
//!          </ul>
//!       </li>
//!       <li>
//!            Triggers USS Capture
//!       </li>
//!       <li>
//!            Wait in user selected power mode (USS_capture_power_mode_option)
//!            until acquisition sequence is done or any of the following errors
//!            occurs:
//!          <ul>
//!            <li>
//!                Data Error Abort
//!            </li>
//!            <li>
//!                ASQ Time Mark 4 time out
//!            </li>
//!          </ul>
//!        </li>
//!     <li>
//!            Verifies that SDHS conversion has not been interrupted
//!     </li>
//!    </ol>
//!
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] powerMode  The following are valid options:
//!    - USS_capture_power_mode_active,
//!    - USS_capture_power_mode_low_power_mode_0
//!    - USS_capture_power_mode_low_power_mode_1
//!
//!
//! \return    USS_message_code

extern USS_message_code USS_startUltrasonicMeasurement(
		USS_SW_Library_configuration *config,
		USS_capture_power_mode_option powerMode);

//! \brief The following API has been power optimized for
//!  ultrasonic capture. The API controls the capture sequence selected by the
//!  user via the sequenceSelection member in USS_Measurement_Configuration
//!	 structure. \b NOTE the API has been optimized for
//!	 USS_measurement_capture_sequence_ch0_ch1 capture mode.
//!  \n \n \b NOTE_1: User must call
//!  "USS_configureUltrasonicMeasurement" before calling this API and USS
//!  trigger must be set to USS_Triger_Configuration_Software_Trigger.
//!	 To reduce power consumption between captures the device will:
//!	 <ul>
//!		<li>Put USS Module in OFF state</li>
//!		<li>Turn off USSXTAL</li>
//!		<li>Go to Low Power Mode 3 (LPM3) mode and wait for timer to expire to
//!         to trigger the next channel input. If there are no more channel
//!         captures pending. then this delay is skipped and the API returns
//!         control to the application.</li>
//!  </ul>
//!  \n \n \b NOTE2: The total delay between Channel acquisition sequences will
//!  be determined by the following USS SW library and USS module configurations:
//!  <ul>
//!     <li>USS SW library configuration
//!         (Please refer to each library configuration parameter some more information):
//!         <ul>
//!             <li>restartLowPowerCaptureCount</li>
//!             <li>ussXTALsettlingCount</li>
//!             <li>ulpBiasDelay</li>
//!         </ul>
//!     </li>
//!     <li>USS module (Please refer to device datasheet for typical UUPS power-up delay):
//!         <ul>
//!             <li>USS UUPS module power-up</li>
//!         </ul>
//!     </li>
//!  </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//!
//! \return    USS_message_code
extern USS_message_code USS_startLowPowerUltrasonicCapture(
		USS_SW_Library_configuration *config);

//! \brief    The following API can be used to stop any active USS capture. The
//!            user has the option to select USS state after conversion is stopped.
//!            The API will report if capture was successfully stopped or if any
//!            errors occurs while stopping the measurement.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \param[in] stopMeasurementState    specifies USS state after sequence is
//!    stopped. Possible values are:
//!    - USS_stop_measurement_state_off
//!    - USS_stop_measurement_state_ready
//!    - USS_stop_measurement_state_stand_by
//!
//! \return USS_message_code
//!
extern USS_message_code USS_stopUltrasonicMeasurement(
		USS_SW_Library_configuration *config,
		USS_stop_measurement_state stopMeasurementState);

//! \brief    The following API can be used to generate a Low Power Mode Delay.
//!        The API allows to select the desired Low Power Mode. Global interrupts
//!        remain enable. The timer clock source for this delay is ACLK. Therefore
//!        the delay generate will be determined based on ACLK frequency (e.g. for
//!     count = 30 and ACLK freq = 32768 HZ a delay of ~915 msec will be
//!        generated.
//!
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] lpmMode selects the low power mode Possible values are:
//!    - USS_low_power_mode_option_low_power_mode_0
//!    - USS_low_power_mode_option_low_power_mode_1
//!    - USS_low_power_mode_option_low_power_mode_2
//!    - USS_low_power_mode_option_low_power_mode_3
//! \param[in] count Determines the delay duration
//!
//! \return USS_message_code
//!
extern void USS_generateLPMDelay(
		USS_SW_Library_configuration *config,
		USS_low_power_mode_option lpmMode,
		uint16_t count);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * USS SW LIBRARY CONFIGURATION UPDATE APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup configuration_api
//! @{
//
//******************************************************************************
//! \brief The following function updates the SDHS module configuration based on
//! user defined USS_Capture_Configuration struct in
//! "ussSwLib_userConfig.c". Note: This API must be called everytime a
//!    USS_Capture_Configuration parameter is updated. This API performs the
//!    following SDHS module configuration.
//!    <ul>
//!        <li>Checks:
//!            <ul>
//!                <li>
//!                      Sample size is within specified range (1-1024)
//!                </li>
//!                <li>
//!                      Window thresholds are properly set
//!                </li>
//!                <ul>
//!                    <li>
//!                        Hi threshold is higher than lower threshold
//!                    </li>
//!                </ul>
//!                <li>
//!                    Threshold values are correct based on output bit
//!                    resolution (e.g. if 12-bit resolution is selected
//!                     windowThreshold needs to be a 12-bit number)
//!                </li>
//!                <li>
//!                      LEA start address falls within 4KB of LEA RAM memory
//!                </li>
//!                <li>
//!                    Checks if there is no active conversion, if there is an
//!                    active conversion the update is cancelled and API
//!                    returns
//!                    USS_message_code_SDHS_update_error_ongoing_conversion
//!                </li>
//!            </ul>
//!        </li>
//!    </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
extern inline USS_message_code USS_updateSDHSConfiguration(
		USS_SW_Library_configuration *config);

//! \brief The following function updates the HSPLL module configuration based
//! on user defined USS_HSPLL_Configuration struct in
//! "ussSwLib_userConfig.c".
//! \n \n \b Note1: This API must be called everytime a
//!    USS_HSPLL_Configuration parameter is updated.
//! \n \n \b Note2: If
//!    "pllOutputFreq_inHz" is modified in USS_HSPLL_Configuration struct,
//!    user must call USS_updateSDHSCapture API since SDHS modulator
//!    optimization bits might need to be updated.
//! \n \n This API performs the following HSPLL module configuration checks:
//!    <ul>
//!        <li> pllXtalFreq_inHz is between (4MHz and 8MHz) </li>
//!        <li>pllOutputFreq_inHz is between (68MHz and 80MHz)</li>
//!        <li>
//!            Checks if there is no active conversion, if there is an
//!            active conversion the update is cancelled and API
//!            returns
//!            USS_message_code_HSPLL_update_error_ongoing_conversion
//!        </li>
//!    </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
extern inline USS_message_code USS_updateHSPLLConfiguration(
		USS_SW_Library_configuration *config);

//! \brief The following function updates the SAPH module configuration based
//! on user defined USS_Measurement_Configuration struct in
//! "ussSwLib_userConfig.c".
//! \n \n \b Note: This API must be called everytime a
//!    USS_Measurement_Configuration parameter is updated.
//! \n \n This API performs
//!    the following SAPH module configuration checks:
//!    <ul>
//!        <li>pulseLowPhasePeriod is between (2-255)</li>
//!        <li>pulseHighPhasePeriod is between (2-255)</li>
//!        <li>numOfExcitationPulses is between (0-63)</li>
//!        <li>numOfStopPulses is between  (0-15)</li>
//!        <li>
//!            Checks if there is no active conversion, if there is an
//!            active conversion the update is cancelled and API
//!            returns
//!            USS_message_code_SAPH_update_error_ongoing_conversion
//!        </li>
//!        <li>Checks user doesn't ARM SDHS during SDHS settling
//!            time
//!        </li>
//! </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
extern inline USS_message_code USS_updateSAPHConfiguration(
		USS_SW_Library_configuration *config);

//! \brief The following function updates the UUPS module configuration based
//! on user defined USS_Trigger_Configuration struct in
//! "ussSwLib_userConfig.c".
//! \n \n \b Note1: This API must be called every time a
//!    USS_Trigger_Configuration parameter is updated.
//! \n \n \b Note2: This API will
//! assert HSPLL.PLLXTEN bit when
//! USS_Triger_Configuration_RTC_Timer_Trigger,
//!    USS_Triger_Configuration_External_Trigger1 or
//!    USS_Triger_Configuration_External_Trigger2.
//! Selecting USS_Triger_Configuration_Software_Trigger will deassert
//!    HSPLL.PLLXTEN bit.
//! \n \n This API performs the following checks:
//!    <ul>
//!        <li>
//!            Checks if there is no active conversion, if there is an
//!            active conversion the update is cancelled and API
//!            returns
//!            USS_message_code_SAPH_update_error_ongoing_conversion
//!        </li>
//! </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
extern inline USS_message_code USS_updateUUPSConfiguration(
		USS_SW_Library_configuration *config);

//! \brief The following function updates the UUPS module interrupt
//!           configuration based on user defined USS_Interrupt_Configuration
//!           struct in "ussSwLib_userConfig.c". Note: This API must be called
//!         every time an USS_Interrupt_Configuration parameter is updated.
//! \n \n This API performs the following checks:
//!    <ul>
//!        <li>
//!            Checks if there is no active conversion, if there is an
//!            active conversion the update is cancelled and API
//!            returns
//!            USS_message_code_SAPH_update_error_ongoing_conversion
//!        </li>
//! </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
extern inline USS_message_code USS_updateUSSInterruptsConfiguration(
		USS_SW_Library_configuration *config);

//!
//! \brief This function allows application to reset the USS module. It is
//! recommended that this API is used when an USS SW Library API return a HSPLL
//! unlock error. \n
//! The API allows user to control the reconfiguration of the measurement. If
//! reconfigUSSModule == true then the measurement is reconfigured. If
//! reconfigUSSModule == false then the USS module will have the default module
//! configuration after a power-up clear (PUC) reset.\n\n
//! \b Note: This functionality is only available in MSP430FR604x Rev B or later
//! silicon. If the code is running on Rev A silicon and this API is called the
//! API will return the following error message
//! USS_meesage_code_silicon_version_does_not_support_this_functionality, it is
//! recommended that the application checks the API return message code to
//! confirm the USS module have been reset successfully.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//! \param[in] reconfigUSSModule specifies if module needs to be reconfigured
//!  after resetting the module.
//!
//! \return USS_meesage_code_silicon_version_does_not_support_this_functionality
//! \return USS_message_code_no_error
//!
extern inline USS_message_code USS_resetUSSModule(
        USS_SW_Library_configuration *config, bool reconfigUSSModule);

//!
//! \brief This function allows user to swap capture channels. This functionality
//! is only available when USS_measurement_capture_sequence_ch0_ch1 or
//! USS_measurement_capture_sequence_ch1_ch0 capture sequence is selected.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
//!
extern inline USS_message_code USS_swapCaptureChannels(
        USS_SW_Library_configuration *config);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * USS SW LIBRARY CALIBRATION APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup calibration_api
//! @{
//
//****************************************************************************** 
//! \brief    The following API allows to verify the HSPLL frequency is
//! set at the user selected pllOutputFreq_inHz in the
//!    USS_HSPLL_Configuration struct. The test consists on using LFXT to
//!    verify HSPLL frequency. The acquisition timer which is sourced by the
//!	   HSPLL will be clear and started for this test. After 30.517 msec the 
//!	   acquisition timer will be stopped and the acquisition timer counter will
//!	   be used to determine if HSPLL is working the tolerance frequency range.
//!	   This test is run 16 times and acquisition timer counts are averaged to 
//!	   determine if the outcome of the test is within the desired tolerance.
//!    <ul>
//!        <li>HSPLL is locked before starting test</li>
//!    </ul>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] testResults is a pointer to the HSPLL test result structure.
//!
//! \return USS_message_code
//!
extern USS_message_code USS_verifyHSPLLFrequency(
		USS_SW_Library_configuration *config,
		USS_calibration_hspll_results *testResults);

//! \brief The following API runs the SDHS Signal Gain calibration routine
//! if no errors are encountered during Signal Gain calibration the
//! config->captureConfig->gainRange value will be configured else it will remain
//! the original value. Please refer to agcConstant parameter description in
//! USS_Capture_Configuration structure for more information regarding
//! the configuration for this signal gain calibration parameter.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code
//!
extern USS_message_code USS_calibrateSignalGain(USS_SW_Library_configuration *config);


//! \brief This API allows the user to estimate the DC offset generated by the
//! SDHS module.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] mode  specifies the calculation mode for DC offset estimation
//!
//! \return USS_message_code
//!
extern USS_message_code USS_estimateDCoffset(
		USS_SW_Library_configuration *config, USS_dcOffEst_Calc_Mode mode);

//! \brief This API allows the user to estimate the DC offset generated by the
//! SDHS module.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] testResults is a pointer to USS_dTof_absTof_offset_results
//!            structure
//! \param[in] testConfig is a pointer to USS_dTof_absTof_offset_test_config
//!            structure
//!
//! \return USS_message_code
//!
extern USS_message_code USS_calculateOffsets(USS_SW_Library_configuration *config,
                          USS_dTof_absTof_offset_results *testResults,
                          USS_dTof_absTof_offset_test_config *testConfig);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * USS SW LIBRARY ALGORITHMS APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup algorithm_api
//! @{
//
//******************************************************************************
//! \brief The following API initializes USS_Algorithms_Handle
//! based on user USS_Algorithms_User_Configuration,
//! USS_System_Configuration, USS_Meter_Configuration and
//! USS_Measurement_Configuration structs.
//! Currently this API does not perform any parameter checks.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code 
extern USS_message_code USS_initAlgorithms(USS_SW_Library_configuration *config);

//! \brief The following API calculates Absolute ToF Upstream, Absolute ToF
//! Downstream, Delta ToF, Volume Flow Rate using fixed point math.
//! //! \n \n \b IMPORTANT: It is recommend to use this version of the API to
//! reduce power consumption numbers
//!
//! \n \n \b Note1: USS_initAlgorithms must be called prior to calling this
//! function.
//!
//! The API expects to have a Up stream (UPS) and Down stream (DNS) of size
//!	"sampleSize" in contiguous LEA memory where the first capture is assumed to
//! be UPS capture and the second capture is assumed to be DNS capture.
//!	Therefore, UPS and DNS capture depends on the type of capture the
//!	user select via sequenceSelection member in USS_Measurement_Configuration structure
//! For example: if sequenceSelection == USS_measurement_capture_sequence_ch0_ch1
//! then USS CH0 is assumed to be UPS capture and CH1 is assumed to be DNS capture
//! and vice versa for USS_measurement_capture_sequence_ch1_ch0
//! If API return message code USS_message_code_valid_results, then algorithms
//! result are valid.
//!
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] results  is a pointer to the USS Algoriithms Result structure
//!
//! \return USS_message_code

extern USS_message_code USS_runAlgorithmsFixedPoint(
		USS_SW_Library_configuration *config,
		USS_Algorithms_Results_fixed_point *results);


//! \brief The following API calculates Absolute ToF Upstream, Absolute ToF
//! Downstream, Delta ToF, Volume Flow Rate.
//! \n \n \b Note1: USS_initAlgorithms must be called prior to calling this
//! function.
//! The API expects to have a Up stream (UPS) and Down stream (DNS) of size
//!	"sampleSize" in contiguous LEA memory where the first capture is assumed to
//! be UPS capture and the second capture is assumed to be DNS capture.
//!	Therefore, UPS and DNS capture depends on the type of capture the
//!	user select via sequenceSelection member in USS_Measurement_Configuration structure
//! For example: if sequenceSelection == USS_measurement_capture_sequence_ch0_ch1
//! then USS CH0 is assumed to be UPS capture and CH1 is assumed to be DNS capture
//! and vice versa for USS_measurement_capture_sequence_ch1_ch0
//! If API return message code USS_message_code_valid_results, then algorithms
//! result are valid.
//! 
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//! \param[in] results  is a pointer to the USS Algorithms Result structure
//!
//! \return USS_message_code 
extern USS_message_code USS_runAlgorithms(USS_SW_Library_configuration *config,
		USS_Algorithms_Results *results);



//! \brief The following API is used to generate the binary pattern used by
//! USS_Alg_AbsToF_Calculation_Option_lobeWide and
//! USS_Alg_AbsToF_Calculation_Option_hilbertWide computation options. Use this
//! when AbsTof computation option is set to
//! USS_Alg_AbsToF_Calculation_Option_lobeWide or
//! USS_Alg_AbsToF_Calculation_Option_hilbertWide and pulse generation is single
//! or dual mode
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code
extern USS_message_code USS_generateMonoDualToneBinaryPattern(
        USS_SW_Library_configuration *config);

//! \brief The following API is used to generate the binary pattern used by
//! USS_Alg_AbsToF_Calculation_Option_lobeWide and
//! USS_Alg_AbsToF_Calculation_Option_hilbertWide computation options. Use this
//! when AbsTof computation option is set to
//! USS_Alg_AbsToF_Calculation_Option_lobeWide or
//! USS_Alg_AbsToF_Calculation_Option_hilbertWide and pulse generation is multi
//! tone mode
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code
extern USS_message_code USS_generateMultiToneBinaryPattern(
        USS_SW_Library_configuration *config);

//! \brief The following API allows user to update the frequency at which the
//!  Absolute Time of Flight is calculated in the algorithms. The iteration
//! value is set via the "absTOFIteration" parameter in the
//! USS_Algorithm_User_Configuration structure.
//!
//! \n \n Increasing the value for "absTOFIteration" reduces the average
//! consumption of the application.
//!
//! \n \n \b Note1: Calling this API will cause that
//! USS_runAlgorithmsFixedPoint or USS_runAlgorithm API computes the Absolute
//! Time of Flight next time they are called.
//!
//! \n \n \b Note2: USS_initAlgorithms must be called prior to calling this
//! function.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code
extern USS_message_code USS_updateABSTOFCalculationIteration(
		USS_SW_Library_configuration *config);


//! \brief The following API allows user to update clockRelativeError
//!        parameter during runtime. The clockRelativeError parameter is used to
//!        by USS_runAlgorithms and  USS_runAlgorithmsFixedPoint APIs. The
//!		   clockRelativeError parameter is used to correct the UPS and DNS
//!		   AbsTOF calculations. Refere to clockRelativeError description for
//!        for guidance on how to estimate this parameter using
//!		   USS_verifyHSPLLFrequency API.
//!
//!
//! \n \n \b Note1: USS_initAlgorithms must be called prior to calling this
//! function.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration
//!
//! \return USS_message_code
extern USS_message_code USS_updateClockRelativeError(
		USS_SW_Library_configuration *config);

//! \brief The following API allows user to convert the fixed point
//!     Algorithm results to floating point representation.
//!
//! \param[in] pFixedResults is a pointer to fixed point algorithm result
//!  structure
//! \param[in] pFloatResults is a pointer to a floating point algorithm result
//!  structure. The converted results are returned in this structure
//!
//! \return USS_message_code
extern USS_message_code USS_getResultsInFloat(
		USS_Algorithms_Results_fixed_point *pFixedResults,
		USS_Algorithms_Results *pFloatResults);

//!
//! \brief This function allows user to swap capture buffers. It is highly
//! recommended to call this function after calling USS_swapCaptureChannels.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code
extern USS_message_code USS_swapAlgorithmsCaptureBuffers(
        USS_SW_Library_configuration *config);

//! \brief This function allows users to query current status Algorithm Capture
//!  buffers
//!
//!
//! \return true Capture buffers are swapped
//!         false Capture buffers are NOT swapped
extern bool USS_isAlgorithmChannelsSwapped(void);

//! \brief This function allows users to update Additional Capture Delay with
//!         with minimal algorithm reconfiguration.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//! \param[in] additionalCaptureDelay new additional capture delay
//!
//! \return USS_message_code
extern USS_message_code USS_updateAdditionalCaptureDelay(
        USS_SW_Library_configuration *config, float additionalCaptureDelay);

//! \brief This function allows users to update Delta Time Of Flight Offset with
//!         with minimal algorithm reconfiguration.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//! \param[in] dTofOffset new dtof Offset
//!
//! \return USS_message_code
extern USS_message_code USS_updateDtoFOffset(
        USS_SW_Library_configuration *config, float dTofOffset);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * USS SW LIBRARY APPLICATION DEBUG APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup applicationdebug_api
//! @{
//
//****************************************************************************** 
//! \brief The following API will output current PPG configuration.
//!    \n \n PPG configuration is controlled via the following
//!    USS_Measurement_Configuration parameter:
//!    <ul>
//!        <li>pulsePolarity</li>
//!        <li>pulseLowPhasePeriod</li>
//!        <li>pulseHighPhasePeriod</li>
//!        <li>isPPGPrescaler</li>
//!        <li>numOfExcitationPulses</li>
//!        <li>numOfStopPulses</li>
//! </ul>
//!    The user can select the USS channel to test the PPG via the
//!    \"pPGChannel\" input parameter.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//! \param[in] PPGChannel  The following are valid options:
//!    - USS_test_channel_0
//!    - USS_test_channel_1
//!
//! \return USS_message_code

extern USS_message_code USS_testPPGconfiguration(
		USS_SW_Library_configuration *config,
		USS_test_channel PPGChannel);

//! \brief The following trigger manual SDHS conversion with current SDHS
//!    configuration. SDHS configuration is controlled via the
//!    USS_Measurement_Configuration struct and .
//!    \n \n The API will perform the following operations:
//!    <ol>
//!       <li>Verify if USS state is READY
//!          <ul>
//!            <li>If USS is not READY state (e.g. OFF or STANDBY) it will set USS
//!                 to READY state following recommended USS Power Up sequence.
//!            </li>
//!          </ul>
//!       </li>
//!       <li>
//!            Triggers manual USS Capture
//!       </li>
//!       <li>
//!            Wait in user selected power mode (USS_capture_power_mode_option)
//!            until acquisition sequence is done or any of the following errors
//!            occurs:
//!          <ul>
//!            <li>
//!                Data Overflow
//!            </li>
//!            <li>
//!                Over Input Level
//!            </li>
//!          </ul>
//!        </li>
//!     <li>
//!            Verifies that SDHS conversion has not been interrupted
//!     </li>
//!    </ol>
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \param[in] SDHSChannel  The following are valid options:
//!    - USS_test_channel_0
//!    - USS_test_channel_1
//! \param[in] mode Specifies the capture mode, valid modes are (active or LMP0)
//!
//! \return USS_message_code

extern USS_message_code USS_testSDHSconversion(
		USS_SW_Library_configuration *config, USS_test_channel SDHSChannel,
		USS_capture_power_mode_option mode);

//! \brief The following API will  the tone generation output in the SAPH
//! module.
//!    \n \n Tone generation is controlled via the following
//!    USS_Measurement_Configuration parameter:
//!    <ul>
//!        <li>pulsePolarity</li>
//!        <li>pulseLowPhasePeriod</li>
//!        <li>pulseHighPhasePeriod</li>
//!        <li>isPPGPrescaler</li>
//! </ul>
//!    The user can select the USS channel to test the PPG via the
//!    \"pPGChannel\" input parameter.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//! \param[in] PPGChannel  The following are valid options:
//!    - USS_test_channel_0
//!    - USS_test_channel_1
//!
//! \return USS_message_code

extern USS_message_code USS_startPPGToneGeneration(
		USS_SW_Library_configuration *config,
		USS_test_channel PPGChannel);

//! \brief The following API will stop the tone generation output in the SAPH
//! module.
//!
//! \param[in] config  is a pointer to the USS SW Library user configuration.
//!
//! \return USS_message_code 
extern USS_message_code USS_stopPPGToneGeneration(
		USS_SW_Library_configuration *config);

//! \brief The following API allows to obtain the address were UPS capture
//!        starts. IMPORTANT: USS_configureUltrasonicMeasurement API must be call in the
//!		   application first before using this API.
//!
//!
//! \return address to first UPS capture
extern int16_t* USS_getUPSPtr(USS_SW_Library_configuration *config);


//! \brief The following API allows to obtain the address were DNS capture
//!        starts. IMPORTANT: USS_configureUltrasonicMeasurement API must be call in the
//!		   application first before using this API.
//!
//!
//! \return address to first DNSS capture
extern int16_t* USS_getDNSPtr(USS_SW_Library_configuration *config);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

/*******************************************************************************
 * USS SW LIBRARY INTERRUPT CALLBACK REGISTER/UNREGISTER APIS
 ******************************************************************************/
//*****************************************************************************
//
//! \addtogroup interrupt_api
//! @{
//
//******************************************************************************
//! \brief The following API registers a function callback to the desired SAPH
//!    interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_SDHS_Interrupt_OVF
//!    - USS_SDHS_Interrupt_OVINLVL
//!    - USS_SDHS_Interrupt_SMPSZ
//!    - USS_SDHS_Interrupt_SSTRG
//!    - USS_SDHS_Interrupt_DTRDY
//!    - USS_SDHS_Interrupt_WINHI
//!    - USS_SDHS_Interrupt_WINLO
//!
//! \param[in] function    Address of function to be registered for callback.
//!
//! \return None
extern void USS_registerSDHSInterruptCallback(USS_SDHS_Interrupt interruptNum,
		void function(void));
//! \brief The following API registers a function callback to the desired SAPH
//!    interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_SAPH_Interrupt_DATAERR
//! - USS_SAPH_Interrupt_TMFTO
//! - USS_SAPH_Interrupt_SEQDN
//! - USS_SAPH_Interrupt_PNGDN
//!
//! \param[in] function    Address of function to be registered for callback.
//!
//! \return None
extern void USS_registerSAPHInterruptCallback(USS_SAPH_Interrupt interruptNum,
		void function(void));

//! \brief The following API registers a function callback to the desired UUPS
//!    interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_UUPS_Interrupt_PREQIG
//! - USS_UUPS_Interrupt_PTMOUT
//!
//! \param[in] function    Address of function to be registered for callback.
//!
//! \return None
extern void USS_registerUUPSInterruptCallback(USS_UUPS_Interrupt interruptNum,
		void function(void));

//! \brief The following API registers a function callback to the desired HSPLL
//!    interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_HSPLL_Interrupt_PLLUNLOCK
//!
//! \param[in] function    Address of function to be registered for callback.
//!
//! \return None
extern void USS_registerHSPLLInterruptCallback(USS_HSPLL_Interrupt interruptNum,
		void function(void));

//! \brief The following API registers a function callback to the desired SAPH
//!    interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_SDHS_Interrupt_OVF
//!    - USS_SDHS_Interrupt_OVINLVL
//!    - USS_SDHS_Interrupt_SMPSZ
//!    - USS_SDHS_Interrupt_SSTRG
//!    - USS_SDHS_Interrupt_DTRDY
//!    - USS_SDHS_Interrupt_WINHI
//!    - USS_SDHS_Interrupt_WINLO
//!
//!
//! \return None
extern void USS_unregisterSDHSInterruptCallback(
		USS_SDHS_Interrupt interruptNum);
//! \brief The following API unregisters a function callback to the desired SAPH
//!           interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_SAPH_Interrupt_DATAERR
//! - USS_SAPH_Interrupt_TMFTO
//! - USS_SAPH_Interrupt_SEQDN
//! - USS_SAPH_Interrupt_PNGDN
//!
//! \return None
extern void USS_unregisterSAPHInterruptCallback(
		USS_SAPH_Interrupt interruptNum);

//! \brief The following API unregisters a function callback to the desired UUPS
//!    interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_UUPS_Interrupt_PREQIG
//! - USS_UUPS_Interrupt_PTMOUT
//!
//! \return None
extern void USS_unregisterUUPSInterruptCallback(
		USS_UUPS_Interrupt interruptNum);

//! \brief The following API unregisters a function callback to the desired
//!    HSPLL interrupt vector.
//!
//! \param[in] interruptNum  Valid parameters:
//!    - USS_HSPLL_Interrupt_PLLUNLOCK
//!
//! \return None
extern void USS_unregisterHSPLLInterruptCallback(
		USS_HSPLL_Interrupt interruptNum);

//******************************************************************************
//
// Close the Doxygen group.
//! @}
//
//******************************************************************************

#ifdef __cplusplus
}
#endif // extern "C"

#endif // end of  _USSSWLIB_H_ definition

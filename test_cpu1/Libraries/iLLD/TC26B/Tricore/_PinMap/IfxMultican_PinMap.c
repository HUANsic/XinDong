/**
 * \file IfxMultican_PinMap.c
 * \brief MULTICAN I/O map
 * \ingroup IfxLld_Multican
 *
 * \version iLLD_1_0_1_12_0_1
 * \copyright Copyright (c) 2013 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 * Use of this file is subject to the terms of use agreed between (i) you or
 * the company in which ordinary course of business you are acting and (ii)
 * Infineon Technologies AG or its licensees. If and as long as no such terms
 * of use are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include "IfxMultican_PinMap.h"

IfxMultican_Rxd_In IfxMultican_RXD0A_P02_1_IN = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P02, 1}, Ifx_RxSel_a};
IfxMultican_Rxd_In IfxMultican_RXD0B_P20_7_IN = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P20, 7}, Ifx_RxSel_b};
IfxMultican_Rxd_In IfxMultican_RXD0D_P02_4_IN = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P02, 4}, Ifx_RxSel_d};
IfxMultican_Rxd_In IfxMultican_RXD0E_P33_7_IN = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P33, 7}, Ifx_RxSel_e};
IfxMultican_Rxd_In IfxMultican_RXD1A_P15_3_IN = {&MODULE_CAN, IfxMultican_NodeId_1, {&MODULE_P15, 3}, Ifx_RxSel_a};
IfxMultican_Rxd_In IfxMultican_RXD1B_P14_1_IN = {&MODULE_CAN, IfxMultican_NodeId_1, {&MODULE_P14, 1}, Ifx_RxSel_b};
IfxMultican_Rxd_In IfxMultican_RXD1D_P00_1_IN = {&MODULE_CAN, IfxMultican_NodeId_1, {&MODULE_P00, 1}, Ifx_RxSel_d};
IfxMultican_Rxd_In IfxMultican_RXD2A_P15_1_IN = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P15, 1}, Ifx_RxSel_a};
IfxMultican_Rxd_In IfxMultican_RXD2B_P02_3_IN = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P02, 3}, Ifx_RxSel_b};
IfxMultican_Rxd_In IfxMultican_RXD2D_P14_8_IN = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P14, 8}, Ifx_RxSel_d};
IfxMultican_Rxd_In IfxMultican_RXD2E_P10_2_IN = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P10, 2}, Ifx_RxSel_e};
IfxMultican_Rxd_In IfxMultican_RXD3A_P00_3_IN = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P00, 3}, Ifx_RxSel_a};
IfxMultican_Rxd_In IfxMultican_RXD3B_P32_2_IN = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P32, 2}, Ifx_RxSel_b};
IfxMultican_Rxd_In IfxMultican_RXD3C_P20_0_IN = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P20, 0}, Ifx_RxSel_c};
IfxMultican_Rxd_In IfxMultican_RXD3D_P11_10_IN = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P11,10}, Ifx_RxSel_d};
IfxMultican_Rxd_In IfxMultican_RXD3E_P20_9_IN = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P20, 9}, Ifx_RxSel_e};
IfxMultican_Txd_Out IfxMultican_TXD0_P02_0_OUT = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P02, 0}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD0_P02_5_OUT = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P02, 5}, IfxPort_OutputIdx_alt2};
IfxMultican_Txd_Out IfxMultican_TXD0_P20_8_OUT = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P20, 8}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD0_P33_8_OUT = {&MODULE_CAN, IfxMultican_NodeId_0, {&MODULE_P33, 8}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD1_P00_0_OUT = {&MODULE_CAN, IfxMultican_NodeId_1, {&MODULE_P00, 0}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD1_P14_0_OUT = {&MODULE_CAN, IfxMultican_NodeId_1, {&MODULE_P14, 0}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD1_P15_2_OUT = {&MODULE_CAN, IfxMultican_NodeId_1, {&MODULE_P15, 2}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD2_P02_2_OUT = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P02, 2}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD2_P10_3_OUT = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P10, 3}, IfxPort_OutputIdx_alt6};
IfxMultican_Txd_Out IfxMultican_TXD2_P14_10_OUT = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P14,10}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD2_P15_0_OUT = {&MODULE_CAN, IfxMultican_NodeId_2, {&MODULE_P15, 0}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD3_P00_2_OUT = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P00, 2}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD3_P11_12_OUT = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P11,12}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD3_P20_10_OUT = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P20,10}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD3_P20_3_OUT = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P20, 3}, IfxPort_OutputIdx_alt5};
IfxMultican_Txd_Out IfxMultican_TXD3_P32_3_OUT = {&MODULE_CAN, IfxMultican_NodeId_3, {&MODULE_P32, 3}, IfxPort_OutputIdx_alt5};


const IfxMultican_Rxd_In *IfxMultican_Rxd_In_pinTable[IFXMULTICAN_PINMAP_NUM_MODULES][IFXMULTICAN_PINMAP_NUM_NODES][IFXMULTICAN_PINMAP_RXD_IN_NUM_ITEMS] = {
    {
        {
            &IfxMultican_RXD0A_P02_1_IN,
            &IfxMultican_RXD0B_P20_7_IN,
            NULL_PTR,
            &IfxMultican_RXD0D_P02_4_IN,
            &IfxMultican_RXD0E_P33_7_IN
        },
        {
            &IfxMultican_RXD1A_P15_3_IN,
            &IfxMultican_RXD1B_P14_1_IN,
            NULL_PTR,
            &IfxMultican_RXD1D_P00_1_IN,
            NULL_PTR
        },
        {
            &IfxMultican_RXD2A_P15_1_IN,
            &IfxMultican_RXD2B_P02_3_IN,
            NULL_PTR,
            &IfxMultican_RXD2D_P14_8_IN,
            &IfxMultican_RXD2E_P10_2_IN
        },
        {
            &IfxMultican_RXD3A_P00_3_IN,
            &IfxMultican_RXD3B_P32_2_IN,
            &IfxMultican_RXD3C_P20_0_IN,
            &IfxMultican_RXD3D_P11_10_IN,
            &IfxMultican_RXD3E_P20_9_IN
        }
    }
};

const IfxMultican_Txd_Out *IfxMultican_Txd_Out_pinTable[IFXMULTICAN_PINMAP_NUM_MODULES][IFXMULTICAN_PINMAP_NUM_NODES][IFXMULTICAN_PINMAP_TXD_OUT_NUM_ITEMS] = {
    {
        {
            &IfxMultican_TXD0_P02_0_OUT,
            &IfxMultican_TXD0_P02_5_OUT,
            &IfxMultican_TXD0_P20_8_OUT,
            &IfxMultican_TXD0_P33_8_OUT,
            NULL_PTR
        },
        {
            &IfxMultican_TXD1_P00_0_OUT,
            &IfxMultican_TXD1_P14_0_OUT,
            &IfxMultican_TXD1_P15_2_OUT,
            NULL_PTR,
            NULL_PTR
        },
        {
            &IfxMultican_TXD2_P02_2_OUT,
            &IfxMultican_TXD2_P10_3_OUT,
            &IfxMultican_TXD2_P14_10_OUT,
            &IfxMultican_TXD2_P15_0_OUT,
            NULL_PTR
        },
        {
            &IfxMultican_TXD3_P00_2_OUT,
            &IfxMultican_TXD3_P11_12_OUT,
            &IfxMultican_TXD3_P20_3_OUT,
            &IfxMultican_TXD3_P20_10_OUT,
            &IfxMultican_TXD3_P32_3_OUT
        }
    }
};

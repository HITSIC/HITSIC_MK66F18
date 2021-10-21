/*
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Copyright 2018 - 2020 HITSIC
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/** HITSIC_Module_DRV */
#include <drv_cam_zf9v034.h>
#include <drv_disp_ssd1306.h>
#include <drv_dmadvp.h>
#include <drv_ftfx_flash.h>

#include <sys_pitmgr.h>
#include <sys_extint.h>

/** HITSIC_Module_LIB */
#include <lib_graphic.h>
#include <app_svbmp.h>
#include "sc_adc.h"
#include "sc_ftm.h"


#include "cm_backtrace.h"
#include "easyflash.h"
#include "drv_imu_invensense.h"

#include "textmenu.h"

void MENU_PitmgrHandler(void *_userData)
{
    MENU_PitIsr();
}

pitmgr_handle_t menu_pitHandle =
{
    .tickInterval = 250UL,
    .tickOffset = 7UL,
    .handler = MENU_PitmgrHandler,
    .pptFlag = pitmgr_pptEnable,
    .userData = NULL,
};


#include "sdmmc_config.h"
#include "ff.h"
FATFS fatfs;                                   //逻辑驱动器的工作区
pitmgr_t pitmgr_main;

extint_t extint_porta, extint_portb, extint_portc, extint_portd, extint_porte;

void MENU_DataSetUp(void);

#include "button.h"

cam_zf9v034_configPacket_t cameraCfg;
dmadvp_config_t dmadvpCfg;
dmadvp_handle_t dmadvpHandle;
void CAM_ZF9V034_DmaCallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);

disp_ssd1306_fb_t dispBuffer;

/** HITSIC_Module_TEST */
#include <drv_cam_zf9v034_test.h>
//#include <app_menu_test.h>
#include <drv_invimu_test.h>
#include <sys_fatfs_test.h>
#include <sys_fatfs_diskioTest.h>
#include <extlib_easyflash_test.h>

/** SCLIB_TEST */
#include <sc_test.h>


void main(void)
{
    /** 初始化阶段，关闭总中断 */
    HAL_EnterCritical();

    /** BSP（板级支持包）初始化 */
    RTECLK_HsRun_180MHz();
    RTEPIN_Basic();
    RTEPIN_Digital();
    RTEPIN_Analog();
    RTEPIN_LPUART0_DBG();
    RTEPIN_UART0_WLAN();
    RTEPIP_Basic();
    RTEPIP_Device();

    /** 初始化调试组件 */
    //DbgConsole_Init(0U, 921600U, kSerialPort_Uart, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    printf("Welcome to HITSIC !\n");
    printf("Compiler: GCC %d.%d.%d\n", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    printf("C++ std = %d\n", __cplusplus);
    printf("Complie time: %s %s\n", __DATE__, __TIME__);
    cm_backtrace_init("HITSIC_MK66F18", "2020-v3.0", "v4.2.0");

    /** 初始化OLED屏幕 */
    DISP_SSD1306_Init();
    extern const uint8_t DISP_image_100thAnniversary[8][128];
    DISP_SSD1306_BufferUpload((uint8_t*) DISP_image_100thAnniversary);
    /** 初始化ftfx_Flash */
    FLASH_SimpleInit();
    easyflash_init();
    /** 初始化PIT中断管理器 */
    NVIC_SetPriority(LPTMR0_IRQn, 4U);
    EnableIRQ(LPTMR0_IRQn);

    PITMGR_Init(&pitmgr_main, 1000U);
    LPTMR_StartTimer(LPTMR0);

    /** 初始化I/O中断管理器 */
    NVIC_SetPriority(PORTA_IRQn, 2);
    NVIC_SetPriority(PORTB_IRQn, 2);
    NVIC_SetPriority(PORTC_IRQn, 2);
    NVIC_SetPriority(PORTD_IRQn, 2);
    NVIC_SetPriority(PORTE_IRQn, 2);
    EnableIRQ(PORTA_IRQn);
    EnableIRQ(PORTB_IRQn);
    EnableIRQ(PORTC_IRQn);
    EnableIRQ(PORTD_IRQn);
    EnableIRQ(PORTE_IRQn);

    EXTINT_Init(&extint_porta);
    EXTINT_Init(&extint_portb);
    EXTINT_Init(&extint_portc);
    EXTINT_Init(&extint_portd);
    EXTINT_Init(&extint_porte);
    /** 初始化菜单 */
    MENU_Init();
    //MENU_Data_NvmReadRegionConfig();
    //MENU_Data_NvmRead(menu_currRegionNum[0]);
    PITMGR_HandleInsert(&pitmgr_main, &menu_pitHandle);
    /** 菜单挂起 */
    MENU_Suspend();
    /** 初始化按键 */
    BUTTON_Init();
    /** 初始化摄像头 */
    //TODO: 在这里初始化摄像头
    /** 初始化IMU */
    //TODO: 在这里初始化IMU（MPU6050）
    /** 菜单就绪 */
    //MENU_Resume();
    /** 控制环初始化 */
    //TODO: 在这里初始化控制环
    /** 初始化结束，开启总中断 */
    HAL_ExitCritical();
    //DISP_SSD1306_delay_ms(100);
    //cDISP_SSD1306_BufferUpload((uint8_t*) DISP_image_100thAnniversary);
    //DISP_SSD1306_delay_ms(100);
    //DISP_SSD1306_BufferUploadDMA((uint8_t*) DISP_image_100thAnniversary);
    //CAM_ZF9V034_UnitTest();
    //DISP_SSD1306_BufferUpload((uint8_t*) &dispBuffer);

    //EF_BasicTest();
    MENU_Resume();
    /** 内置DSP函数测试 */
    float f = arm_sin_f32(0.6f);

//    menu_list_t *list = MENU_DirGetList("/TestList");
//    if(true);
//    menu_itemIfce_t *itme = MENU_DirGetItem(list, "region_i");

    INV_IMU_Example(0, NULL);

    while (true)
    {
        //TODO: 在这里添加车模保护代码
    }
}

void MENU_DataSetUpd(void)
{
    //MENU_ListInsert(menu_menuRoot, MENU_ItemConstruct(nullType, NULL, "EXAMPLE", 0, 0));
    //TODO: 在这里添加子菜单和菜单项
    //MENU_DataSetupTest(menu_menuRoot);
}

void CAM_ZF9V034_DmaCallback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    //TODO: 补完本回调函数，双缓存采图。

    //TODO: 添加图像处理（转向控制也可以写在这里）
}

/**
 * 『灯千结的碎碎念』 Tips by C.M. :
 * 1. 浮点数计算有时（例如除零时）会产生“nan”，即“非数（Not-a-Number）”。
 *      要检测一个变量是否为“nan”，只需判断这个变量是否和自身相等。如果该
 *      变量与自身不相等（表达式“var == var”的值为假），则可判定该浮点数
 *      的值是nan，需要进行车模保护动作。
 * 2. 由于车模震动等因素，IMU可能会断开连接。一旦发现IMU读取失败，应执行车
 *      模保护动作。另外，IMU在单片机复位的瞬间可能正在进行传输，导致时序
 *      紊乱，初始化失败。因此装有IMU的车模复位时必须全车断电。
 * 3. 正常情况下图像帧率为50FPS，即20ms一帧。若摄像头时序紊乱，会导致控制周
 *      期混乱。因而有必要在每次图像采集完成时测量距离上次图像采集完成的时
 *      间间隔，如果明显偏离20ms，须执行车模保护动作。
 * 4. 直立车需特别注意：有时控制输出会使两个电机向相反方向旋转，这在正常运行
 *      中是十分危险的，可能造成车模进入“原地陀螺旋转”的状态，极易损坏车模或
 *      导致人员受伤。在设置电机占空比时务必做好异常保护。
 */


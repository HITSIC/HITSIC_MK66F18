#include "button.h"

void BUTTON_MenuCallback(menu_keyOpCode_t _code, button_status_t _status)
{
    menu_keyOp_t op = _code;
    switch (_status)
    {
    case BUTTON_SHRT_PRES:
        op |= menuOpType_shrt;
        break;
    case BUTTON_LONG_PRES:
        op |= menuOpType_long;
        break;
    case BUTTON_LRPT_PRES:
        op |= menuOpType_lrpt;
        break;
    default:
        break;
    }
    //SYSLOG_V("Button message: %ld", menu_keyOpBuff);
    MENU_KeypadSignal(op);
}

port_interrupt_t BUTTON_InterruptLut(button_interrupt_t _int)
{
    switch (_int)
    {
    case button_interrupt_none:
        return kPORT_InterruptOrDMADisabled;
    case button_interrupt_rise:
        return kPORT_InterruptRisingEdge;
    case button_interrupt_fall:
        return kPORT_InterruptFallingEdge;
    default:
        return kPORT_InterruptOrDMADisabled;
    }
}

// BUTTON OK

button_t button_ok;

uint32_t BUTTON_Read_OK(void)
{
    GPIO_PinRead(RTEPIN_DIGITAL_BUTTON_OK_GPIO, RTEPIN_DIGITAL_BUTTON_OK_PIN);
}
void BUTTON_SetInterrupt_OK(button_interrupt_t _int)
{
    port_interrupt_t config = BUTTON_InterruptLut(_int);
    PORT_SetPinInterruptConfig(RTEPIN_DIGITAL_BUTTON_OK_PORT, RTEPIN_DIGITAL_BUTTON_OK_PIN, config);
}
void BUTTON_Handler_OK(button_status_t _status, void *_userData)
{
    BUTTON_MenuCallback(menuOpCode_5wayStick_ok, _status);
}

void BUTTON_ExtintHandler_OK(void *userData)
{
    BUTTON_ExtIsr(&button_ok);
}

extint_handle_t button_extintHandle_ok =
{
    .index = RTEPIN_DIGITAL_BUTTON_OK_PIN,
    .handler = BUTTON_ExtintHandler_OK,
    .userData = NULL,
};

button_config_t button_cfg_ok =
{
    .read = BUTTON_Read_OK,
    .setInterrupt = BUTTON_SetInterrupt_OK,
    .logic = button_logic_reverse,
    .handler = BUTTON_Handler_OK,
    .userData = NULL,
};

// BUTTON UP

button_t button_up;

uint32_t BUTTON_Read_UP(void)
{
    GPIO_PinRead(RTEPIN_DIGITAL_BUTTON_UP_GPIO, RTEPIN_DIGITAL_BUTTON_UP_PIN);
}
void BUTTON_SetInterrupt_UP(button_interrupt_t _int)
{
    port_interrupt_t config = BUTTON_InterruptLut(_int);
    PORT_SetPinInterruptConfig(RTEPIN_DIGITAL_BUTTON_UP_PORT, RTEPIN_DIGITAL_BUTTON_UP_PIN, config);
}
void BUTTON_Handler_UP(button_status_t _status, void *_userData)
{
    BUTTON_MenuCallback(menuOpCode_5wayStick_up, _status);
}

void BUTTON_ExtintHandler_UP(void *userData)
{
    BUTTON_ExtIsr(&button_up);
}

extint_handle_t button_extintHandle_up =
{
    .index = RTEPIN_DIGITAL_BUTTON_UP_PIN,
    .handler = BUTTON_ExtintHandler_UP,
    .userData = NULL,
};

button_config_t button_cfg_up =
{
    .read = BUTTON_Read_UP,
    .setInterrupt = BUTTON_SetInterrupt_UP,
    .logic = button_logic_reverse,
    .handler = BUTTON_Handler_UP,
    .userData = NULL,
};

// BUTTON DN

button_t button_dn;

uint32_t BUTTON_Read_DN(void)
{
    GPIO_PinRead(RTEPIN_DIGITAL_BUTTON_DN_GPIO, RTEPIN_DIGITAL_BUTTON_DN_PIN);
}
void BUTTON_SetInterrupt_DN(button_interrupt_t _int)
{
    port_interrupt_t config = BUTTON_InterruptLut(_int);
    PORT_SetPinInterruptConfig(RTEPIN_DIGITAL_BUTTON_DN_PORT, RTEPIN_DIGITAL_BUTTON_DN_PIN, config);
}
void BUTTON_Handler_DN(button_status_t _status, void *_userData)
{
    BUTTON_MenuCallback(menuOpCode_5wayStick_dn, _status);
}

void BUTTON_ExtintHandler_DN(void *userData)
{
    BUTTON_ExtIsr(&button_dn);
}

extint_handle_t button_extintHandle_dn =
{
    .index = RTEPIN_DIGITAL_BUTTON_DN_PIN,
    .handler = BUTTON_ExtintHandler_DN,
    .userData = NULL,
};

button_config_t button_cfg_dn =
{
    .read = BUTTON_Read_DN,
    .setInterrupt = BUTTON_SetInterrupt_DN,
    .logic = button_logic_reverse,
    .handler = BUTTON_Handler_DN,
    .userData = NULL,
};

// BUTTON LF

button_t button_lf;

uint32_t BUTTON_Read_LF(void)
{
    GPIO_PinRead(RTEPIN_DIGITAL_BUTTON_LF_GPIO, RTEPIN_DIGITAL_BUTTON_LF_PIN);
}
void BUTTON_SetInterrupt_LF(button_interrupt_t _int)
{
    port_interrupt_t config = BUTTON_InterruptLut(_int);
    PORT_SetPinInterruptConfig(RTEPIN_DIGITAL_BUTTON_LF_PORT, RTEPIN_DIGITAL_BUTTON_LF_PIN, config);
}
void BUTTON_Handler_LF(button_status_t _status, void *_userData)
{
    BUTTON_MenuCallback(menuOpCode_5wayStick_lf, _status);
}

void BUTTON_ExtintHandler_LF(void *userData)
{
    BUTTON_ExtIsr(&button_lf);
}

extint_handle_t button_extintHandle_lf =
{
    .index = RTEPIN_DIGITAL_BUTTON_LF_PIN,
    .handler = BUTTON_ExtintHandler_LF,
    .userData = NULL,
};

button_config_t button_cfg_lf =
{
    .read = BUTTON_Read_LF,
    .setInterrupt = BUTTON_SetInterrupt_LF,
    .logic = button_logic_reverse,
    .handler = BUTTON_Handler_LF,
    .userData = NULL,
};

// BUTTON RT

button_t button_rt;

uint32_t BUTTON_Read_RT(void)
{
    GPIO_PinRead(RTEPIN_DIGITAL_BUTTON_RT_GPIO, RTEPIN_DIGITAL_BUTTON_RT_PIN);
}
void BUTTON_SetInterrupt_RT(button_interrupt_t _int)
{
    port_interrupt_t config = BUTTON_InterruptLut(_int);
    PORT_SetPinInterruptConfig(RTEPIN_DIGITAL_BUTTON_RT_PORT, RTEPIN_DIGITAL_BUTTON_RT_PIN, config);
}
void BUTTON_Handler_RT(button_status_t _status, void *_userData)
{
    BUTTON_MenuCallback(menuOpCode_5wayStick_rt, _status);
}

void BUTTON_ExtintHandler_RT(void *userData)
{
    BUTTON_ExtIsr(&button_rt);
}

extint_handle_t button_extintHandle_rt =
{
    .index = RTEPIN_DIGITAL_BUTTON_RT_PIN,
    .handler = BUTTON_ExtintHandler_RT,
    .userData = NULL,
};

button_config_t button_cfg_rt =
{
    .read = BUTTON_Read_RT,
    .setInterrupt = BUTTON_SetInterrupt_RT,
    .logic = button_logic_reverse,
    .handler = BUTTON_Handler_RT,
    .userData = NULL,
};






void BUTTON_PitIsr(void *userData)
{
    BUTTON_PitIsr(&button_ok);
    BUTTON_PitIsr(&button_up);
    BUTTON_PitIsr(&button_dn);
    BUTTON_PitIsr(&button_lf);
    BUTTON_PitIsr(&button_rt);
}

pitmgr_handle_t button_pitmgrHandle =
{
    .tickInterval = BUTTON_PITMGR_TIME_MS,
    .tickOffset = 0UL,
    .handler = BUTTON_PitIsr,
    .pptFlag = pitmgr_pptEnable,
    .userData = NULL,
};

extern pitmgr_t pitmgr_main;
extern extint_t extint_porta, extint_portb, extint_portc, extint_portd, extint_porte;

void BUTTON_Init(void)
{
    BUTTON_Setup(&button_ok, &button_cfg_ok);
    BUTTON_Setup(&button_up, &button_cfg_up);
    BUTTON_Setup(&button_dn, &button_cfg_dn);
    BUTTON_Setup(&button_lf, &button_cfg_lf);
    BUTTON_Setup(&button_rt, &button_cfg_rt);

    PITMGR_HandleInsert(&pitmgr_main, &button_pitmgrHandle);

    EXTINT_HandleInsert(&extint_porte, &button_extintHandle_ok);
    EXTINT_HandleInsert(&extint_porte, &button_extintHandle_up);
    EXTINT_HandleInsert(&extint_porte, &button_extintHandle_dn);
    EXTINT_HandleInsert(&extint_porte, &button_extintHandle_lf);
    EXTINT_HandleInsert(&extint_porte, &button_extintHandle_rt);
}

#include "usb_cdc_acm_device.h"
#include "usb_device.h"
#include "usb_configuration.h"
#include "usb_bos.h"
#include "usb_bos_dev_cap_platform_ms.h"
#include "usb_bos_dev_cap_webusb_ms.h"

#include "usb_ms_header.h"
#include "usb_ms_config_subset.h"
#include "usb_ms_func_subset.h"
#include "usb_ms_compatible_ID.h"
#include "usb_ms_registry_property.h"

#include "usb_dcd_rp2040.h"
#include "usb_device_controller.h"

#include "uart_rp2040.h"
#include "gpio_rp2040.h"
#include "task.h"
#include "task_monitor.h"
#include "posix_io.h"
#include <cstdio>
#include <cstring>

int main() {
    uart_rp2040 uart; // default is backchannel UART!

    posix_io::inst.register_stdin(uart);
    posix_io::inst.register_stdout(uart);
    posix_io::inst.register_stderr(uart);

    // ESP8266 UART
    uart_rp2040 uart_esp( 1, 20, 21, 115200 );

    gpio_rp2040_pin button_s1( 16 );
    gpio_rp2040_pin button_s2( 17 );
    gpio_rp2040_pin esp_gpio0( 24 );
    gpio_rp2040_pin esp_gpio2( 25 );
    gpio_rp2040_pin esp_reset(  6 );
    gpio_rp2040_pin led(13);

    button_s1.gpioMode( GPIO::INPUT | GPIO::PULLUP);
    button_s2.gpioMode( GPIO::INPUT | GPIO::PULLUP);
    esp_gpio0.gpioMode( GPIO::OUTPUT_OPEN_DRAIN | GPIO::INIT_HIGH );
    esp_gpio2.gpioMode( GPIO::INPUT);
    esp_reset.gpioMode( GPIO::OUTPUT | GPIO::INIT_HIGH );
    led.gpioMode( GPIO::OUTPUT );

    printf("WifiTick Programmer\n");

    // USB Device driver
    usb_dcd_rp2040 &driver = usb_dcd_rp2040::inst();
    // USB device: Root object of USB descriptor tree
    usb_device device;
    // Generic USB Device Controller on top
    usb_device_controller controller(driver, device);

    ////////////////////////
    // USB device descriptor
    ////////////////////////
    device.set_bcdUSB         (0x0210);
    device.set_bMaxPacketSize0(64);
    device.set_idVendor       (0x2e8a);
    device.set_idProduct      (0x0110);
    device.set_Manufacturer   ("FH Aachen");
    device.set_Product        ("WifiTick Programmer");

    ///////////////////////////////
    // USB configuration descriptor
    ///////////////////////////////
    usb_configuration config(device);
    config.set_bConfigurationValue(1);
    config.set_bmAttributes( { .remote_wakeup = 0,
                               .self_powered  = 0,
                               .bus_powered   = 1 } );
    config.set_bMaxPower_mA(100);

    ////////////////////////
    // USB BOS descriptor //
    ////////////////////////
    // {3408b638-09a9-47a0-8bfd-a0768815b665}
    uint8_t uuid1[16] = { 0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47,
                          0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65 };

    // {d8dd60df-4589-4cc7-9cd2-659d9e648a9f}
    uint8_t uuid2[16] = { 0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C,
                          0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F };

    uint32_t win_version    = 0x06030000;

    usb_bos bos(device);

    usb_bos_dev_cap_webusb_ms web_platform(bos);
    web_platform.set_PlatformCapabilityUUID ( uuid1 );
    web_platform.set_bcdVersion             ( 0x0100 );
    web_platform.set_bVendorCode            ( USB::bRequest_t::REQ_GET_STATUS );
    web_platform.set_iLandingPage           ( "\x01" "fh-aachen.de");

    usb_bos_dev_cap_platform_ms cap_platform(bos);
    cap_platform.set_PlatformCapabilityUUID ( uuid2 );
    cap_platform.set_dwWindowsVersion       ( win_version );
    cap_platform.set_bMS_VendorCode         ( USB::bRequest_t::REQ_GET_DESCRIPTOR );
    cap_platform.set_bAltEnumCode           ( 0 );

    usb_ms_header ms_header                 ( cap_platform );
    ms_header.set_dwWindowsVersion          ( win_version );

    usb_ms_config_subset ms_config_subset   ( ms_header );

    usb_ms_func_subset ms_func_subset       ( ms_config_subset );

    usb_ms_compatible_ID ms_compat_id       ( ms_func_subset );
    ms_compat_id.set_compatible_id          ( "WINUSB" );
    ms_compat_id.set_sub_compatible_id      ( "" );

    usb_ms_registry_property ms_reg_prop    ( ms_func_subset );
    ms_reg_prop.add_property_name           ( "DeviceInterfaceGUIDs\0" );
    ms_reg_prop.add_property_value          ( "{CDB3B5AD-293B-4663-AA36-1AAE46463776}" );

//    printf("Header len %04x\n", ms_header.descriptor.wLength);
//    printf("Header ttl %04x\n", ms_header.descriptor.wTotalLength);
//    printf("Config len %04x\n", ms_config_subset.descriptor.wLength);
//    printf("Config ttl %04x\n", ms_config_subset.descriptor.wTotalLength);
//    printf("Func   len %04x\n", ms_func_subset.descriptor.wLength);
//    printf("Func   sub %04x\n", ms_func_subset.descriptor.wSubsetLength);
//    printf("Compat len %04x\n", ms_compat_id.descriptor.wLength);
//    printf("Prop   len %04x\n", ms_reg_prop.get_length());

    uint8_t buff[256];
    uint8_t buff_url[32] {};
    uint8_t * tmp_ptr = buff;

    auto * ptr = &ms_header.descriptor;
    auto   len =  ms_header.descriptor.wLength;
    memcpy(tmp_ptr, ptr, len);
    tmp_ptr += len;
    for (uint16_t c=0; c < ms_header._config_subsets.size(); ++c) {
        auto * ptr = &ms_header._config_subsets[c]->descriptor;
        auto   len =  ms_header._config_subsets[c]->descriptor.wLength;
        memcpy(tmp_ptr, ptr, len);
        tmp_ptr += len;
        for (uint16_t f=0; f < ms_header._config_subsets[c]->_func_subsets.size(); ++f) {
            auto  ptr = &ms_header._config_subsets[c]->_func_subsets[f]->descriptor;
            auto  len  = ms_header._config_subsets[c]->_func_subsets[f]->descriptor.wLength;
            memcpy(tmp_ptr, ptr, len);
            tmp_ptr += len;
            if (ms_header._config_subsets[c]->_func_subsets[f]->_compat_id) {
                auto ptr = &ms_header._config_subsets[c]->_func_subsets[f]->_compat_id->descriptor;
                auto len =  ms_header._config_subsets[c]->_func_subsets[f]->_compat_id->descriptor.wLength;
                memcpy(tmp_ptr, ptr, len);
                tmp_ptr += len;
            }
            for (uint16_t p=0; p < ms_header._config_subsets[c]->_func_subsets[f]->_reg_props.size(); ++p) {
                auto ptr = ms_header._config_subsets[c]->_func_subsets[f]->_reg_props[p]->descriptor();
                auto len = ms_header._config_subsets[c]->_func_subsets[f]->_reg_props[p]->get_length();
                memcpy(tmp_ptr, ptr, len);
                tmp_ptr += len;
            }
        }
    }

    device.setup_handler = [&] (USB::setup_packet_t * pkt) {
        printf("In device vendor handler! %d\n", pkt->wLength);
        if ( (pkt->bRequest == USB::bRequest_t::REQ_GET_STATUS) &&
             (pkt->wIndex == 2) ) {
            // URL
            uint8_t len = usb_strings::inst.prepare_buffer_utf8(pkt->wValue, buff_url);
            if (len > pkt->wLength) len = pkt->wLength;
            controller._ep0_in->start_transfer(buff_url, len);
        } else if ( (pkt->bRequest == USB::bRequest_t::REQ_GET_DESCRIPTOR) &&
                    (pkt->wValue   == 0) && (pkt->wIndex == 7) ) {
            // Header
            controller._ep0_in->start_transfer(buff, tmp_ptr-buff);
        } else {
            controller._ep0_in->send_stall(true);
            controller._ep0_out->send_stall(true);
        }
    };

    /////////////////////
    // USB CDC ACM device
    /////////////////////
    usb_cdc_acm_device acm_device(controller, config);

    bool line_code_updated = false;

    acm_device.line_coding_handler = ([&](uint8_t *,uint16_t) {
        line_code_updated = true;
    });

    bool in_prgm_mode = false;

    acm_device.control_line_handler = ([&](bool dtr, bool rts) {
//        printf("Control signals DTR=%d, RTS=%d\n", dtr, rts);
        if (esp_reset == LOW && rts == LOW) {
            if (dtr) {
                // Programming mode
                printf("Program mode\n");
                uart_esp.setBaudrate(115200);
                in_prgm_mode = true;
            } else {
                // Run mode
                printf("Run mode\n");
                uart_esp.setBaudrate(74880);
                in_prgm_mode = false;
            }
        }
        esp_gpio0 = !dtr;
        esp_reset = !rts;
    });

    acm_device.break_handler = ([](uint16_t millis) {
        printf("Send break for 0x%04x milliseconds\n", millis);
    });

    //////////////////////
    // Activate USB device
    //////////////////////
    driver.pullup_enable(true);

    // GPIO3 is connected to ESP8266 GPIO15, which controls the
    // boot-mode of the ESP8266. The WiFiTick-board pulls this
    // pin to LOW, but the EDU-MKII pulls it to HIGH (because it
    // is the READY-line of the temperature sensor). So force
    // this pin to low so the ESP8266 is booting correctly.
    gpio_rp2040_pin gpio3 (3);
    gpio3.gpioMode(GPIO::OUTPUT | GPIO::INIT_LOW);

    //////////////////////////////////////////
    // Wait until USB enumeration has finished
    //////////////////////////////////////////
    printf("Waiting for USB connection on USB TARGET..\n");
    while (!controller.active_configuration) {
        task::sleep(100);
    }

    CDC::bmUartState_t uart_state;
    uart_state.bRxCarrier_DCD = 1;
    uart_state.bTxCarrier_DSR = 1;
    acm_device.notify_serial_state( uart_state );

    esp_gpio2.gpioAttachIrq(GPIO::RISING | GPIO::FALLING, [&]() {
        led = !esp_gpio2;
    });

    uart_esp.uartAttachIrq([&](char c) {
        if (!in_prgm_mode && (c == 0xb0)) {
            uart_esp.setBaudrate(115200);
            const char * foo = "\r\n---end of boot messages---\r\n\n";
            acm_device.write((uint8_t *)foo, 31);
        } else {
            acm_device.write((uint8_t *)&c, 1);
        }
    });

    printf("Entering endless loop...\n");

    uint8_t buf[64];
    while (1) {
        if (line_code_updated) {
//            const char *parity[5] = {"N", "O", "E", "M", "S"};
//            const char *stop[3] = {"1", "1.5", "2"};
//            printf("Line coding set to %d baud %d%s%s\n",
//                   (int) acm_device.line_coding.dwDTERate,
//                   (int) acm_device.line_coding.bDataBits,
//                   parity[(int) acm_device.line_coding.bParityType],
//                   stop[(int) acm_device.line_coding.bCharFormat]);

            uart_mode_t mode = 0;
            switch ((int) acm_device.line_coding.bDataBits) {
                case 7:
                    mode |= UART::BITS_7;
                    break;
                case 8:
                    mode |= UART::BITS_8;
                    break;
                default:
                    printf("Wrong number of bits!!\n");
            }
            switch ((int) acm_device.line_coding.bParityType) {
                case (int) CDC::bParityType_t::PARITY_NONE:
                    mode |= UART::NO_PARITY;
                    break;
                case (int) CDC::bParityType_t::PARITY_EVEN:
                    mode |= UART::EVEN_PARITY;
                    break;
                case (int) CDC::bParityType_t::PARITY_ODD:
                    mode |= UART::ODD_PARITY;
                    break;
                default:
                    printf("Wrong parity!\n");
            }
            switch ((int) acm_device.line_coding.bCharFormat) {
                case (int) CDC::bCharFormat_t::STOP_BITS_1:
                    mode |= UART::STOPBITS_1;
                    break;
                case (int) CDC::bCharFormat_t::STOP_BITS_2:
                    mode |= UART::STOPBITS_2;
                    break;
                default:
                    printf("Wrong stop bits!\n");
            }
            uart_esp.uartMode(mode);
            uart_esp.setBaudrate(acm_device.line_coding.dwDTERate);
            line_code_updated = false;
        }

        // Handle buttons in main loop
        if (button_s1 == LOW) {
            esp_reset = LOW;
            esp_gpio0 = HIGH;
            uart_esp.setBaudrate(74880);
            in_prgm_mode = false;
            while(!button_s1) ;
            task::sleep(100);
            esp_reset = HIGH;
        }

        // Copy data from host to ESP8266
        uint16_t len = acm_device.read(buf, 64);
        for (int i=0; i < len; ++i) {
            uart_esp.putc(buf[i]);
        }
    }
}

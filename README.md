# XcpLight
Copyright (c) 2016-2019 0xFAB - Fabian Zahn

[![Build Status](https://travis-ci.org/farrrb/XcpLight.svg?branch=master)](https://travis-ci.org/farrrb/XcpLight)

## Summary
This projects goal is to develop a lightweight implementation of the "Universal Measurement and Calibration Protocol" - protocol layer.

## Features
- Login without seed & key (CONNECT & DISCONNECT)
- Seed & Key
- Some Basic Information about the driver (CMD_GET_COMM_MODE_INFO & XCP_CMD_GET_STATUS)
- Read data via polling (UPLOAD & SHORT_UPLOAD)
- Write data (DOWNLOAD)
- User defined command via XCP protocol layer (USER_CMD)

## Limitations
- Block transfer is not supported (UPLOAD, DOWNLOAD, PROGRAM)
- The "ECU_STATES" (STATE_NUMBER) are not supported
- The "RESUME" mode is not supported
- DAQ & STIM are note supported

## TODOs
- Implement SHORT_DOWNLOAD
- Provide basic self information about the driver configuration
- Support dynamic DAQ lists
- Implement PGM

## Integration / API
1. Add all the files from the 'source' directory to your project
2. Write your custom implementation for your platform in XcpLightCfg.h/.c
   You have to implement/rewrite the following functions:

```C
  void XcpLightCfg_sendMessage(XcpLightMessage_t *msg)
  {
    // send a message via your transport layer
  }

  void * XcpLightCfg_getPointer(uint32_t address, uint8_t address_extension)
  {
    // get a pointer from 'address' and the 'address_extension'
  }

  void XcpLightCfg_readFromAddress(uint8_t *source, uint8_t length, uint8_t *buffer)
  {
    // read from 'source 'length' bytes into 'buffer'
  }

  void XcpLightCfg_writeToAddress(uint8_t *dest, uint8_t length, uint8_t *data)
  {
    // write the content of 'data' to 'dest' with 'length' bytes
  }
```

3. And last but not least tell your transport layer to put messages to the XCP command processor

```C
void your_transport_layer_processor(void)
{
  // some code
  if (getType(msg) == MESSAGE_TYPE_XCP)
  {
    XcpLight_processCommand(GenericMsgToXcmsg(msg));
  }
}
```

4. And there is another thing. For the integration of the XcpLight you have to call the following functions

```C
void XcpLight_init(void);                                 // Init the XCP command processor - obviously called once at system startup
void XcpLight_updateTimestampCounter(void);               // Update the timestamp counter - call this every 1 ms
void XcpLight_processCommand(XcpLightMessage_t *msg);     // The command processor - as mentioned above
```

## Resources
[XCP on ASAM Wiki](https://wiki.asam.net/display/STANDARDS/ASAM+MCD-1+XCP)
[XCP Reference Book by Vector Informatik GmbH](https://vector.com/vi_xcp-book_en.html)
[XCP on Wikipedia](https://en.wikipedia.org/wiki/XCP_(protocol))
[XCP 1.0 Specification](https://www.feaser.com/openblt/lib/exe/fetch.php?media=manual:xcp_1_0_specification.zip)

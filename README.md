# XcpLight
Copyright (c) 2016 0xFAB - Fabian Zahn   

[![Build Status](https://travis-ci.org/farrrb/XcpLight.svg?branch=master)](https://travis-ci.org/farrrb/XcpLight)

## Summary
This projects at developing a lightweight implementation of the "Universal Measurement and Calibration Protocol" - protocol layer. Do not use this in a production code environment! It's not done at all, yet.

## Features
- Login works without seed & key (CONNECT & DISCONNECT)
- Some Basic Information about the driver (CMD_GET_COMM_MODE_INFO & XCP_CMD_GET_STATUS)
- Readout data via polling (SHORT_UPLOAD)
- Write data (DOWNLOAD)

## Limitations
- Block transfer is not supported (UPLOAD, DOWNLOAD, PROGRAM)
- The "ECU_STATES" (STATE_NUMBER) are not supported
- The "RESUME" mode is not supported
- DAQ & STIM are note supported

## Todos
- Implement UPLOAD & SHORT_DOWNLOAD
- Provide basic self information about the driver configuration
- Support dynamic DAQ lists

## Integration / API
1. Add all the files from the 'source' directory to your project
2. Write your custom implementation for your platform in XcpLight_config.h/.c 
   You have to implement/rewrite the following functions:

```C
  void XcpLight_SendMessage(XcpLightMessage_t * pMsg)
  {
    // send a message via your transport layer
  }

  void * XcpLight_GetPointer(uint32_t address, uint8_t address_extension)
  {
    // get a pointer from 'address' and the 'address_extension'
  }

  void XcpLight_ReadFromAddress(uint8_t * addr, uint8_t length, uint8_t * buffer)
  {
    // read from 'address 'length' bytes into 'buffer'
  }

  void XcpLight_WriteToAddress(uint8_t * addr, uint8_t length, uint8_t * data)
  {
    // write the content of 'data' to 'address' with 'length' bytes
  }
```

3. And last but not least tell your transport layer to put messages to the XCP command processor

```C
void your_transport_layer_processor(void)
{
  // some code
  if (GetType(msg) == MESSAGE_TYPE_XCP)
  {
    XcpLight_CommandProcessor(GenericMsgToXcpMsg(msg));
  }
}
```

4. And there is another thing. For the integration of the XcpLight you have to call the following functions 

```C 
void XcpLight_Init(void);                                 // init the xcp command processor - obviously called once at system startup
void XcpLight_UpdateTimestampCounter(void);               // update the timestamp counter - call this every 1 ms
void XcpLight_CommandProcessor(XcpLightMessage_t * pMsg); // the command processor - as mentioned above
int  XcpLight_Event(uint8_t eventNo);                     // the DAQ events (not supported yet) - you don't have to call that
```

## Resources
[XCP on ASAM Wiki](https://wiki.asam.net/display/STANDARDS/ASAM+MCD-1+XCP)   
[XCP Reference Book by Vector Informatik GmbH](https://vector.com/vi_xcp-book_en.html)   
[XCP on Wikipedia](https://en.wikipedia.org/wiki/XCP_(protocol))   

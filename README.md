# XcpLight
Copyright (c) 2016 0xFAB - Fabian Zahn   

[![Build Status](https://travis-ci.org/farrrb/XcpLight.svg?branch=master)](https://travis-ci.org/farrrb/XcpLight)

## Summary
This projects at developing a lightweight implementation of the "Universal Measurement and Calibration Protocol" - protocol layer. Do not use this in a production code environment! It's not done at all, yet.

## Features
- Login works without seed & key
- SHORT_UPLOAD and UPLOAD
- SHORT_DOWNLOAD and DOWNLOAD

## Limitations
- Block transfer is not supported (UPLOAD, DOWNLOAD, PROGRAM)
- The "ECU_STATES" (STATE_NUMBER) are not supported
- The "RESUME" mode is not supported
- DAQ & STIM are note supported

## Todos
- Provide basic self information about the driver configuration
- Support dynamic DAQ lists

## Integration / API
- t.b.d.

## Resources
[XCP on ASAM Wiki](https://wiki.asam.net/display/STANDARDS/ASAM+MCD-1+XCP)   
[XCP Reference Book by Vector Informatik GmbH](http://vector.com/portal/medien/solutions_for/xcp/XCP_ReferenceBook_V1.0_EN.pdf)   
[XCP on Wikipedia](https://en.wikipedia.org/wiki/XCP_(protocol))   

# XcpLight
Copyright (c) 2016 0xFAB - Fabian Zahn   

[![Build Status](https://travis-ci.org/farrrb/XcpLight.svg?branch=master)](https://travis-ci.org/farrrb/XcpLight)

## Summary
This projects at developing a lightweight implementation of the "Universal Measurement and Calibration Protocol" - protocol layer. Do not use this in a production code environment! It's not done at all, yet.

## Features
- Login works without seed & key
- SHORT_UPLOAD
- DOWNLOAD

## Limitations
- Block transfer is not supported (UPLOAD, DOWNLOAD, PROGRAM)
- ECU_STATES (STATE_NUMBER) is not supported
- RESUME is not supported

## Todos
- Provide basic self information about the driver configuration
- Synchronous data readout via dynamic DAQ lists
- Choose a unit test framework & implement tests

## Integration / API

## Resources
[XCP on Wikipedia](https://en.wikipedia.org/wiki/XCP_(protocol))   
[XCP Reference Book by Vector Informatik GmbH](http://vector.com/portal/medien/solutions_for/xcp/XCP_ReferenceBook_V1.0_EN.pdf)   

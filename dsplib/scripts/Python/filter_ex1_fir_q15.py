# --COPYRIGHT--,BSD
# Copyright (c) 2016, Texas Instruments Incorporated
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# *  Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# *  Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# *  Neither the name of Texas Instruments Incorporated nor the names of
#    its contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# --/COPYRIGHT--*/

#!/usr/bin/env python

#*******************************************************************************
# filter_ex1_fir_q15.py: Example Python script to generate coefficients for the
# filter_ex1_fir_q15 code example.
#
# Brent Peterson
# Texas Instruments 2016
#*******************************************************************************

import os
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

# Generate 15th order lowpass FIR filter
n = 16
fc = 1200.0
fs = 8192.0
b = signal.firwin(n, fc/(fs/2), pass_zero=True)
w, h = signal.freqz(b)

# Plot frequency and phase response
fig = plt.figure()
plt.title('Digital filter frequency response')
ax1 = fig.add_subplot(111)
plt.plot(fs*w/(2*np.pi), 20 * np.log10(abs(h)), 'b')
plt.ylabel('Amplitude [dB]', color='b')
plt.xlabel('Frequency [Hz]')
ax2 = ax1.twinx()
plt.plot(fs*w/(2*np.pi), np.unwrap(np.angle(h)), 'g')
plt.ylabel('Angle (radians)', color='g')
plt.grid()
plt.show()

# Output coefficients to file
directory = 'output'
name = 'myCoeffs_ex1'

# Create directory if it does not exist
if not os.path.exists(directory):
    os.makedirs(directory)

# Create and write source file
with open('%s/%s.c' % (directory,name), 'w') as fc:
    fc.write('#include "DSPLib.h"\n\n')
    fc.write('#include "%s.h"\n\n' % name)
    fc.write('DSPLIB_DATA(%s,4)\n' % name)
    fc.write('_q15 %s[%d] = {' % (name, b.size))
    for c in b:
        fc.write('\n    _Q15(%+1.6f),' % c)
    fc.write('\n};\n')

# Create and write header file
with open('%s/%s.h' % (directory,name), 'w') as fc:
    fc.write('#ifndef %s_H_\n' % name.upper())
    fc.write('#define %s_H_\n\n' % name.upper())
    fc.write('#include "DSPLib.h"\n\n')
    fc.write('#define FILTER_COEFFS_EX1 %s\n\n' % name)
    fc.write('extern _q15 %s[%d];\n\n' % (name, b.size))
    fc.write('#endif /* %s_H_ */\n' % name.upper())

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
# filter_ex4_biquad_cascade_df2_q15.py: Example Python script to generate 
# coefficients for the filter_ex4_biquad_cascade_df2_q15 code example.
#
# Brent Peterson
# Texas Instruments 2016
#*******************************************************************************

import os
import numpy as np
from scipy import signal
import matplotlib.pyplot as plt

# Generate 16th order lowpass IIR filter
n = 16
fc = 1800.0
fs = 8192.0
rp = 0.0
rs = 80.0
b, a = signal.iirfilter(n, fc/(fs/2), rp, rs, btype='lowpass', ftype='cheby2')
    
# Convert to SOS matrix 
sos = signal.tf2sos(b, a)

# Scale b0, b1 and b2 coefficients using L-inf norm
h = np.ones(256);
g = 1.0;
for i in range(len(sos)):
    h = g * h * signal.freqz(sos[i,0:3], sos[i,3:6], 256)[1];
    g = 1/np.max(np.abs(h));
    sos[i][0:3] = sos[i][0:3] * g;

# Plot frequency and phase response
w, h = signal.freqz(b, a)
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
name = 'myCoeffs_ex4'

# Create directory if it does not exist
if not os.path.exists(directory):
    os.makedirs(directory)

# Create and write source file
with open('%s/%s.c' % (directory,name), 'w') as fc:
    fc.write('#include "DSPLib.h"\n\n')
    fc.write('#include "%s.h"\n\n' % name)
    fc.write('DSPLIB_DATA(%s,4)\n' % name)
    fc.write('msp_biquad_df2_q15_coeffs %s[%d] = {' % (name, sos[:,0].size))
    for stage in sos:
        fc.write('\n    {\n')
        fc.write('        .b0     = _Q15(%+1.6f),\n' % stage[0])
        fc.write('        .b1By2  = _Q15(%+1.6f/2),\n' % stage[1])
        fc.write('        .b2     = _Q15(%+1.6f),\n' % stage[2])
        fc.write('        .a0     = _Q15(%+1.6f),\n' % stage[3])
        fc.write('        .a1By2  = _Q15(%+1.6f/2),\n' % -stage[4])
        fc.write('        .a2     = _Q15(%+1.6f)\n' % -stage[5])
        fc.write('    },')
    fc.write('\n};\n')

# Create and write header file
with open('%s/%s.h' % (directory,name), 'w') as fc:
    fc.write('#ifndef %s_H_\n' % name.upper())
    fc.write('#define %s_H_\n\n' % name.upper())
    fc.write('#include "DSPLib.h"\n\n')
    fc.write('#define FILTER_COEFFS_EX4 %s\n\n' % name)
    fc.write('extern msp_biquad_df2_q15_coeffs %s[%d];\n\n' % (name, sos[:,0].size))
    fc.write('#endif /* %s_H_ */\n' % name.upper())

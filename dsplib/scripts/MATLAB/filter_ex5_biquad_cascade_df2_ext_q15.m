% --COPYRIGHT--,BSD
% Copyright (c) 2016, Texas Instruments Incorporated
% All rights reserved.
%
% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions
% are met:
%
% *  Redistributions of source code must retain the above copyright
%    notice, this list of conditions and the following disclaimer.
%
% *  Redistributions in binary form must reproduce the above copyright
%    notice, this list of conditions and the following disclaimer in the
%    documentation and/or other materials provided with the distribution.
%
% *  Neither the name of Texas Instruments Incorporated nor the names of
%    its contributors may be used to endorse or promote products derived
%    from this software without specific prior written permission.
%
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
% AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
% THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
% PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
% CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
% EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
% PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
% OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
% WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
% OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
% EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
% --/COPYRIGHT--*/

%*******************************************************************************
% filter_ex5_biquad_cascade_df2_ext_q15.m: Example MATLAB script to generate 
% coefficients for the filter_ex5_biquad_cascade_df2_ext_q15 code example.
%
% Brent Peterson
% Texas Instruments 2016
%*******************************************************************************

% Generate 16th order lowpass IIR filter
n = 16;
fc = 1800.0;
fs = 8192.0;
rs = 80;
bias = 0.0;
[b,a] = cheby2(n, rs, fc/(fs/2), 'low');
    
% Convert to SOS matrix 
sos = tf2sos(b,a);

% Scale b0, b1 and b2 coefficients using L-inf norm
h = ones(256,1);
g = 1.0;
for j=1:size(sos,1)
    h = g .* h .* freqz(sos(j,1:3), sos(j,4:6), 256);
    g = 1/max(abs(h));
    sos(j,1:3) = sos(j,1:3) .* g;
end

% Plot frequency and phase response
freqz(sos,[],fs);

% Output coefficients to file
directory = 'output';
name = 'myCoeffs_ex5';

% Create directory if it does not exist
if ~exist(directory, 'dir')
    mkdir(directory);
end

% Create and write source file
fc = fopen(strcat(directory,'/',name,'.c'), 'w');
fprintf(fc, '#include "DSPLib.h"\n\n');
fprintf(fc, ['#include "',name,'.h"\n\n']);
fprintf(fc, ['DSPLIB_DATA(',name,',4)\n']);
fprintf(fc, ['msp_biquad_df2_ext_q15_coeffs ',name,'[',num2str(size(sos,1)),'] = {']);
for i = 1:size(sos,1)
    fprintf(fc, '\n    {\n');
    fprintf(fc, ['        .b0     = _Q15(',num2str(sos(i,1),'%+1.6f'),'),\n']);
    fprintf(fc, ['        .b1By2  = _Q15(',num2str(sos(i,2),'%+1.6f'),'/2),\n']);
    fprintf(fc, ['        .b2     = _Q15(',num2str(sos(i,3),'%+1.6f'),'),\n']);
    fprintf(fc, ['        .a0     = _Q15(',num2str(sos(i,4),'%+1.6f'),'),\n']);
    fprintf(fc, ['        .a1By2  = _Q15(',num2str(-sos(i,5),'%+1.6f'),'/2),\n']);
    fprintf(fc, ['        .a2     = _Q15(',num2str(-sos(i,6),'%+1.6f'),'),\n']);
    fprintf(fc, ['        .bias   = _Q15(',num2str(bias,'%+1.6f'),')\n']);
    fprintf(fc, '    },');
end
fprintf(fc, '\n};\n');
    
% Create and write header file
fh = fopen([directory,'/',name,'.h'], 'w');
fprintf(fh, ['#ifndef ',upper(name),'_H_\n']);
fprintf(fh, ['#define ',upper(name),'_H_\n\n']);
fprintf(fh, '#include "DSPLib.h"\n\n');
fprintf(fh, ['#define FILTER_COEFFS_EX5 ',name,'\n\n']);
fprintf(fh, ['extern msp_biquad_df2_ext_q15_coeffs ',name,'[',num2str(size(sos,1)),'];\n\n']);
fprintf(fh, ['#endif /* ',upper(name),'_H_ */\n']);

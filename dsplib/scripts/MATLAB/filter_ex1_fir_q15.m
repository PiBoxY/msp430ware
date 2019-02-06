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
% filter_ex1_fir_q15.m: Example MATLAB script to generate coefficients for the
% filter_ex1_fir_q15 code example.
%
% Brent Peterson
% Texas Instruments 2016
%*******************************************************************************

% Generate 15th order lowpass FIR filter
n = 15;
fc = 1200.0;
fs = 8192.0;
b = fir1(n, fc/(fs/2), 'low');

% Plot frequency and phase response
freqz(b,1,[],fs);

% Output coefficients to file
directory = 'output';
name = 'myCoeffs_ex1';

% Create directory if it does not exist
if ~exist(directory, 'dir')
    mkdir(directory);
end

% Create and write source file
fc = fopen(strcat(directory,'/',name,'.c'), 'w');
fprintf(fc, '#include "DSPLib.h"\n\n');
fprintf(fc, ['#include "',name,'.h"\n\n']);
fprintf(fc, ['DSPLIB_DATA(',name,',4)\n']);
fprintf(fc, ['_q15 ',name,'[',num2str(length(b)),'] = {']);
for i = 1:length(b)
    fprintf(fc, ['\n    _Q15(',num2str(b(i),'%+1.6f'),'),']);
end
fprintf(fc, '\n};\n');
    
% Create and write header file
fh = fopen([directory,'/',name,'.h'], 'w');
fprintf(fh, ['#ifndef ',upper(name),'_H_\n']);
fprintf(fh, ['#define ',upper(name),'_H_\n\n']);
fprintf(fh, '#include "DSPLib.h"\n\n');
fprintf(fh, ['#define FILTER_COEFFS_EX1 ',name,'\n\n']);
fprintf(fh, ['extern _q15 ',name,'[',num2str(length(b)),'];\n\n']);
fprintf(fh, ['#endif /* ',upper(name),'_H_ */\n']);

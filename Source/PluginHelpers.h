/*
  ==============================================================================

    PluginHelpers.h
    Created: 13 May 2019 8:09:10pm
    Author:  Eric

  ==============================================================================
*/

#pragma once

#include <cmath>
#include <vector>

const int kNumVoices = 16;
const double kParameterSmoothingCoeff_Fine = 0.005f;
const double kMinimumDecibels = -24.0;

inline double pitchShiftMultiplier(double semitonesToShift)
{
    // 2^(N/12)
    return pow(2.0, semitonesToShift / 12.0);
}

// returns the frequency multiplier for the pitch change
inline double getPitchFreqMod(double octaves, double semitones, double cents)
{
    return pitchShiftMultiplier(octaves * 12 + semitones + cents / 100.0);
}

inline double tanh_clip(double x)
{
    return x * (27.0 + x * x) / (27.0 + 9.0 * x * x);
}

inline float linear_interp(float v0, float v1, float t)
{
    return (1 - t) * v0 + t * v1;
}

class Differentiator
{
public:
    Differentiator(double value)
    :   priorValue(value) {}
    
    double process(double value)
    {
        double  result = value - priorValue;
        priorValue = value;
        return result;
    }
    
    void reset(double value) { priorValue = value; }
    
private:
    double priorValue;
};

//
// fft
//
// I grabbed (and slightly modified) this Rabiner & Gold translation...
//
// (could modify for real data, could use a template version, blah blah--just keeping it short)
//
//inline void fft(int N, double *ar, double *ai)
inline void fft(int N, std::vector<double>& ar, std::vector<double>& ai)
/*
 in-place complex fft
 
 After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)
 
 program adapted from FORTRAN
 by K. Steiglitz  (ken@princeton.edu)
 Computer Science Dept.
 Princeton University 08544          */
{
    int i, j, k, L;            /* indexes */
    int M, TEMP, LE, LE1, ip;  /* M = log N */
    int NV2, NM1;
    double t;               /* temp */
    double Ur, Ui, Wr, Wi, Tr, Ti;
    double Ur_old;
    
    // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2
    
    NV2 = N >> 1;
    NM1 = N - 1;
    TEMP = N; /* get M = log N */
    M = 0;
    while (TEMP >>= 1) ++M;
    
    /* shuffle */
    j = 1;
    for (i = 1; i <= NM1; i++)
    {
        if(i<j)
        {             /* swap a[i] and a[j] */
            t = ar[j-1];
            ar[j-1] = ar[i-1];
            ar[i-1] = t;
            t = ai[j-1];
            ai[j-1] = ai[i-1];
            ai[i-1] = t;
        }
        
        k = NV2;             /* bit-reversed counter */
        while(k < j)
        {
            j -= k;
            k /= 2;
        }
        
        j += k;
    }
    
    LE = 1.;
    for (L = 1; L <= M; L++)
    {                                    // stage L
        LE1 = LE;                        // (LE1 = LE/2)
        LE *= 2;                         // (LE = 2^L)
        Ur = 1.0;
        Ui = 0.;
        Wr = cos(M_PI/(float)LE1);
        Wi = -sin(M_PI/(float)LE1); // Cooley, Lewis, and Welch have "+" here
        for (j = 1; j <= LE1; j++)
        {
            for (i = j; i <= N; i += LE) { // butterfly
                ip = i+LE1;
                Tr = ar[ip-1] * Ur - ai[ip-1] * Ui;
                Ti = ar[ip-1] * Ui + ai[ip-1] * Ur;
                ar[ip-1] = ar[i-1] - Tr;
                ai[ip-1] = ai[i-1] - Ti;
                ar[i-1]  = ar[i-1] + Tr;
                ai[i-1]  = ai[i-1] + Ti;
            }
            Ur_old = Ur;
            Ur = Ur_old * Wr - Ui * Wi;
            Ui = Ur_old * Wi + Ui * Wr;
        }
    }
}


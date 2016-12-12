/**
 * \page page_wave_theory Wave theory
 * \tableofcontents
 * \image html Water_wave_theories.svg
 * \image latex Water_wave_theories.eps "Water wave theories" width=0.8\textwidth
 */

/**
 * \page page_spectrum Wave spectra
 * \tableofcontents
 * \section spectrumIntroduction Introduction

    A wave spectrum is the distribution of the wave energy as a function of frequency.
    It describes the total energy transmitted by a wave-field at a given time.

    \f$S_X (\omega ) = \frac{1}{{2\pi }}\int\limits_{ - \infty }^{ + \infty } {R_X (\tau )\exp ( - i\omega \tau )d\tau }\f$

    with:

        - \f$\omega\f$: : wave circular frequency (\f$\textrm{rad}/s\f$)
        - \f$R_X (\tau)=E\left[ {X\left( t \right).X\left( {t + \tau } \right)} \right]\f$
          autocorrelation function of the free surface time series; it also can be written:

            \f[R_X (\tau ) = \int\limits_{ - \infty }^{ + \infty } {S_X (\omega )\exp (i\omega \tau )d\omega } \f]

    If \f$\tau = 0\f$, then \f$R_X (0) = E\left[ {X^2 } \right] = \int\limits_{ - \infty }^{ + \infty } {S_X (\omega )d\omega }  = \int\limits_0^{ + \infty } {\Phi _X (\omega )d\omega } \f$,
    then \f$\Phi _\text{X} \left( \omega  \right)\f$ is the power spectral density function defined for \f$\omega \geqslant 0\f$.
    \f[\begin{cases}
       \Phi _\text{X} \left( \omega  \right) = 2S_X \left( \omega  \right)&\text{ if }\omega  \geqslant 0  \\
       \Phi _\text{X} \left( \omega  \right) = 0&\text{ otherwise}
     \end{cases}\f]

    Seven wave spectra are available within the tool. Each spectrum needs from 1 to 6 parameters. The available spectra are described below.

 * \section section_waves_spectrum_Pierson Spectrum 1: Pierson-Moskovitz spectrum
   The Pierson-Moskovitz spectrum is used for fully developed seas in North Atlantic.

   \f$\Phi_\eta(\omega ) = \frac{A}{{\omega ^5 }}\exp \left[ { - \frac{B}{{\omega ^4 }}} \right],\forall \omega  \geqslant 0\f$

   with:

    - \f$A = 0.0081g^2\f$
    - \f$B = 0.74\left( {\frac{g}{V}} \right)^4 \f$
    - \f$g\f$: constant of gravity, in \f$m/s^2\f$;
    - \f$V\f$: wind speed at a height of 19.5 \f$m\f$ above the sea surface, in \f$m/s\f$.

    Therefore this spectrum needs only one parameter:

    - parameter 1: wind speed (in \f$m/s\f$) at a height of 19.5 \f$m\f$ above the sea surface.

 * \image html spectrumPiersonMoskowitz.svg
 * \image latex spectrumPiersonMoskowitz.eps "Pierson-Moskovitz spectrum" width=0.8\textwidth

 * \section section_waves_spectrum_ITTC Spectrum 2: ITTC spectrum

    \f$\Phi_\eta (\omega ) = \frac{A}{{\omega ^5 }}\exp \left[ { - \frac{B}{{\omega ^4 }}} \right],\forall \omega  \geqslant 0\f$

    with:

    - \f$A = 0.0081g^2\f$
    - \f$B = \frac{{3.11}}{{H_{1/3}^2 }}\f$
    - \f$g\f$: constant of gravity, in \f$m/s^2\f$ ;
    - \f$H_{1/3}\f$: mean of the highest one-third wave height (significant height) in \f$m\f$.

    Therefore this spectrum needs only one parameter:

    - parameter 1: significant wave height (in \f$m\f$).

 * \image html spectrumITTC.svg
 * \image latex spectrumITTC.eps "ITTC spectrum" width=0.8\textwidth

 * \section section_waves_spectrum_ISSC Spectrum 3: ISSC (or 2-parameters Bretschneider) spectrum

   \f$\Phi_\eta (\omega ) = \frac{A}{{\omega ^5 }}\exp \left[ { - \frac{B}{{\omega ^4 }}} \right],\forall \omega  \geqslant 0\f$

   with:

    - \f$A = 5\pi ^4 \frac{{H_s^2 }}{{T_p ^4 }} = \frac{B.H_s^2}{4}\omega_p^4\f$
    - \f$B = \frac{{20\pi ^4 }}{{T_p ^4 }} = \frac{5}{4}\omega_p^4\f$
    - \f$H_s\f$: significant wave height, in \f$m\f$
    - \f$T_p\f$: peak period, in \f$s\f$
    - \f$\omega_p = \frac{{2\pi }}{{T_p }}\f$: angular peak frequency, in \f$\textrm{rad}/s\f$

    Therefore this spectrum needs 2 parameters:

    - parameter 1: significant wave height (in \f$m\f$);
    - parameter 2: peak period (in \f$s\f$).

 * \image html spectrumBretschneider.svg
 * \image latex spectrumBretschneider.eps "ISSC Bretschneider spectrum" width=0.8\textwidth

 * \section section_waves_spectrum_Jonswap Spectrum 4: JONSWAP spectrum

   The JONSWAP (Joint North Sea Wave Project) spectrum is used for non-fully developed seas, for example in North Sea.

   \f$\Phi _\eta  (\omega ) = \left( {1 - 0.287\ln (\gamma )} \right)\frac{A}
     {\omega ^5}\exp \left[ { - \frac{B}
     {\omega ^4}} \right].\gamma^
     {\exp \left[ - \frac{1}{2} \left(\frac{\omega - \omega_p}{\sigma \omega_p}\right)^2 \right]} ,\forall \omega  \geqslant 0
    \f$

   with:

    - \f$A = 5\pi ^4 \frac{{H_s^2 }}{{T_p ^4 }} = \frac{B.H_s^2}{4}\omega_p^4\f$
    - \f$B = \frac{{20\pi ^4 }}{{T_p ^4 }} = \frac{5}{4}\omega_p^4\f$
    - \f$H_s\f$: significant wave height, in \f$m\f$
    - \f$T_p\f$: peak period, in \f$s\f$
    - \f$\gamma\f$: peak enhancement factor
    - \f$\omega_p = \frac{{2\pi }}{{T_p }}\f$: angular peak frequency, in \f$\textrm{rad}/s\f$
    - \f$\sigma\f$: spectral width parameter.

    \f$\sigma=\sigma_a\f$ for \f$\omega<\omega_p\f$

    \f$\sigma=\sigma_b\f$ for \f$\omega>\omega_p\f$

    Usually, \f$\sigma_a=0.07\f$ and \f$\sigma_b=0.09\f$.

    \f$\gamma\f$ is usually taken between 1 and 7, and \f$\gamma=3.3\f$ for North Sea.
    For \f$\gamma=1\f$, the JONSWAP spectrum reduces to the ISSC spectrum.

    Therefore this spectrum needs 3 parameters:
    - parameter 1: significant wave height (in \f$m\f$);
    - parameter 2: peak period (in \f$s\f$);
    - parameter 3: peak enhancement factor \f$\gamma\f$.

 * \image html spectrumJonswap.svg
 * \image latex spectrumJonswap.eps "Jonswap spectrum" width=0.8\textwidth

 * \section section_waves_spectrum_Ochi Spectrum 5: Ochi (or 3-parameters Bretschneider) spectrum

   This spectrum is a generalisation of the ISSC spectrum.

    \f$\Phi _\eta  \left( \omega  \right) = \frac{A}
        {{\omega ^{4\lambda  + 1} }}\frac{{\left( {\frac{{4\lambda  + 1}}
        {5}B} \right)^\lambda  }}
        {{B\Gamma \left( \lambda  \right)}}e^{ - \frac{{\left( {4\lambda  + 1} \right)B}}
        {{5\omega ^4 }}} ,\forall \omega  \geqslant 0\f$

   with:
    - \f$A = 5\pi ^4 \frac{{H_s^2 }}{{T_p ^4 }}= \frac{B.H_s^2}{4}\omega_p^4\f$
    - \f$B = \frac{{20\pi ^4 }}{{T_p ^4 }} = \frac{5}{4}\omega_p^4\f$
    - \f$H_s\f$: significant wave height, in \f$m\f$;
    - \f$T_p\f$: peak period, in \f$s\f$.
    - \f$\lambda\f$: frequency dispersion parameter;
    - \f$\Gamma \left( \lambda  \right) = \int\limits_0^{ + \infty } {t^{\lambda  - 1} e^{ - t} dt}\f$

    This formulation is equivalent to:

        \f$\omega _0  = \frac{{2\pi }}{{T_p }}\f$

        \f$\alpha  = \left( {\frac{{\omega _0 }}{\omega }} \right)^4\f$

        \f$S\left( \omega  \right) = \frac{1}
            {{4\omega }}\frac{{\left( {\left( {\frac{{4\lambda  + 1}}
            {4}} \right)\alpha } \right)^\lambda  }}
            {{\Gamma \left( \lambda  \right)}}\exp \left( { - \left( {\frac{{4\lambda  + 1}}
            {4}} \right)\alpha } \right)
        \f$

    For \f$\lambda=1\f$, the Ochi spectrum reduces to the ISSC spectrum.
    Therefore this spectrum needs 3 parameters:
    - parameter 1: significant wave height (in \f$m\f$);
    - parameter 2: peak period (in \f$s\f$);
    - parameter 3: frequency dispersion parameter \f$\lambda\f$.

 * \image html spectrumOchi.svg
 * \image latex spectrumOchi.eps "Ochi spectrum" width=0.8\textwidth

 * \section section_waves_spectrum_Ochi_Hubble Spectrum 6: Ochi-Hubble spectrum

    This spectrum is the sum of two Ochi spectra. Therefore, it needs 6 parameters:
    - parameter 1: significant wave height (in \f$m\f$) for the first spectrum;
    - parameter 2: significant wave height (in \f$m\f$) for the second spectrum;
    - parameter 3: peak period (in \f$s\f$) for the first spectrum;
    - parameter 4: peak period (in \f$s\f$) for the second spectrum;
    - parameter 5: frequency dispersion parameter for the first spectrum;
    - parameter 6: frequency dispersion parameter for the second spectrum.

 * \section section_waves_spectrum_gaussian Spectrum 7: Gaussian spectrum

    \f$\Phi_\eta (\omega ) = \frac{{\left( {{{H_s } \mathord{\left/
        {\vphantom {{H_s } 4}} \right.
         \kern-\nulldelimiterspace} 4}} \right)^2 }}
        {{\sigma \left( {2\pi } \right)^{{3 \mathord{\left/
        {\vphantom {3 2}} \right.
         \kern-\nulldelimiterspace} 2}} }}\exp \left[ { - \frac{{\left( {\omega  - \omega _p } \right)^2 }}
        {{2\left( {2\pi \sigma } \right)^2 }}} \right],\forall \omega  \geqslant 0\f$

    with:

    - \f$H_s\f$: significant wave height, in \f$m\f$;
    - \f$\omega\f$: angular peak frequency, in \f$\textrm{rad}/s\f$;
    - \f$T_p\f$: peak period, in \f$s\f$;
    - \f$\sigma\f$: standard deviation, in Hz.

    Therefore this spectrum needs 3 parameters:

    - parameter 1: significant wave height (in \f$m\f$);
    - parameter 2: peak period (in \f$s\f$);
    - parameter 3: standard deviation s (in Hz).

 * \image html spectrumGaussian.svg
 * \image latex spectrumGaussian.eps "Gaussian spectrum" width=0.8\textwidth
 *
 * \section section_waves_comparison Spectra comparison

 * \image html spectrumComparison1.svg
 * \image latex spectrumComparison1.eps "Spectra comparison" width=0.8\textwidth
 */


#include "aubio_priv.h"
#include "aubio.h"

int main(){
        uint_t i,j;
        /* allocate some memory */
        uint_t win_s      = 1024;                       /* window size */
        uint_t channels   = 1;                          /* number of channel */
        fvec_t * in       = new_fvec (win_s, channels); /* input buffer */
        cvec_t * fftgrain = new_cvec (win_s, channels); /* fft norm and phase */
        fvec_t * out      = new_fvec (win_s, channels); /* output buffer */
  
        /* allocate fft and other memory space */
        aubio_fft_t * fft      = new_aubio_fft(win_s);              /* fftw interface */
        smpl_t * w             = AUBIO_ARRAY(smpl_t,win_s);         /* window */
        /* complex spectral data */
        fft_data_t ** spec      = AUBIO_ARRAY(fft_data_t*,channels); 
        for (i=0; i < channels; i++)
                spec[i] = AUBIO_ARRAY(fft_data_t,win_s);
        /* initialize the window (see mathutils.c) */
        aubio_window(w,win_s,aubio_win_hanningz);
  
        /* fill input with some data */
  
        /* execute stft */
        for (i=0; i < channels; i++) {
                aubio_fft_do (fft,in->data[i],spec[i],win_s);
                /* put norm and phase into fftgrain */
                aubio_fft_getnorm(fftgrain->norm[i], spec[i], win_s/2+1);
                aubio_fft_getphas(fftgrain->phas[i], spec[i], win_s/2+1);
        }
  
        /* execute inverse fourier transform */
        for (i=0; i < channels; i++) {
                for (j=0; j<win_s/2+1; j++) {
                        spec[i][j]  = CEXPC(I*aubio_unwrap2pi(fftgrain->phas[i][j]));
                        spec[i][j] *= fftgrain->norm[i][j];
                }
                aubio_fft_rdo(fft,spec[i],out->data[i],win_s);
        }
        return 0;
}

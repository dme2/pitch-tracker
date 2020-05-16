/** don't really need classes here as it'll be pretty much data in -> data out
     use ffts for fourier, rtaudio for recording

    functions:
      detect_pitch(&data_buffer) -> String
      mpm(&audio_buffer,samplerate) -> float x
      parabolic_interp() x
      autocor() x
      pick_peaks()  x
      
      
  **/

#include <algorithm>
#include <complex>
#include <float.h>
#include <ffts/ffts.h>
#include <map>
#include <numeric>
#include <stdexcept>
#include <vector>

#define MPM_CUTOFF 0.93
#define MPM_SMALL_CUTOFF 0.5
#define MPM_LOWER_PITCH_CUTOFF 80.0

template <typename T>
std::vector<T> autocor(std::vector<T> &audio_buff){
  long N = audio_buff.size();
  ffts_plan_t *fft_forward;
  ffts_plan_t *fft_backward;

  fft_forward =  ffts_init_1d(N*2, FFTS_FORWARD);
  fft_backward = ffts_init_1d(N*2, FFTS_BACKWARD);

  std::vector<T> out_real(N);
  std::vector<std::complex<float>> out_im(N*2);
  if(N == 0)
    throw std::invalid_argument("audio buffer is empty");
  
  std::transform(audio_buff.begin(), audio_buff.begin()+N,out_im.begin(),
                 [](T x) -> std::complex<T>{
                   return std::complex(x, static_cast<T>(0.0));
                 });

  ffts_execute(fft_forward, out_im.data(), out_im.data());

  std::complex<float> scale = { 1.0f / (float)(N*2), static_cast<T>(0.0)};

  for(int i =0; i < N; ++i){
    out_im[i] *= std::conj(out_im[i]) * scale;
  }

  ffts_execute(fft_backward, out_im.data(), out_im.data());

  std::transform(out_im.begin(), out_im.begin()+N, out_real.begin(),
                 [](std::complex<float> cplx) -> T {return std::real(cplx); });
  return out_real;
}

template <typename T>
std::vector<int> pick_peaks(const std::vector<T> &nsdf){
  std::vector<int> max_pos;
  int pos = 0;
  int cur_max_pos = 0;
  ssize_t size = nsdf.size();

  while (pos < (size-1) / 3 && nsdf[pos]>0)
    pos++;
  while (pos < (size-1) && nsdf[pos] <= 0.0)
    pos++;

  if (pos == 0)
    pos=1;

  while (pos < size-1){
    if ((nsdf[pos] >nsdf[pos-1] && nsdf[pos] >= nsdf[pos+1]) 
	&& (cur_max_pos == 0 || nsdf[pos] > nsdf[cur_max_pos]))
      cur_max_pos=pos;
    pos++;
    if(pos <size -1 && nsdf[pos]<=0){
      if(cur_max_pos >0){
        max_pos.push_back(cur_max_pos);
        cur_max_pos=0;
      }
      while (pos<size-1 && nsdf[pos] <= 0.0)
        pos++;
    }
  }
    if (cur_max_pos > 0)
      max_pos.push_back(cur_max_pos);
    return max_pos;
}

template <typename T>
std::pair<T,T> parabolic_interp(const std::vector<T> &array, int x_){
  int x_adjusted;
  T   x = (T)x_;

  if(x <1){
    x_adjusted = (array[x] <= array[x+1]) ? x : x + 1;
  }else if (x > signed(array.size()-1)) {
    x_adjusted = (array[x] <= array[x-1]) ? x : x - 1;
  }else {
    T den = array[x+1] + array[x-1] - 2 * array[x];
    T delta = array[x-1] - array[x+1];
    return (!den) ? std::make_pair(x, array[x]) : std::make_pair(x+delta / (2*den), array[x]-delta * delta / (8*den));
  }
  return std::make_pair(x_adjusted,array[x_adjusted]);
}
  
template <typename T>
T mpm(std::vector<T>& audio_buff, int sample_rate){
  std::vector<T> out_real = autocor(audio_buff);
  std::vector<std::pair<T,T>> estimates;
  std::vector<int> max_positions = pick_peaks(out_real);
  
  T highest_amplitude = -DBL_MAX;

  for (int i : max_positions){
    highest_amplitude = std::max(highest_amplitude, out_real[i]);
    if(out_real[i]>MPM_SMALL_CUTOFF) {
      auto x =parabolic_interp(out_real,i);
      estimates.push_back(x);
      highest_amplitude = std::max(highest_amplitude,std::get<1>(x));
    }
  }

  if(estimates.empty())
    return -1;

  T actual_cutof = MPM_CUTOFF * highest_amplitude;
  T period = 0;

  for (auto i : estimates) {
    if(std::get<1>(i) >= actual_cutof) {
      period = std::get<0>(i);
      break;
    }
  }

  T pitch_estimate = sample_rate / period ;

  return (pitch_estimate > MPM_LOWER_PITCH_CUTOFF) ? pitch_estimate : -1; 
}

//template double mpm<double>(const std::vector<double> &audio_buffer, int sample_rate);

//template float mpm<float>(const std::vector<float> &audio_buffer, int sample_rate);

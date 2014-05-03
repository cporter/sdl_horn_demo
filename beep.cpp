#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <queue>
#include <cmath>
#include <iostream>
#include <functional>

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;
const double DT = 1.0 / double(FREQUENCY);

struct BeepObject {
  double freq, duration;
  int samplesLeft;
  std::function<double(double, double)> volume; // volume at time (in seconds) = t
  double t { 0.0 };

  void IncTime () { t += DT; }
};

class Beeper
{
private:
    double v;
    std::queue<BeepObject> beeps;
public:
  Beeper();
  ~Beeper();
  void beep(double freq, double duration, std::function<double(double, double)>);
  void generateSamples(Sint16 *stream, int length);
  void wait();
};

void audio_callback(void*, Uint8*, int);

Beeper::Beeper()
{
    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = FREQUENCY;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 2048;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    // you might want to look for errors here
    SDL_OpenAudio(&desiredSpec, &obtainedSpec);

    // start play audio
    SDL_PauseAudio(0);
}

Beeper::~Beeper()
{
    SDL_CloseAudio();
}

void Beeper::generateSamples(Sint16 *stream, int length)
{
    int i = 0;
    while (i < length) {

        if (beeps.empty()) {
            while (i < length) {
                stream[i] = 0;
                i++;
            }
            return;
        }
        BeepObject& bo = beeps.front();

        int samplesToDo = std::min(i + bo.samplesLeft, length);
        bo.samplesLeft -= samplesToDo - i;


        while (i < samplesToDo) {
          double volume = bo.volume(bo.t, bo.duration);
          stream[i] = AMPLITUDE * volume * std::sin(v * 2 * M_PI / FREQUENCY);
          i++;
          v += bo.freq;
          bo . IncTime ();
        }

        if (bo.samplesLeft == 0) {
            beeps.pop();
        }
    }
}

void Beeper::beep(double freq, double duration, std::function<double(double, double)> volume)
{
  // BeepObject bo;
  //   bo.freq = freq;
  //   bo.samplesLeft = duration * FREQUENCY / 1000;

    SDL_LockAudio();
    beeps.push(BeepObject { freq, duration, int(duration * FREQUENCY), volume });
    SDL_UnlockAudio();
}

void Beeper::wait()
{
    int size;
    do {
        SDL_Delay(20);
        SDL_LockAudio();
        size = beeps.size();
        SDL_UnlockAudio();
    } while (size > 0);

}

void audio_callback(void *_beeper, Uint8 *_stream, int _length)
{
    Sint16 *stream = (Sint16*) _stream;
    int length = _length / 2;
    Beeper* beeper = (Beeper*) _beeper;

    beeper->generateSamples(stream, length);
}

std::function<double(double)> campled(std::function<double(double)> f) {
  return [f](double x) -> double {
    double y = f(x);
    return std::max (std::min (1.0, y), 0.0);
  };
}

std::function<double(double, double)> attack (double attack_time,
                                              double fade_out_time,
                                              double attack_vol,
                                              double initial_vol) {
  double t0 = 0.0;
  double t1 = attack_time / 2.0;
  double t2 = attack_time;
  double attack_difference = attack_vol - initial_vol;

  return [t0, t1, t2, attack_difference,
          fade_out_time, attack_vol, initial_vol]
      (double t, double total_time) -> double {
    double t3 = total_time - fade_out_time;
    if (t < t1) {
      return attack_vol * std::sqrt(t / t1);
    } else if (t < t2) {
      return attack_vol -  attack_difference * (t - t1) / (t2 - t1);
    } else if (t < t3) {
      return initial_vol;
    } else {
      double remaining = total_time - t;
      double d = remaining / fade_out_time;
      return initial_vol * d * d;
    }
  };
}

std::function<double(double, double)> const_vol(double x) {
  return [x](double, double) { return x; };
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_AUDIO);

    int duration = 1000;
    double Hz = 440;

    auto full_volume = const_vol (1.0);
    auto no_volume = const_vol (0.0);
    auto fade_in_out = [](double t) -> double {
      if (t < .25) {
        double d = t / .25;
        return d * d;
      } else if (t > .75) {
        double d = (1.0 - t) / .25;
        return d * d;
      } else {
        return 1.0;
      }
    };

    Beeper b;
    
    // b.beep (Hz, 1.0, full_volume);
    b.beep (Hz, 0.5, no_volume);
    for (int i = 0; i < 8; ++i) {
      b.beep (Hz, 1.0, attack (0.1 * i, 0.1, 0.4, 0.3));
    }

    b.wait();

    return 0;
}


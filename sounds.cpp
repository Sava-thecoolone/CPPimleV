#pragma once
#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "raylib.h"
#include "varray.cpp"

tsf *soundfont = nullptr;

void doaudio(void *buffer, unsigned int frames) {
    tsf_render_float(soundfont, (float*)buffer, frames, 0);
}

void setupaudiothread(varray &arr) {
    std::thread audio([&] {
        while (!WindowShouldClose()) {
            tsf_channel_sounds_off_all(soundfont, 0);
            int val = arr.high.lastval.load();
            if (val != -1) {
                double note = ((double)val/arr.len)*80+25;
                tsf_channel_note_on(soundfont, 0, note, 1.0f); // from ArrayV
                tsf_channel_set_pitchwheel(soundfont, 0, (int)((note-((int)note))*8192.0)+8192);
                tsf_channel_midi_control(soundfont, 0, 91, 10);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }
    });
    audio.detach();
}

AudioStream setupsound(varray &arr) {
    InitAudioDevice();
    soundfont = tsf_load_filename("sfx.sf2");
    if (!soundfont) {
        throw std::runtime_error("no sfx.sf2");
    }
    tsf_set_output(soundfont, TSF_STEREO_INTERLEAVED, 44100, 0.0f);
    tsf_channel_set_presetnumber(soundfont, 0, 17, 0);
    tsf_channel_midi_control(soundfont, 0, 72, 127);
    AudioStream stream = LoadAudioStream(44100, 32, 2);
    SetAudioStreamCallback(stream, doaudio);
    PlayAudioStream(stream);
    return stream;
}
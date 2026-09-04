#pragma once
#include <iostream>
#define TSF_IMPLEMENTATION
#include "tsf.h"
#include "raylib.h"
#include "varray.cpp"

tsf *soundfont = nullptr;

void doaudio(void *buffer, unsigned int frames) {
    tsf_render_float(soundfont, (float*)buffer, frames, 0);
}

void setupaudiothread(varray &arr) {
    int note = -1;
    std::thread audio([&] {
        while (!WindowShouldClose()) {
            if (arr.high.cur != -1) {
                note = ((double)arr.values[arr.high.cur]/arr.len)*80+20;
                tsf_note_on(soundfont, 56, note, 0.5f);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (note != -1) tsf_note_off(soundfont, 56, note);
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
    AudioStream stream = LoadAudioStream(44100, 32, 2);
    SetAudioStreamCallback(stream, doaudio);
    PlayAudioStream(stream);
    return stream;
}
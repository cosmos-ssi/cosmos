//*****************************************************************
// This file is part of CosmOS                                    *
// Copyright (C) 2020-2021 Tom Everett                            *
// Released under the stated terms in the file LICENSE            *
// See the file "LICENSE" in the source distribution for details  *
// ****************************************************************

#include <sys/kprintf/kprintf.h>
#include <sys/objecttype/objecttype_dsp.h>
//#include <sys/sound/tone.h>
//#include <sys/sound/wav.h>
#include <tests/dev/test_sb16.h>

void test_sb16() {
    // get the sb
    //  struct device* dsp = devicemgr_find_device("dsp0");
    //  if (0 != dsp) {
    //    struct wav_header* wav = sound_get_tone();

    //  struct objecttype_dsp* dsp_api = (struct objecttype_dsp*)dsp->api;
    //   dsp_play_function play_func = dsp_api->play;
    //        uint8_t* pcmstart = wav_pcm_start(wav);

    //      (*play_func)(dsp, pcmstart, wav->sample_rate, wav->bit_depth, wav->num_channels, wav->data_bytes);
    //  } else {
    //     kprintf("Unable to find dsp0\n");
    // }
}

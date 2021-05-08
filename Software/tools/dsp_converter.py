"""
 *
 * This file is part of LautIO
 * 
 * Copyright 2021 - olel
 * LautIO is licensed under MIT License
 * See LICENSE file for more information
 * 
"""

import xmltodict
import os
import json

# same as #defines in driver/dsp.h
DSP_CONTROL_VOLSLEW         =  0
DSP_CONTROL_MUX             =  1
DSP_CONTROL_EQ_SECOND_ORDER =  2
DSP_CONTROL_DC_SOURCE       =  3
DSP_CONTROL_SINE_SOURCE     =  4
DSP_CONTROL_SQUARE_SOURCE   =  5
DSP_CONTROL_SAWTOOTH_SOURCE =  6
DSP_CONTROL_TRIANGLE_SOURCE =  7
DSP_CONTROL_AUDIO_DELAY     =  8
DSP_CONTROL_EQ_FIRST_ORDER  =  9
DSP_CONTROL_GAIN            = 10
DSP_CONTROL_DEMUX           = 11
DSP_CONTROL_SOFT_CLIP       = 12
DSP_CONTROL_HARD_CLIP       = 13
DSP_CONTROL_COMPRESSOR_RMS  = 14
DSP_CONTROL_COMPRESSOR_PEAK = 15
DSP_CONTROL_TONE_CONTROL    = 16
DSP_CONTROL_STATE_VARIABLE  = 17
DSP_CONTROL_MUTE_ADC        = 18
DSP_CONTROL_MUTE_DAC        = 19

sigma_ctrl_type_names = { # incomplete!
    "SWGain1940DBAlg": DSP_CONTROL_VOLSLEW,
    "StMuxSwSlew": DSP_CONTROL_MUX,
    "EQ1940DualS": DSP_CONTROL_EQ_SECOND_ORDER,
    "DCInpAlg": DSP_CONTROL_DC_SOURCE,
    "sin_lookupAlg1940": DSP_CONTROL_SINE_SOURCE,
    "SquareAlg": DSP_CONTROL_SQUARE_SOURCE,
    "SawAlg": DSP_CONTROL_SAWTOOTH_SOURCE,
    "TriAlg": DSP_CONTROL_TRIANGLE_SOURCE,
    "FistOrderSingle": DSP_CONTROL_EQ_FIRST_ORDER,
    "Gain1940AlgNS": DSP_CONTROL_GAIN,
    "SoftClipAlgG": DSP_CONTROL_SOFT_CLIP,
    "HardClipNewAlg": DSP_CONTROL_HARD_CLIP,
}

ctrl_base_objects = { # incomplete!
    DSP_CONTROL_VOLSLEW: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_VOLSLEW,
        "volume": 0,
        "slew": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_MUX: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_MUX,
        "index": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_EQ_SECOND_ORDER: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_EQ_SECOND_ORDER,
        "Q": 0,
        "S": 0,
        "bandwidth": 0,
        "boost": 0,
        "freq": 0,
        "gain": 0,
        "filter_type": 0,
        "phase": 0,
        "state": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_MUTE_DAC: {
        "type": DSP_CONTROL_MUTE_DAC,
        "mute": False,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_MUTE_ADC: {
        "type": DSP_CONTROL_MUTE_ADC,
        "mute": False,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_DC_SOURCE: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_DC_SOURCE,
        "level": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_SINE_SOURCE: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_SINE_SOURCE,
        "freq": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_SQUARE_SOURCE: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_SQUARE_SOURCE,
        "freq": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_SAWTOOTH_SOURCE: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_SAWTOOTH_SOURCE,
        "freq": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_TRIANGLE_SOURCE: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_TRIANGLE_SOURCE,
        "freq": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_AUDIO_DELAY: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_AUDIO_DELAY,
        "delay": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_EQ_FIRST_ORDER: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_EQ_FIRST_ORDER,
        "freq": 0,
        "gain": 0,
        "filter_type": 0,
        "phase": 0,
        "state": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_GAIN: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_GAIN,
        "gain": 0,
        "channels": 1,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_DEMUX: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_DEMUX,
        "index": 0,
        "num_idx": 1,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_SOFT_CLIP: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_SOFT_CLIP,
        "alpha": 0,
        "ro": False,
        "change": False
    },
    DSP_CONTROL_HARD_CLIP: {
        "name": "unknown",
        "id": 0,
        "addr": 0,
        "type": DSP_CONTROL_HARD_CLIP,
        "high_threshold": 0,
        "low_threshold": 0,
        "ro": False,
        "change": False
    }
}

def save_data_file(data, fname):
    ret = list()
    for b in data.replace(",", "").split(" "):
        hx = b.replace("0x", "")
        val = int(hx, 16)
        ret.append(val)
    with open(fname, "wb+") as target:
        target.write(bytes(ret))

def convert_file(filename, outdir):
    registers = {
        "cr0": [0, 0], # addr, size
        "cr4": [0, 0], 
        "program": [0, 0],
        "param": [0, 0],
        "hwconf": [0, 0]
    }

    controls = {
        "controls": []
    }

    with open(filename, "r") as target:
        root = xmltodict.parse(target.read())

        # loading registers/program (addr, size to config.json binary data to single files)
        cr0 = root["Schematic"]["IC"]["Register"][0]
        paramdata = root["Schematic"]["IC"]["Register"][1]
        hwconf = root["Schematic"]["IC"]["Register"][2]
        cr4 = root["Schematic"]["IC"]["Register"][3]
        programdata = root["Schematic"]["IC"]["Program"]
        
        registers["cr0"] = cr0["Address"], cr0["Size"]
        save_data_file(cr0["Data"], os.path.join(outdir, "cr0.bin"))
        registers["param"] = paramdata["Address"], paramdata["Size"]
        save_data_file(paramdata["Data"], os.path.join(outdir, "param.bin"))
        registers["hwconf"] = hwconf["Address"], hwconf["Size"]
        save_data_file(hwconf["Data"], os.path.join(outdir, "hwconf.bin"))
        registers["cr4"] = cr4["Address"], cr4["Size"]
        save_data_file(cr4["Data"], os.path.join(outdir, "cr4.bin"))
        registers["program"] = programdata["Address"], programdata["Size"]
        save_data_file(programdata["Data"], os.path.join(outdir, "program.bin"))

        ctrl_id = 0

        for mod in root["Schematic"]["IC"]["Module"]:
            alg_name = mod["Algorithm"]["DetailedName"]
            alg_type = -1
            for t in sigma_ctrl_type_names.keys():
                if alg_name.startswith(t):
                    alg_type = sigma_ctrl_type_names[t]

            if (alg_type >= 0):
                base = ctrl_base_objects[alg_type]
                base["id"] = ctrl_id
                base["name"] = mod["CellName"]

                mod_param = mod["Algorithm"]["ModuleParameter"]
                if (type(mod_param) == list):
                    mod_param = mod_param[0]  # todo: taking always the first module parameter object, does this work?
                base["addr"] = mod_param["Address"]

                controls["controls"].append(base)

                ctrl_id += 1
                

    with open(os.path.join(outdir, "registers.json"), "w+") as target:
        json.dump(registers, target)

    with open(os.path.join(outdir, "controls.json"), "w+") as target:
        json.dump(controls, target)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument("input_file", help="path to sigmastudio export .xml file")
    parser.add_argument("outdir", help="output directory")
    args = parser.parse_args()

    if (not os.path.exists(args.outdir)):
        os.mkdir(args.outdir)

    convert_file(args.input_file, args.outdir)
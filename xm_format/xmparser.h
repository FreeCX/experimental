#ifndef __XM_PARSER_H__
#define __XM_PARSER_H__

// info from 'The Unofficial XM File Format Specification'

// signed types
typedef signed char sint8;          // sizeof() = 1
typedef signed short sint16;        // sizeof() = 2
typedef signed int sint32;          // sizeof() = 4
typedef signed long int sint64;     // sizeof() = 8
// unsigned types
typedef unsigned char uint8;        // sizeof() = 1
typedef unsigned short uint16;      // sizeof() = 2
typedef unsigned int uint32;        // sizeof() = 4
typedef unsigned long int uint64;   // sizeof() = 8

#pragma pack( push, 1 )
struct xm_struct {
    uint8  id_text[17];             // ID="Extended module: "
    uint8  module_name[20];         // Module name, padded with zeroes
    uint8  stripped;                // 0x1A
    uint8  tracker_name[20];        // Tracker name
    uint16 revision_number;         // Tracker revision number, 
                                    // hi-byte is major version
    uint32 header_size;             // Header size
    uint16 song_length;             // Song length in patterns
    uint16 restart_position;        // Restart position
    uint16 channels_number;         // Number of channels
    uint16 patterns_number;         // Number of patterns (< 256)
    uint16 instrument_number;       // Number of instruments (<128)
    uint16 flags;                   // Flags:
                                    // 0 - Linear frequency table
                                    // 1 - Amiga frequency table
    uint16 default_tempo;           // Default tempo
    uint16 default_bpm;             // Default BPM
    uint8  pattern_table[256];      // Pattern order table
};
#pragma pack( pop )
typedef struct xm_struct xm_struct_t;

#pragma pack( push, 1 )
struct xm_pattern {
    uint32 header;                  // Pattern header length
    uint8  packing_type;            // Packing type
    uint16 rows;                    // Number of rows in pattern (1..256)
    uint16 data_size;               // Packed pattern data size
// -- not for fread --
    uint8  *packed_data;            // Packed pattern data
};
#pragma pack( pop )
typedef struct xm_pattern xm_pattern_t;

#pragma pack( push, 1 )
struct xm_instrument {
    uint32 size;                    // Instrument size
    uint8  name[22];                // Instrument name
    uint8  type;                    // Instrument type
    uint16 samples_number;          // Number of samples
// -- not for fread --
// If the number of samples is greater than 0, then this will follow:
    uint32 sample_header_size;      //
    uint8  sample_keymap[96];       //
    uint8  volume_envelope[48];     //
    uint8  panning_envelop[48];     //
// Envelope points: 
// x,y...x,y... in couples (2 words/point => a maximum of 12 points).
    uint8  volume_points;           // Number of volume points
    uint8  panning_points;          // Number of panning points
    uint8  volume_sustain;          // Volume sustain point
    uint8  volume_loop_start;       // Volume loop start point
    uint8  volume_loop_end;         // Volume loop end point
    uint8  panning_sustain;         // Panning sustain point
    uint8  panning_loop_start;      // Panning loop start point
    uint8  panning_loop_end;        // Panning loop end point
    uint8  volume_type;             // Volume type
                                    // bit 0: on; bit 1: sustain; bit 3: loop
    uint8  panning_type;            // Panning type
                                    // bit 0: on; bit 1: sustain; bit 3: loop
    uint8  vibrato_type;            // Vibrato type
    uint8  vibrato_sweep;           // Vibrato sweep
    uint8  vibrato_depth;           // Vibrato depth
    uint8  vibrato_rate;            // Vibrato rate
    uint16 volume_fadeout;          // Volume fadeout
    uint8  reserved[22];            // Reserved
};
#pragma pack( pop )
typedef struct xm_instrument xm_instrument_t;

#pragma pack( push, 1 )
struct xm_sample {
    uint32 length;                  // Sample length
    uint32 loop_start;              // Sample loop start
    uint32 loop_end;                // Sample loop end
// Note: If the sample loop length is 0, the sample is *NOT* a looping one, 
// even if the "Forward loop" bit is set in the "TYPE" field.
    uint8  volume;                  // Volume
    uint8  finetune;                // Finetune (-128..+127)
    uint8  type;                    // Type:
                                    // Bit 0-1: 0 = No loop
                                    //          1 = Forward loop
                                    //          2 = Bidirectional loop (aka ping-pong)
                                    // Bit 4  : 0 = 8-bit samples
                                    //          1 = 16-bit samples
    uint8  panning;                 // Panning (0..255)
    sint8  note_number;             // Relative note number
    uint8  packed_value;            // Known values:
                                    // 00 = regular delta packed sample data
                                    // AD = 4-bit ADPCM-compressed sample data
    uint8  name[22];                // Sample name
};
#pragma pack( pop )
typedef struct xm_sample xm_sample_t;

#endif
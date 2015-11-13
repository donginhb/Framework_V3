/*
 *  Copyright (c) 2013 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#include <stdio.h>

#include <iostream>

//#include "gflags/gflags.h"
#include "modules/audio_coding/codecs/amrnb/include/amrnb_interface.h"
#include "modules/audio_coding/neteq/interface/webrtc_neteq.h"
#include "modules/audio_coding/neteq/interface/webrtc_neteq_help_macros.h"
#include "modules/audio_coding/neteq/interface/webrtc_neteq_internal.h"
#include "audio_loop.h"
#include "rtp_generator.h"
#include "typedefs.h"

using webrtc::test::AudioLoop;
using webrtc::test::RtpGenerator;
using webrtc::WebRtcRTPHeader;

#define FLAGS_runtime_ms    200000
#define FLAGS_lossrate      20
#define FLAGS_drift         0

int main(int argc, char* argv[]) {
  static const int kMaxChannels = 1;
  static const int kMaxSamplesPerMs = 48000 / 1000;
  static const int kOutputBlockSizeMs = 10;
  const std::string kInputFileName ="/mnt/nfs/testfile32kHz.pcm";
  const int kSampRateHz = 8000;
  const WebRtcNetEQDecoder kDecoderType = kDecoderAMR;
  const int kPayloadType = 95;

  static FILE * fp = NULL;
  static FILE * amrfp = NULL;
  fp = fopen("/mnt/nfs/neteq_fileout.pcm", "w+");
  amrfp = fopen("/mnt/nfs/guangming.amr", "r");
  
  // Initialize NetEq instance.
  int error;
  int inst_size_bytes;
  error = WebRtcNetEQ_AssignSize(&inst_size_bytes);
  if (error) {
    std::cerr << "Error returned from WebRtcNetEQ_AssignSize." << std::endl;
    exit(1);
  }
  char* inst_mem = new char[inst_size_bytes];
  void* neteq_inst;
  error = WebRtcNetEQ_Assign(&neteq_inst, inst_mem);
  if (error) {
    std::cerr << "Error returned from WebRtcNetEQ_Assign." << std::endl;
    exit(1);
  }
  // Select decoders.
  WebRtcNetEQDecoder decoder_list[] = {kDecoderType};
  int max_number_of_packets;
  int buffer_size_bytes;
  int overhead_bytes_dummy;
  error = WebRtcNetEQ_GetRecommendedBufferSize(
      neteq_inst, decoder_list, sizeof(decoder_list) / sizeof(decoder_list[1]),
      kTCPLargeJitter, &max_number_of_packets, &buffer_size_bytes,
      &overhead_bytes_dummy);
  if (error) {
    std::cerr << "Error returned from WebRtcNetEQ_GetRecommendedBufferSize."
              << std::endl;
    exit(1);
  }
  char* buffer_mem = new char[buffer_size_bytes];
  error = WebRtcNetEQ_AssignBuffer(neteq_inst, max_number_of_packets,
                                   buffer_mem, buffer_size_bytes);
  if (error) {
      std::cerr << "Error returned from WebRtcNetEQ_AssignBuffer." << std::endl;
      exit(1);
    }
  error = WebRtcNetEQ_Init(neteq_inst, kSampRateHz);
  if (error) {
      std::cerr << "Error returned from WebRtcNetEQ_Init." << std::endl;
      exit(1);
    }

    
  // Register decoder.
  WebRtcNetEQ_CodecDef codec_definition;
  SET_CODEC_PAR(codec_definition, kDecoderType, kPayloadType, NULL,
                kSampRateHz);
  SET_AMR_FUNCTIONS(codec_definition);
  error = WebRtcNetEQ_CodecDbAdd(neteq_inst, &codec_definition);
  if (error) {
    std::cerr << "Cannot register decoder." << std::endl;
    exit(1);
  }

  const size_t kInputBlockSizeSamples = 20 * kSampRateHz / 1000;  // 20 ms.

  int32_t time_now_ms = 0;

  // Get first input packet.
  WebRtcRTPHeader rtp_header;
  RtpGenerator rtp_gen(kSampRateHz / 1000);
  // Start with positive drift first half of simulation.
  double drift_factor = 0.1;
  //rtp_gen.set_drift_factor(drift_factor);
  bool drift_flipped = false;
  uint8_t input_payload[kInputBlockSizeSamples * sizeof(int16_t)];
  
  int32_t packet_input_time_ms =
      rtp_gen.GetRtpHeader(kPayloadType, kInputBlockSizeSamples, &rtp_header);
      
  fread(input_payload, 1, 32, amrfp);
  int payload_len = 31;

  std::cout << "packet_input_time_ms is " << packet_input_time_ms <<std::endl;
  
  
  // Main loop.
  while (time_now_ms < FLAGS_runtime_ms) {
    while (packet_input_time_ms <= time_now_ms) {
      // Drop every N packets, where N = FLAGS_lossrate.
      bool lost = false;
      if (FLAGS_lossrate > 0) {
        lost = ((rtp_header.header.sequenceNumber - 1) % FLAGS_lossrate) == 0;
      }
      if (!lost) {
        WebRtcNetEQ_RTPInfo rtp_info;
        rtp_info.payloadType = rtp_header.header.payloadType;
        rtp_info.sequenceNumber = rtp_header.header.sequenceNumber;
        rtp_info.timeStamp = rtp_header.header.timestamp;
        rtp_info.SSRC = rtp_header.header.ssrc;
        rtp_info.markerBit = rtp_header.header.markerBit;
        // Insert packet.
        std::cout << "packet_input_time_ms is " << packet_input_time_ms <<std::endl;
        error = WebRtcNetEQ_RecInRTPStruct(
            neteq_inst, &rtp_info, (input_payload + 1), payload_len,
            packet_input_time_ms * kSampRateHz / 1000);
            
        if (error != 0) {
          std::cerr << "WebRtcNetEQ_RecInRTPStruct returned error code " <<
              WebRtcNetEQ_GetErrorCode(neteq_inst) << std::endl;
          exit(1);
        }
      }

      // Get next packet.
      packet_input_time_ms = rtp_gen.GetRtpHeader(kPayloadType,
                                                  kInputBlockSizeSamples,
                                                  &rtp_header);
                                                  
      fread(input_payload, 1, 32, amrfp);
      payload_len = 31;

    }

    // Get output audio, but don't do anything with it.
    static const int kOutDataLen = kOutputBlockSizeMs * kMaxSamplesPerMs *
        kMaxChannels;
    int16_t out_data[kOutDataLen];
    int16_t samples_per_channel;
    error = WebRtcNetEQ_RecOut(neteq_inst, out_data, &samples_per_channel);
    if (error != 0) {
      std::cerr << "WebRtcNetEQ_RecOut returned error code " <<
          WebRtcNetEQ_GetErrorCode(neteq_inst) << std::endl;
      exit(1);
    }
    
    fwrite((char *)out_data, samples_per_channel*2, 1, fp);
    
    std::cout << "samples_per_channel is " << samples_per_channel <<std::endl;
    
    assert(samples_per_channel == kSampRateHz * 10 / 1000);

    time_now_ms += kOutputBlockSizeMs;
    if (time_now_ms >= FLAGS_runtime_ms / 2 && !drift_flipped) {
      // Apply negative drift second half of simulation.
      //rtp_gen.set_drift_factor(-drift_factor);
      drift_flipped = true;
    }
  }

   fclose(fp);
   fclose(amrfp);
  std::cout << "Simulation done" << std::endl;
  delete [] buffer_mem;
  delete [] inst_mem;
  return 0;
}

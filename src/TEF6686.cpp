#include "TEF6686.h"
#include <map>
#include <Arduino.h>

unsigned long rdstimer = 0;

void TEF6686::init(byte TEF) {
  uint8_t bootstatus;
  Tuner_I2C_Init();
  devTEF_APPL_Get_Operation_Status(&bootstatus);
  if (bootstatus == 0) {
    Tuner_Patch(TEF);
    delay(50);
    if (digitalRead(15) == LOW) Tuner_Init9216(); else Tuner_Init4000();
    power(1);
    Tuner_Init();
  }
}

bool TEF6686::getIdentification(uint16_t &device, uint16_t &hw_version, uint16_t &sw_version) {
  bool result = devTEF_Radio_Get_Identification(&device, &hw_version, &sw_version);
  return device;
  return hw_version;
  return sw_version;
}

void TEF6686::power(bool mode) {
  devTEF_APPL_Set_OperationMode(mode);
  if (mode == 0) devTEF_Set_Cmd(TEF_FM, Cmd_Tune_To, 7, 1, 10000);
}

void TEF6686::SetFreq(uint16_t frequency) {
  devTEF_Radio_Tune_To(frequency);
  currentfreq = frequency;
}

void TEF6686::SetFreqAM(uint16_t frequency) {
  devTEF_Radio_Tune_AM (frequency);
}

void TEF6686::setOffset(int8_t offset) {
  devTEF_Radio_Set_LevelOffset(offset * 10);
}

void TEF6686::setFMBandw(uint16_t bandwidth) {
  devTEF_Radio_Set_Bandwidth(0, bandwidth * 10, 1000, 1000);
}

void TEF6686::setAMBandw(uint16_t bandwidth) {
  devTEF_Radio_Set_BandwidthAM(0, bandwidth * 10, 1000, 1000);
}

void TEF6686::setAMCoChannel(uint16_t start) {
  devTEF_Radio_Set_CoChannel_AM(start * 10);
}

void TEF6686::setSoftmuteAM(uint8_t mode) {
  devTEF_Radio_Set_Softmute_Max_AM(mode);
}

void TEF6686::setSoftmuteFM(uint8_t mode) {
  devTEF_Radio_Set_Softmute_Max_FM(mode);
}

void TEF6686::setAMNoiseBlanker(uint16_t start) {
  if (start == 0) devTEF_Radio_Set_Noiseblanker_AM(0, 1000); else devTEF_Radio_Set_Noiseblanker_AM(1, start * 10);
}

void TEF6686::setAMAttenuation(uint16_t start) {
  devTEF_Radio_Set_Attenuator_AM(start * 10);
}

void TEF6686::setFMABandw() {
  devTEF_Radio_Set_Bandwidth(1, 3110, 1000, 1000);
}

void TEF6686::setiMS(bool mph) {
  devTEF_Radio_Set_MphSuppression(mph);
}

void TEF6686::setEQ(bool eq) {
  devTEF_Radio_Set_ChannelEqualizer(eq);
}

bool TEF6686::getStereoStatus() {
  uint16_t status;
  bool stereo = 0;
  if (1 == devTEF_Radio_Get_Stereo_Status(&status)) stereo = ((status >> 15) & 1) ? 1 : 0;
  return stereo;
}

void TEF6686::setMono(bool mono) {
  devTEF_Radio_Set_Stereo_Min(mono);
}

void TEF6686::setVolume(int8_t volume) {
  devTEF_Audio_Set_Volume(volume);
}

void TEF6686::setMute() {
  mute = true;
  devTEF_Audio_Set_Mute(1);
}

void TEF6686::setUnMute() {
  mute = false;
  devTEF_Audio_Set_Mute(0);
}

void TEF6686::setAGC(uint8_t agc) {
  if (agc == 0) devTEF_Radio_Set_RFAGC(920);
  if (agc == 1) devTEF_Radio_Set_RFAGC(900);
  if (agc == 2) devTEF_Radio_Set_RFAGC(870);
  if (agc == 3) devTEF_Radio_Set_RFAGC(840);
}

void TEF6686::setDeemphasis(uint8_t timeconstant) {
  if (timeconstant == 1) devTEF_Radio_Set_Deemphasis(500);
  if (timeconstant == 2) devTEF_Radio_Set_Deemphasis(750);
  if (timeconstant == 3) devTEF_Radio_Set_Deemphasis(0);
}

void TEF6686::setAudio(uint8_t audio) {
  devTEF_Radio_Specials(audio);
}

void TEF6686::setFMSI(uint8_t mode) {
  if (mode == 1) devTEF_APPL_Set_StereoImprovement(0);
  if (mode == 2) devTEF_APPL_Set_StereoImprovement(1);
}

void TEF6686::setFMSI_Time(uint16_t attack, uint16_t decay) {
  devTEF_APPL_Set_StereoBandBlend_Time(attack, decay);
}

void TEF6686::setFMSI_Gain(uint16_t band1, uint16_t band2, uint16_t band3, uint16_t band4) {
  devTEF_APPL_Set_StereoBandBlend_Gain(band1 * 10, band2 * 10, band3 * 10, band4 * 10);
}

void TEF6686::setFMSI_Bias(int16_t band1, int16_t band2, int16_t band3, int16_t band4) {
  devTEF_APPL_Set_StereoBandBlend_Bias(band1 - 250, band2 - 250, band3 - 250, band4 - 250);
}


void TEF6686::setFMNoiseBlanker(uint16_t start) {
  if (start == 0) devTEF_Radio_Set_NoisBlanker(0, 1000); else devTEF_Radio_Set_NoisBlanker(1, start * 10);
}

void TEF6686::setStereoLevel(uint8_t start) {
  if (start == 0) {
    devTEF_Radio_Set_Stereo_Level(0, start * 10, 60);
    devTEF_Radio_Set_Stereo_Noise(0, 240, 200);
    devTEF_Radio_Set_Stereo_Mph(0, 240, 200);
  } else {
    devTEF_Radio_Set_Stereo_Level(3, start * 10, 60);
    devTEF_Radio_Set_Stereo_Noise(3, 240, 200);
    devTEF_Radio_Set_Stereo_Mph(3, 240, 200);
  }
}

void TEF6686::setHighCutOffset(uint8_t start) {
  if (start == 0) {
    devTEF_Radio_Set_Highcut_Level(0, start * 10, 300);
    devTEF_Radio_Set_Highcut_Noise(0, 360, 300);
    devTEF_Radio_Set_Highcut_Mph(0, 360, 300);
  } else {
    devTEF_Radio_Set_Highcut_Level(3, start * 10, 300);
    devTEF_Radio_Set_Highcut_Noise(3, 360, 300);
    devTEF_Radio_Set_Highcut_Mph(3, 360, 300);
  }
}

void TEF6686::setHighCutLevel(uint16_t limit) {
  devTEF_Radio_Set_Highcut_Max(1, limit * 100);
}

void TEF6686::setStHiBlendLevel(uint16_t limit) {
  devTEF_Radio_Set_StHiBlend_Max(1, limit * 100);
}

void TEF6686::setStHiBlendOffset(uint8_t start) {
  if (start == 0) {
    devTEF_Radio_Set_StHiBlend_Level(0, start * 10, 300);
    devTEF_Radio_Set_StHiBlend_Noise(0, 360, 300);
    devTEF_Radio_Set_StHiBlend_Mph(0, 360, 300);
  } else {
    devTEF_Radio_Set_StHiBlend_Level(3, start * 10, 300);
    devTEF_Radio_Set_StHiBlend_Noise(3, 360, 300);
    devTEF_Radio_Set_StHiBlend_Mph(3, 360, 300);
  }
}

bool TEF6686::getProcessing(uint16_t &highcut, uint16_t &stereo, uint16_t &sthiblend, uint8_t &stband_1, uint8_t &stband_2, uint8_t &stband_3, uint8_t &stband_4) {
  bool result = devTEF_Radio_Get_Processing_Status(&highcut, &stereo, &sthiblend, &stband_1, &stband_2, &stband_3, &stband_4);
  return highcut;
  return stereo;
  return sthiblend;
  return stband_1;
  return stband_2;
  return stband_3;
  return stband_4;
}

bool TEF6686::getStatus(int16_t &level, uint16_t &USN, uint16_t &WAM, int16_t &offset, uint16_t &bandwidth, uint16_t &modulation) {
  bool result = devTEF_Radio_Get_Quality_Status(&level, &USN, &WAM, &offset, &bandwidth, &modulation);
  return level;
  return USN;
  return WAM;
  return bandwidth;
  return modulation;
}

bool TEF6686::getStatusAM(int16_t &level, uint16_t &noise, uint16_t &cochannel, int16_t &offset, uint16_t &bandwidth, uint16_t &modulation) {
  bool result = devTEF_Radio_Get_Quality_Status_AM(&level, &noise, &cochannel, &offset, &bandwidth, &modulation);
  return level;
  return noise;
  return cochannel;
  return bandwidth;
  return modulation;
}

void TEF6686::readRDS(bool showrdserrors)
{
  uint16_t rdsStat;
  uint8_t offset;
  bool rdsReady;
  if (rds.filter) {
    devTEF_Radio_Get_RDS_Status(&rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);
  } else {
    if (millis() >= rdstimer + 87) {
      rdstimer += 87;
      devTEF_Radio_Get_RDS_Data(&rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);

      if ((rdsStat & (1 << 14))) {
        for (int i = 0; i < 22; i++) devTEF_Radio_Get_RDS_Data(&rdsStat, &rds.rdsA, &rds.rdsB, &rds.rdsC, &rds.rdsD, &rds.rdsErr);
      }
    }
  }

  if (rds.rdsB != rdsBprevious) {
    rds.correct = false;
    rds.hasRDS = false;

    if (((rds.rdsErr >> 14) & 0x02) > 1) rds.rdsAerror = true; else rds.rdsAerror = false;            // Any errors in Block A?
    if (((rds.rdsErr >> 12) & 0x02) > 1) rds.rdsBerror = true; else rds.rdsBerror = false;            // Any errors in Block B?
    if (((rds.rdsErr >> 10) & 0x02) > 1) rds.rdsCerror = true; else rds.rdsCerror = false;            // Any errors in Block C?
    if (((rds.rdsErr >> 8) & 0x02) > 1) rds.rdsDerror = true; else rds.rdsDerror = false;             // Any errors in Block D?
    if (!rds.rdsAerror && !rds.rdsBerror && !rds.rdsCerror && !rds.rdsDerror) rds.correct = true;     // Any errors in all blocks?
    if ((rdsStat & (1 << 9))) rds.hasRDS = true;                                                      // RDS decoder synchronized and data available
    if ((rdsStat & (1 << 15))) rdsReady = true;

    if (rdsReady) {                                                                                   // We have all data to decode... let's go...

      //PI decoder
      if (rds.region != 1 && (rds.correct || rds.pierrors)) {
        rds.picode[0] = (rds.rdsA >> 12) & 0xF;
        rds.picode[1] = (rds.rdsA >> 8) & 0xF;
        rds.picode[2] = (rds.rdsA >> 4) & 0xF;
        rds.picode[3] = rds.rdsA & 0xF;
        for (int i = 0; i < 4; i++) {
          if (rds.picode[i] < 10) {
            rds.picode[i] += '0';                                                                     // Add ASCII offset for decimal digits
          } else {
            rds.picode[i] += 'A' - 10;                                                                // Add ASCII offset for hexadecimal letters A-F
          }
        }

        if (!rds.correct) rds.picode[4] = '?'; else rds.picode[4] = ' ';                              // Not sure, add a ?
        rds.picode[5] = '\0';
        if (strcmp(rds.picode, "0000?") == 0) {
          memset(rds.picode, 0, sizeof(rds.picode));
        }
      }

      // USA Station callsign decoder
      if (rds.region == 1) {                                                                          // When ID was decoded correctly before, no need to decode again.
        uint16_t stationID = rds.rdsA;
        if (stationID > 4096) {
          if (stationID > 21671 && (stationID & 0xF00U) >> 8 == 0) stationID = ((uint16_t)uint8_t(0xA0 + ((stationID & 0xF000U) >> 12)) << 8) + lowByte(stationID); // C0DE -> ACDE
          if (stationID > 21671 && lowByte(stationID) == 0) stationID = 0xAF00 + uint8_t(highByte(stationID)); // CD00 -> AFCD
          if (stationID < 39247) {
            if (stationID > 21671) {
              rds.picode[0] = 'W';
              stationID -= 21672;
            } else {
              rds.picode[0] = 'K';
              stationID -= 4096;
            }
            rds.picode[1] = char(stationID / 676 + 65);
            rds.picode[2] = char((stationID - 676 * int(stationID / 676)) / 26 + 65);
            rds.picode[3] = char(((stationID - 676 * int(stationID / 676)) % 26) + 65);
            rds.picode[5] = '\0';
          } else {
            stationID -= 4835;
            rds.picode[0] = 'K';
            rds.picode[1] = char(stationID / 676 + 65);
            rds.picode[2] = char((stationID - 676 * int(stationID / 676)) / 26 + 65);
            rds.picode[3] = char(((stationID - 676 * int(stationID / 676)) % 26) + 65);
            rds.picode[5] = '\0';
          }
        }
        if (rds.rdsAerror) rds.picode[4] = '?'; else rds.picode[4] = ' ';                       // Not sure, add a ?
      }

      // TP Indicator
      rds.hasTP = (bitRead(rds.rdsB, 10));

      switch (rds.rdsB >> 11) {
        case RDS_GROUP_0A:
          {
            //      Serial.println(rds.rdsB,BIN);
            //PS decoder
            if (showrdserrors || rds.correct) {
              offset = rds.rdsB & 0x03;                                                         // Let's get the character offset for PS

              ps_buffer2[(offset * 2) + 0] = ps_buffer[(offset * 2) + 0];                       // Make a copy of the PS buffer
              ps_buffer2[(offset * 2) + 1] = ps_buffer[(offset * 2) + 1];

              ps_buffer[(offset * 2)  + 0] = rds.rdsD >> 8;                                     // First character of segment
              ps_buffer[(offset * 2)  + 1] = rds.rdsD & 0xFF;                                   // Second character of segment
              ps_buffer[(offset * 2)  + 2] = '\0';                                              // Endmarker of segment

              if (offset == 3 && ps_process == true) {                                          // Last chars are received
                if (ps_buffer != ps_buffer2) {                                                  // When difference between old and new, let's go...
                  RDScharConverter(ps_buffer, PStext, sizeof(PStext) / sizeof(wchar_t), true);  // Convert 8 bit ASCII to 16 bit ASCII
                  String utf8String = convertToUTF8(PStext);                                    // Convert RDS characterset to ASCII
                  rds.stationName = extractUTF8Substring(utf8String, 0, 8, true);               // Make sure PS does not exceed 8 characters
                }
              }

              if (ps_process == false) {                                                        // Let's get 2 runs of 8 PS characters fast and without refresh
                ps_counter ++;                                                                  // Let's count each run
                RDScharConverter(ps_buffer, PStext, sizeof(PStext) / sizeof(wchar_t), true);    // Convert 8 bit ASCII to 16 bit ASCII
                String utf8String = convertToUTF8(PStext);                                      // Convert RDS characterset to ASCII
                rds.stationName = extractUTF8Substring(utf8String, 0, 8, true);
                if (ps_counter == 6) ps_process = true;                                         // OK, we had 2 runs, now let's go the idle PS writing
              }

              if (offset == 0) rds.hasDynamicPTY = bitRead(rds.rdsB, 2) & 0x1F;                 // Dynamic PTY flag
              if (offset == 1) rds.hasCompressed = bitRead(rds.rdsB, 2) & 0x1F;                 // Compressed flag
              if (offset == 2) rds.hasArtificialhead = bitRead(rds.rdsB, 2) & 0x1F;             // Artificial head flag
              if (offset == 3) rds.hasStereo = bitRead(rds.rdsB, 2) & 0x1F;                     // Stereo flag
            }

            // PTY decoder
            if (!rds.rdsBerror) {
              rds.stationTypeCode = (rds.rdsB >> 5) & 0x1F;                                     // Get 5 PTY bits from Block B
              if (rds.region == 0) strcpy(rds.stationType, PTY_EU[rds.stationTypeCode]);
              if (rds.region == 1) strcpy(rds.stationType, PTY_USA[rds.stationTypeCode]);
            }

            if (rds.correct) {
              //TA decoder
              rds.hasTA = (bitRead(rds.rdsB, 4)) && (bitRead(rds.rdsB, 10)) & 0x1F;               // Read TA flag

              //MS decoder
              if (((bitRead(rds.rdsB, 3)) & 0x1F) == 1) rds.MS = 1; else rds.MS = 2;              // Read MS flag

              //AF decoder
              if ((rds.rdsB >> 11) == 0 && af_counter < 50) {
                uint16_t buffer0;// = (rds.rdsC >> 8);
                uint16_t buffer1;// = (rds.rdsC & 0xFF);

                if ((rds.rdsC >> 8) > 0 && (rds.rdsC >> 8) < 205) buffer0 = (rds.rdsC >> 8) * 10 + 8750; else buffer0 = 0;
                if ((rds.rdsC & 0xFF) > 0 && (rds.rdsC & 0xFF) < 205) buffer1 = (rds.rdsC & 0xFF) * 10 + 8750; else buffer1 = 0;
                if (buffer0 != 0 || buffer1 != 0) rds.hasAF = true;

                bool isValuePresent = false;
                for (int i = 0; i < 50; i++) {
                  if (buffer0 == currentfreq || buffer0 == 0 || af[i].frequency == buffer0) {
                    isValuePresent = true;
                    break;
                  }
                }

                if (!isValuePresent) {
                  af[af_counter].frequency = buffer0;
                  if ((rds.rdsC & 0xFF) == 205) af[af_counter].filler = true;
                  af_counter++;
                }

                isValuePresent = false;
                for (int i = 0; i < 50; i++) {
                  if (buffer1 == currentfreq || buffer1 == 0 || af[i].frequency == buffer1) {
                    isValuePresent = true;
                    break;
                  }
                }

                if (!isValuePresent) {
                  af[af_counter].frequency = buffer1;
                  af_counter++;
                }

                for (int i = 0; i < 51 - 1; i++) {
                  for (int j = 0; j < 51 - i - 1; j++) {
                    // Ignore elements with value 0
                    if (af[j].frequency == 0) continue;

                    if (af[j].frequency > af[j + 1].frequency && af[j + 1].frequency != 0) {
                      // Swap the elements
                      uint16_t temp = af[j].frequency;
                      bool temp2 = af[j].filler;
                      af[j].frequency = af[j + 1].frequency;
                      af[j].filler = af[j + 1].filler;
                      af[j + 1].frequency = temp;
                      af[j + 1].filler = temp2;
                    }
                  }
                }
              }
            }
          } break;

        case RDS_GROUP_1A: {
            if (rds.correct) {
              if (rds.rdsC >> 12 == 0) {                                                               // ECC code readout
                rds.ECC = rds.rdsC & 0xff;
                rds.hasECC = true;
              }

              if (rds.rdsC >> 12 == 3) {                                                               // ECC code readout
                rds.LIC = rds.rdsC & 0xff;
                rds.hasLIC = true;
              }
            }
          } break;

        case RDS_GROUP_2A:
        case RDS_GROUP_2B: {
            if (showrdserrors || rds.correct) {
              // RT decoder
              rds.rtAB = (bitRead(rds.rdsB, 4));                                                  // Get AB flag

              if (rds.rtAB != rtABold) {                                                          // Erase old RT, because of AB change
                for (byte i = 0; i < 64; i++) {
                  rt_buffer[i] = 0x20;
                }
                rt_buffer[64] = '\0';
                rtABold = rds.rtAB;
              }

              offset = (rds.rdsB & 0xf) * 4;                                                      // Get RT character segment
              rt_buffer[offset + 0] = rds.rdsC >> 8;                                              // First character of segment
              rt_buffer[offset + 1] = rds.rdsC & 0xff;                                            // Second character of segment
              rt_buffer[offset + 2] = rds.rdsD >> 8;                                              // Thirth character of segment
              rt_buffer[offset + 3] = rds.rdsD & 0xff;                                            // Fourth character of segment

              wchar_t RTtext[65] = L"";                                                           // Create 16 bit char buffer for Extended ASCII
              RDScharConverter(rt_buffer, RTtext, sizeof(RTtext) / sizeof(wchar_t), true);              // Convert 8 bit ASCII to 16 bit ASCII
              rds.stationText = convertToUTF8(RTtext);                                            // Convert RDS characterset to ASCII
              rds.stationText = extractUTF8Substring(rds.stationText, 0, 64, true);                     // Make sure RT does not exceed 64 characters
              rds.stationText += " ";                                                             // Add extra space

              for (int i = 0; i < 64; i++) rt_buffer2[i]   = rt_buffer[i];
            }
          } break;

        case RDS_GROUP_4A: {
            if (rds.correct) {
              // CT
              uint32_t mjd;
              rds.hasCT = true;
              mjd = (rds.rdsB & 0x03);
              mjd <<= 15;
              mjd += ((rds.rdsC >> 1) & 0x7FFF);

              long J, C, Y, M;
              uint8_t LocalOffset;
              J = mjd + 2400001 +  68569;
              C = 4 * J / 146097;
              J = J - (146097 * C + 3) / 4;
              Y = 4000 * (J + 1) / 1461001;
              J = J - 1461 * Y / 4 + 31;
              M = 80 * (J + 0) / 2447;

              rds.days = J - 2447 * M / 80;
              J = M / 11;

              rds.months = M +  2 - (12 * J);
              rds.years = 100 * (C - 49) + Y + J;
              rds.hours = (rds.rdsD >> 12) & 0x0f;
              rds.hours += (rds.rdsC << 4) & 0x0010;
              rds.minutes = (rds.rdsD >> 6) & 0x3f;
              rds.offsetplusmin = bitRead(rds.rdsD, 5);
              rds.offset = (rds.rdsD & 0x3f);
              rds.hasCT = true;
            }
          } break;

        case RDS_GROUP_10A: {
            if (rds.correct) {
              // PTYN
              offset = bitRead(rds.rdsB, 0);                                                                // Get char offset

              ptyn_buffer[(offset * 4) + 0] = rds.rdsC >> 8;                                                // Get position 1 and 5
              ptyn_buffer[(offset * 4) + 1] = rds.rdsC & 0xFF;                                              // Get position 2 and 6
              ptyn_buffer[(offset * 4) + 2] = rds.rdsD >> 8;                                                // Get position 3 and 7
              ptyn_buffer[(offset * 4) + 3] = rds.rdsD & 0xFF;                                              // Get position 4 and 8
              RDScharConverter(ptyn_buffer, PTYNtext, sizeof(PTYNtext) / sizeof(wchar_t), false);           // Convert 8 bit ASCII to 16 bit ASCII
              String utf8String = convertToUTF8(PTYNtext);                                                  // Convert RDS characterset to ASCII
              rds.PTYN = extractUTF8Substring(utf8String, 0, 8, false);                                     // Make sure text is not longer than 8 chars
            }
          } break;

        case RDS_GROUP_10B:
        case RDS_GROUP_11A:
        case RDS_GROUP_11B:
        case RDS_GROUP_12A:
        case RDS_GROUP_12B: {
            if (rds.correct) {
              uint16_t  content_byte_1 = (rds.rdsB & 0x07);
              content_byte_1 = (content_byte_1  << 0x03);
              content_byte_1 += (rds.rdsC >> 0x0D);
              uint16_t  content_byte_2 = (rds.rdsC & 0x01);
              content_byte_2 = (content_byte_2 << 0x05);
              content_byte_2 += (rds.rdsD >> 0x0B);
              uint16_t  start_marker_1 = (rds.rdsC >> 0x07);
              start_marker_1 = (start_marker_1 & 0x3F);
              uint16_t length_marker_1 = (rds.rdsC >> 0x01);
              length_marker_1 = (length_marker_1  & 0x3F);
              uint16_t start_marker_2 = (rds.rdsD >> 0x05);
              start_marker_2 = (start_marker_2 & 0x3F);
              uint16_t length_marker_2 = (rds.rdsD & 0x1F);
              rds.hasRDSplus   = true;

              if (content_byte_1 == 0x04) {                                                                   // Artist
                rds.hasArtist = true;
                for (int i = 0; i <= length_marker_1; i++)musicArtist[i] = rt_buffer2[i + start_marker_1];
                musicArtist[length_marker_1 + 1] = 0;
              } else if (content_byte_1 == 0x01) {                                                            // Title
                rds.hasTitle = true;
                for (int i = 0; i <= length_marker_1; i++)musicTitle[i] = rt_buffer2[i + start_marker_1];
                musicTitle[length_marker_1 + 1] = 0;
              }

              if (content_byte_2 == 0x04) {                                                                   // Artist
                rds.hasArtist = true;
                for (int i = 0; i <= length_marker_2; i++)musicArtist[i] = rt_buffer2[i + start_marker_2];
                musicArtist[length_marker_2 + 1] = 0;
              } else if (content_byte_2 == 0x01) {                                                            // Title
                rds.hasTitle = true;
                for (int i = 0; i <= length_marker_2; i++)musicTitle[i] = rt_buffer2[i + start_marker_2];
                musicTitle[length_marker_2 + 1] = 0;
              }

              if (content_byte_1 == 0x24) {                                                                   // Host
                rds.hasHost = true;
                for (int i = 0; i <= length_marker_1; i++)stationHost[i] = rt_buffer2[i + start_marker_1];
                stationHost[length_marker_1 + 1] = 0;
              } else if (content_byte_2 == 0x24) {                                                            // Host
                rds.hasHost = true;
                for (int i = 0; i <= length_marker_2; i++)stationHost[i] = rt_buffer2[i + start_marker_2];
                stationHost[length_marker_2 + 1] = 0;
              }

              if (content_byte_1 == 0x21) {                                                                   // Event
                rds.hasEvent = true;
                for (int i = 0; i <= length_marker_1; i++)stationEvent[i] = rt_buffer2[i + start_marker_1];
                stationEvent[length_marker_1 + 1] = 0;
              } else if (content_byte_2 == 0x21) {                                                            // Event
                rds.hasEvent = true;
                for (int i = 0; i <= length_marker_2; i++)stationEvent[i] = rt_buffer2[i + start_marker_2];
                stationEvent[length_marker_2 + 1] = 0;
              }
            }

            if (rds.hasArtist) {
              wchar_t RTtext[33] = L"";                                                                     // Create 16 bit char buffer for Extended ASCII
              RDScharConverter(musicArtist, RTtext, sizeof(RTtext) / sizeof(wchar_t), false);               // Convert 8 bit ASCII to 16 bit ASCII
              rds.RTArtist = convertToUTF8(RTtext);                                                         // Convert RDS characterset to ASCII
              rds.RTArtist = extractUTF8Substring(rds.RTArtist, 0, 33, false);                              // Make sure RT does not exceed 32 characters
            }

            if (rds.hasTitle) {
              wchar_t RTtext[45] = L"";                                                                     // Create 16 bit char buffer for Extended ASCII
              RDScharConverter(musicTitle, RTtext, sizeof(RTtext) / sizeof(wchar_t), false);                // Convert 8 bit ASCII to 16 bit ASCII
              rds.RTTitle = convertToUTF8(RTtext);                                                          // Convert RDS characterset to ASCII
              rds.RTTitle = extractUTF8Substring(rds.RTTitle, 0, 44, false);                                // Make sure RT does not exceed 32 characters
            }

            if (rds.hasEvent) {
              wchar_t RTtext[45] = L"";                                                                     // Create 16 bit char buffer for Extended ASCII
              RDScharConverter(stationEvent, RTtext, sizeof(RTtext) / sizeof(wchar_t), false);              // Convert 8 bit ASCII to 16 bit ASCII
              rds.RTEvent = convertToUTF8(RTtext);                                                          // Convert RDS characterset to ASCII
              rds.RTEvent = extractUTF8Substring(rds.RTEvent, 0, 44, false);                                // Make sure RT does not exceed 32 characters
            }

            if (rds.hasHost) {
              wchar_t RTtext[45] = L"";                                                                     // Create 16 bit char buffer for Extended ASCII
              RDScharConverter(stationHost, RTtext, sizeof(RTtext) / sizeof(wchar_t), false);               // Convert 8 bit ASCII to 16 bit ASCII
              rds.RTHost = convertToUTF8(RTtext);                                                           // Convert RDS characterset to ASCII
              rds.RTHost = extractUTF8Substring(rds.RTHost, 0, 44, false);                                  // Make sure RT does not exceed 32 characters
            }
          }
          break;

        case RDS_GROUP_14A: {
            // EON
            if (rds.correct) {
              rds.hasEON = true;                                                                                    // Group is there, so we have EON

              bool isValuePresent = false;
              for (int i = 0; i < 5; i++) {
                if (eon[i].pi == rds.rdsD) {                                                                        // Check if EON is already in array
                  isValuePresent = true;
                  break;
                }
              }

              if (!isValuePresent) {
                eon[eon_counter].pi = rds.rdsD;                                                                     // Store PI on next array
                eon_counter++;
              }

              offset = rds.rdsB & 0x0F;                                                                             // Read offset

              if (offset < 4) {
                byte position;
                for (position = 0; position < 5; position++) {
                  if (eon[position].pi == rds.rdsD) {                                                               // Find position in array
                    break;
                  }
                }

                eon_buffer2[position][(offset * 2) + 0] = eon_buffer[position][(offset * 2) + 0];                   // Make a copy of the PS buffer
                eon_buffer2[position][(offset * 2) + 1] = eon_buffer[position][(offset * 2) + 1];

                eon_buffer[position][(offset * 2)  + 0] = rds.rdsC >> 8;                                            // First character of segment
                eon_buffer[position][(offset * 2)  + 1] = rds.rdsC & 0xFF;                                          // Second character of segment
                eon_buffer[position][(offset * 2)  + 2] = '\0';                                                     // Endmarker of segment

                if (offset > 2) {                                                                                   // Last chars are received
                  if (eon_buffer[position] != eon_buffer2[position]) {                                              // When difference between old and new, let's go...
                    RDScharConverter(eon_buffer[position], EONPStext, sizeof(EONPStext) / sizeof(wchar_t), true);   // Convert 8 bit ASCII to 16 bit ASCII
                    String utf8String = convertToUTF8(EONPStext);                                                   // Convert RDS characterset to ASCII
                    eon[position].ps = extractUTF8Substring(utf8String, 0, 8, true);                                // Make sure PS does not exceed 8 characters
                  }
                }

                if (offset > 4 && offset < 9) {
                  if (((rds.rdsC >> 8) * 10 + 8750) == currentfreq) {                                               // Check if mapped frequency belongs to current frequency
                    eon[position].mappedfreq = ((rds.rdsC & 0xFF) * 10 + 8750);                                     // Add mapped frequency to array
                  }
                }

              }
            }
          }
          break;
      }
    }
    rdsBprevious = rds.rdsB;
    rdsCprevious = rds.rdsC;
    rdsDprevious = rds.rdsD;
  }
}

void TEF6686::clearRDS (bool fullsearchrds)
{
  devTEF_Radio_Set_RDS(fullsearchrds);
  uint8_t i;
  rds.stationName = "";
  rds.stationText = "";
  rds.RTArtist = "";
  rds.RTTitle = "";
  rds.RTHost = "";
  rds.RTEvent = "";
  for (i = 0; i < 9; i++) {
    ps_buffer[i] = 0;
    PStext[i] = L'\0';
    ptyn_buffer[i] = 0;
    PTYNtext[i] = L'\0';
  }
  for (i = 0; i < 65; i++) rt_buffer[i] = 0;
  for (i = 0; i < 18; i++) rds.stationType[i] = 0;
  for (i = 0; i < 6; i++) rds.picode[i] = 0;

  for (i = 0; i < 50; i++) {
    af[i].frequency = 0;
    af[i].filler = false;
  }

  for (i = 0; i < 5; i++) {
    eon[i].pi = 0;
    for (int y = 0; y < 9; y++) {
      eon_buffer[i][y] = 0;
      eon_buffer2[i][y] = 0;
    }
    eon[i].ps = "";
  }

  for (i = 0; i < 45; i++) {
    musicArtist[i] = 0;
    musicTitle[i] = 0;
    stationEvent[i] = 0;
    stationHost[i] = 0;
  }

  rds.ECC = 0;
  rds.LIC = 0;
  rds.stationTypeCode = 32;
  rds.hasECC = false;
  rds.hasLIC = false;
  rds.hasRT = false;
  rds.hasRDS = false;
  rds.hasTP = false;
  rds.hasTA = false;
  rds.hasEON = false;
  rds.hasCT = false;
  rds.correct = false;
  rt_process = false;
  ps_process = false;
  rds.rdsreset = true;
  rds.hasTitle = false;
  rds.hasArtist = false;
  rds.hasEvent = false;
  rds.hasHost = false;
  rds.hasArtificialhead = false;
  rds.hasCompressed = false;
  rds.hasDynamicPTY = false;
  rds.hasStereo = false;
  ps_counter = 0;
  af_counter = 0;
  eon_counter = 0;
  rds.MS = 0;
}

void TEF6686::tone(uint16_t time, int16_t amplitude, uint16_t frequency) {
  devTEF_Audio_Set_Mute(0);
  devTEF_Radio_Set_Wavegen(1, amplitude, frequency);
  delay (time);
  devTEF_Radio_Set_Wavegen(0, 0, 0);
}

String TEF6686::convertToUTF8(const wchar_t* input) {
  String output;
  while (*input) {
    uint16_t unicode = *input;
    if (unicode < 0x80) {
      output += (char)unicode;
    } else if (unicode < 0x800) {
      output += (char)(0xC0 | (unicode >> 6));
      output += (char)(0x80 | (unicode & 0x3F));
    } else if (unicode < 0x10000) {
      output += (char)(0xE0 | (unicode >> 12));
      output += (char)(0x80 | ((unicode >> 6) & 0x3F));
      output += (char)(0x80 | (unicode & 0x3F));
    } else {
      output += (char)(0xF0 | (unicode >> 18));
      output += (char)(0x80 | ((unicode >> 12) & 0x3F));
      output += (char)(0x80 | ((unicode >> 6) & 0x3F));
      output += (char)(0x80 | (unicode & 0x3F));
    }
    input++;
  }
  return output;
}

String TEF6686::extractUTF8Substring(const String & utf8String, size_t start, size_t length, bool under) {
  String substring;
  size_t utf8Length = utf8String.length();
  size_t utf8Index = 0;
  size_t charIndex = 0;

  while (utf8Index < utf8Length && charIndex < start + length) {
    uint8_t currentByte = utf8String.charAt(utf8Index);
    uint8_t numBytes = 0;

    if (currentByte < 0x80) {
      numBytes = 1;
    } else if ((currentByte >> 5) == 0x6) {
      numBytes = 2;
    } else if ((currentByte >> 4) == 0xE) {
      numBytes = 3;
    } else if ((currentByte >> 3) == 0x1E) {
      numBytes = 4;
    }

    if (charIndex >= start) {
      substring += utf8String.substring(utf8Index, utf8Index + numBytes);
    }

    utf8Index += numBytes;
    charIndex++;
  }

  if (under && rds.underscore) {
    while (substring.length() < length) {
      substring += '_';
    }
  }

  return substring;
}


void TEF6686::RDScharConverter(const char* input, wchar_t* output, size_t size, bool under) {
  for (size_t i = 0; i < size - 1; i++) {
    char currentChar = input[i];
    switch (currentChar) {
      case 0x20: if (under && rds.underscore) output[i] = L'_'; else output[i] = L' '; break;
      case 0x21 ... 0x5D: output[i] = static_cast<wchar_t>(currentChar); break;
      case 0x5E: output[i] = L'―'; break;
      case 0x5F: output[i] = L'_'; break;
      case 0x60: output[i] = L'‖'; break;
      case 0x61 ... 0x7d: output[i] = static_cast<wchar_t>(currentChar); break;
      case 0x7E: output[i] = L'¯'; break;
      case 0x7F: output[i] = L' '; break;
      case 0x80: output[i] = L'á'; break;
      case 0x81: output[i] = L'à'; break;
      case 0x82: output[i] = L'é'; break;
      case 0x83: output[i] = L'è'; break;
      case 0x84: output[i] = L'í'; break;
      case 0x85: output[i] = L'ì'; break;
      case 0x86: output[i] = L'ó'; break;
      case 0x87: output[i] = L'ò'; break;
      case 0x88: output[i] = L'ú'; break;
      case 0x89: output[i] = L'ù'; break;
      case 0x8A: output[i] = L'Ñ'; break;
      case 0x8B: output[i] = L'Ç'; break;
      case 0x8C: output[i] = L'Ş'; break;
      case 0x8D: output[i] = L'β'; break;
      case 0x8E: output[i] = L'¡'; break;
      case 0x8F: output[i] = L'Ĳ'; break;
      case 0x90: output[i] = L'â'; break;
      case 0x91: output[i] = L'ä'; break;
      case 0x92: output[i] = L'ê'; break;
      case 0x93: output[i] = L'ë'; break;
      case 0x94: output[i] = L'î'; break;
      case 0x95: output[i] = L'ï'; break;
      case 0x96: output[i] = L'ô'; break;
      case 0x97: output[i] = L'ö'; break;
      case 0x98: output[i] = L'û'; break;
      case 0x99: output[i] = L'ü'; break;
      case 0x9A: output[i] = L'ñ'; break;
      case 0x9B: output[i] = L'ç'; break;
      case 0x9C: output[i] = L'ş'; break;
      case 0x9D: output[i] = L'ǧ'; break;
      case 0x9E: output[i] = L'ı'; break;
      case 0x9F: output[i] = L'ĳ'; break;
      case 0xA0: output[i] = L'ª'; break;
      case 0xA1: output[i] = L'α'; break;
      case 0xA2: output[i] = L'©'; break;
      case 0xA3: output[i] = L'‰'; break;
      case 0xA4: output[i] = L'Ǧ'; break;
      case 0xA5: output[i] = L'ě'; break;
      case 0xA6: output[i] = L'ň'; break;
      case 0xA7: output[i] = L'ő'; break;
      case 0xA8: output[i] = L'π'; break;
      case 0xA9: output[i] = L'€'; break;
      case 0xAA: output[i] = L'£'; break;
      case 0xAB: output[i] = L'$'; break;
      case 0xAC: output[i] = L'←'; break;
      case 0xAD: output[i] = L'↑'; break;
      case 0xAE: output[i] = L'→'; break;
      case 0xAF: output[i] = L'↓'; break;
      case 0xB0: output[i] = L'º'; break;
      case 0xB1: output[i] = L'¹'; break;
      case 0xB2: output[i] = L'²'; break;
      case 0xB3: output[i] = L'³'; break;
      case 0xB4: output[i] = L'±'; break;
      case 0xB5: output[i] = L'İ'; break;
      case 0xB6: output[i] = L'ń'; break;
      case 0xB7: output[i] = L'ű'; break;
      case 0xB8: output[i] = L'µ'; break;
      case 0xB9: output[i] = L'¿'; break;
      case 0xBA: output[i] = L'÷'; break;
      case 0xBB: output[i] = L'°'; break;
      case 0xBC: output[i] = L'¼'; break;
      case 0xBD: output[i] = L'½'; break;
      case 0xBE: output[i] = L'¾'; break;
      case 0xBF: output[i] = L'§'; break;
      case 0xC0: output[i] = L'Á'; break;
      case 0xC1: output[i] = L'À'; break;
      case 0xC2: output[i] = L'É'; break;
      case 0xC3: output[i] = L'È'; break;
      case 0xC4: output[i] = L'Í'; break;
      case 0xC5: output[i] = L'Ì'; break;
      case 0xC6: output[i] = L'Ó'; break;
      case 0xC7: output[i] = L'Ò'; break;
      case 0xC8: output[i] = L'Ú'; break;
      case 0xC9: output[i] = L'Ù'; break;
      case 0xCA: output[i] = L'Ř'; break;
      case 0xCB: output[i] = L'Č'; break;
      case 0xCC: output[i] = L'Š'; break;
      case 0xCD: output[i] = L'Ž'; break;
      case 0xCE: output[i] = L'Ð'; break;
      case 0xCF: output[i] = L'Ŀ'; break;
      case 0xD0: output[i] = L'Â'; break;
      case 0xD1: output[i] = L'Ä'; break;
      case 0xD2: output[i] = L'Ê'; break;
      case 0xD3: output[i] = L'Ë'; break;
      case 0xD4: output[i] = L'Î'; break;
      case 0xD5: output[i] = L'Ï'; break;
      case 0xD6: output[i] = L'Ô'; break;
      case 0xD7: output[i] = L'Ö'; break;
      case 0xD8: output[i] = L'Û'; break;
      case 0xD9: output[i] = L'Ü'; break;
      case 0xDA: output[i] = L'ř'; break;
      case 0xDB: output[i] = L'č'; break;
      case 0xDC: output[i] = L'š'; break;
      case 0xDD: output[i] = L'ž'; break;
      case 0xDE: output[i] = L'đ'; break;
      case 0xDF: output[i] = L'ŀ'; break;
      case 0xE0: output[i] = L'Ã'; break;
      case 0xE1: output[i] = L'Å'; break;
      case 0xE2: output[i] = L'Æ'; break;
      case 0xE3: output[i] = L'Œ'; break;
      case 0xE4: output[i] = L'ŷ'; break;
      case 0xE5: output[i] = L'Ý'; break;
      case 0xE6: output[i] = L'Õ'; break;
      case 0xE7: output[i] = L'Ø'; break;
      case 0xE8: output[i] = L'Þ'; break;
      case 0xE9: output[i] = L'Ŋ'; break;
      case 0xEA: output[i] = L'Ŕ'; break;
      case 0xEB: output[i] = L'Ć'; break;
      case 0xEC: output[i] = L'Ś'; break;
      case 0xED: output[i] = L'Ź'; break;
      case 0xEE: output[i] = L'Ŧ'; break;
      case 0xEF: output[i] = L'ð'; break;
      case 0xF0: output[i] = L'ã'; break;
      case 0xF1: output[i] = L'å'; break;
      case 0xF2: output[i] = L'æ'; break;
      case 0xF3: output[i] = L'œ'; break;
      case 0xF4: output[i] = L'ŵ'; break;
      case 0xF5: output[i] = L'ý'; break;
      case 0xF6: output[i] = L'õ'; break;
      case 0xF7: output[i] = L'ø'; break;
      case 0xF8: output[i] = L'þ'; break;
      case 0xF9: output[i] = L'ŋ'; break;
      case 0xFA: output[i] = L'ŕ'; break;
      case 0xFB: output[i] = L'ć'; break;
      case 0xFC: output[i] = L'ś'; break;
      case 0xFD: output[i] = L'ź'; break;
      case 0xFE: output[i] = L'ŧ'; break;
      case 0xFF: output[i] = L' '; break;
    }
  }
  output[size - 1] = L'\0';
}
#include <ESPert.h>

ESPert espert;

void setup() {
  espert.init();

  espert.print("ESPert ID............: ");
  espert.println(espert.info.getId());

  espert.print("ESPert Library.......: ");
  espert.println(espert.info.getLibraryVersion());

  espert.print("FreeHeap.............: ");
  espert.println(espert.info.getFreeHeap());

  espert.print("Chip ID..............: ");
  espert.println(espert.info.getChipId());

  espert.print("Flash Chip ID........: 0x");
  espert.println(espert.info.getFlashChipId(), HEX);

  espert.print("Flash Chip Info......: ");
  espert.println(espert.info.getFlashChipInfo());

  espert.print("Flash Chip Size......: ");
  espert.print(espert.info.getFlashChipSize());
  espert.println(String(" (") + (espert.info.getFlashChipSize() / 1024) + "KB)");

  espert.print("Flash Chip Size by ID: ");
  espert.print(espert.info.getFlashChipSizeByChipId());
  espert.println(String(" (") + (espert.info.getFlashChipSizeByChipId( ) / 1024) + "KB)");

  espert.print("Flash Chip Speed.....: ");
  espert.println(espert.info.getFlashChipSpeed());

  espert.print("Cycle Count..........: ");
  espert.println(espert.info.getCycleCount());
}

void loop() {
}
